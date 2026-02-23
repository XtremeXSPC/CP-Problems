"""Helper utilities shared by the template flattener workflow."""

from __future__ import annotations

import os
import re
from pathlib import Path

# fmt: off
PROJECT_INCLUDE_RE = re.compile(r'^\s*#include\s+"([^"]+)"\s*$')
INCLUDE_DIRECTIVE_PREFIX_RE = re.compile(r"^\s*#\s*include\b")
INLINE_ROOT_PREFIXES = ("modules/", "templates/")
IDENTIFIER_RE = re.compile(r"\b[A-Za-z_]\w*\b")
STRUCT_CLASS_RE = re.compile(r"^\s*(?:template\s*<[^>]*>\s*)?(?:struct|class)\s+([A-Za-z_]\w*)\b")
USING_RE = re.compile(r"^\s*using\s+([A-Za-z_]\w*)\b")
FUNC_LIKE_RE = re.compile(r"([A-Za-z_]\w*)\s*\([^()]*\)\s*(?:const)?\s*(?:\{|$)")
INCLUDE_GUARD_IFNDEF_RE = re.compile(r"^\s*#\s*ifndef\s+([A-Za-z_]\w*)\s*$")
INCLUDE_GUARD_IF_DEFINED_RE = re.compile(
    r"^\s*#\s*if\s+!defined\(\s*([A-Za-z_]\w*)\s*\)\s*$"
)
DEFINE_DIRECTIVE_RE = re.compile(r"^\s*#\s*define\s+([A-Za-z_]\w*)\b")
DEFINE_WITH_VALUE_RE = re.compile(r"^\s*#\s*define\s+([A-Za-z_]\w*)(?:\s+(.*?))?\s*$")
UNDEF_DIRECTIVE_RE = re.compile(r"^\s*#\s*undef\s+([A-Za-z_]\w*)\s*$")
IF_DIRECTIVE_RE = re.compile(r"^\s*#\s*(if|ifdef|ifndef)\b")
ENDIF_DIRECTIVE_RE = re.compile(r"^\s*#\s*endif\b")
ELSE_OR_ELIF_DIRECTIVE_RE = re.compile(r"^\s*#\s*(else|elif)\b")
IFDEF_DIRECTIVE_RE = re.compile(r"^\s*#\s*ifdef\s+([A-Za-z_]\w*)\s*$")
IFNDEF_DIRECTIVE_RE = re.compile(r"^\s*#\s*ifndef\s+([A-Za-z_]\w*)\s*$")
IF_EXPR_DIRECTIVE_RE = re.compile(r"^\s*#\s*if\s+(.+?)\s*$")
SIMPLE_DEFINED_RE = re.compile(r"^defined\s*\(\s*([A-Za-z_]\w*)\s*\)$")
SIMPLE_DEFINED_ALT_RE = re.compile(r"^defined\s+([A-Za-z_]\w*)$")
SIMPLE_NOT_DEFINED_RE = re.compile(r"^!\s*defined\s*\(\s*([A-Za-z_]\w*)\s*\)$")
SIMPLE_NOT_DEFINED_ALT_RE = re.compile(r"^!\s*defined\s+([A-Za-z_]\w*)$")
SIMPLE_IDENT_RE = re.compile(r"^([A-Za-z_]\w*)$")
SIMPLE_NOT_IDENT_RE = re.compile(r"^!\s*([A-Za-z_]\w*)$")
INTEGER_LITERAL_RE = re.compile(r"^[+-]?(?:0|[1-9]\d*|0[xX][0-9A-Fa-f]+)(?:[uUlL]{0,3})?$")

CPP_KEYWORDS = {
    "if", "for", "while", "switch", "return", "sizeof", "alignof", "noexcept",
    "static_assert", "catch", "new", "delete", "operator",
}

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
CONCEPTS_HPP = "Concepts.hpp"
CAST_HPP = "Cast.hpp"
STRONG_TYPE_HPP = "Strong_Type.hpp"

