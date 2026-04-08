#!/usr/bin/env python3
"""
Module Testing Framework for Competitive Programming templates.

This tool validates that configured NEED_* module combinations compile.
"""

from __future__ import annotations

import argparse
import json
import shutil
import subprocess
import sys
import tempfile
import time
from dataclasses import dataclass, field
from datetime import datetime
from pathlib import Path
from typing import Literal, TypedDict

from need_resolver import load_need_mapping


class TemplateConfigPayload(TypedDict, total=False):
    """Validated subset of `.template_config.json` consumed by this tool."""

    compiler: str
    compiler_flags: list[str]


ModuleTestKind = Literal["individual", "combination", "strict_profile", "benchmark"]

DEFAULT_NEED_MACROS = (
    "NEED_CORE",
    "NEED_MATH",
    "NEED_IO",
    "NEED_BIT_OPS",
    "NEED_MOD_INT",
    "NEED_CONTAINERS",
    "NEED_TYPE_SAFETY",
    "NEED_HASHING",
)

DEFAULT_EMPTY_MAIN = "int main() { return 0; }"

INDIVIDUAL_TEST_SNIPPETS = {
    "NEED_CORE": "int main() { VI v; return (int)v.size(); }",
    "NEED_MATH": "int main() { return div_floor<I64>(-3, 2) == -2 ? 0 : 1; }",
    "NEED_IO": "int main() { LL(n); OUT(n); return 0; }",
    "NEED_BIT_OPS": "int main() { I32 x = popcount(15); return x == 4 ? 0 : 1; }",
    "NEED_MOD_INT": "int main() { mint x(5), y(3); mint z = x * y; return (int)I64(z); }",
    "NEED_CONTAINERS": "int main() { VI v = {3,1,2}; auto idx = argsort(v); return (int)idx.size(); }",
    "NEED_TYPE_SAFETY": (
        "CP_DECLARE_STRONG_TYPE(NodeId, I32); "
        "int main() { NodeId id(3); ++id; auto sum = id + NodeId(2); "
        "auto maybe = cp::cast::try_narrow<I32>(cp::unwrap(sum)); "
        "return maybe && *maybe == 6 ? 0 : 1; }"
    ),
    "NEED_HASHING": (
        "int main() { cp::hashing::FastHashMap<I64, I32> freq; freq[42] = 1; "
        "cp::hashing::FastHashSet<I64> st; st.insert(42); "
        "return (freq[42] == 1 && st.contains(42)) ? 0 : 1; }"
    ),
}

COMBINATION_CANDIDATES = (
    (
        ("NEED_CORE", "NEED_IO"),
        "Core + I/O",
        "int main() { LL(n); OUT(n); return 0; }",
    ),
    (
        ("NEED_CORE", "NEED_BIT_OPS"),
        "Core + Bit Operations",
        "int main() { I64 x = 15; I32 cnt = popcount(x); return cnt == 4 ? 0 : 1; }",
    ),
    (
        ("NEED_CORE", "NEED_MOD_INT"),
        "Core + ModInt",
        "int main() { mint a(5), b(3); mint c = a * b; return (int)I64(c); }",
    ),
    (
        ("NEED_CORE", "NEED_CONTAINERS"),
        "Core + Containers",
        "int main() { VI v = {3,1,2}; auto idx = argsort(v); return (int)idx.size(); }",
    ),
    (
        ("NEED_CORE", "NEED_TYPE_SAFETY"),
        "Core + Type Safety",
        "CP_DECLARE_STRONG_TYPE(NodeId, I32); int main() { NodeId id(7); return cp::unwrap(id) == 7 ? 0 : 1; }",
    ),
    (
        ("NEED_CORE", "NEED_HASHING"),
        "Core + Hashing",
        "int main() { cp::hashing::FastHashMap<I64, I64> mp; mp[1] = 2; return mp[1] == 2 ? 0 : 1; }",
    ),
    (
        ("NEED_CORE", "NEED_IO", "NEED_CONTAINERS"),
        "Core + I/O + Containers",
        "int main() { VI v = {1,2,3}; OUT((I64)v.size()); return 0; }",
    ),
    (
        (
            "NEED_CORE",
            "NEED_IO",
            "NEED_BIT_OPS",
            "NEED_MOD_INT",
            "NEED_CONTAINERS",
        ),
        "All modules",
        "int main() { mint x(5); I32 bits = popcount(15); VI v = {1,2}; OUT(bits); return (int)v.size(); }",
    ),
)

