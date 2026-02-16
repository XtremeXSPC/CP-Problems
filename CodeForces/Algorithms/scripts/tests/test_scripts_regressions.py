"""Regression tests for scripts-level bugs and edge cases."""

from __future__ import annotations

import argparse
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

            fake_on = {
                "configure_sec": 1.0,
                "cold_build_sec": 2.0,
                "incremental_build_sec": 1.0,
                "reconfigure_sec": 1.0,
                "post_reconfigure_build_sec": 2.0,
            }
            fake_off = {
                "configure_sec": 1.0,
                "cold_build_sec": 1.5,
                "incremental_build_sec": 1.2,
                "reconfigure_sec": 1.0,
                "post_reconfigure_build_sec": 1.6,
            }

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

    def test_module_tester_fails_when_zero_tests_execute(self) -> None:
        """Zero executed tests should be treated as failure."""
        tester = module_tester.ModuleTester(ALGORITHMS_DIR / "templates")
        tester.need_macros = []
        tester.need_mapping = {}

        success = tester.run_all_tests()

        self.assertFalse(success)
        self.assertEqual(tester.test_results, [])


if __name__ == "__main__":
    unittest.main()
