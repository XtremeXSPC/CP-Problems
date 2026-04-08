#!/usr/bin/env python3
"""
Centralize round-level VS Code entries by linking them to Algorithms/.vscode.
"""

from __future__ import annotations

import argparse
import os
import shutil
from pathlib import Path

from migration_common import (
    is_linked_to,
    iter_round_dirs,
    timestamped_backup_name,
)

DEFAULT_ALGORITHMS_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_ROUNDS_ROOT = DEFAULT_ALGORITHMS_ROOT.parent / "Rounds"
VSCODE_ENTRIES = (
    "settings.json",
    "tasks.json",
    "launch.json",
    "c_cpp_properties.json",
    "scripts",
)
def _remove_path(path: Path) -> None:
    """Remove one existing file/symlink/directory path."""

    if path.is_symlink() or path.is_file():
        path.unlink()
        return
    if path.is_dir():
        shutil.rmtree(path)
        return
    raise FileNotFoundError(path)


def _replace_with_symlink(
    local_path: Path,
    central_path: Path,
    dry_run: bool,
    backup: bool,
) -> str:
    """Replace a local VS Code entry with a relative symlink to the central one."""

    if is_linked_to(local_path, central_path):
        return "already-linked"

    if dry_run:
        return "would-link"

    local_path.parent.mkdir(parents=True, exist_ok=True)
    if local_path.exists() or local_path.is_symlink():
        if backup:
            backup_path = timestamped_backup_name(local_path)
            local_path.rename(backup_path)
        else:
            _remove_path(local_path)

    relative_target = os.path.relpath(
        str(central_path), start=str(local_path.parent.resolve())
    )
    local_path.symlink_to(relative_target, target_is_directory=central_path.is_dir())
    return "linked"


def main() -> int:
    """Migrate round VS Code entries to centralized symlinks."""

    parser = argparse.ArgumentParser(
        description=(
            "Replace round .vscode entries with symlinks to Algorithms/.vscode."
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

    central_entries = {}
    for name in VSCODE_ENTRIES:
        path = central_vscode_dir / name
        if not (path.is_file() or path.is_dir()):
            raise FileNotFoundError(f"Central VS Code entry missing: {path}")
        central_entries[name] = path

    linked = 0
    skipped = 0
    for round_dir in iter_round_dirs(rounds_root, args.round):
        local_vscode_dir = round_dir / ".vscode"
        if local_vscode_dir.is_symlink():
            if is_linked_to(local_vscode_dir, central_vscode_dir):
                skipped += len(VSCODE_ENTRIES)
                print(f"already-linked-dir: {local_vscode_dir} -> {central_vscode_dir}")
            else:
                skipped += len(VSCODE_ENTRIES)
                print(
                    f"warn: {local_vscode_dir} is a symlink to a non-central target; "
                    "skipping to avoid mutating another directory tree"
                )
            continue

        for name, central_path in central_entries.items():
            local_path = local_vscode_dir / name
            state = _replace_with_symlink(
                local_path=local_path,
                central_path=central_path,
                dry_run=args.dry_run,
                backup=args.backup,
            )
            if state == "already-linked":
                skipped += 1
                continue

            linked += 1
            action = "would-link" if args.dry_run else "linked"
            print(f"{action}: {local_path} -> {central_path}")

    mode = "dry-run" if args.dry_run else "write"
    print(f"done ({mode}): linked={linked} skipped={skipped}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
