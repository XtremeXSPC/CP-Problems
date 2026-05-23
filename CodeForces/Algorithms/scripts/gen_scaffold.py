#!/usr/bin/env python3
"""Generate templates/cpp/<scaffold>.cpp from profiles.toml."""

from __future__ import annotations

import sys
from pathlib import Path

from profile_registry import (
    DEFAULT_PROFILES_PATH,
    TEMPLATES_DIR,
    ProfileRegistry,
    ScaffoldProfile,
    load_registry,
    reset_cache,
)

SCAFFOLD_DIR = TEMPLATES_DIR / "cpp"

HEADER_DOC_BLOCK = """\
//===----------------------------------------------------------------------===//
/**
 * @file: __FILE_NAME__
 * @brief __PROBLEM_BRIEF__
 * @author: C.L.
 *
 * @status: In Progress
 */
//===----------------------------------------------------------------------===//

"""


def _render_scaffold(profile: ScaffoldProfile) -> str:
    lines: list[str] = []
    if profile.header_doc:
        lines.append(HEADER_DOC_BLOCK.rstrip("\n"))
        lines.append("")

    lines += [
        "#ifndef CP_TEMPLATE_PROFILE_STRICT",
        "  #define CP_TEMPLATE_PROFILE_STRICT",
        "#endif",
        "#ifndef CP_USE_GLOBAL_STD_NAMESPACE",
        "  #define CP_USE_GLOBAL_STD_NAMESPACE 1",
        "#endif",
        "",
    ]
    if profile.advanced:
        lines += [
            "#ifndef CP_USE_ADVANCED",
            "  #define CP_USE_ADVANCED",
            "#endif",
            "",
        ]
    for need in sorted(profile.needs):
        lines.append(f"#define {need}")
    lines.append(f"#define CP_IO_PROFILE_{profile.io_profile.upper()}")
    lines += [
        "",
        '#include "templates/Base.hpp"',
        "",
        "//===----------------------------------------------------------------------===//",
        "/* Main Solver Function */",
        "",
        "void solve() {",
        "  // Optimized solution here",
        "}",
        "",
        "//===----------------------------------------------------------------------===//",
        "/* Main Function */",
        "",
        "auto main() -> int {",
        "#ifdef LOCAL",
        "  Stopwatch timer;",
        "#endif",
        "",
        "  INT(T);",
        "  FOR(T) solve();",
        "",
        "  return 0;",
        "}",
        "",
        "//===----------------------------------------------------------------------===//",
        "",
    ]
    return "\n".join(lines)


def _write_if_changed(path: Path, content: str) -> bool:
    if not content.endswith("\n"):
        content += "\n"
    if path.is_file() and path.read_text(encoding="utf-8") == content:
        return False
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content, encoding="utf-8")
    return True


def main() -> int:
    reset_cache()
    registry: ProfileRegistry = load_registry(str(DEFAULT_PROFILES_PATH))
    changed = False
    for name, profile in registry.scaffolds.items():
        if _write_if_changed(SCAFFOLD_DIR / f"{name}.cpp", _render_scaffold(profile)):
            print(f"wrote {name}.cpp")
            changed = True
    if not changed:
        print("up-to-date")
    return 0


if __name__ == "__main__":
    sys.exit(main())
