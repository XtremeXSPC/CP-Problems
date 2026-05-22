#!/usr/bin/env python3
"""Generate Config_defaults.hpp and Base_profiles.hpp from profiles.toml."""

from __future__ import annotations

import sys
from pathlib import Path

from profile_registry import (
    DEFAULT_PROFILES_PATH,
    TEMPLATES_DIR,
    ProfileRegistry,
    load_registry,
    reset_cache,
)

CONFIG_OUTPUT = TEMPLATES_DIR / "Config_defaults.hpp"
BASE_PROFILES_OUTPUT = TEMPLATES_DIR / "Base_profiles.hpp"

AUTOGEN_HEADER = "// Generated from profiles.toml. Do not edit by hand.\n"


def _render_config_defaults(registry: ProfileRegistry) -> str:
    lines = ["#pragma once", "", AUTOGEN_HEADER.rstrip("\n"), ""]
    lines.append("#if defined(CP_TEMPLATE_PROFILE_STRICT)")
    for macro, value in registry.defaults.strict_overrides.items():
        lines += [f"  #ifndef {macro}", f"    #define {macro} {value}", f"  #endif"]
    lines.append("#endif")
    lines.append("")
    for macro, value in registry.defaults.base.items():
        lines += [f"#ifndef {macro}", f"  #define {macro} {value}", f"#endif"]
    lines += [
        "",
        "#ifndef CP_IO_ENABLE_COMPOSITE",
        "  #if CP_ENABLE_LEGACY_IO_VEC_MACROS",
        "    #define CP_IO_ENABLE_COMPOSITE 1",
        "  #else",
        "    #define CP_IO_ENABLE_COMPOSITE 0",
        "  #endif",
        "#endif",
        "",
    ]
    return "\n".join(lines)


def _render_base_profiles(registry: ProfileRegistry) -> str:
    profile_macros = sorted(f"CP_IO_PROFILE_{n.upper()}" for n in registry.io_profiles)
    lines = [
        "#pragma once",
        "",
        AUTOGEN_HEADER.rstrip("\n"),
        "",
        "#if (" + " + ".join(f"defined({m})" for m in profile_macros) + ") > 1",
        '  #error "Choose at most one CP_IO_PROFILE_* macro."',
        "#endif",
        "",
    ]
    for name, profile in registry.io_profiles.items():
        macro = f"CP_IO_PROFILE_{name.upper()}"
        lines.append(f"#ifdef {macro}")
        for need in sorted(profile.needs):
            lines += [f"  #ifndef {need}", f"    #define {need}", f"  #endif"]
        for define_name, define_value in profile.defines.items():
            lines += [
                f"  #ifndef {define_name}",
                f"    #define {define_name} {define_value}",
                f"  #endif",
            ]
        lines += ["#endif", ""]

    # NEED_IO loses to NEED_FAST_IO so IN/OUT bind to the fast backend.
    lines += [
        "#if defined(NEED_IO) && defined(NEED_FAST_IO)",
        "  #undef NEED_IO",
        "#endif",
        "",
    ]
    return "\n".join(lines)


def _write_if_changed(path: Path, content: str) -> bool:
    if not content.endswith("\n"):
        content += "\n"
    if path.is_file() and path.read_text(encoding="utf-8") == content:
        return False
    path.write_text(content, encoding="utf-8")
    return True


def main() -> int:
    reset_cache()
    registry = load_registry(str(DEFAULT_PROFILES_PATH))
    changed = False
    if _write_if_changed(CONFIG_OUTPUT, _render_config_defaults(registry)):
        print(f"wrote {CONFIG_OUTPUT}")
        changed = True
    if _write_if_changed(BASE_PROFILES_OUTPUT, _render_base_profiles(registry)):
        print(f"wrote {BASE_PROFILES_OUTPUT}")
        changed = True
    if not changed:
        print("up-to-date")
    return 0


if __name__ == "__main__":
    sys.exit(main())
