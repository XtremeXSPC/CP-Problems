"""Higher-level workflow handlers used by command registration."""

from __future__ import annotations

import shutil
from typing import List

from .orchestration import WorkflowManager, build_cppconf_args, run_step_with_policy
from .types import WorkflowCommandError


def handle_doctor(manager: WorkflowManager, ns) -> None:
    """Run diagnostics around cpp-tools health and environment readiness."""
    manager.note(f"[workflow] cp-tools script: {manager.runner.script_path}")
    manager.note(f"[workflow] cwd: {manager.runner.cwd}")
    manager.note("[workflow] running diagnostics suite...")

    timeout_tool = shutil.which("timeout") or shutil.which("gtimeout")
    if timeout_tool:
        manager.note(f"[workflow] timeout tool: {timeout_tool}")
    else:
        manager.note(
            "[workflow] warning: neither timeout nor gtimeout found in PATH "
            "(install coreutils if needed)."
        )

    run_step_with_policy(manager, ns, "cpphelp")
    run_step_with_policy(manager, ns, "cppinfo")

    diag_result = manager.run_cpp("cppdiag", check=False)
    if diag_result.returncode != 0:
        manager.note(f"[workflow] warning: cppdiag exited with {diag_result.returncode}")
        if ns.strict:
            raise WorkflowCommandError(diag_result)
        diag_result.returncode = 0

    run_step_with_policy(manager, ns, "cppcheck")


def handle_cycle(manager: WorkflowManager, ns) -> None:
    """Run the default CP lifecycle with optional skip flags."""
    input_name = ns.input

    if ns.configure:
        run_step_with_policy(manager, ns, "cppconf", build_cppconf_args(ns))

    if not ns.skip_new:
        target_file = manager.runner.cwd / f"{ns.name}.cpp"
        if ns.new_if_missing and target_file.exists():
            manager.note(f"[workflow] {target_file.name} already exists, skipping cppnew")
        else:
            run_step_with_policy(manager, ns, "cppnew", [ns.name, ns.template])

    if not ns.skip_go:
        go_args = [ns.name]
        if input_name:
            go_args.append(input_name)
        run_step_with_policy(manager, ns, "cppgo", go_args)

    if not ns.skip_judge:
        run_step_with_policy(manager, ns, "cppjudge", [ns.name])

    if not ns.skip_submit:
        submit_args = [ns.name] if not ns.strict_submit else ["--strict", ns.name]
        run_step_with_policy(manager, ns, "cppsubmit", submit_args)
        if not ns.skip_submit_test:
            test_args: List[str] = []
            if ns.strict_submit:
                test_args.append("--strict")
            test_args.extend(["--no-generate", ns.name])
            if input_name:
                test_args.append(input_name)
            run_step_with_policy(manager, ns, "cpptestsubmit", test_args)
    elif not ns.skip_submit_test:
        test_args = [ns.name]
        if ns.strict_submit:
            test_args.insert(0, "--strict")
        if input_name:
            test_args.append(input_name)
        run_step_with_policy(manager, ns, "cpptestsubmit", test_args)
