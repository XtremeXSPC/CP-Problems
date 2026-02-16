#!/usr/bin/env python3

from __future__ import annotations

import os
import re
import sys
from pathlib import Path

from need_resolver import extract_need_macros_from_source, load_need_mapping

# ------------------------------ CONFIGURATION ------------------------------- #

PROJECT_INCLUDE_RE = re.compile(r'^\s*#include\s+"([^"]+)"\s*$')
INLINE_ROOT_PREFIXES = ("modules/", "templates/")
IDENTIFIER_RE = re.compile(r"\b[A-Za-z_]\w*\b")
STRING_LITERAL_RE = re.compile(r'"(?:\\.|[^"\\])*"')
CHAR_LITERAL_RE = re.compile(r"'(?:\\.|[^'\\])*'")
BLOCK_COMMENT_RE = re.compile(r"/\*.*?\*/", re.DOTALL)
LINE_COMMENT_RE = re.compile(r"//[^\n]*")

# Header file name constants
TYPES_HPP = "Types.hpp"
CONSTANTS_HPP = "Constants.hpp"
MACROS_HPP = "Macros.hpp"
MATH_HPP = "Math.hpp"
IO_HPP = "IO.hpp"
FAST_IO_HPP = "Fast_IO.hpp"
PBDS_HPP = "PBDS.hpp"
BIT_OPS_HPP = "Bit_Ops.hpp"
CONTAINERS_HPP = "Containers.hpp"
MOD_INT_HPP = "Mod_Int.hpp"

# Keep this token map compact: black tends to explode huge literal sets vertically.
# fmt: off
OPTIONAL_HEADER_TRIGGER_TOKENS = {
    TYPES_HPP: {
        "I8", "I16", "I32", "I64", "U8", "U16", "U32", "U64", "I128", "U128",
        "F32", "F64", "F80", "F128", "ll", "ull", "ld", "Vec", "Vec2", "Vec3",
        "Deque", "List", "Set", "MultiSet", "UnorderedSet", "Map", "MultiMap",
        "UnorderedMap", "Stack", "Queue", "PriorityQueue", "MinPriorityQueue",
        "VC", "VVC", "VVVC", "Pair", "P", "PII", "PLL", "PLD", "VI", "VLL",
        "VVI", "VVLL", "VB", "VS", "VU8", "VU32", "VU64", "VF", "VPII", "VPLL",
        "VP", "ordered_set", "ordered_multiset", "ordered_map", "gp_hash_table",
        "HAS_INT128", "HAS_FLOAT128",
        "PBDS_AVAILABLE",
    },
    CONSTANTS_HPP: {
        "PI", "E", "PHI", "LN2", "EPS", "DEPS", "infinity", "INF32", "INF64",
        "LINF", "MOD", "MOD2",
        "MOD3",
    },
    MACROS_HPP: {
        "make_nd_vec", "make_vec2", "make_vec3", "make_vec4", "make_vec", "vv",
        "vvv", "vvvv", "FOR", "FOR_R", "REP", "RREP", "ALL", "RALL", "all",
        "rall", "sz", "len", "pb", "eb", "mp", "mt", "fi", "se", "elif",
        "UNIQUE", "LB", "UB", "SUM", "MIN",
        "MAX",
    },
    MATH_HPP: {
        "_gcd", "_lcm", "div_floor", "div_ceil", "mod_floor", "divmod", "_power",
        "chmax", "chmin", "_min", "_max",
    },
    IO_HPP: {
        "fast_io", "IN", "OUT", "FLUSH", "INT", "LL", "ULL", "STR", "CHR",
        "DBL", "VEC", "VV", "YES", "NO", "Yes", "No",
    },
}
# fmt: on

HEADER_DEPENDENCIES = {
    CONSTANTS_HPP: {TYPES_HPP},
    MACROS_HPP: {TYPES_HPP},
    MATH_HPP: {TYPES_HPP},
    IO_HPP: {TYPES_HPP, MACROS_HPP},
    FAST_IO_HPP: {IO_HPP, TYPES_HPP},
    PBDS_HPP: {TYPES_HPP},
    BIT_OPS_HPP: {TYPES_HPP},
    CONTAINERS_HPP: {TYPES_HPP, MACROS_HPP},
    MOD_INT_HPP: {TYPES_HPP, CONSTANTS_HPP},
}


