"""High-level macro extraction from user source.

``extract_macro_values_from_source`` walks the source prefix before
``templates/Base.hpp``, applies a conservative folder to skip unevaluable
``#if`` blocks, then layers in: defaults from ``profile_registry`` (with
strict/relaxed selection), per-IO-profile triggers (``CP_IO_PROFILE_*``),
and the composite-IO opt-in driven by the ``COMPOSITE_IO_TRIGGER_TOKENS``
identifier set.

This is the pipeline-level wrapper around ``flattener_core.macros`` — the
latter handles individual ``#define`` lines, the former composes them
against the project's profile registry.
"""

from __future__ import annotations

import re
from typing import TextIO

from flattener_core.lexer import extract_identifiers, strip_non_code
from flattener_core.macros import MacroValueMap
from flattener_core.preprocessor import (
    ELSE_OR_ELIF_DIRECTIVE_RE,
    ENDIF_DIRECTIVE_RE,
    IF_DIRECTIVE_RE,
    fold_simple_preprocessor_conditionals,
)
from profile_registry import load_registry

_MACRO_DEFINE_RE = re.compile(r"^\s*#\s*define\s+([A-Za-z_]\w*)(?:\s+(.*?))?\s*$")
_MACRO_UNDEF_RE = re.compile(r"^\s*#\s*undef\s+([A-Za-z_]\w*)\s*$")
_NUMERIC_LITERAL_RE = re.compile(r"^[+-]?(?:0|[1-9]\d*|0[xX][0-9A-Fa-f]+)(?:[uUlL]{0,3})?$")

COMPOSITE_IO_TRIGGER_TOKENS: frozenset[str] = frozenset(
    {
        "Vec",
        "Vec2",
        "Vec3",
        "Vec4",
        "VecI32",
        "VecI64",
        "VecBool",
        "VecStr",
        "Pair",
        "PairI32",
        "PairI64",
        "Tuple",
        "VEC",
        "VV",
    }
)


def _parse_macro_value(value_expr: str | None, macro_values: MacroValueMap) -> int | None:
    """Parse a simple ``#define`` value into ``int`` / ``None``.

    Strips inline comments and trailing type suffixes (e.g. ``u``, ``L``).
    Falls back to looking up the token in ``macro_values`` when it is not a
    numeric literal.
    """

    if value_expr is None:
        return 1

    token = value_expr.split("//", 1)[0]
    token = token.split("/*", 1)[0].strip()
    if not token:
        return 1

    token = token.split()[0]
    if _NUMERIC_LITERAL_RE.fullmatch(token):
        suffix_trimmed = re.sub(r"[uUlL]+$", "", token)
        try:
            return int(suffix_trimmed, 0)
        except ValueError:
            return None

    return macro_values.get(token)


def extract_macro_values_from_source(
    source_prefix_content: str,
    *,
    strict_profile_enabled: bool,
    relaxed_profile_enabled: bool,
    warn_stream: TextIO | None = None,
) -> MacroValueMap:
    """Extract simple macro values from user source for conservative folding.

    Defines inside conditional blocks the folder cannot evaluate are skipped.
    When ``warn_stream`` is provided, the skipped names are reported once.
    """

    macro_values: MacroValueMap = {}
    skipped: list[str] = []
    code_only = strip_non_code(source_prefix_content)
    folded_code = fold_simple_preprocessor_conditionals(code_only, {})
    depth = 0

    for raw_line in folded_code.splitlines():
        stripped = raw_line.strip()
        if not stripped or stripped.startswith("//"):
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
            define_match = _MACRO_DEFINE_RE.match(stripped)
            if define_match:
                skipped.append(define_match.group(1))
            continue

        undef_match = _MACRO_UNDEF_RE.match(stripped)
        if undef_match:
            macro_values.pop(undef_match.group(1), None)
            continue

        define_match = _MACRO_DEFINE_RE.match(stripped)
        if not define_match:
            continue

        name, value_expr = define_match.group(1), define_match.group(2)
        macro_values[name] = _parse_macro_value(value_expr, macro_values)

    if warn_stream is not None and skipped:
        warn_stream.write(
            "warning: ignoring conditional #defines (guard unresolved): "
            + ", ".join(sorted(set(skipped)))
            + "\n"
        )

    registry = load_registry()
    apply_strict = strict_profile_enabled and not relaxed_profile_enabled
    for name, value in registry.config_defaults_as_dict(strict=apply_strict).items():
        macro_values.setdefault(name, value)

    if "CP_IO_ENABLE_COMPOSITE" not in macro_values:
        macro_values["CP_IO_ENABLE_COMPOSITE"] = (
            1 if macro_values.get("CP_ENABLE_LEGACY_IO_VEC_MACROS") not in (None, 0) else 0
        )

    def _is_enabled(name: str) -> bool:
        value = macro_values.get(name)
        return value is not None and value != 0

    for profile_name, profile in registry.io_profiles.items():
        guard_macro = f"CP_IO_PROFILE_{profile_name.upper()}"
        if not _is_enabled(guard_macro):
            continue
        for need in profile.needs:
            macro_values.setdefault(need, 1)
        for define_name, define_value in profile.defines.items():
            macro_values.setdefault(define_name, define_value)

    if extract_identifiers(source_prefix_content) & COMPOSITE_IO_TRIGGER_TOKENS:
        macro_values["CP_IO_ENABLE_COMPOSITE"] = 1

    return macro_values
