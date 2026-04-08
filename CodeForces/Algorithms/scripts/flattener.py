#!/usr/bin/env python3

from __future__ import annotations

import os
import re
import shutil
import subprocess
import sys
from dataclasses import dataclass
from enum import Enum, auto
from pathlib import Path

from flattener_core.flattener_helpers import (
    INLINE_ROOT_PREFIXES,
    MODULE_SECTION_SEPARATOR,
    append_with_blank_separator,
    collect_module_leaf_trigger_tokens,
    collect_transitive_template_deps,
    extract_prefix_before_base_include,
    extract_identifiers,
    fold_simple_preprocessor_conditionals,
    inline_local_header,
    parse_project_include_line,
    process_file_content,
    prune_template_headers_with_policy,
    resolve_project_include,
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
VALIDATION_COMPILER_CANDIDATES = ("g++-15", "g++-14", "g++-13", "g++", "clang++")
TYPES_SECTION_END_MARKER = "#endif // __TYPES__"
MacroValueMap = dict[str, int | None]
ModuleLeafTokenMap = dict[str, set[str]]


class FlattenerMode(Enum):
    """Supported output strategies for flattened source generation."""

    COMPACT = "compact"
    SAFE = "safe"
    AUTO = "auto"


class ValidationStatus(Enum):
    """Availability and result of compile-check validation."""

    PASSED = auto()
    FAILED = auto()
    UNAVAILABLE = auto()


@dataclass(frozen=True, slots=True)
class FlattenContext:
    """Immutable input context shared across flattening pipeline stages."""

    source_file: Path
    source_content: str
    source_lines: tuple[str, ...]
    source_masked_lines: tuple[str, ...]
    project_root: Path
    templates_dir: Path
    preamble_path: Path
    preamble_resolved: Path
    need_mapping: dict[str, list[str]]
    used_identifiers: frozenset[str]
    source_module_includes: tuple[Path, ...]
    module_leaf_tokens: ModuleLeafTokenMap | None
    strip_module_docs: bool
    strip_template_docs: bool
    macro_values: MacroValueMap


@dataclass(frozen=True, slots=True)
class FlattenedTemplateBundle:
    """Rendered template section content plus effective macro state."""

    preamble_content: str
    template_sections: tuple[str, ...]
    effective_macro_values: MacroValueMap
    inlined_headers: frozenset[Path]


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

    def _is_enabled(name: str) -> bool:
        value = macro_values.get(name)
        return value is not None and value != 0

    # Mirror templates/Base.hpp I/O profile shorthands so conditional folding
    # can preserve profile-driven includes in flattened output.
    if _is_enabled("CP_IO_PROFILE_SIMPLE"):
        macro_values.setdefault("NEED_IO", 1)

    if _is_enabled("CP_IO_PROFILE_FAST_MINIMAL"):
        macro_values.setdefault("NEED_FAST_IO", 1)

    if _is_enabled("CP_IO_PROFILE_FAST_EXTENDED"):
        macro_values.setdefault("NEED_FAST_IO", 1)
        macro_values.setdefault("NEED_MOD_INT", 1)
        macro_values.setdefault("NEED_TYPE_SAFETY", 1)
        macro_values.setdefault("CP_FAST_IO_ENABLE_MODINT", 1)
        macro_values.setdefault("CP_FAST_IO_ENABLE_STRONG_TYPE", 1)

    return macro_values
def _env_flag_enabled(name: str, default: bool = False) -> bool:
    """Interpret common truthy environment variable spellings."""

    raw = os.environ.get(name)
    if raw is None:
        return default
    return raw.strip().lower() in {"1", "true", "yes", "on"}


def _resolve_flattener_mode() -> FlattenerMode:
    raw_mode = os.environ.get("CP_FLATTENER_MODE", "compact").strip().lower()
    try:
        return FlattenerMode(raw_mode)
    except ValueError:
        pass

    sys.stderr.write(
        f"Warning: unknown CP_FLATTENER_MODE='{raw_mode}'. "
        "Using 'compact'.\n"
    )
    return FlattenerMode.COMPACT


def _parse_positive_int_env(name: str, default_value: int) -> int:
    raw = os.environ.get(name, "").strip()
    if not raw:
        return default_value
    try:
        parsed = int(raw)
    except ValueError:
        return default_value
    return parsed if parsed > 0 else default_value


def _resolve_validation_compiler() -> str | None:
    forced = os.environ.get("CP_FLATTENER_VALIDATION_COMPILER", "").strip()
    if forced:
        return forced

    for candidate in VALIDATION_COMPILER_CANDIDATES:
        resolved = shutil.which(candidate)
        if resolved:
            return resolved
    return None


def _validate_flattened_output(flattened_source: str) -> ValidationStatus:
    """
    Compile-check flattened output.

    Returns:
    - PASSED      : syntax validation succeeded
    - FAILED      : validation ran and failed
    - UNAVAILABLE : validation unavailable/disabled
    """

    if not _env_flag_enabled("CP_FLATTENER_AUTO_VALIDATE", default=True):
        return ValidationStatus.UNAVAILABLE

    compiler = _resolve_validation_compiler()
    if not compiler:
        return ValidationStatus.UNAVAILABLE

    timeout_ms = _parse_positive_int_env("CP_FLATTENER_VALIDATION_TIMEOUT_MS", 3500)
    command = [compiler, "-std=gnu++20", "-fsyntax-only", "-x", "c++", "-"]
    try:
        result = subprocess.run(
            command,
            input=flattened_source,
            text=True,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
            check=False,
            timeout=timeout_ms / 1000.0,
        )
    except (OSError, subprocess.TimeoutExpired):
        return ValidationStatus.UNAVAILABLE

    if result.returncode == 0:
        return ValidationStatus.PASSED
    return ValidationStatus.FAILED


def _collect_needed_template_headers(
    ctx: FlattenContext,
    *,
    need_macros_found: set[str],
    enable_template_pruning: bool,
) -> list[Path]:
    """Resolve and prune template headers implied by enabled NEED_* macros."""

    if "NEED_FAST_IO" in need_macros_found and "NEED_IO" in need_macros_found:
        need_macros_found.remove("NEED_IO")

    files_to_include: list[Path] = []
    included_names: set[str] = set()
    for macro, files in ctx.need_mapping.items():
        if macro not in need_macros_found:
            continue
        for filename in files:
            if filename in included_names:
                continue
            files_to_include.append(ctx.templates_dir / filename)
            included_names.add(filename)

    files_to_include = prune_template_headers_with_policy(
        files_to_include,
        ctx.source_content,
        enable_pruning=enable_template_pruning,
    )

    # Keep template deps from modules in template section (before module separator).
    included_paths = {p.resolve() for p in files_to_include}
    for include_target in ctx.source_module_includes:
        for dep in collect_transitive_template_deps(
            ctx.project_root,
            include_target,
            exclude_template_files={"Preamble.hpp"},
        ):
            dep_resolved = dep.resolve()
            if dep_resolved in included_paths:
                continue
            files_to_include.append(dep)
            included_paths.add(dep_resolved)

    # Never inline Preamble as a template section; it is emitted once at the top.
    return [p for p in files_to_include if p.resolve() != ctx.preamble_resolved]


def _place_global_std_namespace(template_sections: list[str]) -> tuple[str, ...]:
    """Place `using namespace std;` after the Types section when available."""

    namespace_line = "using namespace std;"
    for idx, section in enumerate(template_sections):
        if TYPES_SECTION_END_MARKER not in section:
            continue
        template_sections[idx] = section.replace(
            TYPES_SECTION_END_MARKER,
            f"{TYPES_SECTION_END_MARKER}\n\n{namespace_line}",
            1,
        )
        return tuple(template_sections)

    template_sections.append(namespace_line)
    return tuple(template_sections)


def _render_template_bundle(
    ctx: FlattenContext,
    *,
    enable_template_pruning: bool,
    enable_module_pruning: bool,
) -> FlattenedTemplateBundle:
    """Render preamble and template sections independently from source rewrite."""

    need_macros_found = extract_need_macros_from_source(
        ctx.source_content, ctx.need_mapping.keys()
    )
    effective_macro_values = dict(ctx.macro_values)
    if "NEED_FAST_IO" in need_macros_found:
        effective_macro_values["CP_FAST_IO_NAMESPACE_DEFINED"] = 1

    files_to_include = _collect_needed_template_headers(
        ctx,
        need_macros_found=need_macros_found,
        enable_template_pruning=enable_template_pruning,
    )

    # Expand template headers recursively so template-level shared includes
    # (e.g. IO_Defs.hpp) are preserved in flattened output.
    inlined_headers = {ctx.preamble_resolved}
    template_sections: list[str] = []
    for filepath in files_to_include:
        content = inline_local_header(
            ctx.project_root,
            filepath,
            inlined_headers,
            used_identifiers=set(ctx.used_identifiers),
            module_leaf_tokens=ctx.module_leaf_tokens,
            strip_module_docs=ctx.strip_module_docs,
            strip_template_docs=ctx.strip_template_docs,
            macro_values=effective_macro_values,
            enable_module_pruning=enable_module_pruning,
        )
        if content:
            template_sections.append(content)

    preamble_content = process_file_content(ctx.preamble_path, preserve_includes=True)
    if ctx.strip_template_docs:
        preamble_content = strip_module_docs_and_blank_lines(preamble_content)

    rendered_sections = tuple(template_sections)
    if effective_macro_values.get("CP_USE_GLOBAL_STD_NAMESPACE") not in (None, 0):
        rendered_sections = _place_global_std_namespace(template_sections)

    return FlattenedTemplateBundle(
        preamble_content=preamble_content,
        template_sections=rendered_sections,
        effective_macro_values=effective_macro_values,
        inlined_headers=frozenset(inlined_headers),
    )


def _render_flattened_source(
    ctx: FlattenContext,
    bundle: FlattenedTemplateBundle,
    *,
    enable_module_pruning: bool,
) -> str:
    """Rewrite original source using already-rendered template sections."""

    output_lines: list[str] = []
    skip_need_defines = False
    seen_base_include = False

    # Prevent duplicated template/module bodies when sections are already expanded.
    module_section_emitted = False
    inlined_headers = set(bundle.inlined_headers)

    for idx, line in enumerate(ctx.source_lines):
        stripped = line.strip()
        masked_line = (
            ctx.source_masked_lines[idx] if idx < len(ctx.source_masked_lines) else ""
        )
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
            and need_define_match.group(1) in ctx.need_mapping
        ):
            skip_need_defines = True
            continue

        if is_base_include:
            seen_base_include = True
            while output_lines and not output_lines[-1].strip():
                output_lines.pop()

            if output_lines and output_lines[-1].strip():
                output_lines.append("\n")

            output_lines.append(bundle.preamble_content)
            if bundle.preamble_content and not bundle.preamble_content.endswith("\n\n"):
                output_lines.append("\n")

            for i, section in enumerate(bundle.template_sections):
                if i > 0:
                    output_lines.append("\n")
                output_lines.append(section)
                if section and not section.endswith("\n"):
                    output_lines.append("\n")
            continue

        if include_name:
            include_target = resolve_project_include(
                ctx.project_root,
                ctx.source_file,
                include_name,
            )
            if include_target:
                rel = include_target.relative_to(ctx.project_root).as_posix()
                if rel.startswith(INLINE_ROOT_PREFIXES):
                    inlined = inline_local_header(
                        ctx.project_root,
                        include_target,
                        inlined_headers,
                        used_identifiers=set(ctx.used_identifiers),
                        module_leaf_tokens=ctx.module_leaf_tokens,
                        strip_module_docs=ctx.strip_module_docs,
                        strip_template_docs=ctx.strip_template_docs,
                        macro_values=bundle.effective_macro_values,
                        enable_module_pruning=enable_module_pruning,
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

    return "".join(output_lines)


def build_flattened_output(
    ctx: FlattenContext,
    *,
    enable_template_pruning: bool,
    enable_module_pruning: bool,
) -> str:
    """Build flattened output through explicit dependency/render/rewrite stages."""

    bundle = _render_template_bundle(
        ctx,
        enable_template_pruning=enable_template_pruning,
        enable_module_pruning=enable_module_pruning,
    )
    return _render_flattened_source(
        ctx,
        bundle,
        enable_module_pruning=enable_module_pruning,
    )


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

    source_content = source_file.read_text(encoding="utf-8")
    source_lines = tuple(source_content.splitlines(keepends=True))
    source_masked_lines = tuple(strip_non_code(source_content).splitlines())

    source_prefix = extract_prefix_before_base_include(source_content)
    strict_profile_enabled = bool(
        re.search(
            r"^\s*#\s*define\s+CP_TEMPLATE_PROFILE_STRICT\b",
            source_prefix,
            re.MULTILINE,
        )
    )
    relaxed_profile_enabled = bool(
        re.search(
            r"^\s*#\s*define\s+CP_TEMPLATE_PROFILE_RELAXED\b",
            source_prefix,
            re.MULTILINE,
        )
    )
    macro_values = extract_macro_values_from_source(
        source_prefix,
        strict_profile_enabled=strict_profile_enabled,
        relaxed_profile_enabled=relaxed_profile_enabled,
    )

    used_identifiers = frozenset(extract_identifiers(source_content))
    strip_module_docs = _env_flag_enabled("CP_FLATTENER_STRIP_MODULE_DOCS")
    strip_template_docs = _env_flag_enabled("CP_FLATTENER_STRIP_TEMPLATE_DOCS")
    if strict_profile_enabled and not relaxed_profile_enabled:
        strip_template_docs = True

    source_module_includes: list[Path] = []
    has_module_umbrella_include = False
    for idx, line in enumerate(source_lines):
        masked_line = source_masked_lines[idx] if idx < len(source_masked_lines) else ""
        include_name = parse_project_include_line(line, masked_line=masked_line)
        if not include_name:
            continue
        include_target = resolve_project_include(project_root, source_file, include_name)
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

    mode = _resolve_flattener_mode()
    ctx = FlattenContext(
        source_file=source_file,
        source_content=source_content,
        source_lines=source_lines,
        source_masked_lines=source_masked_lines,
        project_root=project_root,
        templates_dir=templates_dir,
        preamble_path=preamble_path,
        preamble_resolved=preamble_resolved,
        need_mapping=need_mapping,
        used_identifiers=used_identifiers,
        source_module_includes=tuple(source_module_includes),
        module_leaf_tokens=module_leaf_tokens,
        strip_module_docs=strip_module_docs,
        strip_template_docs=strip_template_docs,
        macro_values=macro_values,
    )

    match mode:
        case FlattenerMode.SAFE:
            print(
                build_flattened_output(
                    ctx,
                    enable_template_pruning=False,
                    enable_module_pruning=False,
                ),
                end="",
            )
        case FlattenerMode.COMPACT:
            print(
                build_flattened_output(
                    ctx,
                    enable_template_pruning=True,
                    enable_module_pruning=True,
                ),
                end="",
            )
        case FlattenerMode.AUTO:
            compact_output = build_flattened_output(
                ctx,
                enable_template_pruning=True,
                enable_module_pruning=True,
            )
            compact_valid = _validate_flattened_output(compact_output)
            if compact_valid is ValidationStatus.FAILED:
                safe_output = build_flattened_output(
                    ctx,
                    enable_template_pruning=False,
                    enable_module_pruning=False,
                )
                safe_valid = _validate_flattened_output(safe_output)
                if safe_valid is not ValidationStatus.FAILED:
                    print(safe_output, end="")
                    return
            print(compact_output, end="")


if __name__ == "__main__":
    main()
