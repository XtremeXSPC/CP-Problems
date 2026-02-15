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
        payload = asdict(self)
        if strip_ansi:
            payload["stdout"] = remove_ansi(payload["stdout"])
            payload["stderr"] = remove_ansi(payload["stderr"])
        return payload


def remove_ansi(text: str) -> str:
    return ANSI_RE.sub("", text)
