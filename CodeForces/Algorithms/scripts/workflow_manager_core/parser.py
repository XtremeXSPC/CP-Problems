"""Argparse construction for workflow manager CLI."""

import argparse
from pathlib import Path

from .commands import register_subcommands


def build_parser() -> argparse.ArgumentParser:
    """Build and return the workflow manager CLI parser."""

    parser = argparse.ArgumentParser(
        description=(
            "Competitive Programming workflow manager built on top of cpp-tools."
        )
    )
    parser.add_argument(
        "--cwd",
        type=Path,
        default=Path.cwd(),
        help="working directory where commands are executed",
    )
    parser.add_argument(
        "--cp-tools-script",
        type=Path,
        default=None,
        help="path to competitive.sh (cpp-tools entry script)",
    )
    parser.add_argument(
        "--timeout",
        type=int,
        default=600,
        help="timeout in seconds for each delegated command",
    )
    parser.add_argument(
        "--json",
        action="store_true",
        help="emit structured JSON output instead of plain text",
    )
    parser.add_argument(
        "--verbose",
        action="store_true",
        help="print delegated command names before execution",
    )
    parser.add_argument(
        "--show-load-banner",
        action="store_true",
        help="do not suppress cpp-tools load banner",
    )
    parser.add_argument(
        "--continue-on-error",
        action="store_true",
        help="continue executing subsequent steps even if one step fails",
    )

    subparsers = parser.add_subparsers(dest="command", required=True)
    register_subcommands(subparsers)
    return parser
