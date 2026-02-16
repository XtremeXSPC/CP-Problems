"""Error and result types for workflow manager execution."""

from dataclasses import asdict, dataclass
import re
from typing import List

ANSI_RE = re.compile(r"\x1B\[[0-9;]*[A-Za-z]")


class WorkflowError(RuntimeError):
    """Logical error in workflow manager configuration or usage."""


class WorkflowCommandError(WorkflowError):
    """Raised when a delegated command fails."""

    def __init__(self, result: "CommandResult"):
        """Capture the failing command result for downstream handling."""
        self.result = result
        super().__init__(f"{result.function} failed with exit code {result.returncode}")


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
        """Serialize command result for JSON output."""
        payload = asdict(self)
        if strip_ansi:
            payload["stdout"] = remove_ansi(payload["stdout"])
            payload["stderr"] = remove_ansi(payload["stderr"])
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
