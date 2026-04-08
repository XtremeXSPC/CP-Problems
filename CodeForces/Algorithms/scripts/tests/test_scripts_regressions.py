"""Regression tests for scripts-level bugs and edge cases."""

from __future__ import annotations

import argparse
import json
import os
import shutil
import subprocess
import sys
import tempfile
import textwrap
import unittest
from pathlib import Path
from unittest import mock

SCRIPTS_DIR = Path(__file__).resolve().parents[1]
WORKFLOW_SCRIPT = SCRIPTS_DIR / "workflow_manager.py"
BUILD_TEMPLATE_SCRIPT = SCRIPTS_DIR / "build_template.sh"
MIGRATE_CMAKE_SCRIPT = SCRIPTS_DIR / "migrate_round_cmake.py"
MIGRATE_PRESETS_SCRIPT = SCRIPTS_DIR / "migrate_round_presets.py"
MIGRATE_TOOLCHAINS_SCRIPT = SCRIPTS_DIR / "migrate_round_toolchains.py"
MIGRATE_VSCODE_SCRIPT = SCRIPTS_DIR / "migrate_round_vscode.py"
ALGORITHMS_DIR = SCRIPTS_DIR.parent

if str(SCRIPTS_DIR) not in sys.path:
    sys.path.insert(0, str(SCRIPTS_DIR))

import benchmark_pch  # noqa: E402
import module_tester  # noqa: E402


