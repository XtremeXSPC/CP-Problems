"""Helper utilities shared by the template flattener workflow."""

from __future__ import annotations

import os
import re
from pathlib import Path

# fmt: off
PROJECT_INCLUDE_RE = re.compile(r'^\s*#include\s+"([^"]+)"\s*$')
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
IF_DIRECTIVE_RE = re.compile(r"^\s*#\s*(if|ifdef|ifndef)\b")
ENDIF_DIRECTIVE_RE = re.compile(r"^\s*#\s*endif\b")

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


def trim_outer_blank_lines(content: str) -> str:
    """Trim only leading/trailing blank lines, keeping internal spacing."""
    lines = content.splitlines()
    while lines and not lines[0].strip():
        lines.pop(0)
    while lines and not lines[-1].strip():
        lines.pop()
    return "\n".join(lines)


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

        for line in lines:
            match = PROJECT_INCLUDE_RE.match(line.strip())
            if not match:
                continue
            target = resolve_project_include(project_root, current, match.group(1))
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


def inline_local_header(
    project_root: Path,
    header_path: Path,
    inlined_headers: set[Path],
    *,
    used_identifiers: set[str] | None = None,
    module_leaf_tokens: dict[str, set[str]] | None = None,
    strip_module_docs: bool = False,
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
                        )
                        if nested_content:
                            append_with_blank_separator(content_lines, nested_content)
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
