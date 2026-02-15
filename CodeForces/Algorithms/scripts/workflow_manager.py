#!/usr/bin/env python3
"""
Workflow manager for Competitive Programming that orchestrates existing cpp-tools.

Design goals:
- single source of truth: delegate all workflow logic to `cpp*` shell functions
- robust automation: structured subcommands + consistent error handling
- safe execution: allowlisted function dispatch and strict argument validation
- workflow coherence: paths/output contracts match contest layout used by cpp-tools
"""

from __future__ import annotations

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import time
from dataclasses import asdict, dataclass
from pathlib import Path
from typing import List, Optional, Sequence

DEFAULT_WORKSPACE_ROOT = Path("/Volumes/LCS.Data/CP-Problems")
DEFAULT_CP_TOOLS_SCRIPT = Path(
    "/Users/lcs-dev/Dotfiles/cpp-tools/.config/cpp-tools/competitive.sh"
)

TEMPLATE_CHOICES = ("base", "default", "pbds", "advanced")
BUILD_TYPE_CHOICES = ("Debug", "Release", "Sanitize")
COMPILER_CHOICES = ("gcc", "clang", "auto")
TOGGLE_CHOICES = ("on", "off")
PCH_CHOICES = ("on", "off", "auto")

TARGET_RE = re.compile(r"^[A-Za-z][A-Za-z0-9_]*$")
CONTEST_SEGMENT_RE = re.compile(r"^[A-Za-z0-9._-]+$")
ANSI_RE = re.compile(r"\x1B\[[0-9;]*[A-Za-z]")

ALLOWED_FUNCTIONS = {
    "cpparchive",
    "cppbatch",
    "cppbuild",
    "cppcheck",
    "cppclean",
    "cppconf",
    "cppcontest",
    "cppdeepclean",
    "cppdelete",
    "cppdiag",
    "cppforcego",
    "cppfull",
    "cppgcc",
    "cppgo",
    "cpphelp",
    "cppi",
    "cppinfo",
    "cppinit",
    "cppjudge",
    "cppnew",
    "cpprun",
    "cppstats",
    "cppstress",
    "cppsubmit",
    "cpptestsubmit",
    "cppwatch",
    "cppclang",
    "cppprof",
}


class WorkflowError(RuntimeError):
    """Logical error in workflow manager configuration/usage."""


class WorkflowCommandError(WorkflowError):
    """Raised when a delegated cpp-tools command fails."""

    def __init__(self, result: "CommandResult"):
        self.result = result
        super().__init__(
            f"{result.function} failed with exit code {result.returncode}"
        )


@dataclass
class CommandResult:
    function: str
    args: List[str]
    cwd: str
    returncode: int
    duration_ms: int
    stdout: str
    stderr: str
    timed_out: bool = False

    def to_dict(self, strip_ansi: bool = False) -> dict:
        payload = asdict(self)
        if strip_ansi:
            payload["stdout"] = _strip_ansi(payload["stdout"])
            payload["stderr"] = _strip_ansi(payload["stderr"])
        return payload


def _strip_ansi(text: str) -> str:
    return ANSI_RE.sub("", text)


def _is_under(path: Path, root: Path) -> bool:
    try:
        path.resolve().relative_to(root.resolve())
        return True
    except ValueError:
        return False


def _normalize_target(raw: str) -> str:
    candidate = Path(raw).name
    suffix = Path(candidate).suffix.lower()
    if suffix in {".cpp", ".cc", ".cxx"}:
        candidate = Path(candidate).stem
    if not TARGET_RE.fullmatch(candidate):
        raise argparse.ArgumentTypeError(
            "invalid target name; expected [A-Za-z][A-Za-z0-9_]* (optionally with .cpp/.cc/.cxx)"
        )
    return candidate


def _normalize_input_name(raw: str) -> str:
    name = Path(raw).name
    if not name:
        raise argparse.ArgumentTypeError("input filename cannot be empty")
    return name


