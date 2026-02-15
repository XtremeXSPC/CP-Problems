#!/usr/bin/env python3
"""
Git-backed template version manager.

This replaces the legacy custom snapshot store (.template_versions) with native
git operations scoped to the templates directory.
"""

from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path
from typing import Sequence


class VersionManagerError(RuntimeError):
    """Raised when a git-backed version operation cannot be completed."""


class GitTemplateVersionManager:
    def __init__(self, templates_dir: Path, dry_run: bool = False):
        self.templates_dir = templates_dir.resolve()
        self.dry_run = dry_run
        self.repo_root = self._discover_repo_root()
        self.template_rel = self._relative_template_path()

    def _run_git(
        self,
        args: Sequence[str],
        *,
        check: bool = True,
        capture: bool = True,
        text: bool = True,
        mutates: bool = False,
    ) -> subprocess.CompletedProcess:
        cmd = ["git", "-C", str(self.repo_root), *args]
        if self.dry_run and mutates:
            print(f"[dry-run] {' '.join(cmd)}")
            return subprocess.CompletedProcess(cmd, 0, "", "")
        result = subprocess.run(
            cmd,
            check=False,
            capture_output=capture,
            text=text,
        )
        if check and result.returncode != 0:
            stderr = (result.stderr or "").strip()
            raise VersionManagerError(stderr or f"git command failed: {' '.join(cmd)}")
        return result

    def _discover_repo_root(self) -> Path:
        result = subprocess.run(
            ["git", "-C", str(self.templates_dir), "rev-parse", "--show-toplevel"],
            check=False,
            capture_output=True,
            text=True,
        )
        if result.returncode != 0:
            raise VersionManagerError(
                f"templates directory is not inside a git repository: {self.templates_dir}"
            )
        return Path(result.stdout.strip()).resolve()

    def _relative_template_path(self) -> str:
        try:
            rel = self.templates_dir.relative_to(self.repo_root)
        except ValueError as exc:
            raise VersionManagerError(
                f"templates directory '{self.templates_dir}' is outside repository root '{self.repo_root}'"
            ) from exc
        return rel.as_posix()

    def has_changes(self) -> bool:
        result = self._run_git(
            ["status", "--porcelain", "--", self.template_rel], check=False
        )
        return bool((result.stdout or "").strip())

    def create_version(self, message: str, tag: str | None = None) -> str:
        if not self.has_changes():
            print("No template changes detected.")
            return ""

        self._run_git(["add", "--", self.template_rel], mutates=True)
        self._run_git(
            ["commit", "-m", message, "--", self.template_rel], mutates=True
        )
        commit = (
            self._run_git(["rev-parse", "HEAD"]).stdout.strip()
            if not self.dry_run
            else "<dry-run>"
        )

        if tag:
            self._run_git(
                ["tag", "-a", tag, "-m", f"Template snapshot: {tag}", commit],
                mutates=True,
            )
            print(f"Created version commit {commit} with tag '{tag}'")
        else:
            print(f"Created version commit {commit}")
        return commit

    def list_versions(self, detailed: bool = False) -> None:
        fmt = "%h\t%ad\t%s"
        if detailed:
            fmt = "%H\t%ad\t%an\t%s"
        result = self._run_git(
            [
                "log",
                "--date=iso",
                f"--pretty=format:{fmt}",
                "--",
                self.template_rel,
            ]
        )
        output = (result.stdout or "").strip()
        if not output:
            print("No template history found.")
            return
        print(output)

    def checkout_version(self, ref: str) -> None:
        try:
            self._run_git(
                [
                    "restore",
                    "--source",
                    ref,
                    "--staged",
                    "--worktree",
                    "--",
                    self.template_rel,
                ],
                mutates=True,
            )
        except VersionManagerError:
            # Fallback for older git versions.
            self._run_git(["checkout", ref, "--", self.template_rel], mutates=True)
        print(f"Restored templates from '{ref}'.")

    def diff_versions(self, ref1: str, ref2: str | None = None) -> None:
        if ref2:
            args = ["diff", f"{ref1}..{ref2}", "--", self.template_rel]
        else:
            args = ["diff", ref1, "--", self.template_rel]
        result = self._run_git(args)
        if result.stdout:
            print(result.stdout, end="")

    def export_version(self, ref: str, output_path: Path) -> None:
        output_path = output_path.resolve()
        if output_path.suffix != ".gz":
            print(
                "Warning: output path does not end with .gz; archive is still written as tar.gz"
            )
        self._run_git(
            [
                "archive",
                "--format=tar.gz",
                f"--output={output_path}",
                ref,
                self.template_rel,
            ],
            mutates=True,
        )
        print(f"Exported '{ref}' to {output_path}")

    def auto_version(self, message: str = "Auto-save: template changes") -> bool:
        if not self.has_changes():
            print("No template changes detected.")
            return False
        self.create_version(message=message, tag=None)
        return True


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Git-backed template version manager"
    )
    parser.add_argument(
        "--dir",
        type=Path,
        default=None,
        help="templates directory (default: ./templates)",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="print git commands without modifying repository state",
    )

    subparsers = parser.add_subparsers(dest="command", required=True)

    create_parser = subparsers.add_parser("create", help="create a template version commit")
    create_parser.add_argument("message", help="commit message")
    create_parser.add_argument("--tag", help="optional git tag")

    list_parser = subparsers.add_parser("list", help="list template version history")
    list_parser.add_argument("--detailed", action="store_true", help="show full hash and author")

    checkout_parser = subparsers.add_parser("checkout", help="restore templates from git ref")
    checkout_parser.add_argument("version", help="git ref (commit/tag/branch)")

    diff_parser = subparsers.add_parser("diff", help="diff template versions")
    diff_parser.add_argument("version1", help="first git ref")
    diff_parser.add_argument("version2", nargs="?", help="second git ref (optional)")

    export_parser = subparsers.add_parser("export", help="export templates subtree archive from ref")
    export_parser.add_argument("version", help="git ref")
    export_parser.add_argument("output", type=Path, help="output tar.gz path")

    auto_parser = subparsers.add_parser("auto", help="auto-create version commit if templates changed")
    auto_parser.add_argument(
        "--message",
        default="Auto-save: template changes",
        help="commit message for auto mode",
    )

    return parser


def main() -> int:
    parser = build_parser()
    args = parser.parse_args()

    templates_dir = args.dir.resolve() if args.dir else (Path.cwd() / "templates").resolve()
    if not templates_dir.exists():
        print(f"Templates directory not found: {templates_dir}")
        return 1

    try:
        manager = GitTemplateVersionManager(templates_dir=templates_dir, dry_run=args.dry_run)

        if args.command == "create":
            manager.create_version(args.message, args.tag)
        elif args.command == "list":
            manager.list_versions(args.detailed)
        elif args.command == "checkout":
            manager.checkout_version(args.version)
        elif args.command == "diff":
            manager.diff_versions(args.version1, args.version2)
        elif args.command == "export":
            manager.export_version(args.version, args.output)
        elif args.command == "auto":
            manager.auto_version(message=args.message)
        else:
            parser.print_help()
            return 1
    except VersionManagerError as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())
