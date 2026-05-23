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
    collapse_redundant_blank_lines,
    collect_module_leaf_trigger_tokens,
    collect_transitive_template_deps,
    extract_prefix_before_base_include,
    extract_identifiers,
    fold_simple_preprocessor_conditionals,
    inline_local_header,
    order_template_headers_by_dependencies,
    parse_project_include_line,
    prune_template_headers_with_policy,
    resolve_project_include,
    strip_comments,
    strip_non_code,
    trim_outer_blank_lines,
)
from need_resolver import extract_need_macros_from_source, load_need_mapping
from profile_registry import load_registry

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
MAIN_SOLVER_SECTION_MARKER = "/* Main Solver Function */"
MacroValueMap = dict[str, int | None]
ModuleLeafTokenMap = dict[str, set[str]]


class FlattenerMode(Enum):
    """Supported output strategies for flattened source generation."""

    COMPACT = "compact"
    SAFE = "safe"
    AUTO = "auto"
    SUBMISSION = "submission"


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
    """Parse a simple #define value expression into an int or None.

    Strips inline comments and trailing type suffixes (e.g. `u`, `L`).
    Falls back to looking up the token in ``macro_values`` when it is not
    a numeric literal.
    """
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
    source_prefix_content: str,
    *,
    strict_profile_enabled: bool,
    relaxed_profile_enabled: bool,
    warn_stream=None,
) -> dict[str, int | None]:
    """Extract simple macro values from user source for conservative conditional folding.

    Defines inside conditional blocks the folder cannot evaluate are skipped.
    When ``warn_stream`` is provided, the skipped names are reported once.
    """

    macro_values: dict[str, int | None] = {}
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
            define_match = MACRO_DEFINE_RE.match(stripped)
            if define_match:
                skipped.append(define_match.group(1))
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

    if warn_stream is not None and skipped:
        warn_stream.write(
            "warning: ignoring conditional #defines (guard unresolved): "
            + ", ".join(sorted(set(skipped))) + "\n"
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

    composite_tokens = {
        "Vec", "Vec2", "Vec3", "Vec4", "VecI32", "VecI64", "VecBool", "VecStr",
        "Pair", "PairI32", "PairI64", "Tuple", "VEC", "VV",
    }
    if extract_identifiers(source_prefix_content) & composite_tokens:
        macro_values["CP_IO_ENABLE_COMPOSITE"] = 1

    return macro_values
def _env_flag_enabled(name: str, default: bool = False) -> bool:
    """Interpret common truthy environment variable spellings."""

    raw = os.environ.get(name)
    if raw is None:
        return default
    return raw.strip().lower() in {"1", "true", "yes", "on"}


def _resolve_flattener_mode() -> FlattenerMode:
    """Determine the active flattening mode from the environment.

    Defaults to ``COMPACT``. Emits a warning and still falls back to
    ``COMPACT`` when ``CP_FLATTENER_MODE`` contains an unknown value.
    """
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
    """Read a positive integer from an environment variable.

    Returns ``default_value`` when the variable is missing, empty,
    non-numeric, or not strictly positive.
    """
    raw = os.environ.get(name, "").strip()
    if not raw:
        return default_value
    try:
        parsed = int(raw)
    except ValueError:
        return default_value
    return parsed if parsed > 0 else default_value


def _resolve_validation_compiler() -> str | None:
    """Return an absolute path to a C++ compiler for syntax validation.

    Prefers ``CP_FLATTENER_VALIDATION_COMPILER`` when set, otherwise
    searches a hard-coded candidate list (g++-15, g++-14, g++-13, g++, clang++).
    """
    forced = os.environ.get("CP_FLATTENER_VALIDATION_COMPILER", "").strip()
    if forced:
        return forced

    for candidate in VALIDATION_COMPILER_CANDIDATES:
        resolved = shutil.which(candidate)
        if resolved:
            return resolved
    return None


def _validate_flattened_output(
    flattened_source: str,
    *,
    force: bool | None = None,
    validation_compiler: str | None = None,
    validation_std: str = "gnu++20",
) -> ValidationStatus:
    """
    Compile-check flattened output.

    Returns:
    - PASSED      : syntax validation succeeded
    - FAILED      : validation ran and failed
    - UNAVAILABLE : validation unavailable/disabled
    """

    if force is False:
        return ValidationStatus.UNAVAILABLE
    if force is None and not _env_flag_enabled("CP_FLATTENER_AUTO_VALIDATE", default=True):
        return ValidationStatus.UNAVAILABLE

    compiler = validation_compiler or _resolve_validation_compiler()
    if not compiler:
        return ValidationStatus.UNAVAILABLE

    timeout_ms = _parse_positive_int_env("CP_FLATTENER_VALIDATION_TIMEOUT_MS", 3500)
    command = [compiler, f"-std={validation_std}", "-fsyntax-only", "-x", "c++", "-"]
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


def _prepare_submission_output(flattened_source: str) -> str:
    """Minimize flattened source for online judge submission."""

    stripped = strip_comments(flattened_source)
    compacted = collapse_redundant_blank_lines(stripped)
    return trim_outer_blank_lines(compacted) + "\n"


def _collect_needed_template_headers(
    ctx: FlattenContext,
    *,
    need_macros_found: set[str],
    enable_template_pruning: bool,
    macro_values: dict[str, int | None],
) -> list[Path]:
    """Resolve and prune template headers implied by enabled NEED_* macros."""

    # Mirror Base_profiles.hpp: NEED_FAST_IO wins over NEED_IO.
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
            macro_values=macro_values,
        ):
            dep_resolved = dep.resolve()
            if dep_resolved in included_paths:
                continue
            files_to_include.append(dep)
            included_paths.add(dep_resolved)

    # Never inline Preamble as a template section; it is emitted once at the top.
    files_to_include = [
        p for p in files_to_include if p.resolve() != ctx.preamble_resolved
    ]
    return order_template_headers_by_dependencies(
        ctx.project_root,
        files_to_include,
        macro_values=macro_values,
    )


