"""Shared helpers for round migration scripts."""

from __future__ import annotations

import datetime as dt
from collections.abc import Iterator, Sequence
from pathlib import Path


def timestamped_backup_name(path: Path) -> Path:
    """Build a timestamped backup filename next to the original path."""

    stamp = dt.datetime.now().strftime("%Y%m%d-%H%M%S")
    return path.with_name(f"{path.name}.bak-centralized-{stamp}")


def iter_round_dirs(rounds_root: Path, explicit_rounds: Sequence[str] = ()) -> Iterator[Path]:
    """Yield contest round directories, optionally filtered by explicit names."""

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
        if round_dir.is_dir() and (round_dir / "CMakeLists.txt").is_file():
            yield round_dir


def iter_round_files(
    rounds_root: Path,
    filename: str,
    explicit_rounds: Sequence[str] = (),
) -> Iterator[Path]:
    """Yield one file path per eligible round directory."""

    for round_dir in iter_round_dirs(rounds_root, explicit_rounds):
        yield round_dir / filename


def safe_resolve(path: Path) -> Path:
    """Resolve a path when possible, preserving unresolved paths if missing."""

    try:
        return path.resolve(strict=True)
    except FileNotFoundError:
        return path


def is_linked_to(path: Path, target: Path) -> bool:
    """Return True when `path` is a symlink pointing to `target`."""

    return path.is_symlink() and safe_resolve(path) == safe_resolve(target)
