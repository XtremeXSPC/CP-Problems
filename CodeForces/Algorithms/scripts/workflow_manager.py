#!/usr/bin/env python3
"""
Complete Workflow Automation System for Competitive Programming Template
Integrates testing, versioning, and deployment into a seamless workflow.
"""

import os
import sys
import json
import shutil
import subprocess
from pathlib import Path
from datetime import datetime
from typing import Dict, List, Optional, Tuple
import argparse

# Use environment variables from "competitive.sh"
CP_WORKSPACE_ROOT = os.environ.get("CP_WORKSPACE_ROOT", "/Volumes/LCS.Data/CP-Problems")
CP_ALGORITHMS_DIR = os.environ.get(
    "CP_ALGORITHMS_DIR", "/Volumes/LCS.Data/CP-Problems/CodeForces/Algorithms"
)


# ----------------------------- WORKFLOW MANAGER ----------------------------- #
class WorkflowManager:
    def __init__(self, project_root: Path):
        self.project_root = project_root
        self.templates_dir = project_root / "templates"
        self.modules_dir = project_root / "modules"
        self.scripts_dir = project_root / "scripts"
        self.config_file = project_root / ".template_config.json"

        # Load or create configuration.
        self.config = self.load_config()

        # Initialize subcomponents.
        self.init_components()
        self.compiler = self.resolve_compiler()

    def default_config(self) -> Dict:
        """Build default workflow configuration."""
        return {
            "version": "1.0.0",
            "compiler": "g++",
            "compiler_flags": ["-std=c++23", "-Wall", "-Wextra"],
            "modules": {
                "core": ["Types.hpp", "Constants.hpp", "Macros.hpp", "Math.hpp"],
                "io": ["IO.hpp"],
                "bit_ops": ["Bit_Ops.hpp"],
                "mod_int": ["Mod_Int.hpp"],
                "containers": ["Containers.hpp"],
                "graph": ["Graph.hpp"],
                "string": ["Strings.hpp"],
                "data_structures": ["Data_Structures.hpp"],
                "number_theory": ["Number_Theory.hpp"],
                "geometry": ["Geometry.hpp"],
            },
            "auto_version": True,
            "auto_test": True,
            "default_modules": ["core", "io"],
            "created": datetime.now().isoformat(),
        }

    def load_config(self) -> Dict:
        """Load project configuration."""
        default_config = self.default_config()

        if self.config_file.exists():
            try:
                with open(self.config_file, "r", encoding="utf-8") as f:
                    config = json.load(f)

                changed = False
                for key, value in default_config.items():
                    if key not in config:
                        config[key] = value
                        changed = True

                modules_cfg = config.get("modules", {})
                if (
                    isinstance(modules_cfg, dict)
                    and modules_cfg.get("string") == ["String_Algorithms.hpp"]
                ):
                    modules_cfg["string"] = ["Strings.hpp"]
                    changed = True

                if changed:
                    with open(self.config_file, "w", encoding="utf-8") as f:
                        json.dump(config, f, indent=2)

                return config
            except (json.JSONDecodeError, OSError) as e:
                backup = self.config_file.with_suffix(
                    f".corrupted.{datetime.now().strftime('%Y%m%d_%H%M%S')}.json"
                )
                shutil.copy2(self.config_file, backup)
                print(
                    f"⚠ Invalid config file, using defaults. Backup saved to: {backup}"
                )
                print(f"  Details: {e}")

        with open(self.config_file, "w", encoding="utf-8") as f:
            json.dump(default_config, f, indent=2)

        return default_config

    def resolve_compiler(self) -> str:
        """Resolve compiler with sensible CP-oriented fallbacks."""
        configured = self.config.get("compiler")
        candidates = []

        if configured and configured not in {"g++", "c++"}:
            candidates.append(configured)
        candidates.extend(["g++-15", "g++-14", "g++-13"])
        if configured:
            candidates.append(configured)
        candidates.extend(["g++", "c++"])

        seen = set()
        for candidate in candidates:
            if not candidate or candidate in seen:
                continue
            seen.add(candidate)
            resolved = shutil.which(candidate)
            if resolved:
                return resolved

        return configured or "g++"

    def init_components(self):
        """Initialize workflow components."""
        # Create necessary directories.
        self.templates_dir.mkdir(parents=True, exist_ok=True)
        self.modules_dir.mkdir(parents=True, exist_ok=True)
        self.scripts_dir.mkdir(parents=True, exist_ok=True)

        # Setup module registry.
        self.module_registry_file = self.modules_dir / "data" / "registry.json"
        self.module_registry_file.parent.mkdir(parents=True, exist_ok=True)
        if not self.module_registry_file.exists():
            self.create_module_registry()

    def create_module_registry(self):
        """Create registry of available modules with metadata."""
        registry = {
            "modules": {
                "graph": {
                    "files": ["Graph.hpp"],
                    "dependencies": ["core"],
                    "description": "Graph algorithms and data structures",
                    "features": [
                        "dijkstra",
                        "bellman_ford",
                        "dsu",
                        "lca",
                        "scc",
                        "bridges",
                    ],
                    "size_kb": 15.2,
                    "compile_time_impact": "medium",
                },
                "string": {
                    "files": ["Strings.hpp"],
                    "dependencies": ["core"],
                    "description": "String processing and pattern matching",
                    "features": [
                        "kmp",
                        "z_algorithm",
                        "manacher",
                        "suffix_array",
                        "rolling_hash",
                        "trie",
                    ],
                    "size_kb": 12.8,
                    "compile_time_impact": "medium",
                },
                "data_structures": {
                    "files": ["Data_Structures.hpp"],
                    "dependencies": ["core"],
                    "description": "Advanced data structures",
                    "features": [
                        "segment_tree",
                        "fenwick_tree",
                        "sparse_table",
                        "heap",
                        "treap",
                    ],
                    "size_kb": 18.5,
                    "compile_time_impact": "high",
                },
                "number_theory": {
                    "files": ["Number_Theory.hpp"],
                    "dependencies": ["core", "mod_int"],
                    "description": "Number theory algorithms",
                    "features": [
                        "sieve",
                        "factorization",
                        "euler_phi",
                        "chinese_remainder",
                        "discrete_log",
                    ],
                    "size_kb": 10.3,
                    "compile_time_impact": "low",
                },
                "geometry": {
                    "files": ["Geometry.hpp"],
                    "dependencies": ["core"],
                    "description": "Computational geometry",
                    "features": [
                        "convex_hull",
                        "line_intersection",
                        "polygon_area",
                        "closest_pair",
                    ],
                    "size_kb": 14.7,
                    "compile_time_impact": "medium",
                },
            },
            "updated": datetime.now().isoformat(),
        }

        with open(self.module_registry_file, "w") as f:
            json.dump(registry, f, indent=2)

    def new_solution(
        self,
        problem_name: str,
        modules: List[str],
        contest: Optional[str] = None,
        force: bool = False,
    ) -> Path:
        """Create a new solution file with specified modules."""
        # Determine file path.
        if contest:
            contest_dir = self.project_root / "contests" / contest
            contest_dir.mkdir(parents=True, exist_ok=True)
            solution_file = contest_dir / f"{problem_name}.cpp"
        else:
            solutions_dir = self.project_root / "solutions"
            solutions_dir.mkdir(exist_ok=True)
            solution_file = solutions_dir / f"{problem_name}.cpp"

        if solution_file.exists() and not force:
            raise FileExistsError(
                f"Solution already exists: {solution_file} (use --force to overwrite)"
            )

        # Generate solution template.
        template = self.generate_solution_template(problem_name, modules, contest)

        with open(solution_file, "w", encoding="utf-8") as f:
            f.write(template)

        print(f"✓ Created solution: {solution_file}")

        # Auto-version if enabled
        if self.config.get("auto_version"):
            self.auto_version(f"Created solution: {problem_name}")

        if self.config.get("auto_test"):
            print("Running auto-test (compilation check)...")
            if self.test_solution(solution_file):
                print("✓ Auto-test passed")
            else:
                print("✗ Auto-test failed")

        return solution_file

    def generate_solution_template(
        self, problem_name: str, modules: List[str], contest: Optional[str]
    ) -> str:
        """Generate solution template with specified modules."""
        lines = []

        # Header comment.
        lines.append(
            "//===----------------------------------------------------------------------===//"
        )
        lines.append("/**")
        lines.append(f" * @file: {problem_name}.cpp")
        if contest:
            lines.append(f" * @contest: {contest}")
        lines.append(f" * @problem: {problem_name}")
        lines.append(f" * @author: Costantino Lombardi")
        lines.append(f" * @date: {datetime.now().strftime('%Y-%m-%d')}")
        lines.append(" */")
        lines.append(
            "//===----------------------------------------------------------------------===//"
        )
        lines.append("")

        # Module definitions
        need_macros = []
        if "core" in modules or not modules:
            need_macros.append("NEED_CORE")
        if "io" in modules:
            need_macros.append("NEED_IO")
        if "bit_ops" in modules:
            need_macros.append("NEED_BIT_OPS")
        if "mod_int" in modules:
            need_macros.append("NEED_MOD_INT")
        if "containers" in modules:
            need_macros.append("NEED_CONTAINERS")

        # Add advanced modules if present
        advanced_modules = [
            "graph",
            "string",
            "data_structures",
            "number_theory",
            "geometry",
        ]
        for mod in advanced_modules:
            if mod in modules:
                need_macros.append(f"NEED_{mod.upper()}")

        for macro in need_macros:
            lines.append(f"#define {macro}")

        lines.append("")
        lines.append('#include "templates/Base.hpp"')

        # Include advanced modules if needed
        for mod in advanced_modules:
            if mod in modules:
                module_name = self.get_module_filename(mod)
                lines.append(f'#include "modules/{module_name}"')

        lines.append("")
        lines.append(
            "//===----------------------------------------------------------------------===//"
        )
        lines.append("/* Main Solver Function */")
        lines.append("")
        lines.append("void solve() {")
        lines.append("  // Your solution here")
        lines.append("  ")
        lines.append("}")
        lines.append("")
        lines.append(
            "//===----------------------------------------------------------------------===//"
        )
        lines.append("/* Main Function */")
        lines.append("")
        lines.append("auto main() -> int {")
        lines.append("#ifdef LOCAL")
        lines.append("  Timer timer;")
        lines.append("#endif")
        lines.append("")
        if "io" in modules:
            lines.append("  INT(T);")
            lines.append("  FOR(T) solve();")
        else:
            lines.append("  I32 T = 1;")
            lines.append("  while (T--) solve();")
        lines.append("")
        lines.append("  return 0;")
        lines.append("}")
        lines.append("")

        return "\n".join(lines)

    def get_module_filename(self, module_name: str) -> str:
        """Get filename for a module."""
        mapping = {
            "graph": "Graph.hpp",
            "string": "Strings.hpp",
            "data_structures": "Data_Structures.hpp",
            "number_theory": "Number_Theory.hpp",
            "geometry": "Geometry.hpp",
        }
        return mapping.get(module_name, f"{module_name}.hpp")

    def test_solution(
        self, solution_file: Path, test_cases: Optional[Path] = None
    ) -> bool:
        """Test a solution with provided test cases."""
        print(f"Testing solution: {solution_file}")

        # Compile solution
        output_file = solution_file.with_suffix("")
        compile_cmd = [
            self.compiler,
            *self.config.get("compiler_flags", ["-std=c++23", "-Wall", "-Wextra"]),
            "-O2",
            f"-I{self.project_root}",
            f"-I{self.project_root / 'libs'}",
            "-DLOCAL",
            "-o",
            str(output_file),
            str(solution_file),
        ]

        try:
            result = subprocess.run(
                compile_cmd, capture_output=True, text=True, timeout=10
            )
            if result.returncode != 0:
                print(f"✗ Compilation failed:")
                print(result.stderr)
                return False

            print("✓ Compilation successful")

            # Run test cases if provided
            if test_cases and test_cases.exists():
                with open(test_cases, "r") as f:
                    test_data = json.load(f)

                passed = 0
                total = len(test_data.get("tests", []))

                for i, test in enumerate(test_data.get("tests", [])):
                    input_data = test["input"]
                    expected_output = test["output"].strip()

                    try:
                        result = subprocess.run(
                            [str(output_file)],
                            input=input_data,
                            capture_output=True,
                            text=True,
                            timeout=2,
                        )
                    except subprocess.TimeoutExpired:
                        print(f"  Test {i+1}: ✗ TIMEOUT")
                        continue

                    if result.returncode != 0:
                        print(f"  Test {i+1}: ✗ RUNTIME ERROR (code {result.returncode})")
                        stderr = result.stderr.strip()
                        if stderr:
                            print(f"    Stderr: {stderr}")
                        continue

                    actual_output = result.stdout.strip()

                    if actual_output == expected_output:
                        print(f"  Test {i+1}: ✓ PASS")
                        passed += 1
                    else:
                        print(f"  Test {i+1}: ✗ FAIL")
                        print(f"    Expected: {expected_output}")
                        print(f"    Got: {actual_output}")

                print(f"\nResults: {passed}/{total} tests passed")
                return passed == total

            return True

        except subprocess.TimeoutExpired:
            print("✗ Compilation/execution timeout")
            return False
        except Exception as e:
            print(f"✗ Error: {e}")
            return False
        finally:
            # Clean up.
            output_file.unlink(missing_ok=True)

    def submit_solution(self, solution_file: Path) -> Optional[Path]:
        """Prepare solution for submission."""
        print(f"Preparing solution for submission: {solution_file}")

        # Run flattener
        flattener_script = self.scripts_dir / "flattener.py"
        output_file = solution_file.with_stem(f"{solution_file.stem}_submit")

        try:
            result = subprocess.run(
                ["python3", str(flattener_script), str(solution_file)],
                capture_output=True,
                text=True,
            )

            if result.returncode == 0:
                with open(output_file, "w") as f:
                    f.write(result.stdout)

                # Get file statistics
                source_lines = sum(1 for _ in open(solution_file))
                output_lines = sum(1 for _ in open(output_file))
                output_size = output_file.stat().st_size / 1024

                print(f"✓ Created submission file: {output_file}")
                print(f"  Source: {source_lines} lines")
                print(f"  Output: {output_lines} lines ({output_size:.1f} KB)")

                # Copy to clipboard if possible
                try:
                    if sys.platform == "darwin":
                        subprocess.run(["pbcopy"], input=result.stdout, text=True)
                        print("✓ Copied to clipboard")
                    elif sys.platform == "linux":
                        subprocess.run(
                            ["xclip", "-selection", "clipboard"],
                            input=result.stdout,
                            text=True,
                        )
                        print("✓ Copied to clipboard")
                except:
                    pass

                return output_file
            else:
                print(f"✗ Flattening failed: {result.stderr}")
                return None

        except Exception as e:
            print(f"✗ Error: {e}")
            return None

    def auto_version(self, message: str):
        """Automatically create a version if enabled."""
        if not self.config.get("auto_version"):
            return

        try:
            from pathlib import Path

            # Import version manager
            sys.path.insert(0, str(self.scripts_dir))
            from version_manager import VersionManager

            vm = VersionManager(self.templates_dir)
            if vm.auto_version():
                print(f"✓ Auto-versioned: {message}")
        except ImportError:
            pass

    def doctor(self) -> bool:
        """Check system health and configuration."""
        print("Running system diagnostics...")
        print("=" * 60)

        checks_passed = True

        # Check directories
        dirs_to_check = [
            ("Templates", self.templates_dir),
            ("Modules", self.modules_dir),
            ("Scripts", self.scripts_dir),
        ]

        for name, path in dirs_to_check:
            if path.exists():
                print(f"✓ {name} directory: {path}")
            else:
                print(f"✗ {name} directory missing: {path}")
                checks_passed = False

        # Check essential files
        essential_files = [
            ("Base.hpp", self.templates_dir / "Base.hpp"),
            ("Types.hpp", self.templates_dir / "Types.hpp"),
            ("Flattener", self.scripts_dir / "flattener.py"),
        ]

        for name, path in essential_files:
            if path.exists():
                print(f"✓ {name}: {path}")
            else:
                print(f"✗ {name} missing: {path}")
                checks_passed = False

        # Check compiler
        try:
            result = subprocess.run(
                [self.compiler, "--version"], capture_output=True, timeout=5
            )
            if result.returncode == 0:
                version_line = result.stdout.decode().split("\n")[0]
                print(f"✓ Compiler: {version_line}")
            else:
                print(f"✗ Compiler check failed")
                checks_passed = False
        except:
            print(f"✗ Compiler not found: {self.compiler}")
            checks_passed = False

        # Check Python modules
        required_modules = ["json", "pathlib", "subprocess", "difflib"]
        for module in required_modules:
            try:
                __import__(module)
                print(f"✓ Python module: {module}")
            except ImportError:
                print(f"✗ Python module missing: {module}")
                checks_passed = False

        print("=" * 60)
        if checks_passed:
            print("✓ All checks passed - system is ready!")
        else:
            print("✗ Some checks failed - please fix issues above")

        return checks_passed