def _strip_non_code(text: str) -> str:
    """Remove strings/comments so token scans only see code-like identifiers."""
    text = STRING_LITERAL_RE.sub('""', text)
    text = CHAR_LITERAL_RE.sub("''", text)
    text = BLOCK_COMMENT_RE.sub(" ", text)
    text = LINE_COMMENT_RE.sub("", text)
    return text


def _extract_identifiers(text: str) -> set[str]:
    """Extract identifier tokens from source text."""
    return set(IDENTIFIER_RE.findall(_strip_non_code(text)))


def _prune_template_headers(files_to_include, source_content):
    """
    Conservative tree-shaking for template headers.
    Keeps only headers whose symbols/macros appear in user code, while honoring
    hard dependencies between template headers.
    """
    if os.environ.get("CP_FLATTENER_DISABLE_PRUNING", "") == "1":
        return files_to_include

    used_identifiers = _extract_identifiers(source_content)
    file_lookup = {path.name: path for path in files_to_include}
    selected_names = set(file_lookup.keys())

    for header, trigger_tokens in OPTIONAL_HEADER_TRIGGER_TOKENS.items():
        if header in selected_names and not (used_identifiers & trigger_tokens):
            selected_names.remove(header)

    # Enforce syntactic dependencies so kept headers remain self-consistent.
    changed = True
    while changed:
        changed = False
        for header in tuple(selected_names):
            for dep in HEADER_DEPENDENCIES.get(header, ()):
                if dep in file_lookup and dep not in selected_names:
                    selected_names.add(dep)
                    changed = True

    return [path for path in files_to_include if path.name in selected_names]


def resolve_project_include(project_root, including_file, include_name):
    """Resolve a quoted include within project_root, if it exists."""
    include_target = (including_file.parent / include_name).resolve()
    if not include_target.is_file():
        include_target = (project_root / include_name).resolve()

    if include_target.is_file() and (
        include_target == project_root or project_root in include_target.parents
    ):
        return include_target
    return None


def process_file_content(filepath, *, preserve_includes=False):
    """Process a single file and return its content."""
    if not filepath.is_file():
        return ""

    content_lines = []
    with open(filepath, "r", encoding="utf-8") as f:
        for line in f:
            stripped = line.strip()
            # Always skip pragma once.
            if stripped == "#pragma once":
                continue
            # Skip includes only if not preserving them.
            if not preserve_includes and stripped.startswith("#include"):
                continue
            content_lines.append(line)

    # Remove leading and trailing empty lines.
    while content_lines and not content_lines[0].strip():
        content_lines.pop(0)
    while content_lines and not content_lines[-1].strip():
        content_lines.pop()

    return "".join(content_lines)


def inline_local_header(project_root, header_path, inlined_headers):
    """Inline project-local headers recursively and strip local includes/pragma once."""
    resolved = header_path.resolve()
    if not resolved.is_file():
        return ""
    if project_root not in resolved.parents:
        return ""
    if resolved in inlined_headers:
        return ""

    inlined_headers.add(resolved)
    content_lines = []

    with open(resolved, "r", encoding="utf-8") as f:
        for line in f:
            stripped = line.strip()
            if stripped == "#pragma once":
                continue

            match = PROJECT_INCLUDE_RE.match(stripped)
            if match:
                include_target = resolve_project_include(
                    project_root, resolved, match.group(1)
                )
                if include_target:
                    rel_target = include_target.relative_to(project_root).as_posix()
                    if rel_target.startswith(INLINE_ROOT_PREFIXES):
                        nested_content = inline_local_header(
                            project_root, include_target, inlined_headers
                        )
                        if nested_content:
                            content_lines.append(nested_content)
                            if not nested_content.endswith("\n"):
                                content_lines.append("\n")
                        continue
                content_lines.append(line)
                continue

            content_lines.append(line)

    while content_lines and not content_lines[0].strip():
        content_lines.pop(0)
    while content_lines and not content_lines[-1].strip():
        content_lines.pop()

    return "".join(content_lines)


