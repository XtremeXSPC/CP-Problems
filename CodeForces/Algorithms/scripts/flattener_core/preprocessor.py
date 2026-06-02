"""Preprocessor directive regexes, the conditional folder, and guard stripping.

Holds the canonical ``#if/#ifdef/#ifndef/#else/#elif/#endif`` matchers shared
by every traversal (``includes``, pipeline stages) and two engine pieces built
on :mod:`flattener_core.ppexpr`:

* :class:`ConditionalScanner` — a streaming three-valued conditional tracker
  used by the include-discovery and inlining loops, so they share one state
  machine instead of re-deriving it.
* :func:`fold_simple_preprocessor_conditionals` — a recursive folder that
  collapses any ``#if``/``#elif``/``#else`` chain (including nested ones) whose
  outcome is statically decided under the current macro state, dropping the
  directives and the dead arms. Undecidable chains are preserved verbatim but
  still folded *inside* (so a compiler-guarded block keeps folding its known
  inner config). With ``closed_namespace`` the ``CP_*``/``NEED_*`` config layer
  collapses entirely while compiler/toolchain guards survive.

``strip_outer_include_guard`` removes the conventional ``#ifndef FOO/#define
FOO ... #endif`` wrapper after inlining.
"""

from __future__ import annotations

import re
from collections import namedtuple

from flattener_core.macros import (
    DEFINE_DIRECTIVE_RE,
    MacroValueMap,
    update_macro_state_from_line,
)
from flattener_core.ppexpr import evaluate_condition

IF_DIRECTIVE_RE = re.compile(r"^\s*#\s*(if|ifdef|ifndef)\b")
ELIF_DIRECTIVE_RE = re.compile(r"^\s*#\s*elif\b")
ELSE_DIRECTIVE_RE = re.compile(r"^\s*#\s*else\b")
ENDIF_DIRECTIVE_RE = re.compile(r"^\s*#\s*endif\b")
ELSE_OR_ELIF_DIRECTIVE_RE = re.compile(r"^\s*#\s*(else|elif)\b")
IFDEF_DIRECTIVE_RE = re.compile(r"^\s*#\s*ifdef\s+([A-Za-z_]\w*)\s*$")
IFNDEF_DIRECTIVE_RE = re.compile(r"^\s*#\s*ifndef\s+([A-Za-z_]\w*)\s*$")
IF_EXPR_DIRECTIVE_RE = re.compile(r"^\s*#\s*if\s+(.+?)\s*$")
ELIF_EXPR_DIRECTIVE_RE = re.compile(r"^\s*#\s*elif\s+(.+?)\s*$")
INCLUDE_GUARD_IFNDEF_RE = re.compile(r"^\s*#\s*ifndef\s+([A-Za-z_]\w*)\s*$")
INCLUDE_GUARD_IF_DEFINED_RE = re.compile(r"^\s*#\s*if\s+!defined\(\s*([A-Za-z_]\w*)\s*\)\s*$")


class ConditionalScanner:
    """Streaming ``#if``/``#elif``/``#else``/``#endif`` tracker for traversals.

    Conservative by design, matching the flattener's established include
    semantics: an UNKNOWN ``#if`` counts as *active* (its includes are
    explored/inlined and the block is later preserved by the folder), while any
    ``#elif`` arm is treated as inactive (the opening arm is assumed taken).
    ``active`` gates include handling; ``mutable`` gates authoritative
    macro-state mutation (only when every enclosing branch is statically
    decided).
    """

    def __init__(self) -> None:
        self._active: list[bool] = [True]
        self._certain: list[bool] = [True]

    def open(self, truth: bool | None) -> None:
        self._active.append(truth is None or bool(truth))
        self._certain.append(self._certain[-1] and truth is not None)

    def open_elif(self) -> None:
        if len(self._active) > 1:
            self._active[-1] = False

    def open_else(self) -> None:
        if len(self._active) > 1:
            self._active[-1] = not self._active[-1]

    def close(self) -> None:
        if len(self._active) > 1:
            self._active.pop()
            self._certain.pop()

    @property
    def active(self) -> bool:
        return all(self._active)

    @property
    def mutable(self) -> bool:
        """Whether a ``#define``/``#undef`` here may mutate the macro state."""

        return all(self._active) and all(self._certain)


def _classify_opener(stripped: str) -> tuple[str, str | None]:
    """Classify an ``#if``-family opener into ``(kind, condition-text)``."""

    match = IFDEF_DIRECTIVE_RE.match(stripped)
    if match:
        return "ifdef", match.group(1)
    match = IFNDEF_DIRECTIVE_RE.match(stripped)
    if match:
        return "ifndef", match.group(1)
    match = IF_EXPR_DIRECTIVE_RE.match(stripped)
    if match:
        return "if", match.group(1).strip()
    return "if", ""  # unclassifiable #if* → undecidable → preserved verbatim


