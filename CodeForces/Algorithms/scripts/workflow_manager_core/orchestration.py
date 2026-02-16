"""Workflow orchestration state and execution policies."""

import json
import os
import subprocess
import sys
import time
from pathlib import Path
from typing import List, Optional, Sequence

from .constants import DEFAULT_WORKSPACE_ROOT
from .runner import CppToolsRunner
from .types import (
    CommandResult,
    WorkflowCommandError,
    WorkflowError,
    ensure_text,
    format_timeout_stderr,
)
from .utils import is_under


class WorkflowManager:
    """Tracks workflow execution steps and exposes controlled run helpers."""

    def __init__(self, runner: CppToolsRunner, json_mode: bool, verbose: bool):
        """Initialize execution state and workspace safety notes."""
        self.runner = runner
        self.json_mode = json_mode
        self.verbose = verbose
        self.results: List[CommandResult] = []
        self.notes: List[str] = []

        workspace_root = Path(
            os.environ.get("CP_WORKSPACE_ROOT", str(DEFAULT_WORKSPACE_ROOT))
        )
        if not is_under(self.runner.cwd, workspace_root):
            self.notes.append(
                f"warning: cwd '{self.runner.cwd}' is outside CP_WORKSPACE_ROOT '{workspace_root}'"
            )

    def note(self, message: str) -> None:
        """Record an informational message and print it in text mode."""
        self.notes.append(message)
        if not self.json_mode:
            print(message)

    def run_cpp(
        self,
        function: str,
        args: Sequence[str] = (),
        check: bool = True,
        timeout: Optional[int] = None,
        auto_confirm_deepclean: bool = False,
    ) -> CommandResult:
        """Run one cpp-tools command and store its result."""
        if self.verbose and not self.json_mode:
            print(f"[workflow] -> {' '.join([function, *args])}")

        result = self.runner.run(
            function=function,
            args=args,
            timeout=timeout,
            auto_confirm_deepclean=auto_confirm_deepclean,
        )
        self.results.append(result)

        if not self.json_mode:
            if result.stdout:
                print(result.stdout, end="")
            if result.stderr:
                print(result.stderr, end="", file=sys.stderr)

        if check and result.returncode != 0:
            raise WorkflowCommandError(result)
        return result

    def run_external(
        self,
        argv: Sequence[str],
        check: bool = True,
        timeout: Optional[int] = None,
        env_overrides: Optional[dict] = None,
    ) -> CommandResult:
        """Run an external command with the same reporting semantics as cpp-tools."""
        if not argv:
            raise WorkflowError("run_external requires a non-empty command")

        if self.verbose and not self.json_mode:
            print(f"[workflow] -> {' '.join(argv)}")

        env = os.environ.copy()
        if env_overrides:
            env.update(env_overrides)

        start = time.perf_counter()
        try:
            completed = subprocess.run(
                list(argv),
                cwd=str(self.runner.cwd),
                env=env,
                capture_output=True,
                text=True,
                timeout=timeout if timeout is not None else self.runner.default_timeout,
                check=False,
            )
            elapsed_ms = int((time.perf_counter() - start) * 1000)
            result = CommandResult(
                function=argv[0],
                args=list(argv[1:]),
                cwd=str(self.runner.cwd),
                returncode=completed.returncode,
                duration_ms=elapsed_ms,
                stdout=completed.stdout,
                stderr=completed.stderr,
                timed_out=False,
            )
        except subprocess.TimeoutExpired as exc:
            elapsed_ms = int((time.perf_counter() - start) * 1000)
            result = CommandResult(
                function=argv[0],
                args=list(argv[1:]),
                cwd=str(self.runner.cwd),
                returncode=124,
                duration_ms=elapsed_ms,
                stdout=ensure_text(exc.stdout),
                stderr=format_timeout_stderr(exc.stderr),
                timed_out=True,
            )

        self.results.append(result)

        if not self.json_mode:
            if result.stdout:
                print(result.stdout, end="")
            if result.stderr:
                print(result.stderr, end="", file=sys.stderr)

        if check and result.returncode != 0:
            raise WorkflowCommandError(result)
        return result

    def emit_json(self, status: str) -> None:
        """Emit complete workflow results as JSON."""
        payload = {
            "status": status,
            "cwd": str(self.runner.cwd),
            "cp_tools_script": str(self.runner.script_path),
            "notes": self.notes,
            "steps": [r.to_dict(strip_ansi=True) for r in self.results],
        }
        print(json.dumps(payload, ensure_ascii=False, indent=2))

    def emit_summary(self) -> None:
        """Print a compact final status line in text mode."""
        if self.json_mode or not self.results:
            return
        total_ms = sum(r.duration_ms for r in self.results)
        failed = [r for r in self.results if r.returncode != 0]
        status = "OK" if not failed else "FAILED"
        print(
            f"[workflow] {status} | steps={len(self.results)} | "
            f"failed={len(failed)} | total={total_ms}ms"
        )


def build_cppconf_args(ns) -> List[str]:
    """Translate CLI namespace fields into raw `cppconf` arguments."""
    args: List[str] = []
    if ns.build_type:
        args.append(ns.build_type)
    if ns.compiler:
        args.append(ns.compiler)
    if ns.timing:
        args.append(f"timing={ns.timing}")
    if ns.pch:
        args.append(f"pch={ns.pch}")
    if ns.pch_rebuild:
        args.append("pch-rebuild")
    if ns.extra_conf_arg:
        args.extend(ns.extra_conf_arg)
    return args


def run_step_with_policy(
    manager: WorkflowManager,
    ns,
    function: str,
    args: Sequence[str] = (),
    auto_confirm_deepclean: bool = False,
) -> CommandResult:
    """Run one cpp-tools step honoring `--continue-on-error` policy."""
    check = not getattr(ns, "continue_on_error", False)
    result = manager.run_cpp(
        function=function,
        args=args,
        check=check,
        auto_confirm_deepclean=auto_confirm_deepclean,
    )
    if not check and result.returncode != 0:
        manager.note(
            f"[workflow] step failed but continuing: {function} "
            f"(exit {result.returncode})"
        )
    return result


def run_external_step_with_policy(
    manager: WorkflowManager,
    ns,
    argv: Sequence[str],
    env_overrides: Optional[dict] = None,
) -> CommandResult:
    """Run one external step honoring `--continue-on-error` policy."""
    check = not getattr(ns, "continue_on_error", False)
    result = manager.run_external(
        argv=argv,
        check=check,
        env_overrides=env_overrides,
    )
    if not check and result.returncode != 0:
        manager.note(
            f"[workflow] step failed but continuing: {' '.join(argv)} "
            f"(exit {result.returncode})"
        )
    return result