# -------------------------------- MAIN LOGIC -------------------------------- #


def main():
    """Flatten one source file by expanding template and local project includes."""
    if len(sys.argv) != 2:
        sys.stderr.write(f"Usage: {sys.argv[0]} <source_file.cpp>\n")
        sys.exit(1)

    source_file = Path(sys.argv[1])
    if not source_file.is_file():
        sys.stderr.write(f"Error: File not found: {source_file}\n")
        sys.exit(1)

    script_dir = Path(__file__).parent.resolve()
    project_root = script_dir.parent
    templates_dir = project_root / "templates"
    base_template_path = templates_dir / "Base.hpp"

    with open(source_file, "r", encoding="utf-8") as f:
        source_content = f.read()

    need_mapping = load_need_mapping(base_template_path)
    if not need_mapping:
        sys.stderr.write(
            f"Error: Unable to derive NEED_* mapping from {base_template_path}\n"
        )
        sys.exit(1)

    # 1. Find which templates are needed.
    need_macros_found = extract_need_macros_from_source(
        source_content, need_mapping.keys()
    )

    # 2. Build list of files to include (preserving order).
    files_to_include = []
    included_set = set()

    for macro, files in need_mapping.items():
        if macro in need_macros_found:
            for filename in files:
                if filename not in included_set:
                    files_to_include.append(templates_dir / filename)
                    included_set.add(filename)

    files_to_include = _prune_template_headers(files_to_include, source_content)

    # 3. Process each template file.
    template_sections = []
    for filepath in files_to_include:
        content = process_file_content(filepath)
        if content:
            template_sections.append(content)

    # 4. Get preamble content, preserving its includes.
    preamble_path = templates_dir / "Preamble.hpp"
    preamble_content = process_file_content(preamble_path, preserve_includes=True)

    # 5. Process the source file and replace the base.hpp include.
    output_lines = []
    skip_need_defines = False
    # Prevent duplicated template bodies when NEED_* expanded sections are present.
    inlined_headers = {filepath.resolve() for filepath in files_to_include}

    with open(source_file, "r", encoding="utf-8") as f:
        for line in f:
            stripped = line.strip()

            # Skip NEED_* defines.
            if stripped.startswith("#define NEED_"):
                skip_need_defines = True
                continue

            # Replace the base.hpp include with our flattened content.
            if stripped == '#include "templates/Base.hpp"':
                # Remove trailing blank lines from the source file's header section.
                while output_lines and not output_lines[-1].strip():
                    output_lines.pop()

                # Add a blank line for separation before the preamble.
                output_lines.append("\n")

                # Add preamble.
                output_lines.append(preamble_content)
                if preamble_content and not preamble_content.endswith("\n\n"):
                    output_lines.append("\n")

                # Add template sections with proper spacing.
                for i, section in enumerate(template_sections):
                    if i > 0:  # Add spacing between sections.
                        output_lines.append("\n")
                    output_lines.append(section)
                    if section and not section.endswith("\n"):
                        output_lines.append("\n")
                continue

            include_match = PROJECT_INCLUDE_RE.match(stripped)
            if include_match:
                include_target = resolve_project_include(
                    project_root, source_file, include_match.group(1)
                )
                if include_target:
                    rel = include_target.relative_to(project_root).as_posix()
                    if rel.startswith(INLINE_ROOT_PREFIXES):
                        inlined = inline_local_header(
                            project_root, include_target, inlined_headers
                        )
                        if inlined:
                            if output_lines and output_lines[-1].strip():
                                output_lines.append("\n")
                            output_lines.append(inlined)
                            if not inlined.endswith("\n"):
                                output_lines.append("\n")
                        continue

            # Add back the line if it's not a NEED_* define.
            if (
                skip_need_defines
                and stripped
                and not stripped.startswith("#define NEED_")
            ):
                skip_need_defines = False
                # Add a blank line before the next section if appropriate.
                if output_lines and output_lines[-1].strip():
                    output_lines.append("\n")

            output_lines.append(line)

    # Print the final output.
    print("".join(output_lines), end="")


if __name__ == "__main__":
    main()
