#!/usr/bin/env python3
"""Benchmark PCH impact for CP round builds (cold, incremental, post-reconfigure)."""

from __future__ import annotations

import argparse
import json
import os
import shutil
import subprocess
import time
from pathlib import Path
from typing import Dict, List

DEFAULT_ALGORITHMS_DIR = Path(__file__).resolve().parents[1]


class BenchmarkError(RuntimeError):
    """Raised when a benchmark command fails."""


def _run_command(
    argv: List[str],
    cwd: Path,
    env: Dict[str, str],
    verbose: bool,
) -> float:
    """Run a command and return wall-clock duration in seconds."""
    if verbose:
        print(f"[bench] {' '.join(argv)}")

    start = time.perf_counter()
    completed = subprocess.run(
        argv,
        cwd=str(cwd),
        env=env,
        capture_output=True,
        text=True,
        check=False,
    )
    elapsed = time.perf_counter() - start

    if completed.returncode != 0:
        raise BenchmarkError(
            "Command failed\n"
            f"cmd: {' '.join(argv)}\n"
            f"cwd: {cwd}\n"
            f"exit: {completed.returncode}\n"
            f"stdout:\n{completed.stdout}\n"
            f"stderr:\n{completed.stderr}"
        )
    return elapsed


def _discover_target(round_dir: Path) -> str:
    """Pick a default target from round sources when none is provided."""
    candidates = sorted(
        p for p in round_dir.glob("*.cpp") if "template" not in p.name.lower()
    )
    if not candidates:
        raise BenchmarkError(f"No candidate .cpp targets found in {round_dir}")
    return candidates[0].stem


def _write_touch_marker(source_file: Path, marker: str) -> None:
    """Append a marker comment to force an incremental rebuild."""
    with source_file.open("a", encoding="utf-8") as fh:
        fh.write(f"\n// {marker}\n")


def _write_new_target(round_dir: Path, target_name: str) -> None:
    """Create a minimal benchmark source file for post-reconfigure timing."""
    source = round_dir / f"{target_name}.cpp"
    source.write_text(
        "#include <bits/stdc++.h>\n"
        "int main() {\n"
        "    std::ios::sync_with_stdio(false);\n"
        "    std::cin.tie(nullptr);\n"
        "    return 0;\n"
        "}\n",
        encoding="utf-8",
    )


def _benchmark_mode(
    round_dir: Path,
    target: str,
    compiler: str,
    pch_enabled: bool,
    algorithms_dir: Path,
    verbose: bool,
) -> Dict[str, float]:
    """Measure configure/build timings for one PCH mode."""
    env = os.environ.copy()
    env["CP_ALGORITHMS_DIR"] = str(algorithms_dir)
    env["CCACHE_DISABLE"] = "1"

    configure_preset = f"cp-debug-{compiler}"
    build_preset = f"cp-build-debug-{compiler}"

    mode = "on" if pch_enabled else "off"
    bench_target = f"__bench_new_target_{mode}"
    source_file = round_dir / f"{target}.cpp"
    source_backup = source_file.read_text(encoding="utf-8")
    bench_source = round_dir / f"{bench_target}.cpp"

    configure_argv = [
        "cmake",
        "--preset",
        configure_preset,
        "--fresh",
        f"-DCP_ENABLE_PCH={'ON' if pch_enabled else 'OFF'}",
        "-DCP_FORCE_PCH_REBUILD=OFF",
    ]
    cold_build_argv = [
        "cmake",
        "--build",
        "--preset",
        build_preset,
        "--target",
        target,
    ]

    try:
        configure_sec = _run_command(configure_argv, round_dir, env, verbose)
        cold_build_sec = _run_command(cold_build_argv, round_dir, env, verbose)

        _write_touch_marker(source_file, f"bench-incremental-{mode}")
        incremental_build_sec = _run_command(cold_build_argv, round_dir, env, verbose)

        _write_new_target(round_dir, bench_target)
        reconfigure_sec = _run_command(
            [
                "cmake",
                "--preset",
                configure_preset,
                f"-DCP_ENABLE_PCH={'ON' if pch_enabled else 'OFF'}",
                "-DCP_FORCE_PCH_REBUILD=OFF",
            ],
            round_dir,
            env,
            verbose,
        )
        post_reconfigure_build_sec = _run_command(
            [
                "cmake",
                "--build",
                "--preset",
                build_preset,
                "--target",
                bench_target,
            ],
            round_dir,
            env,
            verbose,
        )
    finally:
        source_file.write_text(source_backup, encoding="utf-8")
        if bench_source.exists():
            bench_source.unlink()

    return {
        "configure_sec": round(configure_sec, 3),
        "cold_build_sec": round(cold_build_sec, 3),
        "incremental_build_sec": round(incremental_build_sec, 3),
        "reconfigure_sec": round(reconfigure_sec, 3),
        "post_reconfigure_build_sec": round(post_reconfigure_build_sec, 3),
    }


