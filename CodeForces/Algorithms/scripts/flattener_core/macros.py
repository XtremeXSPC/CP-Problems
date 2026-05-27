"""Macro-state primitives consumed by the preprocessor folder.

Implements a conservative model of the C++ preprocessor's symbol table:
``update_macro_state_from_line`` applies ``#define``/``#undef`` directives,
``evaluate_simple_if_expression`` decides truth for the restricted
``#if`` forms the flattener can prove (integer literals, ``defined(X)``,
bare identifiers, and their negations). Complex expressions deliberately
return ``None`` so the folder leaves them intact.

The ``MacroValueMap`` alias is the canonical shape of the macro table that
flows through every other ``flattener_core`` module.
"""

from __future__ import annotations

import re

MacroValueMap = dict[str, int | None]

DEFINE_DIRECTIVE_RE = re.compile(r"^\s*#\s*define\s+([A-Za-z_]\w*)\b")
DEFINE_WITH_VALUE_RE = re.compile(
    r"^\s*#\s*define\s+([A-Za-z_]\w*)(?:\s*\([^)]*\))?(?:\s+(.*?))?\s*$"
)
UNDEF_DIRECTIVE_RE = re.compile(r"^\s*#\s*undef\s+([A-Za-z_]\w*)\s*$")
INTEGER_LITERAL_RE = re.compile(r"^[+-]?(?:0|[1-9]\d*|0[xX][0-9A-Fa-f]+)(?:[uUlL]{0,3})?$")
SIMPLE_DEFINED_RE = re.compile(r"^defined\s*\(\s*([A-Za-z_]\w*)\s*\)$")
SIMPLE_DEFINED_ALT_RE = re.compile(r"^defined\s+([A-Za-z_]\w*)$")
SIMPLE_NOT_DEFINED_RE = re.compile(r"^!\s*defined\s*\(\s*([A-Za-z_]\w*)\s*\)$")
SIMPLE_NOT_DEFINED_ALT_RE = re.compile(r"^!\s*defined\s+([A-Za-z_]\w*)$")
SIMPLE_IDENT_RE = re.compile(r"^([A-Za-z_]\w*)$")
SIMPLE_NOT_IDENT_RE = re.compile(r"^!\s*([A-Za-z_]\w*)$")


def parse_macro_numeric_token(token: str, macro_values: MacroValueMap) -> int | None:
    """Interpret a single token as an integer literal or known macro value.

    Strips C++ integer suffixes (``u``, ``L``, ``UL``, ...) before parsing.
    Returns ``None`` when the token is neither a literal nor a known macro.
    """

    stripped = token.strip()
    if not stripped:
        return None

    if INTEGER_LITERAL_RE.fullmatch(stripped):
        suffix_trimmed = re.sub(r"[uUlL]+$", "", stripped)
        try:
            return int(suffix_trimmed, 0)
        except ValueError:
            return None

    return macro_values.get(stripped)


def update_macro_state_from_line(macro_values: MacroValueMap, line: str) -> None:
    """Apply a single ``#define`` or ``#undef`` to the live macro table.

    Only simple numeric / identifier values are tracked; complex expressions
    are stored as ``None``.
    """

    stripped = line.strip()
    if not stripped:
        return

    undef_match = UNDEF_DIRECTIVE_RE.match(stripped)
    if undef_match:
        macro_values.pop(undef_match.group(1), None)
        return

    define_match = DEFINE_WITH_VALUE_RE.match(stripped)
    if not define_match:
        return

    name, value_expr = define_match.group(1), define_match.group(2)
    if not value_expr:
        macro_values[name] = 1
        return

    token = value_expr.split("//", 1)[0]
    token = token.split("/*", 1)[0].strip()
    if not token:
        macro_values[name] = 1
        return

    token = token.split()[0]
    macro_values[name] = parse_macro_numeric_token(token, macro_values)


def evaluate_simple_if_expression(expr: str, macro_values: MacroValueMap) -> bool | None:
    """Evaluate a restricted ``#if`` expression under a known macro state.

    Supported forms (anything else returns ``None``):
      - integer literal (including negated literal)
      - ``defined(NAME)`` / ``defined NAME``
      - ``!defined(NAME)`` / ``!defined NAME``
      - bare identifier (truthy if non-zero)
      - ``!identifier`` (truthy if zero)
    """

    normalized = expr.strip()
    if not normalized:
        return None

    direct_value = parse_macro_numeric_token(normalized, macro_values)
    if direct_value is not None:
        return direct_value != 0

    if normalized.startswith("!"):
        negated_value = parse_macro_numeric_token(normalized[1:].strip(), macro_values)
        if negated_value is not None:
            return negated_value == 0

    for pattern in (
        SIMPLE_DEFINED_RE,
        SIMPLE_DEFINED_ALT_RE,
        SIMPLE_NOT_DEFINED_RE,
        SIMPLE_NOT_DEFINED_ALT_RE,
    ):
        match = pattern.fullmatch(normalized)
        if not match:
            continue
        name = match.group(1)
        is_defined = name in macro_values
        if pattern in {SIMPLE_DEFINED_RE, SIMPLE_DEFINED_ALT_RE}:
            return is_defined
        return not is_defined

    match = SIMPLE_IDENT_RE.fullmatch(normalized)
    if match:
        value = macro_values.get(match.group(1))
        if value is None:
            return None
        return value != 0

    match = SIMPLE_NOT_IDENT_RE.fullmatch(normalized)
    if match:
        value = macro_values.get(match.group(1))
        if value is None:
            return None
        return value == 0

    return None
