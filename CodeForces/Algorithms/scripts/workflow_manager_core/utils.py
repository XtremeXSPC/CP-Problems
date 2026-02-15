"""Validation and discovery helpers for workflow manager CLI."""

import argparse
import os
from pathlib import Path
from typing import List, Optional

from .constants import (
    CONTEST_SEGMENT_RE,
    DEFAULT_CP_TOOLS_SCRIPT,
    TARGET_RE,
)
from .types import WorkflowError


def is_under(path: Path, root: Path) -> bool:
    try:
        path.resolve().relative_to(root.resolve())
        return True
    except ValueError:
        return False


def normalize_target(raw: str) -> str:
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


def normalize_input_name(raw: str) -> str:
    name = Path(raw).name
    if not name:
        raise argparse.ArgumentTypeError("input filename cannot be empty")
    return name


def normalize_contest_dir(raw: str) -> str:
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


def discover_cp_tools_script(explicit: Optional[Path]) -> Path:
    candidates: List[Path] = []
    if explicit is not None:
        candidates.append(explicit.expanduser())

    env_script = os.environ.get("CPP_TOOLS_SCRIPT")
    if env_script:
        candidates.append(Path(env_script).expanduser())

    candidates.append(DEFAULT_CP_TOOLS_SCRIPT)
    candidates.append(Path.home() / ".config/cpp-tools/competitive.sh")

    seen = set()
    for candidate in candidates:
        key = str(candidate)
        if key in seen:
            continue
        seen.add(key)
        if candidate.is_file():
            return candidate

    searched = "\n  - ".join(str(c) for c in candidates)
    raise WorkflowError(
        "unable to locate cpp-tools entry script `competitive.sh`.\n"
        f"Searched:\n  - {searched}\n"
        "Use --cp-tools-script to specify it explicitly."
    )


def discover_algorithms_dir(explicit: Optional[Path]) -> Path:
    candidates: List[Path] = []
    if explicit is not None:
        candidates.append(explicit.expanduser())

    env_algorithms = os.environ.get("CP_ALGORITHMS_DIR")
    if env_algorithms:
        candidates.append(Path(env_algorithms).expanduser())

    candidates.append(Path(__file__).resolve().parents[2])

    seen = set()
    for candidate in candidates:
        key = str(candidate)
        if key in seen:
            continue
        seen.add(key)
        if candidate.is_dir() and (candidate / "gcc-toolchain.cmake").is_file():
            return candidate.resolve()

    searched = "\n  - ".join(str(c) for c in candidates)
    raise WorkflowError(
        "unable to locate centralized Algorithms directory with toolchains.\n"
        f"Searched:\n  - {searched}\n"
        "Use --algorithms-dir to specify it explicitly."
    )