STRICT_PROFILE_CANDIDATES = (
    (
        ("NEED_CORE", "NEED_IO"),
        "Strict + Core + I/O",
        "int main() { LL(n); OUT(n); return 0; }",
    ),
    (
        ("NEED_CORE", "NEED_FAST_IO", "NEED_TYPE_SAFETY"),
        "Strict + Fast I/O + Type Safety",
        "CP_DECLARE_STRONG_TYPE(NodeId, I32); int main() { NodeId id(9); OUT(cp::unwrap(id)); return 0; }",
    ),
)

BENCHMARK_CANDIDATES = (
    (("NEED_CORE",), "Core only"),
    (("NEED_CORE", "NEED_IO"), "Core + I/O"),
    (("NEED_CORE", "NEED_IO", "NEED_CONTAINERS"), "Core + I/O + Containers"),
    (
        (
            "NEED_CORE",
            "NEED_IO",
            "NEED_BIT_OPS",
            "NEED_MOD_INT",
            "NEED_CONTAINERS",
        ),
        "All modules",
    ),
)


@dataclass(frozen=True, slots=True)
class CompilationOutcome:
    """Result of compiling one generated test translation unit."""

    success: bool
    error: str | None = None
    elapsed_seconds: float | None = None


@dataclass(frozen=True, slots=True)
class ModuleTestCase:
    """One logical module test scenario to compile and report."""

    kind: ModuleTestKind
    description: str
    macros: tuple[str, ...]
    source: str = DEFAULT_EMPTY_MAIN
    extra_defines: tuple[str, ...] = ()
    headers: tuple[str, ...] = ()

    @property
    def display_label(self) -> str:
        """Human-readable label for console output."""
        if self.kind == "individual" and self.headers:
            return f"{self.description} ({', '.join(self.headers)})"
        return self.description

    @property
    def display_width(self) -> int:
        """Preferred alignment width for console summaries."""
        return 45 if self.kind == "individual" else 30


@dataclass(frozen=True, slots=True)
class ModuleTestRecord:
    """Serializable record emitted in JSON reports."""

    kind: ModuleTestKind
    description: str
    macros: tuple[str, ...]
    success: bool
    error: str | None
    headers: tuple[str, ...] = ()
    defines: tuple[str, ...] = ()
    elapsed_seconds: float | None = None
    timestamp: str = field(default_factory=lambda: datetime.now().isoformat())

    def to_dict(self) -> dict[str, object]:
        """Serialize one result record to a JSON-friendly mapping."""
        payload: dict[str, object] = {
            "kind": self.kind,
            "description": self.description,
            "success": self.success,
            "error": self.error,
            "timestamp": self.timestamp,
        }
        if len(self.macros) == 1:
            payload["macro"] = self.macros[0]
        else:
            payload["macros"] = list(self.macros)
        if self.headers:
            payload["headers"] = list(self.headers)
        if self.defines:
            payload["defines"] = list(self.defines)
        if self.elapsed_seconds is not None:
            payload["elapsed_seconds"] = self.elapsed_seconds
        return payload


@dataclass(frozen=True, slots=True)
class CompilerInvocation:
    """Compiler executable and flags used to compile generated probes."""

    compiler: str
    compiler_flags: tuple[str, ...]
    include_dir: Path

    def compile_source(self, source: str, *, timeout_seconds: float = 10.0) -> CompilationOutcome:
        """Compile one source string and return normalized result metadata."""
        with tempfile.TemporaryDirectory() as tmpdir:
            source_file = Path(tmpdir) / "probe.cpp"
            source_file.write_text(source, encoding="utf-8")

            command = [
                self.compiler,
                *self.compiler_flags,
                f"-I{self.include_dir}",
                str(source_file),
            ]
            start = time.perf_counter()
            try:
                result = subprocess.run(
                    command,
                    capture_output=True,
                    text=True,
                    timeout=timeout_seconds,
                    check=False,
                )
            except subprocess.TimeoutExpired:
                return CompilationOutcome(success=False, error="Compilation timeout")
            except OSError as exc:
                return CompilationOutcome(
                    success=False,
                    error=f"Unable to execute compiler '{self.compiler}': {exc}",
                )

            elapsed = time.perf_counter() - start
            if result.returncode == 0:
                return CompilationOutcome(success=True, elapsed_seconds=elapsed)
            return CompilationOutcome(
                success=False,
                error=result.stderr,
                elapsed_seconds=elapsed,
            )


