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
from datetime import datetime
from pathlib import Path
from typing import Dict, List, Tuple

from need_resolver import load_need_mapping

DEFAULT_NEED_MACROS = [
    "NEED_CORE",
    "NEED_MATH",
    "NEED_IO",
    "NEED_BIT_OPS",
    "NEED_MOD_INT",
    "NEED_CONTAINERS",
    "NEED_TYPE_SAFETY",
    "NEED_HASHING",
]

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

COMBINATION_CANDIDATES = [
    (
        ["NEED_CORE", "NEED_IO"],
        "Core + I/O",
        "int main() { LL(n); OUT(n); return 0; }",
    ),
    (
        ["NEED_CORE", "NEED_BIT_OPS"],
        "Core + Bit Operations",
        "int main() { I64 x = 15; I32 cnt = popcount(x); return cnt == 4 ? 0 : 1; }",
    ),
    (
        ["NEED_CORE", "NEED_MOD_INT"],
        "Core + ModInt",
        "int main() { mint a(5), b(3); mint c = a * b; return (int)I64(c); }",
    ),
    (
        ["NEED_CORE", "NEED_CONTAINERS"],
        "Core + Containers",
        "int main() { VI v = {3,1,2}; auto idx = argsort(v); return (int)idx.size(); }",
    ),
    (
        ["NEED_CORE", "NEED_TYPE_SAFETY"],
        "Core + Type Safety",
        "CP_DECLARE_STRONG_TYPE(NodeId, I32); int main() { NodeId id(7); return cp::unwrap(id) == 7 ? 0 : 1; }",
    ),
    (
        ["NEED_CORE", "NEED_HASHING"],
        "Core + Hashing",
        "int main() { cp::hashing::FastHashMap<I64, I64> mp; mp[1] = 2; return mp[1] == 2 ? 0 : 1; }",
    ),
    (
        ["NEED_CORE", "NEED_IO", "NEED_CONTAINERS"],
        "Core + I/O + Containers",
        "int main() { VI v = {1,2,3}; OUT((I64)v.size()); return 0; }",
    ),
    (
        [
            "NEED_CORE",
            "NEED_IO",
            "NEED_BIT_OPS",
            "NEED_MOD_INT",
            "NEED_CONTAINERS",
        ],
        "All modules",
        "int main() { mint x(5); I32 bits = popcount(15); VI v = {1,2}; OUT(bits); return (int)v.size(); }",
    ),
]

STRICT_PROFILE_CANDIDATES = [
    (
        ["NEED_CORE", "NEED_IO"],
        "Strict + Core + I/O",
        "int main() { LL(n); OUT(n); return 0; }",
    ),
    (
        ["NEED_CORE", "NEED_FAST_IO", "NEED_TYPE_SAFETY"],
        "Strict + Fast I/O + Type Safety",
        "CP_DECLARE_STRONG_TYPE(NodeId, I32); int main() { NodeId id(9); OUT(cp::unwrap(id)); return 0; }",
    ),
]

BENCHMARK_CANDIDATES = [
    (["NEED_CORE"], "Core only"),
    (["NEED_CORE", "NEED_IO"], "Core + I/O"),
    (["NEED_CORE", "NEED_IO", "NEED_CONTAINERS"], "Core + I/O + Containers"),
    (
        [
            "NEED_CORE",
            "NEED_IO",
            "NEED_BIT_OPS",
            "NEED_MOD_INT",
            "NEED_CONTAINERS",
        ],
        "All modules",
    ),
]