_Arm = namedtuple("_Arm", ["directive", "kind", "cond", "body"])


def _split_conditional(lines: list[str], start: int) -> tuple[list[_Arm], str, int] | None:
    """Split the conditional region opening at ``lines[start]`` into its arms.

    Returns ``(arms, endif_line, next_index)`` or ``None`` when the region is
    unbalanced (no matching ``#endif``), so the caller can emit the opener as a
    plain line. ``body`` lines of each arm are kept verbatim (nested
    conditionals included) for recursive folding.
    """

    kind, cond = _classify_opener(lines[start].strip())
    directive = lines[start]
    body: list[str] = []
    arms: list[_Arm] = []
    depth = 0
    j = start + 1
    n = len(lines)

    while j < n:
        stripped = lines[j].strip()
        if IF_DIRECTIVE_RE.match(stripped):
            depth += 1
            body.append(lines[j])
            j += 1
            continue
        if ENDIF_DIRECTIVE_RE.match(stripped):
            if depth == 0:
                arms.append(_Arm(directive, kind, cond, body))
                return arms, lines[j], j + 1
            depth -= 1
            body.append(lines[j])
            j += 1
            continue
        if depth == 0 and ELIF_DIRECTIVE_RE.match(stripped):
            arms.append(_Arm(directive, kind, cond, body))
            elif_match = ELIF_EXPR_DIRECTIVE_RE.match(stripped)
            directive = lines[j]
            kind = "elif"
            cond = elif_match.group(1).strip() if elif_match else ""
            body = []
            j += 1
            continue
        if depth == 0 and ELSE_DIRECTIVE_RE.match(stripped):
            arms.append(_Arm(directive, kind, cond, body))
            directive = lines[j]
            kind = "else"
            cond = None
            body = []
            j += 1
            continue
        body.append(lines[j])
        j += 1

    return None


def _emit_conditional(
    arms: list[_Arm],
    endif_line: str,
    out: list[str],
    state: MacroValueMap,
    closed: bool,
) -> None:
    """Fold a single conditional chain into ``out`` under the live ``state``."""

    taken: int | None = None
    undecidable = False
    for index, arm in enumerate(arms):
        truth = evaluate_condition(arm.kind, arm.cond, state, closed_namespace=closed)
        if truth is None:
            undecidable = True
            break
        if truth:
            taken = index
            break
        # truth is False → this arm is certainly skipped; try the next one.

    if not undecidable:
        # The whole chain is decided: keep only the taken arm (folded), applying
        # its #define/#undef to the live state; all directives and dead arms go.
        if taken is not None:
            out.extend(_fold_region(arms[taken].body, state, closed))
        return

    # Undecidable: preserve the region verbatim, but recurse into each arm with
    # a private state copy so an unreachable/uncertain arm never leaks defines.
    for arm in arms:
        out.append(arm.directive)
        out.extend(_fold_region(arm.body, dict(state), closed))
    out.append(endif_line)


def _fold_region(lines: list[str], state: MacroValueMap, closed: bool) -> list[str]:
    """Fold a balanced line region top-to-bottom, threading ``state`` forward."""

    out: list[str] = []
    i = 0
    n = len(lines)
    while i < n:
        line = lines[i]
        if IF_DIRECTIVE_RE.match(line.strip()):
            split = _split_conditional(lines, i)
            if split is None:
                out.append(line)
                i += 1
                continue
            arms, endif_line, next_index = split
            _emit_conditional(arms, endif_line, out, state, closed)
            i = next_index
            continue
        out.append(line)
        update_macro_state_from_line(state, line)
        i += 1
    return out


def fold_simple_preprocessor_conditionals(
    content: str,
    macro_values: MacroValueMap | None,
    *,
    closed_namespace: bool = False,
) -> str:
    """Collapse statically-decided ``#if``/``#elif``/``#else`` chains.

    Handles arbitrary nesting and ``#elif``/``#else``. A chain is folded (its
    directives and dead arms removed) when its outcome is known under
    ``macro_values``; otherwise it is preserved verbatim, with inner decidable
    chains still folded. ``closed_namespace`` treats absent ``CP_*``/``NEED_*``
    macros as undefined (see :func:`flattener_core.ppexpr.is_macro_defined`),
    collapsing the resolved config layer while leaving compiler/toolchain
    guards intact.
    """

    if not content:
        return content
    lines = content.splitlines(keepends=True)
    if not lines:
        return content
    state: MacroValueMap = dict(macro_values or {})
    return "".join(_fold_region(lines, state, closed_namespace))


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