def _render_template_bundle(
    ctx: FlattenContext,
    *,
    enable_template_pruning: bool,
    enable_module_pruning: bool,
) -> FlattenedTemplateBundle:
    """Render preamble and template sections independently from source rewrite."""

    need_macros_found = extract_need_macros_from_source(
        ctx.source_content, ctx.need_mapping.keys(), warn_stream=sys.stderr
    )
    effective_macro_values = dict(ctx.macro_values)
    if "NEED_FAST_IO" in need_macros_found:
        effective_macro_values["CP_FAST_IO_NAMESPACE_DEFINED"] = 1

    files_to_include = _collect_needed_template_headers(
        ctx,
        need_macros_found=need_macros_found,
        enable_template_pruning=enable_template_pruning,
        macro_values=effective_macro_values,
    )

    preamble_inlined_headers: set[Path] = set()
    preamble_content = inline_local_header(
        ctx.project_root,
        ctx.preamble_path,
        preamble_inlined_headers,
        used_identifiers=set(ctx.used_identifiers),
        module_leaf_tokens=ctx.module_leaf_tokens,
        strip_module_docs=False,
        strip_template_docs=ctx.strip_template_docs,
        macro_values=effective_macro_values,
        enable_module_pruning=enable_module_pruning,
    )

    # Expand template headers recursively so template-level shared includes
    # (e.g. IO_Defs.hpp) are preserved in flattened output.
    inlined_headers = set(preamble_inlined_headers)
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

    return FlattenedTemplateBundle(
        preamble_content=preamble_content,
        template_sections=tuple(template_sections),
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
    skip_blank_after_sections = False
    should_emit_std_namespace = (
        bundle.effective_macro_values.get("CP_USE_GLOBAL_STD_NAMESPACE") not in (None, 0)
        and "using namespace std;" not in ctx.source_content
    )
    defer_std_namespace_to_solver_section = (
        should_emit_std_namespace and MAIN_SOLVER_SECTION_MARKER in ctx.source_content
    )

    # Prevent duplicated template/module bodies when sections are already expanded.
    module_section_emitted = False
    inlined_headers = set(bundle.inlined_headers)

    for idx, line in enumerate(ctx.source_lines):
        stripped = line.strip()
        if skip_blank_after_sections:
            if not stripped:
                continue
            skip_blank_after_sections = False
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

            # Emit preamble + each template section with exactly one blank
            # line separator; the inliners already strip trailing newlines.
            preamble_text = bundle.preamble_content.rstrip("\n")
            if preamble_text:
                output_lines.append(preamble_text)
                output_lines.append("\n\n")

            for section in bundle.template_sections:
                section_text = section.rstrip("\n")
                if not section_text:
                    continue
                output_lines.append(section_text)
                output_lines.append("\n\n")
            if should_emit_std_namespace and not defer_std_namespace_to_solver_section:
                output_lines.append("using namespace std;\n")
                should_emit_std_namespace = False
            skip_blank_after_sections = True
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
        if stripped == MAIN_SOLVER_SECTION_MARKER and defer_std_namespace_to_solver_section:
            output_lines.append("\nusing namespace std;\n")
            should_emit_std_namespace = False
            defer_std_namespace_to_solver_section = False

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


def _build_arg_parser() -> "argparse.ArgumentParser":
    """Construct the argparse parser for the flattener CLI."""
    import argparse

    parser = argparse.ArgumentParser(
        description="Flatten a competitive-programming source file into a single submission.",
    )
    parser.add_argument("source_file", type=Path, help="path to the .cpp to flatten")
    parser.add_argument(
        "--mode",
        choices=[m.value for m in FlattenerMode],
        default=None,
        help="output mode (default: env CP_FLATTENER_MODE or 'compact')",
    )
    parser.add_argument(
        "--validate",
        choices=("auto", "on", "off"),
        default=None,
        help="syntax-check the output (default: env CP_FLATTENER_AUTO_VALIDATE)",
    )
    parser.add_argument(
        "--validation-compiler",
        default=None,
        help="override the compiler used for --validate (default: env or auto-detect)",
    )
    parser.add_argument(
        "--validation-std",
        default=None,
        help="override the -std= flag used for --validate (default: gnu++20)",
    )
    parser.add_argument(
        "--strip-docs",
        action="store_true",
        help="strip template documentation comments (independent of CP_TEMPLATE_PROFILE_STRICT)",
    )
    parser.add_argument(
        "--no-prune",
        action="store_true",
        help="disable template-header tree-shaking (equivalent to safe mode for that axis)",
    )
    parser.add_argument(
        "--no-module-prune",
        action="store_true",
        help="disable per-module-leaf pruning",
    )
    return parser


def main() -> None:
    """Flatten one source file by expanding template and local project includes."""

    import argparse

    parser = _build_arg_parser()
    try:
        args = parser.parse_args()
    except SystemExit:
        raise

    source_file = args.source_file
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
        warn_stream=sys.stderr,
    )

    used_identifiers = frozenset(extract_identifiers(source_content))
    if used_identifiers & {
        "Vec", "Vec2", "Vec3", "Vec4", "VecI32", "VecI64", "VecBool", "VecStr",
        "Pair", "PairI32", "PairI64", "Tuple", "VEC", "VV",
    }:
        macro_values["CP_IO_ENABLE_COMPOSITE"] = 1

    strip_module_docs = _env_flag_enabled("CP_FLATTENER_STRIP_MODULE_DOCS")
    strip_template_docs = (
        args.strip_docs or _env_flag_enabled("CP_FLATTENER_STRIP_TEMPLATE_DOCS")
    )

    mode = FlattenerMode(args.mode) if args.mode else _resolve_flattener_mode()
    if args.no_prune:
        mode = FlattenerMode.SAFE
    if mode is FlattenerMode.SUBMISSION:
        macro_values["CP_USE_BITS_HEADER"] = 1
        strip_module_docs = True
        strip_template_docs = True

    enable_module_pruning = not args.no_module_prune and mode is not FlattenerMode.SAFE

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

    validation_std = args.validation_std or os.environ.get("CP_FLATTENER_VALIDATION_STD", "gnu++20")
    validation_compiler = args.validation_compiler or os.environ.get("CP_FLATTENER_VALIDATION_COMPILER", "")
    validate_mode = args.validate or "auto"

    def _run_validation(source: str) -> ValidationStatus:
        force = {"on": True, "off": False, "auto": None}.get(validate_mode)
        return _validate_flattened_output(
            source,
            force=force,
            validation_compiler=validation_compiler or None,
            validation_std=validation_std,
        )

    match mode:
        case FlattenerMode.SAFE:
            print(
                build_flattened_output(
                    ctx,
                    enable_template_pruning=False,
                    enable_module_pruning=enable_module_pruning,
                ),
                end="",
            )
        case FlattenerMode.COMPACT:
            print(
                build_flattened_output(
                    ctx,
                    enable_template_pruning=True,
                    enable_module_pruning=enable_module_pruning,
                ),
                end="",
            )
        case FlattenerMode.AUTO:
            compact_output = build_flattened_output(
                ctx,
                enable_template_pruning=True,
                enable_module_pruning=enable_module_pruning,
            )
            compact_valid = _run_validation(compact_output)
            if compact_valid is ValidationStatus.FAILED:
                sys.stderr.write(
                    "warning: compact output failed syntax check; trying safe variant\n"
                )
                safe_output = build_flattened_output(
                    ctx,
                    enable_template_pruning=False,
                    enable_module_pruning=enable_module_pruning,
                )
                safe_valid = _run_validation(safe_output)
                if safe_valid is not ValidationStatus.FAILED:
                    print(safe_output, end="")
                    return
                sys.stderr.write(
                    "warning: safe variant also failed syntax check; emitting compact output anyway\n"
                )
            print(compact_output, end="")
        case FlattenerMode.SUBMISSION:
            submission_output = build_flattened_output(
                ctx,
                enable_template_pruning=True,
                enable_module_pruning=enable_module_pruning,
            )
            print(_prepare_submission_output(submission_output), end="")


if __name__ == "__main__":
    main()
