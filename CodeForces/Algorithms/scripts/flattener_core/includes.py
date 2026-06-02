"""Project-local include resolution, inlining, and dependency traversal.

This is the include subsystem of the flattener. ``parse_project_include_line``
and ``resolve_project_include`` answer "what file does this ``#include``
point to?"; ``inline_local_header`` recursively expands a header while
honoring ``#pragma once``, conditional active-region tracking, and the
umbrella-module leaf pruner. ``collect_transitive_template_deps`` and
``order_template_headers_by_dependencies`` discover and topologically order
the template-section closure for a given user source.

Layer position: depends on ``lexer``, ``macros``, ``preprocessor``; consumed
by ``headers`` and by every pipeline stage that emits or rewrites includes.
"""

from __future__ import annotations

import os
import re
from pathlib import Path

from flattener_core.lexer import (
    collapse_redundant_blank_lines,
    strip_comments,
    strip_module_docs_and_blank_lines,
    strip_non_code,
    trim_outer_blank_lines,
)
from flattener_core.macros import (
    MacroValueMap,
    update_macro_state_from_line,
)
from flattener_core.ppexpr import evaluate_condition
from flattener_core.preprocessor import (
    ELIF_DIRECTIVE_RE,
    ELSE_DIRECTIVE_RE,
    ENDIF_DIRECTIVE_RE,
    IF_EXPR_DIRECTIVE_RE,
    IFDEF_DIRECTIVE_RE,
    IFNDEF_DIRECTIVE_RE,
    ConditionalScanner,
    fold_simple_preprocessor_conditionals,
    strip_outer_include_guard,
)
from flattener_core.symbols import ModuleLeafTokenMap

PROJECT_INCLUDE_RE = re.compile(r'^\s*#include\s+"([^"]+)"\s*$')
INCLUDE_DIRECTIVE_PREFIX_RE = re.compile(r"^\s*#\s*include\b")
INLINE_ROOT_PREFIXES = ("modules/", "templates/")

_MODULE_SECTION_BANNER = (
    "//===----------------------------------------------------------------------===//"
)


def _apply_conditional_directive(
    scanner: ConditionalScanner, directive: str, macro_state: MacroValueMap
) -> bool:
    """Feed one masked directive line to ``scanner``; return whether it was one.

    Conditions are evaluated against ``macro_state`` under the closed-namespace
    rule (the traversals always run on the resolved configuration), so the
    discovery/inlining loops and the folder agree on which branches are live.
    """

    match = IFDEF_DIRECTIVE_RE.match(directive)
    if match:
        scanner.open(
            evaluate_condition("ifdef", match.group(1), macro_state, closed_namespace=True)
        )
        return True
    match = IFNDEF_DIRECTIVE_RE.match(directive)
    if match:
        scanner.open(
            evaluate_condition("ifndef", match.group(1), macro_state, closed_namespace=True)
        )
        return True
    match = IF_EXPR_DIRECTIVE_RE.match(directive)
    if match:
        scanner.open(evaluate_condition("if", match.group(1), macro_state, closed_namespace=True))
        return True
    if ELIF_DIRECTIVE_RE.match(directive):
        scanner.open_elif()
        return True
    if ELSE_DIRECTIVE_RE.match(directive):
        scanner.open_else()
        return True
    if ENDIF_DIRECTIVE_RE.match(directive):
        scanner.close()
        return True
    return False


def parse_project_include_line(line: str, *, masked_line: str | None = None) -> str | None:
    """Parse a quoted project include, optionally guarded by a lexical mask line."""

    if masked_line is not None and not INCLUDE_DIRECTIVE_PREFIX_RE.match(masked_line):
        return None

    stripped = strip_comments(line).strip()
    match = PROJECT_INCLUDE_RE.match(stripped)
    if not match:
        return None
    return match.group(1)


def resolve_project_include(
    project_root: Path, including_file: Path, include_name: str
) -> Path | None:
    """Resolve a quoted include within ``project_root``, if it exists."""

    include_target = (including_file.parent / include_name).resolve()
    if not include_target.is_file():
        include_target = (project_root / include_name).resolve()

    resolved_root = project_root.resolve()
    if include_target.is_file() and (
        include_target == resolved_root or resolved_root in include_target.parents
    ):
        return include_target
    return None


def extract_prefix_before_base_include(source_content: str) -> str:
    """Return source prefix before the first ``templates/Base.hpp`` include."""

    comments_stripped = strip_comments(source_content)
    masked_lines = strip_non_code(source_content).splitlines()
    prefix_lines: list[str] = []
    for idx, raw_line in enumerate(comments_stripped.splitlines()):
        masked_line = masked_lines[idx] if idx < len(masked_lines) else ""
        include_name = parse_project_include_line(raw_line, masked_line=masked_line)
        if include_name and include_name.replace("\\", "/").endswith("templates/Base.hpp"):
            break
        prefix_lines.append(raw_line)
    return "\n".join(prefix_lines)