def _build_result(
    round_dir: Path,
    target: str,
    compiler: str,
    threshold_sec: float,
    on_metrics: Dict[str, float],
    off_metrics: Dict[str, float],
) -> Dict[str, object]:
    """Assemble benchmark output payload and recommendation."""
    incremental_gain_sec = round(
        off_metrics["incremental_build_sec"] - on_metrics["incremental_build_sec"],
        3,
    )
    cold_penalty_sec = round(
        on_metrics["cold_build_sec"] - off_metrics["cold_build_sec"],
        3,
    )
    post_reconfigure_penalty_sec = round(
        on_metrics["post_reconfigure_build_sec"]
        - off_metrics["post_reconfigure_build_sec"],
        3,
    )

    recommendation = (
        "disable_default_pch"
        if incremental_gain_sec < threshold_sec
        else "keep_default_pch"
    )

    return {
        "round_dir": str(round_dir),
        "target": target,
        "compiler": compiler,
        "threshold_sec": threshold_sec,
        "pch_on": on_metrics,
        "pch_off": off_metrics,
        "delta": {
            "incremental_gain_sec": incremental_gain_sec,
            "cold_penalty_sec": cold_penalty_sec,
            "post_reconfigure_penalty_sec": post_reconfigure_penalty_sec,
        },
        "recommendation": recommendation,
    }


def parse_args() -> argparse.Namespace:
    """Parse benchmark CLI arguments."""
    parser = argparse.ArgumentParser(
        description="Benchmark PCH value for CP round builds (Debug presets)."
    )
    parser.add_argument(
        "--round-dir",
        type=Path,
        required=True,
        help="absolute or relative path to a round directory containing CMakeLists.txt",
    )
    parser.add_argument(
        "--target",
        default=None,
        help="target/problem name to benchmark (defaults to first .cpp file)",
    )
    parser.add_argument(
        "--compiler",
        choices=("gcc", "clang"),
        default="gcc",
        help="debug preset compiler family",
    )
    parser.add_argument(
        "--algorithms-dir",
        type=Path,
        default=DEFAULT_ALGORITHMS_DIR,
        help="path to CodeForces/Algorithms directory",
    )
    parser.add_argument(
        "--threshold-sec",
        type=float,
        default=1.0,
        help="incremental gain threshold used for recommendation",
    )
    parser.add_argument(
        "--json-out",
        type=Path,
        default=None,
        help="optional path where JSON result is saved",
    )
    parser.add_argument(
        "--verbose",
        action="store_true",
    )
    return parser.parse_args()


def main() -> int:
    """Execute PCH benchmark workflow and emit JSON output."""
    ns = parse_args()
    round_dir = ns.round_dir.expanduser().resolve()
    algorithms_dir = ns.algorithms_dir.expanduser().resolve()

    if not round_dir.is_dir() or not (round_dir / "CMakeLists.txt").is_file():
        raise BenchmarkError(f"Invalid round dir: {round_dir}")
    if not algorithms_dir.is_dir() or not (algorithms_dir / "cmake").is_dir():
        raise BenchmarkError(f"Invalid algorithms dir: {algorithms_dir}")

    target = ns.target or _discover_target(round_dir)

    on_metrics = _benchmark_mode(
        round_dir,
        target,
        ns.compiler,
        True,
        algorithms_dir,
        ns.verbose,
    )
    off_metrics = _benchmark_mode(
        round_dir,
        target,
        ns.compiler,
        False,
        algorithms_dir,
        ns.verbose,
    )

    result = _build_result(
        round_dir,
        target,
        ns.compiler,
        ns.threshold_sec,
        on_metrics,
        off_metrics,
    )

    print(json.dumps(result, ensure_ascii=False, indent=2))
    if ns.json_out is not None:
        json_out = ns.json_out.expanduser()
        json_out.parent.mkdir(parents=True, exist_ok=True)
        json_out.write_text(
            json.dumps(result, ensure_ascii=False, indent=2) + "\n",
            encoding="utf-8",
        )
        print(f"[bench] wrote {json_out}")

    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except BenchmarkError as exc:
        print(f"Error: {exc}")
        raise SystemExit(1)
