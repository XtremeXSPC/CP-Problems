"""Environment-driven mode resolution and syntax-only compile validation.

Centralizes the env-flag plumbing the flattener uses
(``CP_FLATTENER_MODE``, ``CP_FLATTENER_AUTO_VALIDATE``,
``CP_FLATTENER_VALIDATION_*``) and offers ``validate_flattened_output``,
which pipes the flattened source through ``g++ -fsyntax-only`` (or another
candidate compiler) via the shared ``_lib.process.run_capture`` layer.

Validation returns ``PASSED``/``FAILED``/``UNAVAILABLE`` so the AUTO mode
in ``cli`` can decide whether to retry with the safe variant.
"""

from __future__ import annotations

import os
import shutil
import sys

from _lib.process import ProcessRequest, run_capture
from flattener_pipeline.context import FlattenerMode, ValidationStatus

VALIDATION_COMPILER_CANDIDATES: tuple[str, ...] = (
    "g++-15",
    "g++-14",
    "g++-13",
    "g++",
    "clang++",
)
_TRUTHY_VALUES: frozenset[str] = frozenset({"1", "true", "yes", "on"})


def env_flag_enabled(name: str, default: bool = False) -> bool:
    """Interpret common truthy environment variable spellings."""

    raw = os.environ.get(name)
    if raw is None:
        return default
    return raw.strip().lower() in _TRUTHY_VALUES


def resolve_flattener_mode() -> FlattenerMode:
    """Determine the active flattening mode from the environment.

    Defaults to ``COMPACT``. Emits a warning and still falls back to
    ``COMPACT`` when ``CP_FLATTENER_MODE`` contains an unknown value.
    """

    raw_mode = os.environ.get("CP_FLATTENER_MODE", "compact").strip().lower()
    try:
        return FlattenerMode(raw_mode)
    except ValueError:
        sys.stderr.write(f"Warning: unknown CP_FLATTENER_MODE='{raw_mode}'. Using 'compact'.\n")
        return FlattenerMode.COMPACT


def parse_positive_int_env(name: str, default_value: int) -> int:
    """Read a positive integer from an environment variable.

    Returns ``default_value`` when the variable is missing, empty,
    non-numeric, or not strictly positive.
    """

    raw = os.environ.get(name, "").strip()
    if not raw:
        return default_value
    try:
        parsed = int(raw)
    except ValueError:
        return default_value
    return parsed if parsed > 0 else default_value


def resolve_validation_compiler() -> str | None:
    """Return an absolute path to a C++ compiler for syntax validation.

    Prefers ``CP_FLATTENER_VALIDATION_COMPILER`` when set, otherwise searches
    a hard-coded candidate list (``g++-15``, ``g++-14``, ``g++-13``, ``g++``, ``clang++``).
    """

    forced = os.environ.get("CP_FLATTENER_VALIDATION_COMPILER", "").strip()
    if forced:
        return forced

    for candidate in VALIDATION_COMPILER_CANDIDATES:
        resolved = shutil.which(candidate)
        if resolved:
            return resolved
    return None


def validate_flattened_output(
    flattened_source: str,
    *,
    force: bool | None = None,
    validation_compiler: str | None = None,
    validation_std: str = "gnu++20",
) -> ValidationStatus:
    """Compile-check the flattened output.

    Returns:
      - ``PASSED``      — syntax validation succeeded
      - ``FAILED``      — validation ran and failed
      - ``UNAVAILABLE`` — validation disabled or compiler not found
    """

    if force is False:
        return ValidationStatus.UNAVAILABLE
    if force is None and not env_flag_enabled("CP_FLATTENER_AUTO_VALIDATE", default=True):
        return ValidationStatus.UNAVAILABLE

    compiler = validation_compiler or resolve_validation_compiler()
    if not compiler:
        return ValidationStatus.UNAVAILABLE

    timeout_ms = parse_positive_int_env("CP_FLATTENER_VALIDATION_TIMEOUT_MS", 3500)
    try:
        result = run_capture(
            ProcessRequest(
                argv=[compiler, f"-std={validation_std}", "-fsyntax-only", "-x", "c++", "-"],
                stdin=flattened_source,
                timeout=timeout_ms / 1000.0,
            )
        )
    except OSError:
        return ValidationStatus.UNAVAILABLE

    if result.timed_out:
        return ValidationStatus.UNAVAILABLE
    if result.returncode == 0:
        return ValidationStatus.PASSED
    return ValidationStatus.FAILED
