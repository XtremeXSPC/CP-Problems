"""Preprocessor directive regex plus conditional folding and guard stripping.

Holds the canonical ``#if/#ifdef/#ifndef/#else/#elif/#endif`` matchers that
all the higher-level traversal code (``includes``, pipeline stages) shares.

``fold_simple_preprocessor_conditionals`` collapses ``#ifdef/#ifndef/#if``
blocks whose truth value is known under the current macro state, but only
when the block has no nested conditionals or ``#else``/``#elif`` — anything
ambiguous is preserved verbatim. ``strip_outer_include_guard`` removes the
conventional ``#ifndef FOO/#define FOO ... #endif`` wrapper after inlining.
"""

from __future__ import annotations

import re

from flattener_core.macros import (
    DEFINE_DIRECTIVE_RE,
    MacroValueMap,
    evaluate_simple_if_expression,
    update_macro_state_from_line,
)

IF_DIRECTIVE_RE = re.compile(r"^\s*#\s*(if|ifdef|ifndef)\b")
ELIF_DIRECTIVE_RE = re.compile(r"^\s*#\s*elif\b")
ELSE_DIRECTIVE_RE = re.compile(r"^\s*#\s*else\b")
ENDIF_DIRECTIVE_RE = re.compile(r"^\s*#\s*endif\b")
ELSE_OR_ELIF_DIRECTIVE_RE = re.compile(r"^\s*#\s*(else|elif)\b")
IFDEF_DIRECTIVE_RE = re.compile(r"^\s*#\s*ifdef\s+([A-Za-z_]\w*)\s*$")
IFNDEF_DIRECTIVE_RE = re.compile(r"^\s*#\s*ifndef\s+([A-Za-z_]\w*)\s*$")
IF_EXPR_DIRECTIVE_RE = re.compile(r"^\s*#\s*if\s+(.+?)\s*$")
INCLUDE_GUARD_IFNDEF_RE = re.compile(r"^\s*#\s*ifndef\s+([A-Za-z_]\w*)\s*$")
INCLUDE_GUARD_IF_DEFINED_RE = re.compile(r"^\s*#\s*if\s+!defined\(\s*([A-Za-z_]\w*)\s*\)\s*$")


def fold_simple_preprocessor_conditionals(content: str, macro_values: MacroValueMap | None) -> str:
    """Fold simple preprocessor blocks when the condition truth is known.

    Conservative on purpose:
      - supports ``#ifdef``, ``#ifndef``, and ``#if <simple-expr>``
      - folds only blocks without nested conditionals or ``#else/#elif``
      - preserves unknown/complex conditionals unchanged
    """

    if not content:
        return content

    macro_state: MacroValueMap = dict(macro_values or {})
    lines = content.splitlines(keepends=True)
    if not lines:
        return content

    output: list[str] = []
    i = 0
    n = len(lines)

    while i < n:
        line = lines[i]
        stripped = line.strip()

        cond_kind: str | None = None
        cond_value: str | None = None
        match_ifdef = IFDEF_DIRECTIVE_RE.match(stripped)
        if match_ifdef:
            cond_kind = "ifdef"
            cond_value = match_ifdef.group(1)
        else:
            match_ifndef = IFNDEF_DIRECTIVE_RE.match(stripped)
            if match_ifndef:
                cond_kind = "ifndef"
                cond_value = match_ifndef.group(1)
            else:
                match_ifexpr = IF_EXPR_DIRECTIVE_RE.match(stripped)
                if match_ifexpr:
                    cond_kind = "if"
                    cond_value = match_ifexpr.group(1).strip()

        if not cond_kind:
            output.append(line)
            update_macro_state_from_line(macro_state, line)
            i += 1
            continue

        depth = 0
        end_idx = -1
        else_idx = -1
        simple_block = True
        j = i + 1
        while j < n:
            candidate = lines[j].strip()
            if IF_DIRECTIVE_RE.match(candidate):
                # A nested conditional makes the block non-flat; keep verbatim.
                depth += 1
                simple_block = False
                j += 1
                continue
            if ENDIF_DIRECTIVE_RE.match(candidate):
                if depth == 0:
                    end_idx = j
                    break
                depth -= 1
                j += 1
                continue
            if depth == 0:
                if ELIF_DIRECTIVE_RE.match(candidate):
                    # ``#elif`` chains are not modeled; leave them intact.
                    simple_block = False
                elif ELSE_DIRECTIVE_RE.match(candidate):
                    if else_idx == -1:
                        else_idx = j
                    else:
                        simple_block = False
            j += 1

        if end_idx < 0:
            output.append(line)
            i += 1
            continue

        if not simple_block:
            output.extend(lines[i : end_idx + 1])
            i = end_idx + 1
            continue

        cond_result: bool | None
        if cond_kind == "ifdef":
            cond_result = cond_value in macro_state
        elif cond_kind == "ifndef":
            cond_result = cond_value not in macro_state
        else:
            cond_result = evaluate_simple_if_expression(cond_value or "", macro_state)

        if cond_result is None:
            output.extend(lines[i : end_idx + 1])
            i = end_idx + 1
            continue

        # Keep only the taken branch, dropping the guard directives. Handles a
        # flat ``#if/#endif`` and a flat ``#if/#else/#endif`` (no nesting/elif).
        if else_idx == -1:
            taken = lines[i + 1 : end_idx] if cond_result else []
        elif cond_result:
            taken = lines[i + 1 : else_idx]
        else:
            taken = lines[else_idx + 1 : end_idx]
        output.extend(taken)
        for body_line in taken:
            update_macro_state_from_line(macro_state, body_line)

        i = end_idx + 1

    return "".join(output)


def strip_outer_include_guard(content: str) -> str:
    """Strip a conventional outer include-guard wrapper from inlined content.

    Supported forms:
      - ``#ifndef FOO`` / ``#define FOO`` ... ``#endif``
      - ``#if !defined(FOO)`` / ``#define FOO`` ... ``#endif``
    """

    lines = content.splitlines(keepends=True)
    if not lines:
        return content

    def is_significant(line: str) -> bool:
        stripped = line.strip()
        return stripped != "" and not stripped.startswith("//")

    sig_idx = [i for i, line in enumerate(lines) if is_significant(line)]
    if len(sig_idx) < 3:
        return content

    first, second = sig_idx[0], sig_idx[1]

    guard_match = INCLUDE_GUARD_IFNDEF_RE.match(
        lines[first].strip()
    ) or INCLUDE_GUARD_IF_DEFINED_RE.match(lines[first].strip())
    if guard_match is None:
        return content

    guard_name = guard_match.group(1)
    m_define = DEFINE_DIRECTIVE_RE.match(lines[second].strip())
    if not m_define or m_define.group(1) != guard_name:
        return content

    depth = 0
    match_endif: int | None = None
    for i in range(first, len(lines)):
        stripped = lines[i].strip()
        if IF_DIRECTIVE_RE.match(stripped):
            depth += 1
        elif ENDIF_DIRECTIVE_RE.match(stripped):
            depth -= 1
            if depth == 0:
                match_endif = i
                break
            if depth < 0:
                return content

    if match_endif is None or match_endif != sig_idx[-1]:
        return content

    guard_indices = {first, second, match_endif}
    return "".join(line for i, line in enumerate(lines) if i not in guard_indices)