def _normalize_contest_dir(raw: str) -> str:
    p = Path(raw)
    if p.is_absolute():
        raise argparse.ArgumentTypeError("contest path must be workspace-relative")
    if ".." in p.parts:
        raise argparse.ArgumentTypeError("contest path cannot contain '..'")
    if not p.parts:
        raise argparse.ArgumentTypeError("contest path cannot be empty")
    for seg in p.parts:
        if seg in {".", ""}:
            raise argparse.ArgumentTypeError("contest path contains invalid segment")
        if not CONTEST_SEGMENT_RE.fullmatch(seg):
            raise argparse.ArgumentTypeError(
                f"invalid contest path segment '{seg}' (allowed: letters, digits, '.', '_' and '-')"
            )
    return str(p)


def _discover_cp_tools_script(explicit: Optional[Path]) -> Path:
    candidates: List[Path] = []
    if explicit is not None:
        candidates.append(explicit.expanduser())

    env_script = os.environ.get("CPP_TOOLS_SCRIPT")
    if env_script:
        candidates.append(Path(env_script).expanduser())

    candidates.append(DEFAULT_CP_TOOLS_SCRIPT)
    candidates.append(Path.home() / ".config/cpp-tools/competitive.sh")

    seen = set()
    for candidate in candidates:
        key = str(candidate)
        if key in seen:
            continue
        seen.add(key)
        if candidate.is_file():
            return candidate

    searched = "\n  - ".join(str(c) for c in candidates)
    raise WorkflowError(
        "unable to locate cpp-tools entry script `competitive.sh`.\n"
        f"Searched:\n  - {searched}\n"
        "Use --cp-tools-script to specify it explicitly."
    )


class CppToolsRunner:
    def __init__(
        self,
        cp_tools_script: Path,
        cwd: Path,
        default_timeout: int,
        quiet_load: bool = True,
    ):
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
                stdout=exc.stdout or "",
                stderr=(exc.stderr or "") + "\nCommand timed out.",
                timed_out=True,
            )


class WorkflowManager:
    def __init__(self, runner: CppToolsRunner, json_mode: bool, verbose: bool):
        self.runner = runner
        self.json_mode = json_mode
        self.verbose = verbose
        self.results: List[CommandResult] = []
        self.notes: List[str] = []

        workspace_root = Path(
            os.environ.get("CP_WORKSPACE_ROOT", str(DEFAULT_WORKSPACE_ROOT))
        )
        if not _is_under(self.runner.cwd, workspace_root):
            self.notes.append(
                f"warning: cwd '{self.runner.cwd}' is outside CP_WORKSPACE_ROOT '{workspace_root}'"
            )

    def note(self, message: str) -> None:
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
        if self.verbose and not self.json_mode:
            command_preview = " ".join([function, *args])
            print(f"[workflow] -> {command_preview}")

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

    def emit_json(self, status: str) -> None:
        payload = {
            "status": status,
            "cwd": str(self.runner.cwd),
            "cp_tools_script": str(self.runner.script_path),
            "notes": self.notes,
            "steps": [r.to_dict(strip_ansi=True) for r in self.results],
        }
        print(json.dumps(payload, ensure_ascii=False, indent=2))

    def emit_summary(self) -> None:
        if self.json_mode:
            return
        if not self.results:
            return
        total_ms = sum(r.duration_ms for r in self.results)
        failed = [r for r in self.results if r.returncode != 0]
        status = "OK" if not failed else "FAILED"
        print(
            f"[workflow] {status} | steps={len(self.results)} | "
            f"failed={len(failed)} | total={total_ms}ms"
        )


def _build_cppconf_args(ns: argparse.Namespace) -> List[str]:
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


def _run_step_with_policy(
    manager: WorkflowManager,
    ns: argparse.Namespace,
    function: str,
    args: Sequence[str] = (),
    auto_confirm_deepclean: bool = False,
) -> CommandResult:
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


