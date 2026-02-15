#!/usr/bin/env python3
"""
Create thin round-level CMakePresets.json wrappers that include centralized
presets from CodeForces/Algorithms/cmake/CPRoundPresets.json.
"""

from __future__ import annotations

import argparse
import datetime as dt
from pathlib import Path
from typing import Iterable, List

DEFAULT_ROUNDS_ROOT = Path("/Volumes/LCS.Data/CP-Problems/CodeForces/Rounds")
PRESETS_FILENAME = "CMakePresets.json"

THIN_PRESETS = """{
  "version": 6,
  "include": [
    "algorithms/cmake/CPRoundPresets.json"
  ]
}
"""


def _backup_name(path: Path) -> Path:
    stamp = dt.datetime.now().strftime("%Y%m%d-%H%M%S")
    return path.with_name(f"{path.name}.bak-centralized-{stamp}")


def _iter_round_dirs(rounds_root: Path, explicit_rounds: List[str]) -> Iterable[Path]:
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


def main() -> int:
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
    for round_dir in _iter_round_dirs(rounds_root, args.round):
        presets_path = round_dir / PRESETS_FILENAME
        current = presets_path.read_text(encoding="utf-8") if presets_path.is_file() else ""

        if current == THIN_PRESETS:
            skipped += 1
            continue

        if args.dry_run:
            print(f"would-write: {presets_path}")
        else:
            if presets_path.exists() and args.backup:
                backup_path = _backup_name(presets_path)
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
