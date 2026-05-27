"""Layered backend for the ``workflow_manager`` CLI front-end.

Split by concern so each piece can be unit-tested in isolation: ``types``
(error/result dataclasses), ``constants`` (CLI enums + project paths),
``runner`` (cpp-tools delegation through ``_lib.process``), ``commands``
(command registry + handlers), ``orchestration`` (multi-step workflow
execution + JSON serialization), ``workflows`` (high-level recipes like
``cycle`` and ``doctor``), ``parser`` (argparse wiring), and ``utils``
(validation + discovery helpers).

The top-level ``workflow_manager.py`` script is the thin entry point that
glues these together.
"""

__all__ = [
    "commands",
    "constants",
    "orchestration",
    "parser",
    "runner",
    "types",
    "utils",
    "workflows",
]