def collect_transitive_template_deps(
    project_root: Path,
    start_header: Path,
    *,
    exclude_template_files: set[str] | None = None,
    macro_values: MacroValueMap | None = None,
) -> list[Path]:
    """Collect ``templates/*.hpp`` reached through local include recursion.

    Order is discovery order (stable) to keep output deterministic and readable.
    Excluded template files are traversal barriers: they are neither emitted
    nor scanned for nested includes.
    """

    excluded = exclude_template_files or {"Preamble.hpp"}
    macro_state: MacroValueMap = macro_values or {}
    stack = [start_header.resolve()]
    visited: set[Path] = set()
    discovered_templates: list[Path] = []
    discovered_set: set[Path] = set()

    resolved_root = project_root.resolve()
    while stack:
        current = stack.pop()
        if current in visited or not current.is_file():
            continue
        visited.add(current)

        try:
            with open(current, encoding="utf-8") as f:
                lines = f.readlines()
        except OSError:
            continue

        masked_lines = strip_non_code("".join(lines)).splitlines()
        scanner = ConditionalScanner()

        for idx, line in enumerate(lines):
            masked_line = masked_lines[idx] if idx < len(masked_lines) else ""
            if _apply_conditional_directive(scanner, masked_line.strip(), macro_state):
                continue
            if not scanner.active:
                continue

            include_name = parse_project_include_line(line, masked_line=masked_line)
            if not include_name:
                continue
            target = resolve_project_include(resolved_root, current, include_name)
            if not target:
                continue

            rel = target.relative_to(resolved_root).as_posix()
            if rel.startswith("templates/") and target.name in excluded:
                continue

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


def _active_project_include_targets(
    project_root: Path,
    current: Path,
    *,
    macro_values: MacroValueMap | None = None,
) -> list[Path]:
    """Return project-local include targets active under simple macro state."""

    if not current.is_file():
        return []

    macro_state: MacroValueMap = dict(macro_values or {})
    resolved_root = project_root.resolve()
    try:
        lines = current.read_text(encoding="utf-8").splitlines()
    except OSError:
        return []

    masked_lines = strip_non_code("\n".join(lines)).splitlines()
    scanner = ConditionalScanner()
    targets: list[Path] = []

    for idx, line in enumerate(lines):
        masked_line = masked_lines[idx] if idx < len(masked_lines) else ""
        if _apply_conditional_directive(scanner, masked_line.strip(), macro_state):
            continue
        if not scanner.active:
            continue

        include_name = parse_project_include_line(line, masked_line=masked_line)
        if include_name:
            target = resolve_project_include(resolved_root, current, include_name)
            if target:
                targets.append(target)

        update_macro_state_from_line(macro_state, masked_line)

    return targets


def order_template_headers_by_dependencies(
    project_root: Path,
    headers: list[Path],
    *,
    macro_values: MacroValueMap | None = None,
) -> list[Path]:
    """Topologically order already-selected template section headers.

    Intentionally does *not* expand the dependency closure: context-sensitive
    helper headers such as ``IO_Composite.hpp`` must stay nested inside their
    owning backend header instead of becoming standalone sections.
    """

    selected: list[Path] = []
    selected_set: set[Path] = set()
    resolved_root = project_root.resolve()

    for header in headers:
        resolved = header.resolve()
        if not resolved.is_file():
            continue
        if resolved_root not in resolved.parents:
            continue
        if resolved in selected_set:
            continue
        selected.append(resolved)
        selected_set.add(resolved)

    priority = {path: idx for idx, path in enumerate(selected)}
    deps: dict[Path, list[Path]] = {path: [] for path in selected}

    for header in selected:
        for target in _active_project_include_targets(
            resolved_root, header, macro_values=macro_values
        ):
            resolved_target = target.resolve()
            if resolved_target in selected_set:
                deps[header].append(resolved_target)

    ordered: list[Path] = []
    visiting: set[Path] = set()
    visited: set[Path] = set()

    def visit(path: Path) -> None:
        if path in visited or path in visiting:
            return
        visiting.add(path)
        for dep in sorted(deps[path], key=lambda p: priority[p]):
            visit(dep)
        visiting.remove(path)
        visited.add(path)
        ordered.append(path)

    for header in selected:
        visit(header)

    return ordered


def _trailing_newline_count(chunks: list[str]) -> int:
    count = 0
    for chunk in reversed(chunks):
        idx = len(chunk) - 1
        while idx >= 0 and chunk[idx] == "\n":
            count += 1
            idx -= 1
        if idx >= 0:
            break
    return count