class ScriptRegressionTests(unittest.TestCase):
    def _run_workflow(
        self, *args: str, env: dict[str, str] | None = None
    ) -> subprocess.CompletedProcess:
        """Run workflow_manager.py with optional environment overrides."""

        return subprocess.run(
            [sys.executable, str(WORKFLOW_SCRIPT), *args],
            capture_output=True,
            text=True,
            check=False,
            env=env,
        )

    @unittest.skipIf(shutil.which("zsh") is None, "zsh is required for runtime smoke")
    def test_cpp_timeout_with_stderr_does_not_traceback(self) -> None:
        """Timeout on cpp-tools command should return clean error output."""

        with tempfile.TemporaryDirectory() as tmpdir:
            tmp = Path(tmpdir)
            cp_tools = tmp / "competitive_stub.sh"
            cp_tools.write_text(
                textwrap.dedent(
                    """\
                    cpphelp() {
                      print -- "stub-output-before-timeout"
                      print -u2 -- "stub-stderr-before-timeout"
                      sleep 2
                    }
                    """
                ),
                encoding="utf-8",
            )

            result = self._run_workflow(
                "--cp-tools-script",
                str(cp_tools),
                "--cwd",
                str(tmp),
                "--timeout",
                "1",
                "help",
            )

            self.assertEqual(result.returncode, 124)
            self.assertIn("Command timed out.", result.stderr)
            self.assertNotIn("Traceback", result.stderr + result.stdout)

    @unittest.skipIf(shutil.which("zsh") is None, "zsh is required for runtime smoke")
    def test_external_timeout_with_stderr_does_not_traceback(self) -> None:
        """Timeout on external command path should avoid traceback crashes."""

        with tempfile.TemporaryDirectory() as tmpdir:
            tmp = Path(tmpdir)
            cp_tools = tmp / "competitive_stub.sh"
            cp_tools.write_text("cpphelp() { :; }\n", encoding="utf-8")

            fake_bin = tmp / "bin"
            fake_bin.mkdir()
            fake_cmake = fake_bin / "cmake"
            fake_cmake.write_text(
                textwrap.dedent(
                    """\
                    #!/usr/bin/env bash
                    echo "cmake-stdout-before-timeout"
                    echo "cmake-stderr-before-timeout" >&2
                    sleep 2
                    """
                ),
                encoding="utf-8",
            )
            fake_cmake.chmod(0o755)

            env = dict(os.environ)
            env["PATH"] = f"{fake_bin}:{env['PATH']}"

            result = self._run_workflow(
                "--cp-tools-script",
                str(cp_tools),
                "--cwd",
                str(tmp),
                "--timeout",
                "1",
                "preset-conf",
                "--preset",
                "cp-debug-gcc",
                "--algorithms-dir",
                str(ALGORITHMS_DIR),
                env=env,
            )

            self.assertEqual(result.returncode, 124)
            self.assertIn("Command timed out.", result.stderr)
            self.assertNotIn("Traceback", result.stderr + result.stdout)

    def test_build_template_without_argument_uses_default_output(self) -> None:
        """build_template.sh should default to template.hpp when no arg is passed."""

        with tempfile.TemporaryDirectory() as tmpdir:
            tmp = Path(tmpdir)
            result = subprocess.run(
                ["bash", str(BUILD_TEMPLATE_SCRIPT)],
                cwd=str(tmp),
                capture_output=True,
                text=True,
                check=False,
            )
            self.assertEqual(result.returncode, 0, msg=result.stderr)
            self.assertTrue((tmp / "template.hpp").is_file())

    def test_benchmark_json_out_creates_parent_directories(self) -> None:
        """Benchmark JSON output should create missing parent directories."""

        with tempfile.TemporaryDirectory() as tmpdir:
            tmp = Path(tmpdir)
            round_dir = tmp / "Round_Test"
            round_dir.mkdir()
            (round_dir / "CMakeLists.txt").write_text(
                "cmake_minimum_required(VERSION 3.20)\nproject(test)\n",
                encoding="utf-8",
            )
            (round_dir / "problem_A.cpp").write_text(
                "int main() { return 0; }\n",
                encoding="utf-8",
            )

            json_out = tmp / "reports" / "pch" / "result.json"
            ns = argparse.Namespace(
                round_dir=round_dir,
                target="problem_A",
                compiler="gcc",
                algorithms_dir=ALGORITHMS_DIR,
                threshold_sec=1.0,
                json_out=json_out,
                verbose=False,
            )

            fake_on = benchmark_pch.BenchmarkMetrics(
                configure_sec=1.0,
                cold_build_sec=2.0,
                incremental_build_sec=1.0,
                reconfigure_sec=1.0,
                post_reconfigure_build_sec=2.0,
            )
            fake_off = benchmark_pch.BenchmarkMetrics(
                configure_sec=1.0,
                cold_build_sec=1.5,
                incremental_build_sec=1.2,
                reconfigure_sec=1.0,
                post_reconfigure_build_sec=1.6,
            )

            with (
                mock.patch.object(benchmark_pch, "parse_args", return_value=ns),
                mock.patch.object(
                    benchmark_pch,
                    "_benchmark_mode",
                    side_effect=[fake_on, fake_off],
                ),
            ):
                rc = benchmark_pch.main()

            self.assertEqual(rc, 0)
            self.assertTrue(json_out.is_file())
            payload = json.loads(json_out.read_text(encoding="utf-8"))
            self.assertEqual(payload["round_dir"], str(round_dir))
            self.assertEqual(payload["pch_on"]["incremental_build_sec"], 1.0)
            self.assertEqual(payload["delta"]["incremental_gain_sec"], 0.2)
            self.assertEqual(payload["recommendation"], "disable_default_pch")

    @unittest.skipIf(shutil.which("zsh") is None, "zsh is required for runtime smoke")
    def test_workflow_doctor_marks_cppdiag_failure_as_non_fatal(self) -> None:
        """Non-strict doctor should preserve cppdiag failure details without failing the workflow."""

        with tempfile.TemporaryDirectory() as tmpdir:
            tmp = Path(tmpdir)
            cp_tools = tmp / "competitive_stub.sh"
            cp_tools.write_text(
                textwrap.dedent(
                    """\
                    cpphelp() {
                      print -- "help-ok"
                    }
                    cppinfo() {
                      print -- "info-ok"
                    }
                    cppdiag() {
                      print -u2 -- "diag-failed"
                      return 7
                    }
                    cppcheck() {
                      print -- "check-ok"
                    }
                    """
                ),
                encoding="utf-8",
            )

            result = self._run_workflow(
                "--json",
                "--cp-tools-script",
                str(cp_tools),
                "--cwd",
                str(tmp),
                "doctor",
            )

            self.assertEqual(result.returncode, 0, msg=result.stderr)
            payload = json.loads(result.stdout)
            self.assertEqual(payload["status"], "ok")
            diag_step = next(step for step in payload["steps"] if step["function"] == "cppdiag")
            self.assertEqual(diag_step["returncode"], 7)
            self.assertTrue(diag_step["non_fatal"])
            self.assertIn("diag-failed", diag_step["stderr"])

    def test_module_tester_fails_when_zero_tests_execute(self) -> None:
        """Zero executed tests should be treated as failure."""

        tester = module_tester.ModuleTester(ALGORITHMS_DIR / "templates")
        tester.need_macros = []
        tester.need_mapping = {}

        success = tester.run_all_tests()

        self.assertFalse(success)
        self.assertEqual(tester.test_results, [])

    @unittest.skipIf(shutil.which("zsh") is None, "zsh is required for runtime smoke")
    def test_workflow_new_if_missing_skips_existing_cc_source(self) -> None:
        """`new --if-missing` should honor existing .cc sources, not only .cpp."""

        with tempfile.TemporaryDirectory() as tmpdir:
            tmp = Path(tmpdir)
            (tmp / "problem_A.cc").write_text("int main() { return 0; }\n", encoding="utf-8")
            cp_tools = tmp / "competitive_stub.sh"
            cp_tools.write_text(
                textwrap.dedent(
                    """\
                    cppnew() {
                      print -- "cppnew-called"
                    }
                    """
                ),
                encoding="utf-8",
            )

            result = self._run_workflow(
                "--json",
                "--cp-tools-script",
                str(cp_tools),
                "--cwd",
                str(tmp),
                "new",
                "problem_A",
                "--if-missing",
            )

            self.assertEqual(result.returncode, 0, msg=result.stderr)
            payload = json.loads(result.stdout)
            self.assertEqual(payload["steps"], [])
            self.assertTrue(
                any(
                    "problem_A.cc already exists, skipping cppnew" in note
                    for note in payload["notes"]
                )
            )

    @unittest.skipIf(shutil.which("zsh") is None, "zsh is required for runtime smoke")
    def test_workflow_cycle_new_if_missing_skips_existing_cxx_source(self) -> None:
        """`cycle --new-if-missing` should honor existing .cxx sources."""

        with tempfile.TemporaryDirectory() as tmpdir:
            tmp = Path(tmpdir)
            (tmp / "problem_A.cxx").write_text("int main() { return 0; }\n", encoding="utf-8")
            cp_tools = tmp / "competitive_stub.sh"
            cp_tools.write_text(
                textwrap.dedent(
                    """\
                    cppnew() {
                      print -- "cppnew-called"
                    }
                    """
                ),
                encoding="utf-8",
            )

            result = self._run_workflow(
                "--json",
                "--cp-tools-script",
                str(cp_tools),
                "--cwd",
                str(tmp),
                "cycle",
                "problem_A",
                "--new-if-missing",
                "--skip-go",
                "--skip-judge",
                "--skip-submit",
                "--skip-submit-test",
            )

            self.assertEqual(result.returncode, 0, msg=result.stderr)
            payload = json.loads(result.stdout)
            self.assertEqual(payload["steps"], [])
            self.assertTrue(
                any(
                    "problem_A.cxx already exists, skipping cppnew" in note
                    for note in payload["notes"]
                )
            )

    def test_migrate_round_vscode_links_scripts_directory(self) -> None:
        """VS Code migration should also centralize the shared scripts directory."""

        with tempfile.TemporaryDirectory() as tmpdir:
            tmp = Path(tmpdir)
            rounds_root = tmp / "Rounds"
            round_dir = rounds_root / "Round_Test"
            algorithms_root = tmp / "Algorithms"
            central_vscode = algorithms_root / ".vscode"

            round_dir.mkdir(parents=True)
            (round_dir / "CMakeLists.txt").write_text("cmake_minimum_required(VERSION 3.20)\n", encoding="utf-8")
            central_vscode.mkdir(parents=True)
            (central_vscode / "scripts").mkdir()
            (central_vscode / "scripts" / "cp_sanitize_task.sh").write_text("#!/bin/sh\n", encoding="utf-8")
            for name in ("settings.json", "tasks.json", "launch.json", "c_cpp_properties.json"):
                (central_vscode / name).write_text("{}\n", encoding="utf-8")

            result = subprocess.run(
                [
                    sys.executable,
                    str(MIGRATE_VSCODE_SCRIPT),
                    "--rounds-root",
                    str(rounds_root),
                    "--algorithms-root",
                    str(algorithms_root),
                ],
                capture_output=True,
                text=True,
                check=False,
            )

            self.assertEqual(result.returncode, 0, msg=result.stderr)
            scripts_link = round_dir / ".vscode" / "scripts"
            self.assertTrue(scripts_link.is_symlink())
            self.assertEqual(
                scripts_link.resolve(),
                (central_vscode / "scripts").resolve(),
            )

    def test_migrate_round_vscode_skips_directory_symlink_rounds(self) -> None:
        """Migration should skip rounds already using a directory symlink to central .vscode."""

        with tempfile.TemporaryDirectory() as tmpdir:
            tmp = Path(tmpdir)
            rounds_root = tmp / "Rounds"
            round_dir = rounds_root / "Round_Test"
            algorithms_root = tmp / "Algorithms"
            central_vscode = algorithms_root / ".vscode"

            round_dir.mkdir(parents=True)
            (round_dir / "CMakeLists.txt").write_text("cmake_minimum_required(VERSION 3.20)\n", encoding="utf-8")
            central_vscode.mkdir(parents=True)
            (central_vscode / "scripts").mkdir()
            (central_vscode / "scripts" / "cp_sanitize_task.sh").write_text("#!/bin/sh\n", encoding="utf-8")
            original_settings = "{\n  \"marker\": true\n}\n"
            (central_vscode / "settings.json").write_text(original_settings, encoding="utf-8")
            for name in ("tasks.json", "launch.json", "c_cpp_properties.json"):
                (central_vscode / name).write_text("{}\n", encoding="utf-8")

            (round_dir / ".vscode").symlink_to(
                os.path.relpath(central_vscode, start=round_dir),
                target_is_directory=True,
            )

            result = subprocess.run(
                [
                    sys.executable,
                    str(MIGRATE_VSCODE_SCRIPT),
                    "--rounds-root",
                    str(rounds_root),
                    "--algorithms-root",
                    str(algorithms_root),
                ],
                capture_output=True,
                text=True,
                check=False,
            )

            self.assertEqual(result.returncode, 0, msg=result.stderr)
            self.assertIn("already-linked-dir", result.stdout)
            self.assertEqual(
                (central_vscode / "settings.json").read_text(encoding="utf-8"),
                original_settings,
            )

    def test_migrate_round_cmake_creates_backup_before_rewrite(self) -> None:
        """CMake migration should preserve the previous file when backups are enabled."""

        with tempfile.TemporaryDirectory() as tmpdir:
            tmp = Path(tmpdir)
            rounds_root = tmp / "Rounds"
            round_dir = rounds_root / "Round_Test"
            round_dir.mkdir(parents=True)
            cmake_path = round_dir / "CMakeLists.txt"
            original = "cmake_minimum_required(VERSION 3.20)\nproject(old_round)\n"
            cmake_path.write_text(original, encoding="utf-8")

            result = subprocess.run(
                [
                    sys.executable,
                    str(MIGRATE_CMAKE_SCRIPT),
                    "--rounds-root",
                    str(rounds_root),
                ],
                capture_output=True,
                text=True,
                check=False,
            )

            self.assertEqual(result.returncode, 0, msg=result.stderr)
            backups = list(round_dir.glob("CMakeLists.txt.bak-centralized-*"))
            self.assertEqual(len(backups), 1)
            self.assertEqual(backups[0].read_text(encoding="utf-8"), original)
            self.assertEqual(
                cmake_path.read_text(encoding="utf-8"),
                (
                    "# Thin round-level CMakeLists: delegates all build logic to centralized modules.\n"
                    "cmake_minimum_required(VERSION 3.20)\n"
                    "project(competitive_programming LANGUAGES CXX)\n\n"
                    'set(CP_ROUND_BOOTSTRAP "${CMAKE_CURRENT_SOURCE_DIR}/algorithms/cmake/CPRoundBootstrap.cmake")\n'
                    'if(NOT EXISTS "${CP_ROUND_BOOTSTRAP}")\n'
                    "  message(FATAL_ERROR\n"
                    '    "Central build bootstrap not found: ${CP_ROUND_BOOTSTRAP}\\n"\n'
                    '    "Run cppinit (or recreate algorithms/cmake symlink) to restore shared build modules.")\n'
                    "endif()\n\n"
                    'include("${CP_ROUND_BOOTSTRAP}")\n'
                ),
            )

    def test_migrate_round_presets_creates_backup_before_rewrite(self) -> None:
        """Preset migration should preserve the previous file when backups are enabled."""
        with tempfile.TemporaryDirectory() as tmpdir:
            tmp = Path(tmpdir)
            rounds_root = tmp / "Rounds"
            round_dir = rounds_root / "Round_Test"
            round_dir.mkdir(parents=True)
            (round_dir / "CMakeLists.txt").write_text("cmake_minimum_required(VERSION 3.20)\n", encoding="utf-8")
            presets_path = round_dir / "CMakePresets.json"
            original = '{\n  "version": 3\n}\n'
            presets_path.write_text(original, encoding="utf-8")

            result = subprocess.run(
                [
                    sys.executable,
                    str(MIGRATE_PRESETS_SCRIPT),
                    "--rounds-root",
                    str(rounds_root),
                ],
                capture_output=True,
                text=True,
                check=False,
            )

            self.assertEqual(result.returncode, 0, msg=result.stderr)
            backups = list(round_dir.glob("CMakePresets.json.bak-centralized-*"))
            self.assertEqual(len(backups), 1)
            self.assertEqual(backups[0].read_text(encoding="utf-8"), original)
            self.assertEqual(
                presets_path.read_text(encoding="utf-8"),
                (
                    "{\n"
                    '  "version": 6,\n'
                    '  "include": [\n'
                    '    "algorithms/cmake/CPRoundPresets.json"\n'
                    "  ]\n"
                    "}\n"
                ),
            )

    def test_migrate_round_toolchains_respects_explicit_round_selection(self) -> None:
        """Toolchain migration should only affect explicitly selected rounds."""
        with tempfile.TemporaryDirectory() as tmpdir:
            tmp = Path(tmpdir)
            rounds_root = tmp / "Rounds"
            central_root = tmp / "Algorithms"
            central_root.mkdir(parents=True)
            for name in ("gcc-toolchain.cmake", "clang-toolchain.cmake"):
                (central_root / name).write_text("# central\n", encoding="utf-8")

            selected_round = rounds_root / "Round_Selected"
            untouched_round = rounds_root / "Round_Untouched"
            for round_dir in (selected_round, untouched_round):
                round_dir.mkdir(parents=True)
                (round_dir / "CMakeLists.txt").write_text("cmake_minimum_required(VERSION 3.20)\n", encoding="utf-8")
                for name in ("gcc-toolchain.cmake", "clang-toolchain.cmake"):
                    (round_dir / name).write_text(f"# local {round_dir.name}\n", encoding="utf-8")

            result = subprocess.run(
                [
                    sys.executable,
                    str(MIGRATE_TOOLCHAINS_SCRIPT),
                    "--rounds-root",
                    str(rounds_root),
                    "--central-root",
                    str(central_root),
                    "--round",
                    "Round_Selected",
                ],
                capture_output=True,
                text=True,
                check=False,
            )

            self.assertEqual(result.returncode, 0, msg=result.stderr)
            for name in ("gcc-toolchain.cmake", "clang-toolchain.cmake"):
                self.assertFalse((selected_round / name).exists())
                self.assertEqual(
                    len(list(selected_round.glob(f"{name}.bak-centralized-*"))),
                    1,
                )
                self.assertTrue((untouched_round / name).is_file())


if __name__ == "__main__":
    unittest.main()
