#!/usr/bin/env python3
"""Shared NEED_* parsing helpers for CP template scripts."""

from __future__ import annotations

import re
from collections import OrderedDict
from pathlib import Path
from typing import Iterable

PROJECT_INCLUDE_RE = re.compile(r'^\s*#include\s+"([^"]+)"\s*$')
NEED_IFDEF_RE = re.compile(r"^\s*#\s*ifdef\s+(NEED_\w+)\s*$")
IF_DIRECTIVE_RE = re.compile(r"^\s*#\s*(if|ifdef|ifndef)\b")
ENDIF_DIRECTIVE_RE = re.compile(r"^\s*#\s*endif\b")
NEED_DEFINE_RE = re.compile(r"^\s*#\s*define\s+(NEED_\w+)\s*$")


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

    with open(base_header, "r", encoding="utf-8") as f:
        for raw_line in f:
            stripped = raw_line.strip()

            need_match = NEED_IFDEF_RE.match(stripped)
            if need_match:
                macro = need_match.group(1)
                mapping.setdefault(macro, [])
                active_stack.append(macro)
                continue

            if IF_DIRECTIVE_RE.match(stripped):
                # Nested non-NEED conditionals inherit outer NEED context.
                active_stack.append(active_stack[-1])
                continue

            if ENDIF_DIRECTIVE_RE.match(stripped):
                if len(active_stack) > 1:
                    active_stack.pop()
                continue

            include_match = PROJECT_INCLUDE_RE.match(stripped)
            if include_match and active_stack[-1] is not None:
                include_name = Path(include_match.group(1)).name
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

    for raw_line in source_content.splitlines():
        stripped = raw_line.strip()
        if not stripped or stripped.startswith("//"):
            continue
        match = NEED_DEFINE_RE.match(stripped)
        if not match:
            continue
        macro = match.group(1)
        if macro in known:
            found.add(macro)

    # Core should be included whenever any module macro is enabled.
    if found and "NEED_CORE" in known:
        found.add("NEED_CORE")

    return found