# -------------------------------- MAIN LOGIC -------------------------------- #


def main():
    parser = argparse.ArgumentParser(
        description="Competitive Programming Workflow Manager"
    )
    parser.add_argument("--root", type=Path, help="Project root directory")

    subparsers = parser.add_subparsers(dest="command", help="Commands")

    # New solution.
    new_parser = subparsers.add_parser("new", help="Create new solution")
    new_parser.add_argument("name", help="Problem name")
    new_parser.add_argument(
        "--modules", nargs="+", default=["core", "io"], help="Modules to include"
    )
    new_parser.add_argument("--contest", help="Contest name")
    new_parser.add_argument(
        "--force", action="store_true", help="Overwrite existing solution file"
    )

    # Test solution.
    test_parser = subparsers.add_parser("test", help="Test solution")
    test_parser.add_argument("file", type=Path, help="Solution file")
    test_parser.add_argument("--cases", type=Path, help="Test cases JSON file")

    # Submit solution.
    submit_parser = subparsers.add_parser("submit", help="Prepare for submission")
    submit_parser.add_argument("file", type=Path, help="Solution file")

    # System doctor.
    doctor_parser = subparsers.add_parser("doctor", help="Check system health")

    args = parser.parse_args()

    # Find project root.
    if args.root:
        project_root = args.root
    else:
        project_root = Path.cwd()

    wm = WorkflowManager(project_root)

    if args.command == "new":
        try:
            wm.new_solution(args.name, args.modules, args.contest, args.force)
        except FileExistsError as e:
            print(f"✗ {e}")
            return 1
    elif args.command == "test":
        wm.test_solution(args.file, args.cases)
    elif args.command == "submit":
        wm.submit_solution(args.file)
    elif args.command == "doctor":
        wm.doctor()
    else:
        parser.print_help()
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
