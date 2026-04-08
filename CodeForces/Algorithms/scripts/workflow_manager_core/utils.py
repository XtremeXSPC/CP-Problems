"""Validation and discovery helpers for workflow manager CLI."""

import argparse
import os
from collections.abc import Sequence
from pathlib import Path

from .constants import (
    CONTEST_SEGMENT_RE,
    DEFAULT_CP_TOOLS_SCRIPT,
    TARGET_RE,
)
from .types import WorkflowError

SOURCE_SUFFIXES = (".cpp", ".cc", ".cxx")


def is_under(path: Path, root: Path) -> bool:
    """Return True when `path` resolves under `root`."""

    try:
        path.resolve().relative_to(root.resolve())
        return True
    except ValueError:
        return False


def normalize_target(raw: str) -> str:
    """Normalize and validate a target/problem identifier."""

    candidate = Path(raw).name
    suffix = Path(candidate).suffix.lower()
    if suffix in {".cpp", ".cc", ".cxx"}:
        candidate = Path(candidate).stem
    if not TARGET_RE.fullmatch(candidate):
        raise argparse.ArgumentTypeError(
            "invalid target name; expected [A-Za-z][A-Za-z0-9_]* "
            "(optionally with .cpp/.cc/.cxx)"
        )
    return candidate


def parse_positive_int(raw: str) -> int:
    """Parse a strictly positive integer for CLI arguments."""

    try:
        value = int(raw)
    except ValueError as exc:
        raise argparse.ArgumentTypeError("expected a positive integer") from exc
    if value <= 0:
        raise argparse.ArgumentTypeError("expected a positive integer")
    return value


def find_existing_target_source(cwd: Path, raw: str) -> Path | None:
    """Return the first existing source file for `raw` under `cwd`, if any."""

    target = normalize_target(raw)
    for suffix in SOURCE_SUFFIXES:
        candidate = cwd / f"{target}{suffix}"
        if candidate.is_file():
            return candidate
    return None


def normalize_input_name(raw: str) -> str:
    """Normalize an input filename while forbidding empty values."""

    name = Path(raw).name
    if not name:
        raise argparse.ArgumentTypeError("input filename cannot be empty")
    return name


def normalize_contest_dir(raw: str) -> str:
    """Validate a workspace-relative contest directory path."""

    p = Path(raw)
    if p.is_absolute():
        raise argparse.ArgumentTypeError("contest path must be workspace-relative")
    if ".." in p.parts:
        raise argparse.ArgumentTypeError("contest path cannot contain '..'")
    if not p.parts:
        raise argparse.ArgumentTypeError("contest path cannot be empty")
    for seg in p.parts:
        if seg in {".", ""}:
            raise argparse.ArgumentTypeError("contest path contains invalid segment")
        if not CONTEST_SEGMENT_RE.fullmatch(seg):
            raise argparse.ArgumentTypeError(
                f"invalid contest path segment '{seg}' "
                "(allowed: letters, digits, '.', '_' and '-')"
            )
    return str(p)


def _unique_candidates(paths: Sequence[Path]) -> list[Path]:
    """Deduplicate candidate paths while preserving their original order."""

    unique: list[Path] = []
    seen: set[str] = set()
    for candidate in paths:
        key = str(candidate)
        if key in seen:
            continue
        seen.add(key)
        unique.append(candidate)
    return unique


def discover_cp_tools_script(explicit: Path | None) -> Path:
    """Locate `competitive.sh` from CLI args, env, or common defaults."""

    candidates: list[Path] = []
    if explicit is not None:
        candidates.append(explicit.expanduser())

    env_script = os.environ.get("CPP_TOOLS_SCRIPT")
    if env_script:
        candidates.append(Path(env_script).expanduser())

    candidates.append(DEFAULT_CP_TOOLS_SCRIPT)
    candidates.append(Path.home() / ".config/cpp-tools/competitive.sh")

    unique_candidates = _unique_candidates(candidates)
    for candidate in unique_candidates:
        if candidate.is_file():
            return candidate

    searched = "\n  - ".join(str(c) for c in unique_candidates)
    raise WorkflowError(
        "unable to locate cpp-tools entry script `competitive.sh`.\n"
        f"Searched:\n  - {searched}\n"
        "Use --cp-tools-script to specify it explicitly."
    )


def discover_algorithms_dir(explicit: Path | None) -> Path:
    """Locate the centralized Algorithms directory used by preset workflows."""

    candidates: list[Path] = []
    if explicit is not None:
        candidates.append(explicit.expanduser())

    env_algorithms = os.environ.get("CP_ALGORITHMS_DIR")
    if env_algorithms:
        candidates.append(Path(env_algorithms).expanduser())

    candidates.append(Path(__file__).resolve().parents[2])

    unique_candidates = _unique_candidates(candidates)
    for candidate in unique_candidates:
        if candidate.is_dir() and (candidate / "gcc-toolchain.cmake").is_file():
            return candidate.resolve()

    searched = "\n  - ".join(str(c) for c in unique_candidates)
    raise WorkflowError(
        "unable to locate centralized Algorithms directory with toolchains.\n"
        f"Searched:\n  - {searched}\n"
        "Use --algorithms-dir to specify it explicitly."
    )
