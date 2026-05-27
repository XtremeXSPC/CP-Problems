"""Error and result value objects shared across the workflow layers.

``WorkflowError`` / ``WorkflowCommandError`` are the canonical exception
hierarchy that handlers raise on logical or delegated-command failures.

``CommandResult`` is the immutable record emitted for every delegated
invocation, with ``failed``/``non_fatal`` semantics, ANSI-stripping JSON
serialization, and a ``Self``-typed ``as_non_fatal`` builder for the
orchestration layer's demotion rule.
"""

import re
from dataclasses import dataclass, replace
from typing import Self

ANSI_RE = re.compile(r"\x1B\[[0-9;]*[A-Za-z]")


class WorkflowError(RuntimeError):
    """Logical error in workflow manager configuration or usage."""


class WorkflowCommandError(WorkflowError):
    """Raised when a delegated command fails."""

    def __init__(self, result: "CommandResult"):
        """Capture the failing command result for downstream handling."""

        self.result = result
        super().__init__(f"{result.function} failed with exit code {result.returncode}")


@dataclass(frozen=True, slots=True)
class CommandResult:
    function: str
    args: tuple[str, ...]
    cwd: str
    returncode: int
    duration_ms: int
    stdout: str
    stderr: str
    timed_out: bool = False
    non_fatal: bool = False

    @property
    def failed(self) -> bool:
        """Return True when the command should count as a workflow failure."""
        return self.returncode != 0 and not self.non_fatal

    def as_non_fatal(self) -> Self:
        """Return a copy of this result marked as non-fatal for workflow status."""
        return replace(self, non_fatal=True)

    def to_dict(self, strip_ansi: bool = False) -> dict[str, object]:
        """Serialize command result for JSON output."""
        payload: dict[str, object] = {
            "function": self.function,
            "args": list(self.args),
            "cwd": self.cwd,
            "returncode": self.returncode,
            "duration_ms": self.duration_ms,
            "stdout": self.stdout,
            "stderr": self.stderr,
            "timed_out": self.timed_out,
            "non_fatal": self.non_fatal,
        }
        if strip_ansi:
            payload["stdout"] = remove_ansi(self.stdout)
            payload["stderr"] = remove_ansi(self.stderr)
        return payload


def remove_ansi(text: str) -> str:
    """Strip ANSI escape sequences from a text payload."""

    return ANSI_RE.sub("", text)


def ensure_text(payload: str | bytes | None) -> str:
    """Normalize subprocess output payloads to text."""

    if payload is None:
        return ""
    if isinstance(payload, bytes):
        return payload.decode("utf-8", errors="replace")
    return payload


def format_timeout_stderr(payload: str | bytes | None) -> str:
    """Append a timeout marker preserving any captured stderr content."""

    text = ensure_text(payload).rstrip("\n")
    if not text:
        return "Command timed out."
    return f"{text}\nCommand timed out."