OPTIONAL_HEADER_TRIGGER_TOKENS = {
    TYPES_HPP: {
        "I8", "I16", "I32", "I64", "U8", "U16", "U32", "U64", "I128", "U128",
        "F32", "F64", "F80", "F128", "Vec", "Vec2", "Vec3",
        "Deque", "List", "Set", "MultiSet", "UnorderedSet", "Map", "MultiMap",
        "UnorderedMap", "Stack", "Queue", "PriorityQueue", "MinPriorityQueue",
        "Pair", "PII", "PLL", "PLD", "VI", "VLL",
        "VVI", "VVLL", "VB", "VS", "VU8", "VU32", "VU64", "VF", "VPII", "VPLL",
        "ordered_set", "ordered_multiset", "ordered_map", "gp_hash_table",
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
        "rall", "sz", "len", "eb", "elif",
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
    FAST_IO_HPP: {
        "fast_io", "load_input", "read_integer", "read_char", "read_string",
        "write_integer", "write_char", "write_string", "flush_output", "IOFlusher",
        "IN", "OUT", "FLUSH", "INT", "LL", "ULL", "STR", "CHR", "DBL", "VEC", "VV",
        "YES", "NO", "Yes", "No",
    },
    PBDS_HPP: {
        "ordered_set", "ordered_multiset", "ordered_map", "gp_hash_table",
        "tree_order_statistics_node_update", "find_by_order", "order_of_key",
    },
    BIT_OPS_HPP: {
        "popcount", "leading_zeros", "trailing_zeros", "bit_width", "bit_floor",
        "bit_ceil", "parity_sign", "kth_bit", "has_kth_bit", "bit_range",
        "subset_range",
    },
    CONTAINERS_HPP: {
        "binary_search", "binary_search_real", "argsort", "rearrange", "cumsum",
        "concat", "sum_as", "string_to_ints", "pop_val",
    },
    MOD_INT_HPP: {
        "ModInt", "mint", "mint2",
    },
    CONCEPTS_HPP: {
        "remove_cvref_t", "Integral", "SignedIntegral", "UnsignedIntegral",
        "NonBoolIntegral", "Floating", "Arithmetic", "IndexLike", "Enum", "Predicate",
    },
    CAST_HPP: {
        "to_underlying", "enum_cast", "narrow",
    },
    STRONG_TYPE_HPP: {
        "StrongType", "unwrap", "make_strong",
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
    BIT_OPS_HPP: {CONCEPTS_HPP},
    CONTAINERS_HPP: {CONCEPTS_HPP, MACROS_HPP},
    MOD_INT_HPP: {TYPES_HPP, CONSTANTS_HPP},
    CONCEPTS_HPP: {TYPES_HPP},
    CAST_HPP: {CONCEPTS_HPP},
    STRONG_TYPE_HPP: {TYPES_HPP},
}

MODULE_SECTION_SEPARATOR = (
    "//===----------------------------------------------------------------------===//\n"
    "/* Data Structures & Algorithms for the Problem */\n"
)


def extract_identifiers(text: str) -> set[str]:
    """Extract identifier tokens from source text."""
    return set(IDENTIFIER_RE.findall(_strip_non_code(text)))


def append_with_blank_separator(chunks: list[str], content: str) -> None:
    """Append a chunk ensuring exactly one blank line of separation."""
    if not content:
        return

    if chunks:
        last = chunks[-1]
        if not last.endswith("\n"):
            chunks.append("\n")
            last = "\n"
        if not last.endswith("\n\n"):
            chunks.append("\n")

    chunks.append(content)
    if not content.endswith("\n"):
        chunks.append("\n")


def parse_project_include_line(line: str, *, masked_line: str | None = None) -> str | None:
    """Parse a quoted project include, optionally guarded by a lexical mask line."""
    if masked_line is not None and not INCLUDE_DIRECTIVE_PREFIX_RE.match(masked_line):
        return None

    stripped = strip_comments(line).strip()
    match = PROJECT_INCLUDE_RE.match(stripped)
    if not match:
        return None
    return match.group(1)


def trim_outer_blank_lines(content: str) -> str:
    """Trim only leading/trailing blank lines, keeping internal spacing."""
    lines = content.splitlines()
    while lines and not lines[0].strip():
        lines.pop(0)
    while lines and not lines[-1].strip():
        lines.pop()
    return "\n".join(lines)


def collapse_redundant_blank_lines(content: str) -> str:
    """Collapse consecutive blank lines to at most one empty line."""
    lines = content.splitlines()
    normalized: list[str] = []
    prev_blank = False
    for line in lines:
        if line.strip() == "":
            if not prev_blank:
                normalized.append("")
            prev_blank = True
            continue

        normalized.append(line.rstrip())
        prev_blank = False

    return "\n".join(normalized)


def strip_module_docs_and_blank_lines(content: str) -> str:
    """
    Strip only documentation comments from already-inlined module sections.

    Kept intentionally conservative for readability:
    - remove doc block comments (/** ... */ and /*! ... */)
    - remove doc line comments (/// ... and //! ...)
    - remove trailing doc comments (///< ... and //!< ...)
    - preserve regular comments and most blank lines
    """
    masked, literals = _mask_code_literals(content)

    def _doc_block_replacer(match: re.Match[str]) -> str:
        block = match.group(0)
        newline_count = block.count("\n")
        return "\n" * newline_count if newline_count else " "

    masked = re.sub(
        r"/\*(?:\*|!).*?\*/",
        _doc_block_replacer,
        masked,
        flags=re.DOTALL,
    )
    masked = re.sub(r"(?m)^[ \t]*///.*$", "", masked)
    masked = re.sub(r"(?m)^[ \t]*//!.*$", "", masked)
    masked = re.sub(r"(?m)[ \t]*//[/!]<.*$", "", masked)

    content = _restore_code_literals(masked, literals)

    lines = content.splitlines()
    normalized: list[str] = []
    prev_blank = False
    for line in lines:
        stripped = line.strip()
        if stripped == "":
            if not prev_blank:
                normalized.append("")
            prev_blank = True
        else:
            normalized.append(line.rstrip())
            prev_blank = False
    return trim_outer_blank_lines("\n".join(normalized))


def strip_outer_include_guard(content: str) -> str:
    """
    Strip a conventional outer include guard wrapper from already inlined content.

    Supported forms:
    - #ifndef FOO / #define FOO ... #endif
    - #if !defined(FOO) / #define FOO ... #endif
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

    first = sig_idx[0]
    second = sig_idx[1]

    m_ifndef = INCLUDE_GUARD_IFNDEF_RE.match(lines[first].strip())
    m_ifdef = INCLUDE_GUARD_IF_DEFINED_RE.match(lines[first].strip())
    if not m_ifndef and not m_ifdef:
        return content

    guard_name = (m_ifndef or m_ifdef).group(1)
    m_define = DEFINE_DIRECTIVE_RE.match(lines[second].strip())
    if not m_define or m_define.group(1) != guard_name:
        return content

    depth = 0
    match_endif = None
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

    out = []
    for i, line in enumerate(lines):
        if i == first or i == second or i == match_endif:
            continue
        out.append(line)
    return "".join(out)


def collect_module_leaf_trigger_tokens(project_root: Path) -> dict[str, set[str]]:
    """Build trigger token map for granular module headers (modules/<domain>/*.hpp)."""
    modules_root = project_root / "modules"
    if not modules_root.is_dir():
        return {}

    token_map: dict[str, set[str]] = {}
    for domain_dir in modules_root.iterdir():
        if not domain_dir.is_dir():
            continue

        for header in sorted(domain_dir.glob("*.hpp")):
            if header.name == "_Common.hpp":
                continue
            try:
                content = header.read_text(encoding="utf-8")
            except OSError:
                continue

            rel = header.relative_to(project_root).as_posix()
            tokens = _extract_public_symbols(content)
            tokens.add(header.stem)
            token_map[rel] = tokens

    return token_map


def collect_transitive_template_deps(
    project_root: Path, start_header: Path, *, exclude_template_files: set[str] | None = None
) -> list[Path]:
    """
    Collect templates/*.hpp reached through local include recursion from a header.

    Order is discovery order (stable) to keep output deterministic and readable.
    """
    excluded = exclude_template_files or {"Preamble.hpp"}
    stack = [start_header.resolve()]
    visited: set[Path] = set()
    discovered_templates: list[Path] = []
    discovered_set: set[Path] = set()

    while stack:
        current = stack.pop()
        if current in visited or not current.is_file():
            continue
        visited.add(current)

        try:
            with open(current, "r", encoding="utf-8") as f:
                lines = f.readlines()
        except OSError:
            continue

        masked_lines = strip_non_code("".join(lines)).splitlines()
        for idx, line in enumerate(lines):
            masked_line = masked_lines[idx] if idx < len(masked_lines) else ""
            include_name = parse_project_include_line(line, masked_line=masked_line)
            if not include_name:
                continue
            target = resolve_project_include(project_root, current, include_name)
            if not target:
                continue

            rel = target.relative_to(project_root).as_posix()
            if (
                rel.startswith("templates/")
                and target.name not in excluded
                and target not in discovered_set
            ):
                discovered_templates.append(target)
                discovered_set.add(target)

            if rel.startswith(INLINE_ROOT_PREFIXES):
                stack.append(target)

    return discovered_templates


def prune_template_headers(files_to_include: list[Path], source_content: str) -> list[Path]:
    """
    Conservative tree-shaking for template headers.
    Keeps only headers whose symbols/macros appear in user code, while honoring
    hard dependencies between template headers.
    """
    if os.environ.get("CP_FLATTENER_DISABLE_PRUNING", "") == "1":
        return files_to_include

    used_identifiers = extract_identifiers(source_content)
    file_lookup = {path.name: path for path in files_to_include}
    selected_names = set(file_lookup.keys())

    for header, trigger_tokens in OPTIONAL_HEADER_TRIGGER_TOKENS.items():
        if header in selected_names and not (used_identifiers & trigger_tokens):
            selected_names.remove(header)

    changed = True
    while changed:
        changed = False
        for header in tuple(selected_names):
            for dep in HEADER_DEPENDENCIES.get(header, ()):
                if dep in file_lookup and dep not in selected_names:
                    selected_names.add(dep)
                    changed = True

    return [path for path in files_to_include if path.name in selected_names]


def resolve_project_include(
    project_root: Path, including_file: Path, include_name: str
) -> Path | None:
    """Resolve a quoted include within project_root, if it exists."""
    include_target = (including_file.parent / include_name).resolve()
    if not include_target.is_file():
        include_target = (project_root / include_name).resolve()

    if include_target.is_file() and (
        include_target == project_root or project_root in include_target.parents
    ):
        return include_target
    return None


def process_file_content(filepath: Path, *, preserve_includes: bool = False) -> str:
    """Process a single file and return its content."""
    if not filepath.is_file():
        return ""

    content_lines = []
    with open(filepath, "r", encoding="utf-8") as f:
        for line in f:
            stripped = line.strip()
            if stripped == "#pragma once":
                continue
            if not preserve_includes and stripped.startswith("#include"):
                continue
            content_lines.append(line)

    while content_lines and not content_lines[0].strip():
        content_lines.pop(0)
    while content_lines and not content_lines[-1].strip():
        content_lines.pop()

    return "".join(content_lines)


def _parse_macro_numeric_token(
    token: str, macro_values: dict[str, int | None]
) -> int | None:
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


def _update_macro_state_from_line(
    macro_values: dict[str, int | None], line: str
) -> None:
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
    value = _parse_macro_numeric_token(token, macro_values)
    macro_values[name] = value


def _evaluate_simple_if_expression(
    expr: str, macro_values: dict[str, int | None]
) -> bool | None:
    normalized = expr.strip()
    if not normalized:
        return None

    direct_value = _parse_macro_numeric_token(normalized, macro_values)
    if direct_value is not None:
        return direct_value != 0

    if normalized.startswith("!"):
        negated_value = _parse_macro_numeric_token(normalized[1:].strip(), macro_values)
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
        if pattern in (SIMPLE_DEFINED_RE, SIMPLE_DEFINED_ALT_RE):
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


def fold_simple_preprocessor_conditionals(
    content: str, macro_values: dict[str, int | None] | None
) -> str:
    """
    Fold simple preprocessor blocks when condition truth is known.

    Scope is intentionally conservative:
    - supports `#ifdef`, `#ifndef`, and `#if <simple-expr>`
    - folds only blocks without nested conditionals or `#else/#elif`
    - preserves unknown/complex conditionals unchanged
    """
    if not content:
        return content

    macro_state = dict(macro_values or {})
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
            _update_macro_state_from_line(macro_state, line)
            i += 1
            continue

        depth = 0
        end_idx = -1
        simple_block = True
        j = i + 1
        while j < n:
            candidate = lines[j].strip()
            if IF_DIRECTIVE_RE.match(candidate):
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
            if depth == 0 and ELSE_OR_ELIF_DIRECTIVE_RE.match(candidate):
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

        cond_result: bool | None = None
        if cond_kind == "ifdef":
            cond_result = cond_value in macro_state
        elif cond_kind == "ifndef":
            cond_result = cond_value not in macro_state
        else:
            cond_result = _evaluate_simple_if_expression(cond_value or "", macro_state)

        if cond_result is None:
            output.extend(lines[i : end_idx + 1])
            i = end_idx + 1
            continue

        if cond_result:
            body = lines[i + 1 : end_idx]
            output.extend(body)
            for body_line in body:
                _update_macro_state_from_line(macro_state, body_line)

        i = end_idx + 1

    return "".join(output)


def inline_local_header(
    project_root: Path,
    header_path: Path,
    inlined_headers: set[Path],
    *,
    used_identifiers: set[str] | None = None,
    module_leaf_tokens: dict[str, set[str]] | None = None,
    strip_module_docs: bool = False,
    strip_template_docs: bool = False,
    macro_values: dict[str, int | None] | None = None,
) -> str:
    """Inline project-local headers recursively and strip local includes/pragma once."""
    resolved = header_path.resolve()
    if not resolved.is_file():
        return ""
    if project_root not in resolved.parents:
        return ""
    if resolved in inlined_headers:
        return ""

    inlined_headers.add(resolved)
    content_lines: list[str] = []
    rel_self = resolved.relative_to(project_root).as_posix()
    is_module_umbrella = rel_self.startswith("modules/") and rel_self.count("/") == 1
    module_pruning_enabled = (
        os.environ.get("CP_FLATTENER_DISABLE_MODULE_PRUNING", "") != "1"
        and used_identifiers is not None
        and module_leaf_tokens is not None
    )

    with open(resolved, "r", encoding="utf-8") as f:
        file_text = f.read()

    file_lines = file_text.splitlines(keepends=True)
    masked_lines = strip_non_code(file_text).splitlines()
    for idx, line in enumerate(file_lines):
        masked_line = masked_lines[idx] if idx < len(masked_lines) else ""
        stripped = line.strip()
        if stripped == "#pragma once":
            continue

        include_name = parse_project_include_line(line, masked_line=masked_line)
        if include_name:
            include_target = resolve_project_include(
                project_root, resolved, include_name
            )
            if include_target:
                rel_target = include_target.relative_to(project_root).as_posix()
                if (
                    module_pruning_enabled
                    and is_module_umbrella
                    and rel_target in module_leaf_tokens
                ):
                    if not (used_identifiers & module_leaf_tokens[rel_target]):
                        continue

                if rel_target.startswith(INLINE_ROOT_PREFIXES):
                    nested_content = inline_local_header(
                        project_root,
                        include_target,
                        inlined_headers,
                        used_identifiers=used_identifiers,
                        module_leaf_tokens=module_leaf_tokens,
                        strip_module_docs=strip_module_docs,
                        strip_template_docs=strip_template_docs,
                        macro_values=macro_values,
                    )
                    if nested_content:
                        # Keep include replacement tight. Avoid adding extra blank lines.
                        if content_lines and not content_lines[-1].endswith("\n"):
                            content_lines.append("\n")
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

    output = "".join(content_lines)
    if rel_self.startswith("modules/"):
        output = strip_outer_include_guard(output)
        if strip_module_docs:
            output = strip_module_docs_and_blank_lines(output)
        output = collapse_redundant_blank_lines(output)
        output = trim_outer_blank_lines(output)
    elif rel_self.startswith("templates/"):
        output = strip_outer_include_guard(output)
        output = fold_simple_preprocessor_conditionals(output, macro_values)
        if strip_template_docs:
            output = strip_module_docs_and_blank_lines(output)
        output = collapse_redundant_blank_lines(output)
        output = trim_outer_blank_lines(output)
    return output


def _consume_quoted_literal(text: str, start: int) -> int:
    """Return index right after a C/C++ quoted literal starting at `start`."""
    n = len(text)
    quote = text[start]
    j = start + 1
    while j < n:
        ch = text[j]
        if ch == "\\" and j + 1 < n:
            j += 2
            continue
        if ch == quote:
            return j + 1
        # Keep scanner robust on malformed literals.
        if ch == "\n":
            return j
        j += 1
    return j


def _consume_raw_string_literal(text: str, start: int) -> int | None:
    """
    Return index right after a C++ raw string literal, or None if not at one.

    Supports prefixes: R, u8R, uR, UR, LR.
    """
    prefixes = ("u8R\"", "uR\"", "UR\"", "LR\"", "R\"")
    prefix = next((p for p in prefixes if text.startswith(p, start)), None)
    if not prefix:
        return None

    n = len(text)
    delim_start = start + len(prefix)
    j = delim_start
    while j < n and text[j] not in {"(", "\n"}:
        j += 1
    if j >= n or text[j] != "(":
        return None

    delimiter = text[delim_start:j]
    terminator = ")" + delimiter + "\""
    end_pos = text.find(terminator, j + 1)
    if end_pos < 0:
        return n
    return end_pos + len(terminator)


def _scan_cpp_text(
    text: str, *, keep_comments: bool, literal_mode: str
) -> tuple[str, dict[str, str]]:
    """
    Single-pass scanner used by flattener lexical helpers.

    `literal_mode`:
      - "space": replace literal payload with spaces/newlines
      - "token": replace each literal with a stable placeholder token
      - "keep":  keep literals as-is
    """
    out: list[str] = []
    literals: dict[str, str] = {}
    token_id = 0
    i = 0
    n = len(text)

    while i < n:
        c = text[i]

        # Line comment.
        if c == "/" and i + 1 < n and text[i + 1] == "/":
            j = i + 2
            while j < n and text[j] != "\n":
                j += 1
            if keep_comments:
                out.append(text[i:j])
            i = j
            continue

        # Block comment.
        if c == "/" and i + 1 < n and text[i + 1] == "*":
            j = i + 2
            while j + 1 < n and not (text[j] == "*" and text[j + 1] == "/"):
                j += 1
            j = j + 2 if j + 1 < n else n
            block = text[i:j]
            if keep_comments:
                out.append(block)
            else:
                for ch in block:
                    out.append("\n" if ch == "\n" else " ")
            i = j
            continue

        # Raw string literal.
        raw_end = _consume_raw_string_literal(text, i)
        if raw_end is not None:
            literal = text[i:raw_end]
            if literal_mode == "token":
                token = f"__CP_FLATTENER_LIT_{token_id}__"
                token_id += 1
                literals[token] = literal
                out.append(token)
            elif literal_mode == "space":
                for ch in literal:
                    out.append("\n" if ch == "\n" else " ")
            else:  # keep
                out.append(literal)
            i = raw_end
            continue

        # String/char literal.
        if c in {'"', "'"}:
            j = _consume_quoted_literal(text, i)
            literal = text[i:j]
            if literal_mode == "token":
                token = f"__CP_FLATTENER_LIT_{token_id}__"
                token_id += 1
                literals[token] = literal
                out.append(token)
            elif literal_mode == "space":
                for ch in literal:
                    out.append("\n" if ch == "\n" else " ")
            else:  # keep
                out.append(literal)
            i = j
            continue

        out.append(c)
        i += 1

    return "".join(out), literals


def _strip_non_code(text: str) -> str:
    """Remove strings/comments so token scans only see code-like identifiers."""
    return strip_non_code(text)


def strip_comments(text: str) -> str:
    """Remove comments while preserving literals and line structure."""
    return _scan_cpp_text(text, keep_comments=False, literal_mode="keep")[0]


def strip_non_code(text: str) -> str:
    """Public wrapper to remove strings/comments while preserving line structure."""
    return _scan_cpp_text(text, keep_comments=False, literal_mode="space")[0]


def _mask_code_literals(text: str) -> tuple[str, dict[str, str]]:
    """
    Replace string/char literals with temporary tokens while preserving comments.

    This lets regex-based comment stripping operate safely without touching
    literal payloads that may contain comment-like substrings.
    """
    return _scan_cpp_text(text, keep_comments=True, literal_mode="token")


def _restore_code_literals(masked: str, literals: dict[str, str]) -> str:
    """Restore literal tokens emitted by _mask_code_literals."""
    for token, literal in literals.items():
        masked = masked.replace(token, literal)
    return masked


def _extract_public_symbols(header_content: str) -> set[str]:
    """
    Extract top-level exported symbols from a header.

    The extraction is conservative: we only consider declarations at brace depth 0.
    """
    content = _strip_non_code(header_content)
    symbols: set[str] = set()
    depth = 0

    for raw in content.splitlines():
        line = raw.strip()
        if depth == 0 and line:
            match = STRUCT_CLASS_RE.match(line)
            if match:
                symbols.add(match.group(1))

            match = USING_RE.match(line)
            if match:
                symbols.add(match.group(1))

            match = FUNC_LIKE_RE.search(line)
            if match:
                name = match.group(1)
                if name not in CPP_KEYWORDS:
                    symbols.add(name)

        depth += raw.count("{")
        depth -= raw.count("}")
        if depth < 0:
            depth = 0

    return symbols
