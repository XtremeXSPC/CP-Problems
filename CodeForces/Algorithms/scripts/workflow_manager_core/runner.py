"""Low-level command runner for cpp-tools delegation."""

import os
import shutil
import subprocess
import time
from pathlib import Path
from typing import Optional, Sequence

from .constants import ALLOWED_FUNCTIONS
from .types import CommandResult, WorkflowError, ensure_text, format_timeout_stderr


class CppToolsRunner:
    """Executes allowlisted cpp-tools functions in a controlled shell wrapper."""

    def __init__(
        self,
        cp_tools_script: Path,
        cwd: Path,
        default_timeout: int,
        quiet_load: bool = True,
    ):
        """Validate runtime prerequisites and cache immutable runner settings."""
        if shutil.which("zsh") is None:
            raise WorkflowError("zsh is required but was not found in PATH")
        if not cp_tools_script.is_file():
            raise WorkflowError(f"cpp-tools script not found: {cp_tools_script}")
        if not cwd.exists() or not cwd.is_dir():
            raise WorkflowError(f"invalid working directory: {cwd}")

        self.script_path = cp_tools_script.resolve()
        self.cwd = cwd.resolve()
        self.default_timeout = default_timeout
        self.quiet_load = quiet_load
        self._wrapper_script = self._build_wrapper_script()

    @staticmethod
    def _build_wrapper_script() -> str:
        """Return the in-memory zsh wrapper enforcing allowlisted functions."""
        allowed_cases = "\n".join(
            f"  {name}) ;;" for name in sorted(ALLOWED_FUNCTIONS)
        )
        return (
            "set -eo pipefail\n"
            'export CP_QUIET_LOAD="${CP_QUIET_LOAD:-1}"\n'
            'source "$1"\n'
            "shift\n"
            'func="$1"\n'
            "shift\n"
            "case \"$func\" in\n"
            f"{allowed_cases}\n"
            "  *)\n"
            '    print -u2 -- "Unsupported cpp-tools function: $func"\n'
            "    exit 64\n"
            "    ;;\n"
            "esac\n"
            "if [[ \"$func\" == \"cppdeepclean\" && "
            '"${CP_AUTO_CONFIRM_DEEPCLEAN:-0}" == "1" ]]; then\n'
            "  printf 'y\\n' | cppdeepclean \"$@\"\n"
            "else\n"
            "  \"$func\" \"$@\"\n"
            "fi\n"
        )

    def run(
        self,
        function: str,
        args: Sequence[str] = (),
        timeout: Optional[int] = None,
        auto_confirm_deepclean: bool = False,
    ) -> CommandResult:
        """Execute one cpp-tools function and return normalized command metadata."""
        if function not in ALLOWED_FUNCTIONS:
            raise WorkflowError(f"function not allowlisted: {function}")

        cmd = [
            "zsh",
            "-lc",
            self._wrapper_script,
            "workflow_manager",
            str(self.script_path),
            function,
            *args,
        ]
        env = os.environ.copy()
        env["CP_QUIET_LOAD"] = "1" if self.quiet_load else "0"
        if auto_confirm_deepclean:
            env["CP_AUTO_CONFIRM_DEEPCLEAN"] = "1"

        start = time.perf_counter()
        try:
            completed = subprocess.run(
                cmd,
                cwd=str(self.cwd),
                env=env,
                capture_output=True,
                text=True,
                timeout=timeout if timeout is not None else self.default_timeout,
                check=False,
            )
            elapsed_ms = int((time.perf_counter() - start) * 1000)
            return CommandResult(
                function=function,
                args=list(args),
                cwd=str(self.cwd),
                returncode=completed.returncode,
                duration_ms=elapsed_ms,
                stdout=completed.stdout,
                stderr=completed.stderr,
                timed_out=False,
            )
        except subprocess.TimeoutExpired as exc:
            elapsed_ms = int((time.perf_counter() - start) * 1000)
            return CommandResult(
                function=function,
                args=list(args),
                cwd=str(self.cwd),
                returncode=124,
                duration_ms=elapsed_ms,
                stdout=ensure_text(exc.stdout),
                stderr=format_timeout_stderr(exc.stderr),
                timed_out=True,
            )