def handle_init(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    _run_step_with_policy(manager, ns, "cppinit")


def handle_contest(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    _run_step_with_policy(manager, ns, "cppcontest", [ns.directory])


def handle_new(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    target_file = manager.runner.cwd / f"{ns.name}.cpp"
    if ns.if_missing and target_file.exists():
        manager.note(f"[workflow] {target_file.name} already exists, skipping cppnew")
        return
    _run_step_with_policy(manager, ns, "cppnew", [ns.name, ns.template])


def handle_batch(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    _run_step_with_policy(manager, ns, "cppbatch", [str(ns.count), ns.template])


def handle_delete(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    args: List[str] = []
    if ns.yes:
        args.append("-y")
    if ns.no_config:
        args.append("--no-config")
    args.extend(ns.names)
    _run_step_with_policy(manager, ns, "cppdelete", args)


def handle_conf(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    args = _build_cppconf_args(ns)
    _run_step_with_policy(manager, ns, "cppconf", args)


def handle_build(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    args = [ns.target] if ns.target else []
    _run_step_with_policy(manager, ns, "cppbuild", args)


def handle_run(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    args = [ns.target] if ns.target else []
    _run_step_with_policy(manager, ns, "cpprun", args)


def handle_go(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    args: List[str] = []
    if ns.force:
        args.append("--force")
    if ns.target:
        args.append(ns.target)
    if ns.input:
        args.append(ns.input)
    _run_step_with_policy(manager, ns, "cppgo", args)


def handle_forcego(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    args: List[str] = ["--force"]
    if ns.target:
        args.append(ns.target)
    _run_step_with_policy(manager, ns, "cppgo", args)


def handle_judge(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    args = [ns.target] if ns.target else []
    _run_step_with_policy(manager, ns, "cppjudge", args)


def handle_stress(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    args: List[str] = []
    if ns.target:
        args.append(ns.target)
    if ns.iterations:
        args.append(str(ns.iterations))
    _run_step_with_policy(manager, ns, "cppstress", args)


def handle_submit(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    args: List[str] = []
    if getattr(ns, "strict", False):
        args.append("--strict")
    if ns.target:
        args.append(ns.target)
    _run_step_with_policy(manager, ns, "cppsubmit", args)


def handle_test_submit(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    args: List[str] = []
    if ns.strict:
        args.append("--strict")
    if ns.no_generate:
        args.append("--no-generate")
    if ns.target:
        args.append(ns.target)
    if ns.input:
        args.append(ns.input)
    _run_step_with_policy(manager, ns, "cpptestsubmit", args)


def handle_full(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    args: List[str] = []
    if ns.target:
        args.append(ns.target)
    if ns.input:
        args.append(ns.input)
    _run_step_with_policy(manager, ns, "cppfull", args)


def handle_check(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    _run_step_with_policy(manager, ns, "cppcheck")


def handle_info(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    _run_step_with_policy(manager, ns, "cppinfo")


def handle_diag(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    _run_step_with_policy(manager, ns, "cppdiag")


def handle_help(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    _run_step_with_policy(manager, ns, "cpphelp")


def handle_clean(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    _run_step_with_policy(manager, ns, "cppclean")


def handle_deepclean(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    if not ns.yes:
        raise WorkflowError("deepclean is destructive; pass --yes to confirm")
    _run_step_with_policy(
        manager,
        ns,
        "cppdeepclean",
        auto_confirm_deepclean=True,
    )


def handle_stats(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    _run_step_with_policy(manager, ns, "cppstats")


def handle_archive(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    _run_step_with_policy(manager, ns, "cpparchive")


def handle_watch(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    args = [ns.target] if ns.target else []
    _run_step_with_policy(manager, ns, "cppwatch", args)


def handle_exec(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    if not ns.allow_exec:
        raise WorkflowError(
            "exec command is disabled by default for safety. "
            "Re-run with --allow-exec to enable it explicitly."
        )
    _run_step_with_policy(manager, ns, ns.function, ns.args)


def handle_doctor(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    manager.note(f"[workflow] cp-tools script: {manager.runner.script_path}")
    manager.note(f"[workflow] cwd: {manager.runner.cwd}")
    manager.note("[workflow] running diagnostics suite...")

    _run_step_with_policy(manager, ns, "cpphelp")
    _run_step_with_policy(manager, ns, "cppinfo")

    # `cppdiag` can return non-zero even when most diagnostics are printed.
    # Treat it as warning by default, and allow strict mode when desired.
    diag_result = manager.run_cpp("cppdiag", check=False)
    if diag_result.returncode != 0:
        manager.note(
            f"[workflow] warning: cppdiag exited with {diag_result.returncode}"
        )
        if ns.strict:
            raise WorkflowCommandError(diag_result)
        diag_result.returncode = 0

    _run_step_with_policy(manager, ns, "cppcheck")


def handle_cycle(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    """
    High-level contest loop:
    - optional configure
    - optional new
    - optional go
    - optional judge
    - optional submit
    - optional test-submit
    """

    input_name = ns.input

    if ns.configure:
        conf_args = _build_cppconf_args(ns)
        _run_step_with_policy(manager, ns, "cppconf", conf_args)

    if not ns.skip_new:
        target_file = manager.runner.cwd / f"{ns.name}.cpp"
        if ns.new_if_missing and target_file.exists():
            manager.note(
                f"[workflow] {target_file.name} already exists, skipping cppnew"
            )
        else:
            _run_step_with_policy(manager, ns, "cppnew", [ns.name, ns.template])

    if not ns.skip_go:
        go_args = [ns.name]
        if input_name:
            go_args.append(input_name)
        _run_step_with_policy(manager, ns, "cppgo", go_args)

    if not ns.skip_judge:
        _run_step_with_policy(manager, ns, "cppjudge", [ns.name])

    if not ns.skip_submit:
        submit_args = [ns.name]
        if ns.strict_submit:
            submit_args = ["--strict", ns.name]
        _run_step_with_policy(manager, ns, "cppsubmit", submit_args)
        if not ns.skip_submit_test:
            test_args = []
            if ns.strict_submit:
                test_args.append("--strict")
            test_args.extend(["--no-generate", ns.name])
            if input_name:
                test_args.append(input_name)
            _run_step_with_policy(manager, ns, "cpptestsubmit", test_args)
    elif not ns.skip_submit_test:
        # If submit is skipped but submission test is requested, let cpptestsubmit
        # generate the submission itself.
        test_args = []
        if ns.strict_submit:
            test_args.append("--strict")
        test_args.append(ns.name)
        if input_name:
            test_args.append(input_name)
        _run_step_with_policy(manager, ns, "cpptestsubmit", test_args)


def _add_target_arg(
    parser: argparse.ArgumentParser, required: bool = False, name: str = "target"
) -> None:
    parser.add_argument(
        name,
        nargs=None if required else "?",
        type=_normalize_target,
        help="target/problem name (e.g. problem_A or problem_A.cpp)",
    )


def _add_conf_options(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("--build-type", choices=BUILD_TYPE_CHOICES)
    parser.add_argument("--compiler", choices=COMPILER_CHOICES)
    parser.add_argument("--timing", choices=TOGGLE_CHOICES)
    parser.add_argument("--pch", choices=PCH_CHOICES)
    parser.add_argument("--pch-rebuild", action="store_true")
    parser.add_argument(
        "--extra-conf-arg",
        action="append",
        default=[],
        help="extra raw argument forwarded to cppconf (repeatable)",
    )


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description=(
            "Competitive Programming workflow manager built on top of cpp-tools."
        )
    )
    parser.add_argument(
        "--cwd",
        type=Path,
        default=Path.cwd(),
        help="working directory where commands are executed",
    )
    parser.add_argument(
        "--cp-tools-script",
        type=Path,
        default=None,
        help="path to competitive.sh (cpp-tools entry script)",
    )
    parser.add_argument(
        "--timeout",
        type=int,
        default=600,
        help="timeout in seconds for each delegated command",
    )
    parser.add_argument(
        "--json",
        action="store_true",
        help="emit structured JSON output instead of plain text",
    )
    parser.add_argument(
        "--verbose",
        action="store_true",
        help="print delegated command names before execution",
    )
    parser.add_argument(
        "--show-load-banner",
        action="store_true",
        help="do not suppress cpp-tools load banner",
    )
    parser.add_argument(
        "--continue-on-error",
        action="store_true",
        help="continue executing subsequent steps even if one step fails",
    )
    parser.add_argument(
        "--allow-exec",
        action="store_true",
        help="allow the expert 'exec' subcommand (disabled by default for safety)",
    )

    subparsers = parser.add_subparsers(dest="command", required=True)

    # setup
    p = subparsers.add_parser("init", help="run cppinit")
    p.set_defaults(handler=handle_init)

    p = subparsers.add_parser("contest", help="run cppcontest <dir>")
    p.add_argument("directory", type=_normalize_contest_dir)
    p.set_defaults(handler=handle_contest)

    p = subparsers.add_parser("new", help="run cppnew <name> [template]")
    p.add_argument("name", type=_normalize_target)
    p.add_argument("--template", choices=TEMPLATE_CHOICES, default="base")
    p.add_argument(
        "--if-missing",
        action="store_true",
        help="skip cppnew if <name>.cpp already exists",
    )
    p.set_defaults(handler=handle_new)

    p = subparsers.add_parser("batch", help="run cppbatch")
    p.add_argument("count", type=int)
    p.add_argument("--template", choices=TEMPLATE_CHOICES, default="base")
    p.set_defaults(handler=handle_batch)

    p = subparsers.add_parser("delete", help="run cppdelete for one or more problems")
    p.add_argument("names", nargs="+", type=_normalize_target)
    p.add_argument("-y", "--yes", action="store_true")
    p.add_argument("--no-config", action="store_true")
    p.set_defaults(handler=handle_delete)

    p = subparsers.add_parser("conf", help="run cppconf")
    _add_conf_options(p)
    p.set_defaults(handler=handle_conf)

    # build/run/test
    p = subparsers.add_parser("build", help="run cppbuild")
    _add_target_arg(p)
    p.set_defaults(handler=handle_build)

    p = subparsers.add_parser("run", help="run cpprun")
    _add_target_arg(p)
    p.set_defaults(handler=handle_run)

    p = subparsers.add_parser("go", help="run cppgo")
    _add_target_arg(p)
    p.add_argument(
        "--force",
        action="store_true",
        help="force rebuild by touching the target source before build",
    )
    p.add_argument(
        "--input",
        type=_normalize_input_name,
        help="input filename inside input_cases/",
    )
    p.set_defaults(handler=handle_go)

    p = subparsers.add_parser("forcego", help="run cppforcego")
    _add_target_arg(p)
    p.set_defaults(handler=handle_forcego)

    p = subparsers.add_parser("judge", help="run cppjudge")
    _add_target_arg(p)
    p.set_defaults(handler=handle_judge)

    # compatibility alias: old manager used 'test'
    p = subparsers.add_parser("test", help="alias of judge for compatibility")
    _add_target_arg(p)
    p.set_defaults(handler=handle_judge)

    p = subparsers.add_parser("stress", help="run cppstress")
    _add_target_arg(p)
    p.add_argument("--iterations", type=int, default=None)
    p.set_defaults(handler=handle_stress)

    # submission
    p = subparsers.add_parser("submit", help="run cppsubmit")
    _add_target_arg(p)
    p.add_argument(
        "--strict",
        action="store_true",
        help="use judge-like strict submission compilation profile",
    )
    p.set_defaults(handler=handle_submit)

    p = subparsers.add_parser("test-submit", help="run cpptestsubmit")
    _add_target_arg(p)
    p.add_argument(
        "--input",
        type=_normalize_input_name,
        help="input filename inside input_cases/",
    )
    p.add_argument(
        "--strict",
        action="store_true",
        help="use judge-like strict submission compilation profile",
    )
    p.add_argument("--no-generate", action="store_true")
    p.set_defaults(handler=handle_test_submit)

    p = subparsers.add_parser("full", help="run cppfull")
    _add_target_arg(p)
    p.add_argument(
        "--input",
        type=_normalize_input_name,
        help="input filename inside input_cases/",
    )
    p.set_defaults(handler=handle_full)

    # utilities
    p = subparsers.add_parser("check", help="run cppcheck")
    p.set_defaults(handler=handle_check)

    p = subparsers.add_parser("info", help="run cppinfo")
    p.set_defaults(handler=handle_info)

    p = subparsers.add_parser("diag", help="run cppdiag")
    p.set_defaults(handler=handle_diag)

    p = subparsers.add_parser("help", help="run cpphelp")
    p.set_defaults(handler=handle_help)

    p = subparsers.add_parser("clean", help="run cppclean")
    p.set_defaults(handler=handle_clean)

    p = subparsers.add_parser("deepclean", help="run cppdeepclean (requires --yes)")
    p.add_argument("--yes", action="store_true")
    p.set_defaults(handler=handle_deepclean)

    p = subparsers.add_parser("stats", help="run cppstats")
    p.set_defaults(handler=handle_stats)

    p = subparsers.add_parser("archive", help="run cpparchive")
    p.set_defaults(handler=handle_archive)

    p = subparsers.add_parser("watch", help="run cppwatch")
    _add_target_arg(p)
    p.set_defaults(handler=handle_watch)

    # diagnosis/orchestration
    p = subparsers.add_parser("doctor", help="run manager + cpp-tools diagnostics")
    p.add_argument(
        "--strict",
        action="store_true",
        help="treat cppdiag non-zero exit as fatal",
    )
    p.set_defaults(handler=handle_doctor)

    p = subparsers.add_parser(
        "cycle",
        help="orchestrate a standard CP loop (new/go/judge/submit/test-submit)",
    )
    p.add_argument("name", type=_normalize_target)
    p.add_argument("--template", choices=TEMPLATE_CHOICES, default="base")
    p.add_argument(
        "--input",
        type=_normalize_input_name,
        help="input filename inside input_cases/",
    )
    p.add_argument("--configure", action="store_true", help="run cppconf first")
    _add_conf_options(p)
    p.add_argument(
        "--new-if-missing",
        action="store_true",
        help="skip cppnew if the target source already exists",
    )
    p.add_argument("--skip-new", action="store_true")
    p.add_argument("--skip-go", action="store_true")
    p.add_argument("--skip-judge", action="store_true")
    p.add_argument("--skip-submit", action="store_true")
    p.add_argument("--skip-submit-test", action="store_true")
    p.add_argument(
        "--strict-submit",
        action="store_true",
        help="run submission and submission-test with judge-like strict profile",
    )
    p.set_defaults(handler=handle_cycle)

    # expert escape hatch
    p = subparsers.add_parser(
        "exec",
        help="execute a raw allowlisted cpp-tools function (expert mode)",
    )
    p.add_argument("function", choices=sorted(ALLOWED_FUNCTIONS))
    p.add_argument("args", nargs=argparse.REMAINDER, default=[])
    p.set_defaults(handler=handle_exec)

    return parser


def main(argv: Optional[Sequence[str]] = None) -> int:
    parser = build_parser()
    ns = parser.parse_args(argv)

    manager: Optional[WorkflowManager] = None
    exit_code = 0

    try:
        cp_tools_script = _discover_cp_tools_script(ns.cp_tools_script)
        runner = CppToolsRunner(
            cp_tools_script=cp_tools_script,
            cwd=ns.cwd,
            default_timeout=ns.timeout,
            quiet_load=not ns.show_load_banner,
        )
        manager = WorkflowManager(runner=runner, json_mode=ns.json, verbose=ns.verbose)
        ns.handler(manager, ns)
        if any(step.returncode != 0 for step in manager.results):
            exit_code = 1
    except WorkflowCommandError as exc:
        exit_code = exc.result.returncode if exc.result.returncode != 0 else 1
        if manager is None:
            if ns.json:
                print(
                    json.dumps(
                        {"status": "error", "error": str(exc)}, ensure_ascii=False, indent=2
                    )
                )
            else:
                print(f"Error: {exc}", file=sys.stderr)
            return exit_code
    except WorkflowError as exc:
        exit_code = 1
        if ns.json:
            print(
                json.dumps(
                    {"status": "error", "error": str(exc)}, ensure_ascii=False, indent=2
                )
            )
        else:
            print(f"Error: {exc}", file=sys.stderr)
        return exit_code
    except KeyboardInterrupt:
        exit_code = 130
        if ns.json:
            print(
                json.dumps(
                    {"status": "error", "error": "interrupted"}, ensure_ascii=False, indent=2
                )
            )
        else:
            print("Interrupted.", file=sys.stderr)
        return exit_code

    assert manager is not None
    if ns.json:
        manager.emit_json("ok" if exit_code == 0 else "error")
    else:
        manager.emit_summary()
    return exit_code


if __name__ == "__main__":
    sys.exit(main())
