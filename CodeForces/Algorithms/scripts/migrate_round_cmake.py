#!/usr/bin/env python3
"""
Migrate round CMakeLists.txt files to the centralized thin wrapper format.
"""

from __future__ import annotations

import argparse
from pathlib import Path

from migration_common import iter_round_files, timestamped_backup_name

DEFAULT_ALGORITHMS_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_ROUNDS_ROOT = DEFAULT_ALGORITHMS_ROOT.parent / "Rounds"

THIN_CMAKELISTS = """# Thin round-level CMakeLists: delegates all build logic to centralized modules.
cmake_minimum_required(VERSION 3.20)
project(competitive_programming LANGUAGES CXX)

set(CP_ROUND_BOOTSTRAP "${CMAKE_CURRENT_SOURCE_DIR}/algorithms/cmake/CPRoundBootstrap.cmake")
if(NOT EXISTS "${CP_ROUND_BOOTSTRAP}")
  message(FATAL_ERROR
    "Central build bootstrap not found: ${CP_ROUND_BOOTSTRAP}\\n"
    "Run cppinit (or recreate algorithms/cmake symlink) to restore shared build modules.")
endif()

include("${CP_ROUND_BOOTSTRAP}")
"""

def main() -> int:
    """Rewrite round CMakeLists files to the centralized thin wrapper format."""

    parser = argparse.ArgumentParser(
        description="Convert round CMakeLists.txt files to centralized thin wrappers."
    )
    parser.add_argument(
        "--rounds-root",
        type=Path,
        default=DEFAULT_ROUNDS_ROOT,
        help="Path containing round directories (default: %(default)s)",
    )
    parser.add_argument(
        "--round",
        action="append",
        default=[],
        help="Specific round directory name to migrate (repeatable).",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Print files that would be migrated without writing changes.",
    )
    parser.add_argument(
        "--backup",
        action=argparse.BooleanOptionalAction,
        default=True,
        help="Backup existing CMakeLists.txt before rewriting (default: true).",
    )
    args = parser.parse_args()

    rounds_root = args.rounds_root.resolve()
    if not rounds_root.is_dir():
        raise FileNotFoundError(f"Rounds root not found: {rounds_root}")

    changed = 0
    skipped = 0
    for cmake_path in iter_round_files(rounds_root, "CMakeLists.txt", args.round):
        current = cmake_path.read_text(encoding="utf-8")
        if current == THIN_CMAKELISTS:
            skipped += 1
            continue

        if args.dry_run:
            print(f"would-migrate: {cmake_path}")
        else:
            if cmake_path.exists() and args.backup:
                backup_path = timestamped_backup_name(cmake_path)
                cmake_path.rename(backup_path)
                print(f"backup: {cmake_path} -> {backup_path}")
            cmake_path.write_text(THIN_CMAKELISTS, encoding="utf-8")
            print(f"migrated: {cmake_path}")
        changed += 1

    mode = "dry-run" if args.dry_run else "write"
    print(f"done ({mode}): changed={changed} skipped={skipped}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
