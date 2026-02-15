#!/usr/bin/env python3
"""
Remove round-local toolchain files so rounds use only centralized toolchains.

This script is intentionally destructive for local `gcc-toolchain.cmake` and
`clang-toolchain.cmake` files found in round directories (optionally with
timestamped backups).
"""

from __future__ import annotations

import argparse
import datetime as dt
from pathlib import Path

DEFAULT_ROUNDS_ROOT = Path("/Volumes/LCS.Data/CP-Problems/CodeForces/Rounds")
DEFAULT_CENTRAL_ROOT = Path("/Volumes/LCS.Data/CP-Problems/CodeForces/Algorithms")
TOOLCHAIN_FILES = ("gcc-toolchain.cmake", "clang-toolchain.cmake")


def _backup_name(path: Path) -> Path:
    stamp = dt.datetime.now().strftime("%Y%m%d-%H%M%S")
    return path.with_name(f"{path.name}.bak-centralized-{stamp}")


def _safe_resolve(path: Path) -> Path:
    try:
        return path.resolve(strict=True)
    except FileNotFoundError:
        return path


def _is_linked_to(path: Path, target: Path) -> bool:
    return path.is_symlink() and _safe_resolve(path) == _safe_resolve(target)


def main() -> int:
    parser = argparse.ArgumentParser(
        description=(
            "Remove per-round toolchain files to enforce centralized toolchain usage."
        )
    )
    parser.add_argument("--rounds-root", type=Path, default=DEFAULT_ROUNDS_ROOT)
    parser.add_argument("--central-root", type=Path, default=DEFAULT_CENTRAL_ROOT)
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument(
        "--backup",
        action=argparse.BooleanOptionalAction,
        default=True,
        help="Backup pre-existing round toolchain files before removal (default: true).",
    )
    parser.add_argument(
        "--force",
        action="store_true",
        help="Also remove non-centralized symlinks/files.",
    )
    args = parser.parse_args()

    rounds_root = args.rounds_root.resolve()
    central_root = args.central_root.resolve()

    if not rounds_root.is_dir():
        raise FileNotFoundError(f"Rounds root not found: {rounds_root}")
    if not central_root.is_dir():
        raise FileNotFoundError(f"Central root not found: {central_root}")

    central_toolchains = {name: central_root / name for name in TOOLCHAIN_FILES}
    for name, path in central_toolchains.items():
        if not path.is_file():
            raise FileNotFoundError(f"Central toolchain missing: {path} ({name})")

    removed = 0
    skipped = 0

    for round_dir in sorted(p for p in rounds_root.iterdir() if p.is_dir()):
        cmake_file = round_dir / "CMakeLists.txt"
        if not cmake_file.is_file():
            continue

        for name in TOOLCHAIN_FILES:
            local_path = round_dir / name
            if not (local_path.exists() or local_path.is_symlink()):
                skipped += 1
                continue

            if local_path.is_symlink() and (not _is_linked_to(local_path, central_toolchains[name])) and not args.force:
                print(
                    f"warn: {local_path} is a symlink to a non-central target; "
                    "use --force to remove it"
                )
                skipped += 1
                continue

            if args.dry_run:
                print(f"would-remove: {local_path}")
            else:
                if args.backup:
                    backup_path = _backup_name(local_path)
                    local_path.rename(backup_path)
                    print(f"backup: {local_path} -> {backup_path}")
                else:
                    local_path.unlink()
                    print(f"removed: {local_path}")
            removed += 1

    mode = "dry-run" if args.dry_run else "write"
    print(f"done ({mode}): removed={removed} skipped={skipped}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
