#!/usr/bin/env python3
"""Thin CLI entrypoint for the competitive programming workflow manager."""

from __future__ import annotations

import json
import sys
from typing import Optional, Sequence

from workflow_manager_core.orchestration import WorkflowManager
from workflow_manager_core.parser import build_parser
from workflow_manager_core.runner import CppToolsRunner
from workflow_manager_core.types import WorkflowCommandError, WorkflowError
from workflow_manager_core.utils import discover_cp_tools_script


def main(argv: Optional[Sequence[str]] = None) -> int:
    parser = build_parser()
    ns = parser.parse_args(argv)

    manager: Optional[WorkflowManager] = None
    exit_code = 0

    try:
        cp_tools_script = discover_cp_tools_script(ns.cp_tools_script)
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
                        {"status": "error", "error": str(exc)},
                        ensure_ascii=False,
                        indent=2,
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
                    {"status": "error", "error": str(exc)},
                    ensure_ascii=False,
                    indent=2,
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
                    {"status": "error", "error": "interrupted"},
                    ensure_ascii=False,
                    indent=2,
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
