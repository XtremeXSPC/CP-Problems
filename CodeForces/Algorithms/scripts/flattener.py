#!/usr/bin/env python3

from __future__ import annotations

import os
import re
import sys
from pathlib import Path

from flattener_core.flattener_helpers import (
    INLINE_ROOT_PREFIXES,
    MODULE_SECTION_SEPARATOR,
    append_with_blank_separator,
    collect_module_leaf_trigger_tokens,
    collect_transitive_template_deps,
    extract_identifiers,
    fold_simple_preprocessor_conditionals,
    inline_local_header,
    parse_project_include_line,
    process_file_content,
    prune_template_headers,
    resolve_project_include,
    strip_comments,
    strip_non_code,
    strip_module_docs_and_blank_lines,
)
from need_resolver import extract_need_macros_from_source, load_need_mapping

MACRO_DEFINE_RE = re.compile(
    r"^\s*#\s*define\s+([A-Za-z_]\w*)(?:\s+(.*?))?\s*$"
)
MACRO_UNDEF_RE = re.compile(r"^\s*#\s*undef\s+([A-Za-z_]\w*)\s*$")
NUMERIC_LITERAL_RE = re.compile(
    r"^[+-]?(?:0|[1-9]\d*|0[xX][0-9A-Fa-f]+)(?:[uUlL]{0,3})?$"
)
NEED_DEFINE_LINE_RE = re.compile(r"^\s*#\s*define\s+(NEED_\w+)\b")
IF_DIRECTIVE_RE = re.compile(r"^\s*#\s*(if|ifdef|ifndef)\b")
ENDIF_DIRECTIVE_RE = re.compile(r"^\s*#\s*endif\b")
ELSE_OR_ELIF_DIRECTIVE_RE = re.compile(r"^\s*#\s*(else|elif)\b")


def _parse_macro_value(
    value_expr: str | None, macro_values: dict[str, int | None]
) -> int | None:
    if value_expr is None:
        return 1

    token = value_expr.split("//", 1)[0]
    token = token.split("/*", 1)[0].strip()
    if not token:
        return 1

    token = token.split()[0]
    if NUMERIC_LITERAL_RE.fullmatch(token):
        suffix_trimmed = re.sub(r"[uUlL]+$", "", token)
        try:
            return int(suffix_trimmed, 0)
        except ValueError:
            return None

    return macro_values.get(token)


def extract_macro_values_from_source(
    source_prefix_content: str, *, strict_profile_enabled: bool, relaxed_profile_enabled: bool
) -> dict[str, int | None]:
    """Extract simple macro values from user source for conservative conditional folding."""
    macro_values: dict[str, int | None] = {}
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
            continue

        undef_match = MACRO_UNDEF_RE.match(stripped)
        if undef_match:
            macro_values.pop(undef_match.group(1), None)
            continue

        define_match = MACRO_DEFINE_RE.match(stripped)
        if not define_match:
            continue

        name, value_expr = define_match.group(1), define_match.group(2)
        macro_values[name] = _parse_macro_value(value_expr, macro_values)

    # Keep strict-mode defaults aligned with templates/Base.hpp.
    if strict_profile_enabled and not relaxed_profile_enabled:
        strict_defaults = {
            "CP_STRICT_TEMPLATE_NEEDS": 1,
            "CP_CORE_ENABLE_MATH": 0,
            "CP_USE_GLOBAL_STD_NAMESPACE": 0,
            "CP_ENABLE_LEGACY_IO_VEC_MACROS": 0,
            "CP_ENABLE_DEPRECATED_COMPAT": 0,
        }
        for name, value in strict_defaults.items():
            macro_values.setdefault(name, value)

    return macro_values


def extract_prefix_before_base_include(source_content: str) -> str:
    """Return source prefix (with comments stripped) before `#include \"templates/Base.hpp\"`."""
    comments_stripped = strip_comments(source_content)
    stripped_lines = comments_stripped.splitlines()
    masked_lines = strip_non_code(source_content).splitlines()
    prefix_lines: list[str] = []
    for idx, raw_line in enumerate(stripped_lines):
        masked_line = masked_lines[idx] if idx < len(masked_lines) else ""
        include_name = parse_project_include_line(raw_line, masked_line=masked_line)
        if include_name:
            include_target = include_name.replace("\\", "/")
            if include_target.endswith("templates/Base.hpp"):
                break
        prefix_lines.append(raw_line)
    return "\n".join(prefix_lines)