def _load_template_config(templates_dir: Path) -> TemplateConfigPayload:
    """Load and lightly validate `.template_config.json` if present."""

    config_path = templates_dir.parent / ".template_config.json"
    if not config_path.is_file():
        return {}

    try:
        payload = json.loads(config_path.read_text(encoding="utf-8"))
    except (json.JSONDecodeError, OSError):
        return {}

    if not isinstance(payload, dict):
        return {}

    config: TemplateConfigPayload = {}
    compiler = payload.get("compiler")
    if isinstance(compiler, str) and compiler.strip():
        config["compiler"] = compiler.strip()

    compiler_flags = payload.get("compiler_flags")
    if isinstance(compiler_flags, list) and all(isinstance(flag, str) for flag in compiler_flags):
        config["compiler_flags"] = list(compiler_flags)

    return config


def _build_compiler_flags(config: TemplateConfigPayload) -> tuple[str, ...]:
    """Build compiler flags from config plus tester defaults."""

    config_flags = list(config.get("compiler_flags", ()))
    if not any(flag.startswith("-std=") for flag in config_flags):
        config_flags.insert(0, "-std=c++23")
    if "-fsyntax-only" not in config_flags:
        config_flags.append("-fsyntax-only")
    return tuple(config_flags)


def _compiler_supports_std_headers(compiler: str) -> bool:
    """Quick probe to ensure compiler supports template header style."""

    with tempfile.TemporaryDirectory() as tmpdir:
        probe_file = Path(tmpdir) / "probe.cpp"
        probe_file.write_text(
            "#include <bits/stdc++.h>\nint main() { return 0; }\n",
            encoding="utf-8",
        )
        try:
            result = subprocess.run(
                [compiler, "-std=c++23", "-fsyntax-only", str(probe_file)],
                capture_output=True,
                text=True,
                timeout=5,
                check=False,
            )
        except (OSError, subprocess.TimeoutExpired):
            return False
        return result.returncode == 0


def _select_compiler(config: TemplateConfigPayload) -> str:
    """Pick the most suitable compiler for CP templates."""

    configured = config.get("compiler")
    candidates = [configured, "g++-15", "g++-14", "g++-13", "g++", "c++"]
    seen: set[str] = set()
    for candidate in candidates:
        if not candidate or candidate in seen:
            continue
        seen.add(candidate)
        resolved = shutil.which(candidate)
        if resolved and _compiler_supports_std_headers(resolved):
            return resolved
    return configured or "g++"


def _discover_need_mapping(templates_dir: Path) -> dict[str, list[str]]:
    """Load NEED_* mapping from Base.hpp; fallback to known defaults."""

    mapping = load_need_mapping(templates_dir / "Base.hpp")
    if mapping:
        return dict(mapping)
    return {macro: [] for macro in DEFAULT_NEED_MACROS}