class ModuleTester:
    def __init__(self, templates_dir: Path):
        """Initialize tester state, compiler selection, and NEED_* discovery."""
        self.templates_dir = templates_dir
        self.test_results: List[Dict] = []
        self.config = self.load_config()
        self.compiler = self.select_compiler()
        self.compiler_flags = self.build_compiler_flags()
        self.need_mapping = self.discover_need_mapping()
        self.need_macros = list(self.need_mapping.keys())

    def load_config(self) -> Dict:
        """Load project config if available."""
        config_path = self.templates_dir.parent / ".template_config.json"
        if not config_path.exists():
            return {}
        try:
            with open(config_path, "r", encoding="utf-8") as f:
                return json.load(f)
        except (json.JSONDecodeError, OSError):
            return {}

    def build_compiler_flags(self) -> List[str]:
        """Build compiler flags from config plus tester defaults."""
        config_flags = list(self.config.get("compiler_flags", []))
        if not any(flag.startswith("-std=") for flag in config_flags):
            config_flags.insert(0, "-std=c++23")
        if "-fsyntax-only" not in config_flags:
            config_flags.append("-fsyntax-only")
        return config_flags

    def can_compile_std_headers(self, compiler: str) -> bool:
        """Quick probe to ensure compiler supports template header style."""
        with tempfile.NamedTemporaryFile(mode="w", suffix=".cpp", delete=False) as f:
            f.write("#include <bits/stdc++.h>\nint main() { return 0; }\n")
            temp_file = f.name
        try:
            result = subprocess.run(
                [compiler, "-std=c++23", "-fsyntax-only", temp_file],
                capture_output=True,
                timeout=5,
            )
            return result.returncode == 0
        except Exception:
            return False
        finally:
            Path(temp_file).unlink(missing_ok=True)

    def select_compiler(self) -> str:
        """Pick the most suitable compiler for CP templates."""
        configured = self.config.get("compiler")
        candidates = [configured, "g++-15", "g++-14", "g++-13", "g++", "c++"]
        seen = set()
        for candidate in candidates:
            if not candidate or candidate in seen:
                continue
            seen.add(candidate)
            resolved = shutil.which(candidate)
            if not resolved:
                continue
            if self.can_compile_std_headers(resolved):
                return resolved
        return configured or "g++"

    def discover_need_mapping(self) -> Dict[str, List[str]]:
        """Load NEED_* mapping from Base.hpp; fallback to known defaults."""
        mapping = load_need_mapping(self.templates_dir / "Base.hpp")
        if mapping:
            return dict(mapping)
        return {macro: [] for macro in DEFAULT_NEED_MACROS}

    def create_test_file(
        self,
        macros: List[str],
        test_code: str = "",
        extra_defines: List[str] | None = None,
    ) -> str:
        """Create a temporary test file with specified NEED_* macros."""
        content: List[str] = []
        seen = set()
        for define in extra_defines or []:
            if define in seen:
                continue
            content.append(f"#define {define}")
            seen.add(define)
        for macro in macros:
            if not macro.startswith("NEED_"):
                continue
            if macro in seen:
                continue
            content.append(f"#define {macro}")
            seen.add(macro)
        content.append('#include "templates/Base.hpp"')
        content.append("")
        content.append(test_code if test_code else DEFAULT_EMPTY_MAIN)
        return "\n".join(content)

    def compile_test(self, test_content: str) -> Tuple[bool, str]:
        """Compile test file and return success status and error message."""
        with tempfile.NamedTemporaryFile(mode="w", suffix=".cpp", delete=False) as f:
            f.write(test_content)
            temp_file = f.name
        try:
            include_flag = f"-I{self.templates_dir.parent}"
            cmd = [self.compiler] + self.compiler_flags + [include_flag, temp_file]
            result = subprocess.run(cmd, capture_output=True, text=True, timeout=10)
            success = result.returncode == 0
            return success, "" if success else result.stderr
        except subprocess.TimeoutExpired:
            return False, "Compilation timeout"
        except Exception as e:
            return False, str(e)
        finally:
            Path(temp_file).unlink(missing_ok=True)

    def _record(self, payload: Dict) -> None:
        """Store one test result payload with a timestamp."""
        payload["timestamp"] = datetime.now().isoformat()
        self.test_results.append(payload)

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

    def test_individual_modules(self):
        """Test each discovered NEED_* macro individually."""
        print("Testing individual modules...")
        print("-" * 50)
        for need_macro in self.need_macros:
            test_code = INDIVIDUAL_TEST_SNIPPETS.get(need_macro, DEFAULT_EMPTY_MAIN)
            test_content = self.create_test_file([need_macro], test_code)
            success, error = self.compile_test(test_content)

            headers = self.need_mapping.get(need_macro, [])
            label = f"{need_macro} ({', '.join(headers)})" if headers else need_macro
            status = "PASS" if success else "FAIL"
            print(f"{label:45} {status}")
            if not success and error:
                print(f"  Error: {error[:200]}...")

            self._record(
                {
                    "kind": "individual",
                    "macro": need_macro,
                    "headers": headers,
                    "success": success,
                    "error": error if not success else None,
                }
            )

    def test_module_combinations(self):
        """Test common module combinations available in current templates."""
        print("\nTesting module combinations...")
        print("-" * 50)

        combinations = self._available_candidates(COMBINATION_CANDIDATES)
        if not combinations and len(self.need_macros) > 1:
            combinations = [
                (
                    self.need_macros,
                    "All discovered modules",
                    DEFAULT_EMPTY_MAIN,
                )
            ]

        for macros, description, test_code in combinations:
            test_content = self.create_test_file(list(macros), test_code)
            success, error = self.compile_test(test_content)
            status = "PASS" if success else "FAIL"
            print(f"{description:30} {status}")
            if not success and error:
                print(f"  Error: {error[:200]}...")

            self._record(
                {
                    "kind": "combination",
                    "description": description,
                    "macros": list(macros),
                    "success": success,
                    "error": error if not success else None,
                }
            )

    def test_strict_profile_combinations(self):
        """Compile-check strict profile combinations when their macros are available."""
        print("\nTesting strict-profile combinations...")
        print("-" * 50)

        combinations = self._available_candidates(STRICT_PROFILE_CANDIDATES)
        if not combinations:
            print("No strict-profile candidates available for current NEED_* set.")
            return

        for macros, description, test_code in combinations:
            test_content = self.create_test_file(
                list(macros),
                test_code,
                extra_defines=["CP_TEMPLATE_PROFILE_STRICT"],
            )
            success, error = self.compile_test(test_content)
            status = "PASS" if success else "FAIL"
            print(f"{description:30} {status}")
            if not success and error:
                print(f"  Error: {error[:200]}...")

            self._record(
                {
                    "kind": "strict_profile",
                    "description": description,
                    "macros": list(macros),
                    "defines": ["CP_TEMPLATE_PROFILE_STRICT"],
                    "success": success,
                    "error": error if not success else None,
                }
            )

    def test_performance_benchmarks(self):
        """Benchmark compilation time for available combinations."""
        print("\nBenchmarking compilation times...")
        print("-" * 50)

        benchmarks = self._available_candidates(BENCHMARK_CANDIDATES)
        if not benchmarks:
            benchmarks = [([self.need_macros[0]], "Single discovered macro")] if self.need_macros else []

        for macros, description in benchmarks:
            test_content = self.create_test_file(list(macros))
            start_time = time.time()
            success, error = self.compile_test(test_content)
            elapsed = time.time() - start_time
            status = "PASS" if success else "FAIL"
            print(f"{description:30} {elapsed:.3f}s {status}")
            if not success and error:
                print(f"  Error: {error[:200]}...")

            self._record(
                {
                    "kind": "benchmark",
                    "description": description,
                    "macros": list(macros),
                    "elapsed_seconds": elapsed,
                    "success": success,
                    "error": error if not success else None,
                }
            )

    def generate_report(self, output_file: Path):
        """Generate a JSON report of test results."""
        report = {
            "timestamp": datetime.now().isoformat(),
            "templates_dir": str(self.templates_dir),
            "compiler": self.compiler,
            "compiler_flags": self.compiler_flags,
            "need_macros": self.need_macros,
            "test_results": self.test_results,
        }
        output_file.parent.mkdir(parents=True, exist_ok=True)
        with open(output_file, "w", encoding="utf-8") as f:
            json.dump(report, f, indent=2)
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

        passed = sum(1 for r in self.test_results if r.get("success", False))
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


def main() -> None:
    """Run module tests and exit with a CI-friendly status code."""
    parser = build_parser()
    args = parser.parse_args()

    if args.templates_dir is not None:
        templates_dir = args.templates_dir
    else:
        script_dir = Path(__file__).parent.resolve()
        templates_dir = script_dir.parent / "templates"

    if not templates_dir.exists():
        print(f"Error: Templates directory not found: {templates_dir}")
        sys.exit(1)

    tester = ModuleTester(templates_dir)
    success = tester.run_all_tests()

    if args.report is not None:
        tester.generate_report(args.report)

    sys.exit(0 if success else 1)


if __name__ == "__main__":
    main()
