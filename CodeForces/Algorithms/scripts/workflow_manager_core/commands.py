"""Command registry and handlers for workflow manager CLI."""

from __future__ import annotations

import argparse
from dataclasses import dataclass
from pathlib import Path
from typing import Callable, Iterable, List, Optional

from .constants import (
    BUILD_TYPE_CHOICES,
    CMAKE_BUILD_PRESET_CHOICES,
    CMAKE_CONFIG_PRESET_CHOICES,
    COMPILER_CHOICES,
    INPUT_FILE_HELP,
    PCH_CHOICES,
    TEMPLATE_CHOICES,
    TOGGLE_CHOICES,
)
from .orchestration import (
    WorkflowManager,
    build_cppconf_args,
    run_external_step_with_policy,
    run_step_with_policy,
)
from .types import WorkflowCommandError, WorkflowError
from .utils import (
    discover_algorithms_dir,
    normalize_contest_dir,
    normalize_input_name,
    normalize_target,
)

ParserConfigurator = Callable[[argparse.ArgumentParser], None]
CommandHandler = Callable[[WorkflowManager, argparse.Namespace], None]
ArgsBuilder = Callable[[argparse.Namespace], List[str]]
PreHook = Callable[[WorkflowManager, argparse.Namespace], bool]


@dataclass(frozen=True)
class CommandSpec:
    name: str
    help: str
    handler: CommandHandler
    configure: Optional[ParserConfigurator] = None


