#!/usr/bin/env python3
"""Syntax-check each modified C++ module via -fsyntax-only.

Generates a probe.cpp per file that #includes templates/Base.hpp + the file,
then runs g++ -fsyntax-only. No linking, no execution.

Usage:
  python3 scripts/syntax_check.py FILE [FILE...]
  python3 scripts/syntax_check.py --git-modified
"""

from __future__ import annotations
import argparse
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
COMPILER = "g++-15"
CXX_FLAGS = ["-std=c++23", "-fsyntax-only", "-Wno-unused-variable", "-Wno-unused-parameter"]
INCLUDE_DIR = str(ROOT)

PROBE_TEMPLATE = """\
#define CP_USE_ADVANCED
#define NEED_CORE
#define NEED_MATH
#define NEED_HASHING
#define NEED_CONTAINERS
#define NEED_BIT_OPS
#define NEED_MOD_INT
#define NEED_IO
#include "templates/Base.hpp"
#include "{include_path}"
int main() {{ return 0; }}
"""


def probe_for(file: Path) -> str:
  """Generate probe.cpp source for syntax-checking the given file."""

    rel = file.resolve().relative_to(ROOT)
    return PROBE_TEMPLATE.format(include_path=rel.as_posix())


def check(file: Path) -> tuple[bool, str]:
"""Check syntax of the given file. Returns (ok, error_message)."""

    source = probe_for(file)
    with tempfile.NamedTemporaryFile(suffix=".cpp", mode="w", delete=False) as tf:
        tf.write(source)
        probe = tf.name
    try:
        cp = subprocess.run(
            [COMPILER, *CXX_FLAGS, f"-I{INCLUDE_DIR}", probe],
            capture_output=True, text=True, timeout=120,
        )
    except subprocess.TimeoutExpired:
        return False, "TIMEOUT"
    finally:
        Path(probe).unlink(missing_ok=True)
    if cp.returncode == 0:
        return True, ""
    return False, cp.stderr.strip()


def git_modified() -> list[Path]:
  """Return list of .hpp files modified in git (compared to HEAD)."""

    cp = subprocess.run(
        ["git", "diff", "--name-only", "HEAD", "--", "*.hpp"],
        capture_output=True, text=True, check=True,
        cwd=ROOT.parent.parent,
    )
    paths = []
    for line in cp.stdout.splitlines():
        p = (ROOT.parent.parent / line).resolve()
        if p.exists() and p.suffix == ".hpp":
            paths.append(p)
    return paths


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("files", nargs="*", type=Path)
    ap.add_argument("--git-modified", action="store_true")
    ap.add_argument("--quiet-pass", action="store_true", help="only print failures")
    args = ap.parse_args()

    files = list(args.files)
    if args.git_modified:
        files += git_modified()
    files = sorted(set(f.resolve() for f in files))

    if not files:
        print("no files to check", file=sys.stderr)
        return 1

    passed = failed = 0
    for f in files:
        ok, err = check(f)
        rel = f.relative_to(ROOT) if str(f).startswith(str(ROOT)) else f
        if ok:
            if not args.quiet_pass:
                print(f"PASS  {rel}")
            passed += 1
        else:
            print(f"FAIL  {rel}")
            for line in err.splitlines()[:8]:
                print(f"      {line}")
            failed += 1

    print(f"\n{passed} passed, {failed} failed", file=sys.stderr)
    return 0 if failed == 0 else 2


if __name__ == "__main__":
    raise SystemExit(main())
