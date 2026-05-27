"""Unified subprocess execution layer.

Five places previously reinvented the *run-with-timeout* pattern (the
workflow runner, the orchestration external path, the PCH benchmark, the
module verifier compile probes, and the module tester compiler invocation).
This module owns the single canonical implementation.

Every consumer goes through :func:`run_capture`, which takes a fully
parameterized :class:`ProcessRequest` and returns a :class:`ProcessResult`
that normalizes:

- timeouts → ``returncode == 124`` and ``timed_out is True``
- text decoding → ``stdout``/``stderr`` are always ``str``
- elapsed wall time → reported in milliseconds for parity with the existing
  ``CommandResult`` payload consumed by ``workflow_manager``.
"""

from __future__ import annotations

import os
import subprocess
import time
from collections.abc import Mapping, Sequence
from dataclasses import dataclass, field
from pathlib import Path
from typing import Protocol, runtime_checkable

__all__ = [
    "DEFAULT_PROCESS_TIMEOUT_SECONDS",
    "ProcessRequest",
    "ProcessResult",
    "Runnable",
    "run_capture",
]

DEFAULT_PROCESS_TIMEOUT_SECONDS: float = 600.0


@dataclass(frozen=True, slots=True)
class ProcessRequest:
    """Immutable description of a subprocess invocation.

    ``argv`` is mandatory; everything else carries a sensible default. The
    request is intentionally a value object so consumers can build it once
    and pass it around without worrying about accidental mutation.
    """

    argv: Sequence[str]
    cwd: Path | None = None
    env_overrides: Mapping[str, str] = field(default_factory=dict)
    timeout: float | None = None
    stdin: str | None = None


@dataclass(frozen=True, slots=True)
class ProcessResult:
    """Normalized subprocess outcome shared across callers.

    Mirrors the public surface of :class:`workflow_manager_core.types.CommandResult`
    enough that adapters between the two are trivial (``returncode``,
    ``duration_ms``, ``stdout``, ``stderr``, ``timed_out``).
    """

    argv: tuple[str, ...]
    cwd: str
    returncode: int
    duration_ms: int
    stdout: str
    stderr: str
    timed_out: bool = False

    @property
    def failed(self) -> bool:
        """True when the command exited non-zero, including timeouts (124)."""

        return self.returncode != 0


@runtime_checkable
class Runnable(Protocol):
    """Anything that produces a :class:`ProcessResult` from a request.

    The concrete production implementation is :func:`run_capture`, but tests
    and higher-level wrappers may compose alternative implementations (e.g.
    a recording double, a parallel pool dispatcher). Kept as a Protocol so
    no inheritance is required.
    """

    def __call__(self, request: ProcessRequest) -> ProcessResult: ...


def _build_env(overrides: Mapping[str, str]) -> dict[str, str]:
    """Return a fresh env dict, layering ``overrides`` over ``os.environ``."""

    env = os.environ.copy()
    env.update(overrides)
    return env


def _append_timeout_marker(payload: str | bytes | None) -> str:
    """Replicate the legacy timeout stderr format used by workflow_manager.

    The two existing call sites used the same marker; centralizing avoids
    drift if the format ever changes.
    """

    text = _ensure_text(payload).rstrip("\n")
    if not text:
        return "Command timed out."
    return f"{text}\nCommand timed out."


def _ensure_text(payload: str | bytes | None) -> str:
    """Decode raw subprocess payloads into UTF-8 text with replacement."""

    if payload is None:
        return ""
    if isinstance(payload, bytes):
        return payload.decode("utf-8", errors="replace")
    return payload


def run_capture(request: ProcessRequest) -> ProcessResult:
    """Execute one subprocess and return a normalized :class:`ProcessResult`.

    Behavior contract:

    - ``argv`` must be non-empty (raises ``ValueError`` otherwise).
    - When ``request.timeout`` is ``None`` the helper applies
      :data:`DEFAULT_PROCESS_TIMEOUT_SECONDS`.
    - On timeout the result reports ``returncode = 124`` and
      ``timed_out = True``; ``stderr`` is augmented with
      ``"Command timed out."`` exactly like the legacy code paths.
    - On ``OSError`` (e.g. missing executable) the helper re-raises so the
      caller can distinguish from a normal non-zero exit. Callers that want
      a graceful degradation should catch it explicitly.
    """

    if not request.argv:
        raise ValueError("run_capture requires a non-empty argv")

    cwd = str(request.cwd) if request.cwd is not None else None
    env = _build_env(request.env_overrides)
    timeout = request.timeout if request.timeout is not None else DEFAULT_PROCESS_TIMEOUT_SECONDS

    start = time.perf_counter()
    try:
        completed = subprocess.run(
            list(request.argv),
            cwd=cwd,
            env=env,
            input=request.stdin,
            capture_output=True,
            text=True,
            timeout=timeout,
            check=False,
        )
    except subprocess.TimeoutExpired as exc:
        elapsed_ms = int((time.perf_counter() - start) * 1000)
        return ProcessResult(
            argv=tuple(request.argv),
            cwd=cwd or "",
            returncode=124,
            duration_ms=elapsed_ms,
            stdout=_ensure_text(exc.stdout),
            stderr=_append_timeout_marker(exc.stderr),
            timed_out=True,
        )

    elapsed_ms = int((time.perf_counter() - start) * 1000)
    return ProcessResult(
        argv=tuple(request.argv),
        cwd=cwd or "",
        returncode=completed.returncode,
        duration_ms=elapsed_ms,
        stdout=completed.stdout,
        stderr=completed.stderr,
        timed_out=False,
    )
