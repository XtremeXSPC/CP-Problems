"""Smoke tests for workflow_manager CLI contract."""

import json
import shutil
import subprocess
import sys
import tempfile
import textwrap
import unittest
from pathlib import Path

SCRIPT = Path(__file__).resolve().parents[1] / "workflow_manager.py"


class WorkflowManagerCliSmokeTests(unittest.TestCase):
    def _run(self, *args: str) -> subprocess.CompletedProcess:
        """Execute workflow_manager.py with provided arguments."""
        return subprocess.run(
            [sys.executable, str(SCRIPT), *args],
            capture_output=True,
            text=True,
            check=False,
        )

    def test_root_help_includes_cycle_and_excludes_exec(self) -> None:
        """Root help should expose cycle and not mention removed exec."""
        result = self._run("--help")
        self.assertEqual(result.returncode, 0)
        self.assertIn("cycle", result.stdout)
        self.assertNotIn(" exec ", result.stdout)

    def test_exec_subcommand_is_removed(self) -> None:
        """Removed `exec` subcommand should be rejected by argparse."""
        result = self._run("exec")
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("invalid choice", result.stderr)

    def test_cycle_help_is_available(self) -> None:
        """Cycle subcommand help should render successfully."""
        result = self._run("cycle", "--help")
        self.assertEqual(result.returncode, 0)
        self.assertIn("--strict-submit", result.stdout)

    @unittest.skipIf(shutil.which("zsh") is None, "zsh is required for runtime smoke")
    def test_json_help_command_with_stub_cp_tools(self) -> None:
        """JSON mode should serialize successful delegated command output."""
        with tempfile.TemporaryDirectory() as tmpdir:
            tmp = Path(tmpdir)
            cp_tools = tmp / "competitive_stub.sh"
            cp_tools.write_text(
                textwrap.dedent(
                    """\
                    cpphelp() {
                      print -- "stub-help-ok"
                    }
                    """
                ),
                encoding="utf-8",
            )

            result = self._run(
                "--json",
                "--cp-tools-script",
                str(cp_tools),
                "--cwd",
                str(tmp),
                "help",
            )
            self.assertEqual(result.returncode, 0, msg=result.stderr)

            payload = json.loads(result.stdout)
            self.assertEqual(payload.get("status"), "ok")
            self.assertEqual(len(payload.get("steps", [])), 1)
            self.assertEqual(payload["steps"][0]["function"], "cpphelp")
            self.assertIn("stub-help-ok", payload["steps"][0]["stdout"])


if __name__ == "__main__":
    unittest.main()
