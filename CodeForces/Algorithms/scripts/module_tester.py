#!/usr/bin/env python3
"""
Module Testing Framework for Competitive Programming Template
This script automatically tests each module for compilation and functionality.
"""

import subprocess
import tempfile
import sys
from pathlib import Path
from typing import List, Dict, Tuple
import json
from datetime import datetime


class ModuleTester:
    def __init__(self, templates_dir: Path):
        self.templates_dir = templates_dir
        self.test_results = []
        self.compiler = "g++"
        self.compiler_flags = ["-std=c++23", "-Wall", "-Wextra", "-fsyntax-only"]

    def create_test_file(self, modules: List[str], test_code: str = "") -> str:
        """Create a temporary test file with specified modules."""
        content = []

        # Add necessary defines.
        if "Types.hpp" in modules or "NEED_CORE" in modules:
            content.append("#define NEED_CORE")
        if "IO.hpp" in modules or "NEED_IO" in modules:
            content.append("#define NEED_IO")
        if "Bit_Ops.hpp" in modules or "NEED_BIT_OPS" in modules:
            content.append("#define NEED_BIT_OPS")
        if "Mod_Int.hpp" in modules or "NEED_MOD_INT" in modules:
            content.append("#define NEED_MOD_INT")
        if "Containers.hpp" in modules or "NEED_CONTAINERS" in modules:
            content.append("#define NEED_CONTAINERS")

        content.append('#include "templates/base.hpp"')
        content.append("")

        # Add test code or default main.
        if test_code:
            content.append(test_code)
        else:
            content.append("int main() { return 0; }")

        return "\n".join(content)

    def compile_test(self, test_content: str) -> Tuple[bool, str]:
        """Compile test file and return success status and error message."""
        with tempfile.NamedTemporaryFile(mode="w", suffix=".cpp", delete=False) as f:
            f.write(test_content)
            temp_file = f.name

        try:
            # Adjust include path to find templates.
            include_flag = f"-I{self.templates_dir.parent}"
            cmd = [self.compiler] + self.compiler_flags + [include_flag, temp_file]

            result = subprocess.run(cmd, capture_output=True, text=True, timeout=10)

            success = result.returncode == 0
            error_msg = result.stderr if not success else ""
            return success, error_msg

        except subprocess.TimeoutExpired:
            return False, "Compilation timeout"
        except Exception as e:
            return False, str(e)
        finally:
            Path(temp_file).unlink(missing_ok=True)

    def test_individual_modules(self):
        """Test each module individually."""
        print("Testing individual modules...")
        print("-" * 50)

        modules = [
            ("Types.hpp", "NEED_CORE", "using VI = VC<I64>; VI v;"),
            ("Constants.hpp", "NEED_CORE", "constexpr I64 x = INF64;"),
            ("Macros.hpp", "NEED_CORE", "int main() { FOR(i, 10) {} return 0; }"),
            (
                "Math.hpp",
                "NEED_CORE",
                "int main() { I64 x = gcd(12LL, 8LL); return 0; }",
            ),
            (
                "Bit_Ops.hpp",
                "NEED_BIT_OPS",
                "int main() { I32 x = popcount(15); return 0; }",
            ),
            ("IO.hpp", "NEED_IO", "int main() { return 0; }"),
            ("Mod_Int.hpp", "NEED_MOD_INT", "int main() { mint x(5); return 0; }"),
            (
                "Containers.hpp",
                "NEED_CONTAINERS",
                "int main() { VI v = {1,2,3}; auto sorted = argsort(v); return 0; }",
            ),
        ]

        for module_file, need_flag, test_code in modules:
            test_content = self.create_test_file([need_flag], test_code)
            success, error = self.compile_test(test_content)

            status = "✓ PASS" if success else "✗ FAIL"
            print(f"{module_file:20} {status}")

            if not success and error:
                print(f"  Error: {error[:200]}...")

            self.test_results.append(
                {
                    "module": module_file,
                    "success": success,
                    "error": error if not success else None,
                    "timestamp": datetime.now().isoformat(),
                }
            )

    def test_module_combinations(self):
        """Test common combinations of modules."""
        print("\nTesting module combinations...")
        print("-" * 50)

        combinations = [
            (
                ["NEED_CORE", "NEED_IO"],
                "Basic I/O",
                "int main() { LL(n); OUT(n); return 0; }",
            ),
            (
                ["NEED_CORE", "NEED_BIT_OPS"],
                "Core + Bit Operations",
                "int main() { I64 x = 15; I32 cnt = popcount(x); return 0; }",
            ),
            (
                ["NEED_CORE", "NEED_MOD_INT"],
                "Core + ModInt",
                "int main() { mint a(5), b(3); mint c = a * b; return 0; }",
            ),
            (
                ["NEED_CORE", "NEED_CONTAINERS"],
                "Core + Containers",
                "int main() { VI v = {3,1,2}; auto idx = argsort(v); return 0; }",
            ),
            (
                ["NEED_CORE", "NEED_IO", "NEED_CONTAINERS"],
                "Core + I/O + Containers",
                "int main() { VI v = {1,2,3}; return 0; }",
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
                "int main() { mint x(5); I32 bits = popcount(15); VI v = {1,2}; return 0; }",
            ),
        ]

        for modules, description, test_code in combinations:
            test_content = self.create_test_file(modules, test_code)
            success, error = self.compile_test(test_content)

            status = "✓ PASS" if success else "✗ FAIL"
            print(f"{description:30} {status}")

            if not success and error:
                print(f"  Error: {error[:200]}...")

            self.test_results.append(
                {
                    "combination": description,
                    "modules": modules,
                    "success": success,
                    "error": error if not success else None,
                    "timestamp": datetime.now().isoformat(),
                }
            )

    def test_performance_benchmarks(self):
        """Test compilation time for different module combinations."""
        print("\nBenchmarking compilation times...")
        print("-" * 50)

        import time

        benchmarks = [
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

        for modules, description in benchmarks:
            test_content = self.create_test_file(modules)

            start_time = time.time()
            success, _ = self.compile_test(test_content)
            elapsed = time.time() - start_time

            print(f"{description:30} {elapsed:.3f}s")

    def generate_report(self, output_file: Path):
        """Generate a JSON report of test results."""
        report = {
            "timestamp": datetime.now().isoformat(),
            "templates_dir": str(self.templates_dir),
            "compiler": self.compiler,
            "compiler_flags": self.compiler_flags,
            "test_results": self.test_results,
        }

        with open(output_file, "w") as f:
            json.dump(report, f, indent=2)

        print(f"\nTest report saved to: {output_file}")

    def run_all_tests(self):
        """Run all test suites."""
        print("=" * 50)
        print("MODULE TESTING FRAMEWORK")
        print("=" * 50)

        self.test_individual_modules()
        self.test_module_combinations()
        self.test_performance_benchmarks()

        # Summary.
        passed = sum(1 for r in self.test_results if r.get("success", False))
        total = len(self.test_results)

        print("\n" + "=" * 50)
        print(f"SUMMARY: {passed}/{total} tests passed")
        print("=" * 50)

        return passed == total


# -------------------------------- MAIN LOGIC -------------------------------- #


def main():
    if len(sys.argv) > 1:
        templates_dir = Path(sys.argv[1])
    else:
        # Try to find templates directory.
        script_dir = Path(__file__).parent.resolve()
        templates_dir = script_dir.parent / "templates"

    if not templates_dir.exists():
        print(f"Error: Templates directory not found: {templates_dir}")
        sys.exit(1)

    tester = ModuleTester(templates_dir)
    success = tester.run_all_tests()

    # Generate report.
    report_file = Path("test_report.json")
    tester.generate_report(report_file)

    sys.exit(0 if success else 1)


if __name__ == "__main__":
    main()