class ModuleTester:
    """Orchestrate generation, compilation, reporting, and summaries of module tests."""

    def __init__(self, templates_dir: Path):
        """Initialize tester state, compiler selection, and NEED_* discovery."""
        self.templates_dir = templates_dir.expanduser().resolve()
        self.test_results: list[ModuleTestRecord] = []
        self.config = _load_template_config(self.templates_dir)
        self.compiler = _select_compiler(self.config)
        self.compiler_flags = _build_compiler_flags(self.config)
        self.need_mapping = _discover_need_mapping(self.templates_dir)
        self.need_macros = list(self.need_mapping.keys())
        self._compiler_invocation = CompilerInvocation(
            compiler=self.compiler,
            compiler_flags=self.compiler_flags,
            include_dir=self.templates_dir.parent,
        )

    def create_test_file(
        self,
        macros: list[str] | tuple[str, ...],
        test_code: str = "",
        extra_defines: list[str] | tuple[str, ...] | None = None,
    ) -> str:
        """Create a temporary test file with specified NEED_* macros."""

        content: list[str] = []
        seen: set[str] = set()
        for define in extra_defines or ():
            if define in seen:
                continue
            content.append(f"#define {define}")
            seen.add(define)
        for macro in macros:
            if not macro.startswith("NEED_") or macro in seen:
                continue
            content.append(f"#define {macro}")
            seen.add(macro)
        content.append('#include "templates/Base.hpp"')
        content.append("")
        content.append(test_code if test_code else DEFAULT_EMPTY_MAIN)
        return "\n".join(content)

    def compile_test(self, test_content: str) -> tuple[bool, str]:
        """Compile test file and return legacy `(success, error)` metadata."""

        outcome = self._compiler_invocation.compile_source(test_content)
        return outcome.success, outcome.error or ""

    def _available_candidates(self, candidates):
        """Filter candidate macro sets to those supported by current templates."""

        available = set(self.need_macros)
        filtered = []
        seen = set()
        for entry in candidates:
            macros = tuple(entry[0])
            if not set(macros).issubset(available):
                continue
            if macros in seen:
                continue
            seen.add(macros)
            filtered.append(entry)
        return filtered

    def _build_individual_cases(self) -> list[ModuleTestCase]:
        """Build compile probes for each discovered NEED_* macro."""

        cases: list[ModuleTestCase] = []
        for need_macro in self.need_macros:
            cases.append(
                ModuleTestCase(
                    kind="individual",
                    description=need_macro,
                    macros=(need_macro,),
                    source=INDIVIDUAL_TEST_SNIPPETS.get(need_macro, DEFAULT_EMPTY_MAIN),
                    headers=tuple(self.need_mapping.get(need_macro, ())),
                )
            )
        return cases

    def _build_combination_cases(self) -> list[ModuleTestCase]:
        """Build compile probes for supported common module combinations."""

        combinations = self._available_candidates(COMBINATION_CANDIDATES)
        if not combinations and len(self.need_macros) > 1:
            combinations = [
                (
                    tuple(self.need_macros),
                    "All discovered modules",
                    DEFAULT_EMPTY_MAIN,
                )
            ]
        return [
            ModuleTestCase(
                kind="combination",
                description=description,
                macros=tuple(macros),
                source=test_code,
            )
            for macros, description, test_code in combinations
        ]

    def _build_strict_profile_cases(self) -> list[ModuleTestCase]:
        """Build compile probes for strict-profile combinations."""

        combinations = self._available_candidates(STRICT_PROFILE_CANDIDATES)
        return [
            ModuleTestCase(
                kind="strict_profile",
                description=description,
                macros=tuple(macros),
                source=test_code,
                extra_defines=("CP_TEMPLATE_PROFILE_STRICT",),
            )
            for macros, description, test_code in combinations
        ]

    def _build_benchmark_cases(self) -> list[ModuleTestCase]:
        """Build compile probes used for rough timing comparisons."""

        benchmarks = self._available_candidates(BENCHMARK_CANDIDATES)
        if not benchmarks:
            benchmarks = (
                [((self.need_macros[0],), "Single discovered macro")]
                if self.need_macros
                else []
            )
        return [
            ModuleTestCase(
                kind="benchmark",
                description=description,
                macros=tuple(macros),
            )
            for macros, description in benchmarks
        ]

    def _compile_case(self, case: ModuleTestCase) -> CompilationOutcome:
        """Compile one logical test case through the shared compiler invocation."""

        test_content = self.create_test_file(
            case.macros,
            case.source,
            extra_defines=case.extra_defines,
        )
        return self._compiler_invocation.compile_source(test_content)

    def _record_case(self, case: ModuleTestCase, outcome: CompilationOutcome) -> None:
        """Persist one test record for JSON reporting and summaries."""

        self.test_results.append(
            ModuleTestRecord(
                kind=case.kind,
                description=case.description,
                macros=case.macros,
                success=outcome.success,
                error=None if outcome.success else outcome.error,
                headers=case.headers,
                defines=case.extra_defines,
                elapsed_seconds=outcome.elapsed_seconds if case.kind == "benchmark" else None,
            )
        )

    def _print_case_result(self, case: ModuleTestCase, outcome: CompilationOutcome) -> None:
        """Emit one console result line plus trimmed error context when needed."""

        status = "PASS" if outcome.success else "FAIL"
        if case.kind == "benchmark" and outcome.elapsed_seconds is not None:
            print(f"{case.display_label:{case.display_width}} {outcome.elapsed_seconds:.3f}s {status}")
        else:
            print(f"{case.display_label:{case.display_width}} {status}")
        if not outcome.success and outcome.error:
            print(f"  Error: {outcome.error[:200]}...")

    def _run_case_group(
        self,
        *,
        title: str,
        cases: list[ModuleTestCase],
        empty_message: str | None = None,
    ) -> None:
        """Run a homogeneous case group with shared console/reporting behavior."""

        print(title)
        print("-" * 50)
        if not cases and empty_message:
            print(empty_message)
            return

        for case in cases:
            outcome = self._compile_case(case)
            self._print_case_result(case, outcome)
            self._record_case(case, outcome)

    def test_individual_modules(self) -> None:
        """Test each discovered NEED_* macro individually."""

        self._run_case_group(
            title="Testing individual modules...",
            cases=self._build_individual_cases(),
        )

    def test_module_combinations(self) -> None:
        """Test common module combinations available in current templates."""

        self._run_case_group(
            title="\nTesting module combinations...",
            cases=self._build_combination_cases(),
        )

    def test_strict_profile_combinations(self) -> None:
        """Compile-check strict profile combinations when their macros are available."""

        self._run_case_group(
            title="\nTesting strict-profile combinations...",
            cases=self._build_strict_profile_cases(),
            empty_message="No strict-profile candidates available for current NEED_* set.",
        )

    def test_performance_benchmarks(self) -> None:
        """Benchmark compilation time for available combinations."""

        self._run_case_group(
            title="\nBenchmarking compilation times...",
            cases=self._build_benchmark_cases(),
        )

    def generate_report(self, output_file: Path) -> None:
        """Generate a JSON report of test results."""

        report = {
            "timestamp": datetime.now().isoformat(),
            "templates_dir": str(self.templates_dir),
            "compiler": self.compiler,
            "compiler_flags": list(self.compiler_flags),
            "need_macros": list(self.need_macros),
            "test_results": [result.to_dict() for result in self.test_results],
        }
        output_file.parent.mkdir(parents=True, exist_ok=True)
        output_file.write_text(
            json.dumps(report, indent=2) + "\n",
            encoding="utf-8",
        )
        print(f"\nTest report saved to: {output_file}")

    def run_all_tests(self) -> bool:
        """Run all test suites."""

        print("=" * 50)
        print("MODULE TESTING FRAMEWORK")
        print("=" * 50)

        self.test_individual_modules()
        self.test_module_combinations()
        self.test_strict_profile_combinations()
        self.test_performance_benchmarks()

        passed = sum(1 for result in self.test_results if result.success)
        total = len(self.test_results)
        if total == 0:
            print("\n" + "=" * 50)
            print("SUMMARY: 0 tests executed (treating as failure)")
            print("=" * 50)
            return False

        print("\n" + "=" * 50)
        print(f"SUMMARY: {passed}/{total} tests passed")
        print("=" * 50)
        return passed == total


def build_parser() -> argparse.ArgumentParser:
    """Build CLI parser for module test execution."""

    parser = argparse.ArgumentParser(
        description="Compile-test CP template NEED_* modules."
    )
    parser.add_argument(
        "templates_dir",
        nargs="?",
        type=Path,
        default=None,
        help="path to templates directory (default: auto-detect)",
    )
    parser.add_argument(
        "--report",
        type=Path,
        default=None,
        help="optional output path for JSON report",
    )
    return parser


def main() -> int:
    """Run module tests and return a CI-friendly process exit code."""

    parser = build_parser()
    args = parser.parse_args()

    if args.templates_dir is not None:
        templates_dir = args.templates_dir.expanduser().resolve()
    else:
        script_dir = Path(__file__).parent.resolve()
        templates_dir = script_dir.parent / "templates"

    if not templates_dir.is_dir():
        print(f"Error: Templates directory not found: {templates_dir}")
        return 1

    tester = ModuleTester(templates_dir)
    success = tester.run_all_tests()

    if args.report is not None:
        tester.generate_report(args.report.expanduser())

    return 0 if success else 1


if __name__ == "__main__":
    raise SystemExit(main())