def main() -> None:
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
    preamble_path = templates_dir / "Preamble.hpp"
    preamble_resolved = preamble_path.resolve()

    with open(source_file, "r", encoding="utf-8") as f:
        source_content = f.read()
    source_lines = source_content.splitlines(keepends=True)
    source_masked_lines = strip_non_code(source_content).splitlines()

    source_prefix = extract_prefix_before_base_include(source_content)
    strict_profile_enabled = bool(
        re.search(r"^\s*#\s*define\s+CP_TEMPLATE_PROFILE_STRICT\b", source_prefix, re.MULTILINE)
    )
    relaxed_profile_enabled = bool(
        re.search(r"^\s*#\s*define\s+CP_TEMPLATE_PROFILE_RELAXED\b", source_prefix, re.MULTILINE)
    )
    macro_values = extract_macro_values_from_source(
        source_prefix,
        strict_profile_enabled=strict_profile_enabled,
        relaxed_profile_enabled=relaxed_profile_enabled,
    )

    used_identifiers = extract_identifiers(source_content)
    strip_module_docs = os.environ.get("CP_FLATTENER_STRIP_MODULE_DOCS", "") == "1"
    strip_template_docs = os.environ.get("CP_FLATTENER_STRIP_TEMPLATE_DOCS", "") == "1"
    if strict_profile_enabled and not relaxed_profile_enabled:
        strip_template_docs = True

    source_module_includes: list[Path] = []
    has_module_umbrella_include = False
    for idx, line in enumerate(source_lines):
        masked_line = source_masked_lines[idx] if idx < len(source_masked_lines) else ""
        include_name = parse_project_include_line(line, masked_line=masked_line)
        if not include_name:
            continue
        include_target = resolve_project_include(
            project_root, source_file, include_name
        )
        if not include_target:
            continue
        rel = include_target.relative_to(project_root).as_posix()
        if not rel.startswith("modules/"):
            continue
        source_module_includes.append(include_target)
        if rel.count("/") == 1:
            has_module_umbrella_include = True

    module_leaf_tokens = (
        collect_module_leaf_trigger_tokens(project_root)
        if has_module_umbrella_include
        else None
    )

    need_mapping = load_need_mapping(base_template_path)
    if not need_mapping:
        sys.stderr.write(
            f"Error: Unable to derive NEED_* mapping from {base_template_path}\n"
        )
        sys.exit(1)

    need_macros_found = extract_need_macros_from_source(
        source_content, need_mapping.keys()
    )
    if "NEED_FAST_IO" in need_macros_found and "NEED_IO" in need_macros_found:
        need_macros_found.remove("NEED_IO")

    files_to_include: list[Path] = []
    included_names = set()
    for macro, files in need_mapping.items():
        if macro not in need_macros_found:
            continue
        for filename in files:
            if filename in included_names:
                continue
            files_to_include.append(templates_dir / filename)
            included_names.add(filename)

    files_to_include = prune_template_headers(files_to_include, source_content)

    # Keep template deps from modules in template section (before module separator).
    included_paths = {p.resolve() for p in files_to_include}
    for include_target in source_module_includes:
        for dep in collect_transitive_template_deps(
            project_root, include_target, exclude_template_files={"Preamble.hpp"}
        ):
            dep_resolved = dep.resolve()
            if dep_resolved in included_paths:
                continue
            files_to_include.append(dep)
            included_paths.add(dep_resolved)

    # Never inline Preamble as a template section; it is emitted once at the top.
    files_to_include = [p for p in files_to_include if p.resolve() != preamble_resolved]

    # Expand template headers recursively so template-level shared includes
    # (e.g. IO_Defs.hpp) are preserved in flattened output.
    inlined_headers = {preamble_resolved}
    template_sections = []
    for filepath in files_to_include:
        content = inline_local_header(
            project_root,
            filepath,
            inlined_headers,
            used_identifiers=used_identifiers,
            module_leaf_tokens=module_leaf_tokens,
            strip_module_docs=strip_module_docs,
            strip_template_docs=strip_template_docs,
            macro_values=macro_values,
        )
        if content:
            template_sections.append(content)

    preamble_content = process_file_content(preamble_path, preserve_includes=True)
    if strip_template_docs:
        preamble_content = strip_module_docs_and_blank_lines(preamble_content)

    output_lines: list[str] = []
    skip_need_defines = False
    seen_base_include = False

    # Prevent duplicated template/module bodies when sections are already expanded.
    module_section_emitted = False

    for idx, line in enumerate(source_lines):
        stripped = line.strip()
        masked_line = source_masked_lines[idx] if idx < len(source_masked_lines) else ""
        include_name = parse_project_include_line(line, masked_line=masked_line)
        is_base_include = False
        if include_name:
            is_base_include = include_name.replace("\\", "/").endswith(
                "templates/Base.hpp"
            )

        need_define_match = NEED_DEFINE_LINE_RE.match(stripped)
        if (
            not seen_base_include
            and need_define_match
            and need_define_match.group(1) in need_mapping
        ):
            skip_need_defines = True
            continue

        if is_base_include:
            seen_base_include = True
            while output_lines and not output_lines[-1].strip():
                output_lines.pop()

            output_lines.append("\n")
            output_lines.append(preamble_content)
            if preamble_content and not preamble_content.endswith("\n\n"):
                output_lines.append("\n")

            for i, section in enumerate(template_sections):
                if i > 0:
                    output_lines.append("\n")
                output_lines.append(section)
                if section and not section.endswith("\n"):
                    output_lines.append("\n")
            continue

        if include_name:
            include_target = resolve_project_include(
                project_root, source_file, include_name
            )
            if include_target:
                rel = include_target.relative_to(project_root).as_posix()
                if rel.startswith(INLINE_ROOT_PREFIXES):
                    inlined = inline_local_header(
                        project_root,
                        include_target,
                        inlined_headers,
                        used_identifiers=used_identifiers,
                        module_leaf_tokens=module_leaf_tokens,
                        strip_module_docs=strip_module_docs,
                        strip_template_docs=strip_template_docs,
                        macro_values=macro_values,
                    )
                    if inlined:
                        if rel.startswith("modules/") and not module_section_emitted:
                            append_with_blank_separator(
                                output_lines, MODULE_SECTION_SEPARATOR
                            )
                            module_section_emitted = True
                        append_with_blank_separator(output_lines, inlined)
                    continue

        if (
            skip_need_defines
            and stripped
            and not stripped.startswith("#define NEED_")
        ):
            skip_need_defines = False
            if output_lines and output_lines[-1].strip():
                output_lines.append("\n")

        output_lines.append(line)

    print("".join(output_lines), end="")


if __name__ == "__main__":
    main()