def inline_local_header(
    project_root: Path,
    header_path: Path,
    inlined_headers: set[Path],
    *,
    used_identifiers: set[str] | None = None,
    module_leaf_tokens: ModuleLeafTokenMap | None = None,
    strip_module_docs: bool = False,
    strip_template_docs: bool = False,
    macro_values: MacroValueMap | None = None,
    enable_module_pruning: bool = True,
) -> str:
    """Inline project-local headers recursively and strip local includes/``#pragma once``."""

    resolved = header_path.resolve()
    if not resolved.is_file():
        return ""
    resolved_root = project_root.resolve()
    if resolved_root not in resolved.parents:
        return ""
    if resolved in inlined_headers:
        return ""

    inlined_headers.add(resolved)
    content_lines: list[str] = []

    rel_self = resolved.relative_to(resolved_root).as_posix()
    is_module_umbrella = rel_self.startswith("modules/") and rel_self.count("/") == 1
    module_pruning_enabled = (
        enable_module_pruning
        and os.environ.get("CP_FLATTENER_DISABLE_MODULE_PRUNING", "") != "1"
        and used_identifiers is not None
        and module_leaf_tokens is not None
    )

    with open(resolved, encoding="utf-8") as f:
        file_text = f.read()

    file_lines = file_text.splitlines(keepends=True)
    masked_lines = strip_non_code(file_text).splitlines()
    macro_state: MacroValueMap = dict(macro_values or {})
    # ``ConditionalScanner`` tracks active/certain branch state under the closed
    # namespace: the inliner inlines includes only in live branches and mutates
    # the authoritative macro state only where every enclosing branch is
    # statically decided (a ``#define``/``#undef`` in an uncertain
    # compiler-conditional branch must not poison the model). The conditional
    # directives themselves are kept; the post-pass folder collapses whatever is
    # decidable.
    scanner = ConditionalScanner()

    for idx, line in enumerate(file_lines):
        masked_line = masked_lines[idx] if idx < len(masked_lines) else ""
        stripped = line.strip()
        if stripped == "#pragma once":
            continue

        if _apply_conditional_directive(scanner, masked_line.strip(), macro_state):
            content_lines.append(line)
            continue

        include_name = parse_project_include_line(line, masked_line=masked_line)
        if include_name:
            if not scanner.active:
                content_lines.append(line)
                continue

            include_target = resolve_project_include(resolved_root, resolved, include_name)
            if include_target:
                rel_target = include_target.relative_to(resolved_root).as_posix()
                if (
                    module_pruning_enabled
                    and is_module_umbrella
                    and module_leaf_tokens is not None
                    and used_identifiers is not None
                    and rel_target in module_leaf_tokens
                    and not (used_identifiers & module_leaf_tokens[rel_target])
                ):
                    continue

                if rel_target.startswith(INLINE_ROOT_PREFIXES):
                    nested_content = inline_local_header(
                        resolved_root,
                        include_target,
                        inlined_headers,
                        used_identifiers=used_identifiers,
                        module_leaf_tokens=module_leaf_tokens,
                        strip_module_docs=strip_module_docs,
                        strip_template_docs=strip_template_docs,
                        macro_values=macro_state,
                        enable_module_pruning=enable_module_pruning,
                    )
                    if nested_content:
                        if content_lines and not content_lines[-1].endswith("\n"):
                            content_lines.append("\n")
                        if (
                            nested_content.startswith(_MODULE_SECTION_BANNER)
                            and _trailing_newline_count(content_lines) < 2
                        ):
                            content_lines.append("\n")
                        content_lines.append(nested_content)
                        if not nested_content.endswith("\n"):
                            content_lines.append("\n")
                    continue

            content_lines.append(line)
            if scanner.mutable:
                update_macro_state_from_line(macro_state, masked_line)
            continue

        content_lines.append(line)
        if scanner.mutable:
            update_macro_state_from_line(macro_state, masked_line)

    while content_lines and not content_lines[0].strip():
        content_lines.pop(0)
    while content_lines and not content_lines[-1].strip():
        content_lines.pop()

    output = "".join(content_lines)
    if rel_self.startswith("modules/"):
        output = strip_outer_include_guard(output)
        output = fold_simple_preprocessor_conditionals(output, macro_values, closed_namespace=True)
        if strip_module_docs:
            output = strip_module_docs_and_blank_lines(output)
        output = collapse_redundant_blank_lines(output)
        output = trim_outer_blank_lines(output)
    elif rel_self.startswith("templates/"):
        output = strip_outer_include_guard(output)
        output = fold_simple_preprocessor_conditionals(output, macro_values, closed_namespace=True)
        if strip_template_docs:
            output = strip_module_docs_and_blank_lines(output)
        output = collapse_redundant_blank_lines(output)
        output = trim_outer_blank_lines(output)
    return output