def _add_target_arg(
    parser: argparse.ArgumentParser, required: bool = False, name: str = "target"
) -> None:
    parser.add_argument(
        name,
        nargs=None if required else "?",
        type=normalize_target,
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


def _add_algorithms_dir_option(parser: argparse.ArgumentParser) -> None:
    parser.add_argument(
        "--algorithms-dir",
        type=Path,
        default=None,
        help="override centralized Algorithms directory path",
    )


def _no_args(_: argparse.Namespace) -> List[str]:
    return []


def _target_args(ns: argparse.Namespace) -> List[str]:
    return [ns.target] if ns.target else []


def _build_args(ns: argparse.Namespace) -> List[str]:
    return _target_args(ns)


def _run_args(ns: argparse.Namespace) -> List[str]:
    return _target_args(ns)


def _go_args(ns: argparse.Namespace) -> List[str]:
    args: List[str] = []
    if ns.force:
        args.append("--force")
    if ns.target:
        args.append(ns.target)
    if ns.input:
        args.append(ns.input)
    return args


def _forcego_args(ns: argparse.Namespace) -> List[str]:
    args: List[str] = ["--force"]
    if ns.target:
        args.append(ns.target)
    return args


def _stress_args(ns: argparse.Namespace) -> List[str]:
    args: List[str] = []
    if ns.target:
        args.append(ns.target)
    if ns.iterations:
        args.append(str(ns.iterations))
    return args


def _submit_args(ns: argparse.Namespace) -> List[str]:
    args: List[str] = []
    if getattr(ns, "strict", False):
        args.append("--strict")
    if ns.target:
        args.append(ns.target)
    return args


def _test_submit_args(ns: argparse.Namespace) -> List[str]:
    args: List[str] = []
    if ns.strict:
        args.append("--strict")
    if ns.no_generate:
        args.append("--no-generate")
    if ns.target:
        args.append(ns.target)
    if ns.input:
        args.append(ns.input)
    return args


def _full_args(ns: argparse.Namespace) -> List[str]:
    args: List[str] = []
    if ns.target:
        args.append(ns.target)
    if ns.input:
        args.append(ns.input)
    return args


def _new_args(ns: argparse.Namespace) -> List[str]:
    return [ns.name, ns.template]


def _batch_args(ns: argparse.Namespace) -> List[str]:
    return [str(ns.count), ns.template]


def _delete_args(ns: argparse.Namespace) -> List[str]:
    args: List[str] = []
    if ns.yes:
        args.append("-y")
    if ns.no_config:
        args.append("--no-config")
    args.extend(ns.names)
    return args


def _conf_args(ns: argparse.Namespace) -> List[str]:
    return build_cppconf_args(ns)


def _skip_new_if_missing(manager: WorkflowManager, ns: argparse.Namespace) -> bool:
    target_file = manager.runner.cwd / f"{ns.name}.cpp"
    if ns.if_missing and target_file.exists():
        manager.note(f"[workflow] {target_file.name} already exists, skipping cppnew")
        return True
    return False


def _require_deepclean_yes(_: WorkflowManager, ns: argparse.Namespace) -> bool:
    if not ns.yes:
        raise WorkflowError("deepclean is destructive; pass --yes to confirm")
    return False


def _cpp_handler(
    function: str,
    *,
    args_builder: ArgsBuilder = _no_args,
    pre_hook: Optional[PreHook] = None,
    auto_confirm_deepclean: bool = False,
) -> CommandHandler:
    def _handler(manager: WorkflowManager, ns: argparse.Namespace) -> None:
        if pre_hook is not None and pre_hook(manager, ns):
            return
        run_step_with_policy(
            manager,
            ns,
            function,
            args_builder(ns),
            auto_confirm_deepclean=auto_confirm_deepclean,
        )

    return _handler


def _handle_preset_conf(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    algorithms_dir = discover_algorithms_dir(ns.algorithms_dir)
    env = {"CP_ALGORITHMS_DIR": str(algorithms_dir)}

    argv: List[str] = ["cmake", "--preset", ns.preset]
    if ns.fresh:
        argv.append("--fresh")

    run_external_step_with_policy(manager, ns, argv, env_overrides=env)


def _handle_preset_build(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    if ns.jobs is not None and ns.jobs <= 0:
        raise WorkflowError("--jobs must be a positive integer")

    argv: List[str] = ["cmake", "--build", "--preset", ns.preset]
    if ns.target:
        argv.extend(["--target", ns.target])
    if ns.jobs:
        argv.extend(["-j", str(ns.jobs)])

    run_external_step_with_policy(manager, ns, argv)


def _handle_doctor(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    manager.note(f"[workflow] cp-tools script: {manager.runner.script_path}")
    manager.note(f"[workflow] cwd: {manager.runner.cwd}")
    manager.note("[workflow] running diagnostics suite...")

    run_step_with_policy(manager, ns, "cpphelp")
    run_step_with_policy(manager, ns, "cppinfo")

    diag_result = manager.run_cpp("cppdiag", check=False)
    if diag_result.returncode != 0:
        manager.note(f"[workflow] warning: cppdiag exited with {diag_result.returncode}")
        if ns.strict:
            raise WorkflowCommandError(diag_result)
        diag_result.returncode = 0

    run_step_with_policy(manager, ns, "cppcheck")


def _handle_cycle(manager: WorkflowManager, ns: argparse.Namespace) -> None:
    input_name = ns.input

    if ns.configure:
        run_step_with_policy(manager, ns, "cppconf", build_cppconf_args(ns))

    if not ns.skip_new:
        target_file = manager.runner.cwd / f"{ns.name}.cpp"
        if ns.new_if_missing and target_file.exists():
            manager.note(
                f"[workflow] {target_file.name} already exists, skipping cppnew"
            )
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


def _configure_contest(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("directory", type=normalize_contest_dir)


def _configure_new(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("name", type=normalize_target)
    parser.add_argument("--template", choices=TEMPLATE_CHOICES, default="base")
    parser.add_argument(
        "--if-missing",
        action="store_true",
        help="skip cppnew if <name>.cpp already exists",
    )


def _configure_batch(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("count", type=int)
    parser.add_argument("--template", choices=TEMPLATE_CHOICES, default="base")


def _configure_delete(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("names", nargs="+", type=normalize_target)
    parser.add_argument("-y", "--yes", action="store_true")
    parser.add_argument("--no-config", action="store_true")


def _configure_conf(parser: argparse.ArgumentParser) -> None:
    _add_conf_options(parser)


def _configure_preset_conf(parser: argparse.ArgumentParser) -> None:
    parser.add_argument(
        "--preset",
        choices=CMAKE_CONFIG_PRESET_CHOICES,
        default="cp-debug-gcc",
    )
    parser.add_argument(
        "--fresh",
        action="store_true",
        help="pass --fresh to cmake preset configure",
    )
    _add_algorithms_dir_option(parser)


def _configure_preset_build(parser: argparse.ArgumentParser) -> None:
    parser.add_argument(
        "--preset",
        choices=CMAKE_BUILD_PRESET_CHOICES,
        default="cp-build-debug-gcc",
    )
    parser.add_argument("--target", default=None, help="optional build target name")
    parser.add_argument("--jobs", type=int, default=None, help="parallel build jobs")


def _configure_target_optional(parser: argparse.ArgumentParser) -> None:
    _add_target_arg(parser)


def _configure_go(parser: argparse.ArgumentParser) -> None:
    _add_target_arg(parser)
    parser.add_argument(
        "--force",
        action="store_true",
        help="force rebuild by touching the target source before build",
    )
    parser.add_argument("--input", type=normalize_input_name, help=INPUT_FILE_HELP)


def _configure_stress(parser: argparse.ArgumentParser) -> None:
    _add_target_arg(parser)
    parser.add_argument("--iterations", type=int, default=None)


def _configure_submit(parser: argparse.ArgumentParser) -> None:
    _add_target_arg(parser)
    parser.add_argument(
        "--strict",
        action="store_true",
        help="use judge-like strict submission compilation profile",
    )


def _configure_test_submit(parser: argparse.ArgumentParser) -> None:
    _add_target_arg(parser)
    parser.add_argument("--input", type=normalize_input_name, help=INPUT_FILE_HELP)
    parser.add_argument(
        "--strict",
        action="store_true",
        help="use judge-like strict submission compilation profile",
    )
    parser.add_argument("--no-generate", action="store_true")


def _configure_full(parser: argparse.ArgumentParser) -> None:
    _add_target_arg(parser)
    parser.add_argument("--input", type=normalize_input_name, help=INPUT_FILE_HELP)


def _configure_deepclean(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("--yes", action="store_true")


def _configure_doctor(parser: argparse.ArgumentParser) -> None:
    parser.add_argument(
        "--strict",
        action="store_true",
        help="treat cppdiag non-zero exit as fatal",
    )


def _configure_cycle(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("name", type=normalize_target)
    parser.add_argument("--template", choices=TEMPLATE_CHOICES, default="base")
    parser.add_argument("--input", type=normalize_input_name, help=INPUT_FILE_HELP)
    parser.add_argument("--configure", action="store_true", help="run cppconf first")
    _add_conf_options(parser)
    parser.add_argument(
        "--new-if-missing",
        action="store_true",
        help="skip cppnew if the target source already exists",
    )
    parser.add_argument("--skip-new", action="store_true")
    parser.add_argument("--skip-go", action="store_true")
    parser.add_argument("--skip-judge", action="store_true")
    parser.add_argument("--skip-submit", action="store_true")
    parser.add_argument("--skip-submit-test", action="store_true")
    parser.add_argument(
        "--strict-submit",
        action="store_true",
        help="run submission and submission-test with judge-like strict profile",
    )


def get_command_specs() -> Iterable[CommandSpec]:
    """Return declarative command definitions for parser and dispatcher wiring."""

    return (
        CommandSpec("init", "run cppinit", _cpp_handler("cppinit")),
        CommandSpec(
            "contest",
            "run cppcontest <dir>",
            _cpp_handler("cppcontest", args_builder=lambda ns: [ns.directory]),
            _configure_contest,
        ),
        CommandSpec(
            "new",
            "run cppnew <name> [template]",
            _cpp_handler("cppnew", args_builder=_new_args, pre_hook=_skip_new_if_missing),
            _configure_new,
        ),
        CommandSpec(
            "batch",
            "run cppbatch",
            _cpp_handler("cppbatch", args_builder=_batch_args),
            _configure_batch,
        ),
        CommandSpec(
            "delete",
            "run cppdelete for one or more problems",
            _cpp_handler("cppdelete", args_builder=_delete_args),
            _configure_delete,
        ),
        CommandSpec(
            "conf",
            "run cppconf",
            _cpp_handler("cppconf", args_builder=_conf_args),
            _configure_conf,
        ),
        CommandSpec(
            "preset-conf",
            "configure with centralized CMake presets (cmake --preset ...)",
            _handle_preset_conf,
            _configure_preset_conf,
        ),
        CommandSpec(
            "preset-build",
            "build with centralized CMake build presets (cmake --build --preset ...)",
            _handle_preset_build,
            _configure_preset_build,
        ),
        CommandSpec(
            "build",
            "run cppbuild",
            _cpp_handler("cppbuild", args_builder=_build_args),
            _configure_target_optional,
        ),
        CommandSpec(
            "run",
            "run cpprun",
            _cpp_handler("cpprun", args_builder=_run_args),
            _configure_target_optional,
        ),
        CommandSpec(
            "go",
            "run cppgo",
            _cpp_handler("cppgo", args_builder=_go_args),
            _configure_go,
        ),
        CommandSpec(
            "forcego",
            "run cppforcego",
            _cpp_handler("cppgo", args_builder=_forcego_args),
            _configure_target_optional,
        ),
        CommandSpec(
            "judge",
            "run cppjudge",
            _cpp_handler("cppjudge", args_builder=_target_args),
            _configure_target_optional,
        ),
        CommandSpec(
            "test",
            "alias of judge for compatibility",
            _cpp_handler("cppjudge", args_builder=_target_args),
            _configure_target_optional,
        ),
        CommandSpec(
            "stress",
            "run cppstress",
            _cpp_handler("cppstress", args_builder=_stress_args),
            _configure_stress,
        ),
        CommandSpec(
            "submit",
            "run cppsubmit",
            _cpp_handler("cppsubmit", args_builder=_submit_args),
            _configure_submit,
        ),
        CommandSpec(
            "test-submit",
            "run cpptestsubmit",
            _cpp_handler("cpptestsubmit", args_builder=_test_submit_args),
            _configure_test_submit,
        ),
        CommandSpec(
            "full",
            "run cppfull",
            _cpp_handler("cppfull", args_builder=_full_args),
            _configure_full,
        ),
        CommandSpec("check", "run cppcheck", _cpp_handler("cppcheck")),
        CommandSpec("info", "run cppinfo", _cpp_handler("cppinfo")),
        CommandSpec("diag", "run cppdiag", _cpp_handler("cppdiag")),
        CommandSpec("help", "run cpphelp", _cpp_handler("cpphelp")),
        CommandSpec("clean", "run cppclean", _cpp_handler("cppclean")),
        CommandSpec(
            "deepclean",
            "run cppdeepclean (requires --yes)",
            _cpp_handler(
                "cppdeepclean",
                pre_hook=_require_deepclean_yes,
                auto_confirm_deepclean=True,
            ),
            _configure_deepclean,
        ),
        CommandSpec("stats", "run cppstats", _cpp_handler("cppstats")),
        CommandSpec("archive", "run cpparchive", _cpp_handler("cpparchive")),
        CommandSpec(
            "watch",
            "run cppwatch",
            _cpp_handler("cppwatch", args_builder=_target_args),
            _configure_target_optional,
        ),
        CommandSpec(
            "doctor",
            "run manager + cpp-tools diagnostics",
            _handle_doctor,
            _configure_doctor,
        ),
        CommandSpec(
            "cycle",
            "orchestrate a standard CP loop (new/go/judge/submit/test-submit)",
            _handle_cycle,
            _configure_cycle,
        ),
    )


def register_subcommands(subparsers) -> None:
    """Attach all command specs to argparse subparsers."""

    for spec in get_command_specs():
        parser = subparsers.add_parser(spec.name, help=spec.help)
        if spec.configure is not None:
            spec.configure(parser)
        parser.set_defaults(handler=spec.handler)
