#!/usr/bin/env python3
"""Shared NEED_* parsing helpers for CP template scripts."""

from __future__ import annotations

import re
from collections import OrderedDict
from collections.abc import Iterable
from pathlib import Path

from flattener_core.flattener_helpers import (
    extract_prefix_before_base_include,
    fold_simple_preprocessor_conditionals,
    parse_project_include_line,
    strip_comments,
    strip_non_code,
)
from profile_registry import load_registry

NEED_IFDEF_RE = re.compile(r"^\s*#\s*ifdef\s+(NEED_\w+)\s*$")
NEED_IF_DEFINED_RE = re.compile(
    r"^\s*#\s*if\s+defined(?:\s*\(\s*|\s+)(NEED_\w+)\s*\)?\s*$"
)
IF_DIRECTIVE_RE = re.compile(r"^\s*#\s*(if|ifdef|ifndef)\b")
ENDIF_DIRECTIVE_RE = re.compile(r"^\s*#\s*endif\b")
ELSE_OR_ELIF_DIRECTIVE_RE = re.compile(r"^\s*#\s*(else|elif)\b")
NEED_DEFINE_RE = re.compile(r"^\s*#\s*define\s+(NEED_\w+)\b")
NEED_UNDEF_RE = re.compile(r"^\s*#\s*undef\s+(NEED_\w+)\b")
IO_PROFILE_DEFINE_RE = re.compile(r"^\s*#\s*define\s+(CP_IO_PROFILE_[A-Z_]+)\b")
IO_PROFILE_UNDEF_RE = re.compile(r"^\s*#\s*undef\s+(CP_IO_PROFILE_[A-Z_]+)\b")


def _io_profile_to_needs() -> dict[str, tuple[str, ...]]:
    """CP_IO_PROFILE_* → NEED_* mapping derived from templates/profiles.toml."""

    return load_registry().io_profile_to_needs()


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
            include_name = include_name_raw
            if include_name.startswith("templates/"):
                include_name = include_name[len("templates/"):]
            entries = mapping[active_stack[-1]]
            if include_name not in entries:
                entries.append(include_name)

    return mapping


def extract_need_macros_from_source(
    source_content: str,
    known_macros: Iterable[str],
    *,
    warn_stream=None,
) -> set[str]:
    """Extract enabled NEED_* macros from a source file content.

    NEED_* defines inside conditional blocks the folder could not evaluate are
    silently skipped (the flattener can't know whether they'd fire). When
    ``warn_stream`` is provided, a one-line warning is emitted for each such
    skipped macro.
    """

    known = set(known_macros)
    found: set[str] = set()
    enabled_io_profiles: set[str] = set()
    skipped_needs: list[str] = []

    code_only_prefix = strip_non_code(extract_prefix_before_base_include(source_content))
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
            define_match = NEED_DEFINE_RE.match(stripped)
            if define_match and define_match.group(1) in known:
                skipped_needs.append(define_match.group(1))
            continue

        undef_match = NEED_UNDEF_RE.match(stripped)
        if undef_match:
            macro = undef_match.group(1)
            if macro in known:
                found.discard(macro)
            continue

        profile_undef_match = IO_PROFILE_UNDEF_RE.match(stripped)
        if profile_undef_match:
            enabled_io_profiles.discard(profile_undef_match.group(1))
            continue

        define_match = NEED_DEFINE_RE.match(stripped)
        if define_match:
            macro = define_match.group(1)
            if macro in known:
                found.add(macro)
            continue

        profile_define_match = IO_PROFILE_DEFINE_RE.match(stripped)
        if profile_define_match:
            enabled_io_profiles.add(profile_define_match.group(1))
            continue

    if warn_stream is not None and skipped_needs:
        unique = sorted(set(skipped_needs))
        warn_stream.write(
            "warning: ignoring conditional NEED_* defines (guard unresolved): "
            + ", ".join(unique) + "\n"
        )

    io_profile_to_needs = _io_profile_to_needs()
    for profile_name in enabled_io_profiles:
        for macro in io_profile_to_needs.get(profile_name, ()):
            if macro in known:
                found.add(macro)

    return found
