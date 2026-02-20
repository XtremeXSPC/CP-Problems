#!/usr/bin/env python3

from __future__ import annotations

import argparse
import difflib
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path


@dataclass
class CaseResult:
    problem: str
    case_name: str
    ok: bool
    message: str = ""


def normalize_output(text: str) -> str:
    lines = text.splitlines()
    while lines and lines[-1].strip() == "":
        lines.pop()
    return "\n".join(line.rstrip() for line in lines)


def find_cases(problem: str, input_dir: Path) -> list[Path]:
    numbered = sorted(input_dir.glob(f"{problem}_*.in"))
    if numbered:
        return numbered
    single = input_dir / f"{problem}.in"
    if single.is_file():
        return [single]
    return []


def run_case(
    python_exec: str,
    solution: Path,
    in_file: Path,
    exp_file: Path,
    out_file: Path,
    timeout_s: float,
) -> CaseResult:
    if not exp_file.is_file():
        return CaseResult(
            problem=solution.stem,
            case_name=in_file.name,
            ok=False,
            message=f"Missing expected file: {exp_file}",
        )

    case_stem = in_file.stem
    try:
        input_text = in_file.read_text(encoding="utf-8")
        expected_text = exp_file.read_text(encoding="utf-8")
    except OSError as exc:
        return CaseResult(
            problem=solution.stem,
            case_name=case_stem,
            ok=False,
            message=f"I/O error: {exc}",
        )

    try:
        proc = subprocess.run(
            [python_exec, str(solution)],
            input=input_text,
            capture_output=True,
            text=True,
            timeout=timeout_s,
            check=False,
        )
    except subprocess.TimeoutExpired:
        return CaseResult(
            problem=solution.stem,
            case_name=case_stem,
            ok=False,
            message=f"Timeout after {timeout_s:.2f}s",
        )

    out_file.parent.mkdir(parents=True, exist_ok=True)
    out_file.write_text(proc.stdout, encoding="utf-8")

    if proc.returncode != 0:
        stderr_preview = proc.stderr.strip().splitlines()[:4]
        preview = " | ".join(stderr_preview) if stderr_preview else "(no stderr)"
        return CaseResult(
            problem=solution.stem,
            case_name=case_stem,
            ok=False,
            message=f"Runtime error (code {proc.returncode}): {preview}",
        )

    got = normalize_output(proc.stdout)
    exp = normalize_output(expected_text)
    if got == exp:
        return CaseResult(problem=solution.stem, case_name=case_stem, ok=True)

    diff = difflib.unified_diff(
        exp.splitlines(),
        got.splitlines(),
        fromfile=f"expected/{exp_file.name}",
        tofile=f"output/{out_file.name}",
        lineterm="",
    )
    diff_preview = "\n".join(list(diff)[:12])
    return CaseResult(
        problem=solution.stem,
        case_name=case_stem,
        ok=False,
        message=f"Wrong answer\n{diff_preview}",
    )


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Run Python CP tests.")
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument("--problem", type=str, help="Problem id (e.g. problem_a)")
    mode.add_argument("--all", action="store_true", help="Test all src/*.py")
    parser.add_argument("--python", default="python3", help="Python executable")
    parser.add_argument("--timeout", type=float, default=2.0, help="Timeout seconds")
    parser.add_argument("--src-dir", default="src")
    parser.add_argument("--input-dir", default="input_cases")
    parser.add_argument("--expected-dir", default="expected_output")
    parser.add_argument("--output-dir", default="output_cases")
    return parser.parse_args()


def discover_problems(src_dir: Path, only_problem: str | None) -> list[str]:
    if only_problem is not None:
        return [only_problem]

    problems: list[str] = []
    for file_path in sorted(src_dir.glob("*.py")):
        if file_path.name.startswith("_"):
            continue
        problems.append(file_path.stem)
    return problems


def main() -> int:
    args = parse_args()
    src_dir = Path(args.src_dir)
    input_dir = Path(args.input_dir)
    expected_dir = Path(args.expected_dir)
    output_dir = Path(args.output_dir)

    problems = discover_problems(src_dir, args.problem if not args.all else None)
    if not problems:
        print("No problems found in src/.", file=sys.stderr)
        return 2

    total = 0
    passed = 0
    failed = 0

    for problem in problems:
        solution = src_dir / f"{problem}.py"
        if not solution.is_file():
            print(f"[ERR] {problem}: missing source file {solution}")
            failed += 1
            total += 1
            continue

        cases = find_cases(problem, input_dir)
        if not cases:
            print(f"[ERR] {problem}: no input cases in {input_dir}")
            failed += 1
            total += 1
            continue

        print(f"\n===----- {problem} -----===")
        for in_file in cases:
            exp_file = expected_dir / (in_file.stem + ".exp")
            out_file = output_dir / (in_file.stem + ".out")
            result = run_case(
                args.python,
                solution,
                in_file,
                exp_file,
                out_file,
                args.timeout,
            )
            total += 1
            if result.ok:
                passed += 1
                print(f"[OK ] {result.case_name}")
            else:
                failed += 1
                print(f"[ERR] {result.case_name}: {result.message}")

    print(f"\nSummary: {passed}/{total} passed, {failed} failed.")
    return 0 if failed == 0 else 1


if __name__ == "__main__":
    raise SystemExit(main())
