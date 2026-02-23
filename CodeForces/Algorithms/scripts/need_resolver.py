#!/usr/bin/env python3
"""Shared NEED_* parsing helpers for CP template scripts."""

from __future__ import annotations

import re
from collections import OrderedDict
from pathlib import Path
from typing import Iterable

from flattener_core.flattener_helpers import (
    fold_simple_preprocessor_conditionals,
    parse_project_include_line,
    strip_comments,
    strip_non_code,
)

NEED_IFDEF_RE = re.compile(r"^\s*#\s*ifdef\s+(NEED_\w+)\s*$")
NEED_IF_DEFINED_RE = re.compile(
    r"^\s*#\s*if\s+defined(?:\s*\(\s*|\s+)(NEED_\w+)\s*\)?\s*$"
)
IF_DIRECTIVE_RE = re.compile(r"^\s*#\s*(if|ifdef|ifndef)\b")
ENDIF_DIRECTIVE_RE = re.compile(r"^\s*#\s*endif\b")
ELSE_OR_ELIF_DIRECTIVE_RE = re.compile(r"^\s*#\s*(else|elif)\b")
NEED_DEFINE_RE = re.compile(r"^\s*#\s*define\s+(NEED_\w+)\b")
NEED_UNDEF_RE = re.compile(r"^\s*#\s*undef\s+(NEED_\w+)\b")


def load_need_mapping(base_header: Path) -> OrderedDict[str, list[str]]:
    """
    Parse templates/Base.hpp and return NEED_* -> [header files] mapping.

    Mapping order follows macro declaration order in Base.hpp.
    Header order follows include order within each NEED_* block.
    """
    mapping: OrderedDict[str, list[str]] = OrderedDict()
    if not base_header.is_file():
        return mapping

    # Stack stores the currently-active NEED_* macro at each conditional depth.
    active_stack: list[str | None] = [None]
    frame_kind_stack: list[str] = ["root"]

    with open(base_header, "r", encoding="utf-8") as f:
        file_text = f.read()

    stripped_lines = strip_comments(file_text).splitlines()
    masked_lines = strip_non_code(file_text).splitlines()
    for idx, raw_line in enumerate(stripped_lines):
        masked_line = masked_lines[idx] if idx < len(masked_lines) else ""
        stripped = masked_line.strip()

        need_match = NEED_IFDEF_RE.match(stripped) or NEED_IF_DEFINED_RE.match(
            stripped
        )
        if need_match:
            macro = need_match.group(1)
            mapping.setdefault(macro, [])
            active_stack.append(macro)
            frame_kind_stack.append("need")
            continue

        if IF_DIRECTIVE_RE.match(stripped):
            # Nested non-NEED conditionals inherit outer NEED context.
            active_stack.append(active_stack[-1])
            frame_kind_stack.append("other")
            continue

        if ELSE_OR_ELIF_DIRECTIVE_RE.match(stripped):
            # Alternate branch of the NEED guard should not contribute to
            # the NEED_* mapping.
            if len(active_stack) > 1 and frame_kind_stack[-1] == "need":
                active_stack[-1] = None
            continue

        if ENDIF_DIRECTIVE_RE.match(stripped):
            if len(active_stack) > 1:
                active_stack.pop()
                frame_kind_stack.pop()
            continue

        include_name_raw = parse_project_include_line(
            raw_line, masked_line=masked_line
        )
        if include_name_raw and active_stack[-1] is not None:
            include_name = Path(include_name_raw).name
            entries = mapping[active_stack[-1]]
            if include_name not in entries:
                entries.append(include_name)

    return mapping


def extract_need_macros_from_source(
    source_content: str, known_macros: Iterable[str]
) -> set[str]:
    """Extract enabled NEED_* macros from a source file content."""
    known = set(known_macros)
    found: set[str] = set()

    comments_stripped = strip_comments(source_content)
    masked_lines = strip_non_code(source_content).splitlines()
    prefix_lines: list[str] = []
    for idx, raw_line in enumerate(comments_stripped.splitlines()):
        masked_line = masked_lines[idx] if idx < len(masked_lines) else ""
        stripped = masked_line.strip()
        if not stripped:
            continue

        include_name = parse_project_include_line(raw_line, masked_line=masked_line)
        if include_name:
            include_target = include_name.replace("\\", "/")
            if include_target.endswith("templates/Base.hpp"):
                break

        prefix_lines.append(raw_line)

    code_only_prefix = strip_non_code("\n".join(prefix_lines))
    folded_code = fold_simple_preprocessor_conditionals(code_only_prefix, {})
    depth = 0

    for raw_line in folded_code.splitlines():
        stripped = raw_line.strip()
        if not stripped:
            continue

        if IF_DIRECTIVE_RE.match(stripped):
            depth += 1
            continue

        if ENDIF_DIRECTIVE_RE.match(stripped):
            depth = max(0, depth - 1)
            continue

        if ELSE_OR_ELIF_DIRECTIVE_RE.match(stripped):
            continue

        if depth > 0:
            continue

        undef_match = NEED_UNDEF_RE.match(stripped)
        if undef_match:
            macro = undef_match.group(1)
            if macro in known:
                found.discard(macro)
            continue

        define_match = NEED_DEFINE_RE.match(stripped)
        if not define_match:
            continue
        macro = define_match.group(1)
        if macro in known:
            found.add(macro)

    # Core should be included whenever any module macro is enabled.
    if found and "NEED_CORE" in known:
        found.add("NEED_CORE")

    return found
