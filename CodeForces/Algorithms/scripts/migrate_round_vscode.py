#!/usr/bin/env python3
"""
Centralize round-level VS Code files by linking them to Algorithms/.vscode.
"""

from __future__ import annotations

import argparse
import datetime as dt
import os
from pathlib import Path
from typing import Iterable, List

DEFAULT_ROUNDS_ROOT = Path("/Volumes/LCS.Data/CP-Problems/CodeForces/Rounds")
DEFAULT_ALGORITHMS_ROOT = Path("/Volumes/LCS.Data/CP-Problems/CodeForces/Algorithms")
VSCODE_FILENAMES = (
    "settings.json",
    "tasks.json",
    "launch.json",
    "c_cpp_properties.json",
)


def _backup_name(path: Path) -> Path:
    """Build a timestamped backup filename next to the original file."""
    stamp = dt.datetime.now().strftime("%Y%m%d-%H%M%S")
    return path.with_name(f"{path.name}.bak-centralized-{stamp}")


def _iter_round_dirs(rounds_root: Path, explicit_rounds: List[str]) -> Iterable[Path]:
    """Yield round directories selected for VS Code migration."""
    if explicit_rounds:
        for name in explicit_rounds:
            round_dir = rounds_root / name
            if not round_dir.is_dir():
                raise FileNotFoundError(f"Round directory not found: {round_dir}")
            if not (round_dir / "CMakeLists.txt").is_file():
                raise FileNotFoundError(f"Round missing CMakeLists.txt: {round_dir}")
            yield round_dir
        return

    for round_dir in sorted(rounds_root.iterdir()):
        if not round_dir.is_dir():
            continue
        if (round_dir / "CMakeLists.txt").is_file():
            yield round_dir


def _safe_resolve(path: Path) -> Path:
    """Resolve a path when possible, preserving unresolved paths if missing."""
    try:
        return path.resolve(strict=True)
    except FileNotFoundError:
        return path


def _is_linked_to(path: Path, target: Path) -> bool:
    """Return True when `path` is a symlink pointing to `target`."""
    return path.is_symlink() and _safe_resolve(path) == _safe_resolve(target)


def _replace_with_symlink(
    local_file: Path,
    central_file: Path,
    dry_run: bool,
    backup: bool,
) -> str:
    """Replace local file with a relative symlink to the central file."""
    if _is_linked_to(local_file, central_file):
        return "already-linked"

    if dry_run:
        return "would-link"

    local_file.parent.mkdir(parents=True, exist_ok=True)
    if local_file.exists() or local_file.is_symlink():
        if backup:
            backup_path = _backup_name(local_file)
            local_file.rename(backup_path)
        else:
            local_file.unlink()

    relative_target = os.path.relpath(
        str(central_file), start=str(local_file.parent.resolve())
    )
    local_file.symlink_to(relative_target)
    return "linked"


def main() -> int:
    """Migrate round VS Code files to centralized symlinks."""
    parser = argparse.ArgumentParser(
        description=(
            "Replace round .vscode JSON files with symlinks to Algorithms/.vscode."
        )
    )
    parser.add_argument("--rounds-root", type=Path, default=DEFAULT_ROUNDS_ROOT)
    parser.add_argument("--algorithms-root", type=Path, default=DEFAULT_ALGORITHMS_ROOT)
    parser.add_argument(
        "--round",
        action="append",
        default=[],
        help="Specific round directory name to migrate (repeatable).",
    )
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument(
        "--backup",
        action=argparse.BooleanOptionalAction,
        default=True,
        help="Backup existing round VS Code files before replacing (default: true).",
    )
    args = parser.parse_args()

    rounds_root = args.rounds_root.resolve()
    algorithms_root = args.algorithms_root.resolve()
    central_vscode_dir = algorithms_root / ".vscode"

    if not rounds_root.is_dir():
        raise FileNotFoundError(f"Rounds root not found: {rounds_root}")
    if not central_vscode_dir.is_dir():
        raise FileNotFoundError(f"Central VS Code directory not found: {central_vscode_dir}")

    central_files = {}
    for name in VSCODE_FILENAMES:
        path = central_vscode_dir / name
        if not path.is_file():
            raise FileNotFoundError(f"Central VS Code file missing: {path}")
        central_files[name] = path

    linked = 0
    skipped = 0
    for round_dir in _iter_round_dirs(rounds_root, args.round):
        local_vscode_dir = round_dir / ".vscode"
        for name, central_file in central_files.items():
            local_file = local_vscode_dir / name
            state = _replace_with_symlink(
                local_file=local_file,
                central_file=central_file,
                dry_run=args.dry_run,
                backup=args.backup,
            )
            if state == "already-linked":
                skipped += 1
                continue

            linked += 1
            action = "would-link" if args.dry_run else "linked"
            print(f"{action}: {local_file} -> {central_file}")

    mode = "dry-run" if args.dry_run else "write"
    print(f"done ({mode}): linked={linked} skipped={skipped}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
