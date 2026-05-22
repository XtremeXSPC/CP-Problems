#!/usr/bin/env python3
"""
Create thin round-level CMakePresets.json wrappers that include centralized
presets from CodeForces/Algorithms/cmake/CPRoundPresets.json.
"""

from __future__ import annotations

import argparse
from pathlib import Path

from migration_common import iter_round_dirs, timestamped_backup_name

DEFAULT_ALGORITHMS_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_ROUNDS_ROOT = DEFAULT_ALGORITHMS_ROOT.parent / "Rounds"
PRESETS_FILENAME = "CMakePresets.json"

THIN_PRESETS = """{
  "version": 6,
  "include": [
    "algorithms/cmake/CPRoundPresets.json"
  ]
}
"""

def main() -> int:
    """Rewrite round CMakePresets files as thin includes of central presets."""

    parser = argparse.ArgumentParser(
        description=(
            "Write thin round-level CMakePresets.json files that include the centralized presets."
        )
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
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument(
        "--backup",
        action=argparse.BooleanOptionalAction,
        default=True,
        help="Backup existing CMakePresets.json before rewriting (default: true).",
    )
    args = parser.parse_args()

    rounds_root = args.rounds_root.resolve()
    if not rounds_root.is_dir():
        raise FileNotFoundError(f"Rounds root not found: {rounds_root}")

    changed = 0
    skipped = 0
    for round_dir in iter_round_dirs(rounds_root, args.round):
        presets_path = round_dir / PRESETS_FILENAME
        current = presets_path.read_text(encoding="utf-8") if presets_path.is_file() else ""

        if current == THIN_PRESETS:
            skipped += 1
            continue

        if args.dry_run:
            print(f"would-write: {presets_path}")
        else:
            if presets_path.exists() and args.backup:
                backup_path = timestamped_backup_name(presets_path)
                presets_path.rename(backup_path)
                print(f"backup: {presets_path} -> {backup_path}")
            presets_path.write_text(THIN_PRESETS, encoding="utf-8")
            print(f"written: {presets_path}")
        changed += 1

    mode = "dry-run" if args.dry_run else "write"
    print(f"done ({mode}): changed={changed} skipped={skipped}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
