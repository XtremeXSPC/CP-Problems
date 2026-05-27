"""Pytest fixtures shared across the ``scripts/tests`` suite.

Centralizes the boilerplate that every test file used to repeat:

- ``sys.path`` bootstrap so plain ``import flattener_core`` works without
  per-file ``sys.path.insert`` calls;
- a clean ``CP_*`` environment so tests don't leak each other's state;
- a paired ``(tmp_workspace, write_source)`` pair for quickly staging a
  source file under a fresh temp directory;
- an in-process flattener invoker (``flatten_inproc``) that replaces the
  ``subprocess.run([python, flattener.py, ...])`` pattern with a direct
  call to ``flattener_pipeline.cli.main`` — roughly ~50x faster per test
  and yields real Python tracebacks instead of opaque subprocess stderr.

unittest.TestCase subclasses continue to work unchanged; pytest discovers
them through the ``python_classes`` glob in ``pyproject.toml``.
"""

from __future__ import annotations

import contextlib
import io
import os
import sys
from collections.abc import Callable, Iterator, Sequence
from dataclasses import dataclass
from pathlib import Path

import pytest

SCRIPTS_DIR = Path(__file__).resolve().parents[1]
ALGORITHMS_DIR = SCRIPTS_DIR.parent

if str(SCRIPTS_DIR) not in sys.path:
    sys.path.insert(0, str(SCRIPTS_DIR))

from flattener_pipeline import cli as flattener_cli  # noqa: E402


@pytest.fixture(scope="session")
def scripts_dir() -> Path:
    """Absolute path to ``CodeForces/Algorithms/scripts/``."""

    return SCRIPTS_DIR


@pytest.fixture(scope="session")
def algorithms_dir() -> Path:
    """Absolute path to the ``CodeForces/Algorithms/`` project root."""

    return ALGORITHMS_DIR


@pytest.fixture
def clean_cp_env(monkeypatch: pytest.MonkeyPatch) -> None:
    """Strip every ``CP_*`` env var so tests see a deterministic baseline.

    Tests that want to set a specific ``CP_FLATTENER_MODE`` (etc.) should
    then ``monkeypatch.setenv(...)`` explicitly after pulling this fixture.
    """

    for key in list(os.environ):
        if key.startswith("CP_"):
            monkeypatch.delenv(key, raising=False)


@dataclass(frozen=True, slots=True)
class FlattenResult:
    """In-process equivalent of the ``subprocess.CompletedProcess`` shape."""

    returncode: int
    stdout: str
    stderr: str


@pytest.fixture
def write_source(tmp_path: Path) -> Callable[[str, str], Path]:
    """Helper that writes ``content`` to ``tmp_path / name`` and returns the path."""

    def _write(name: str, content: str) -> Path:
        target = tmp_path / name
        target.write_text(content, encoding="utf-8")
        return target

    return _write


@pytest.fixture
def flatten_inproc(
    monkeypatch: pytest.MonkeyPatch,
) -> Iterator[Callable[..., FlattenResult]]:
    """Invoke ``flattener_pipeline.cli.main`` in-process and capture I/O.

    Returns a callable ``run(source_path, *extra_argv, env=None) -> FlattenResult``
    that:

    - rebinds ``sys.argv`` to ``[flattener.py, source_path, *extra_argv]``;
    - applies any ``env`` mapping via ``monkeypatch.setenv``;
    - captures stdout/stderr;
    - translates ``SystemExit`` into the result ``returncode``.
    """

    def _run(
        source: Path | str,
        *extra_argv: str,
        env: dict[str, str] | None = None,
    ) -> FlattenResult:
        argv: list[str] = ["flattener.py", str(source), *extra_argv]
        monkeypatch.setattr(sys, "argv", argv)
        if env:
            for key, value in env.items():
                monkeypatch.setenv(key, value)

        stdout_buf = io.StringIO()
        stderr_buf = io.StringIO()
        returncode = 0
        with (
            contextlib.redirect_stdout(stdout_buf),
            contextlib.redirect_stderr(stderr_buf),
        ):
            try:
                flattener_cli.main()
            except SystemExit as exc:
                returncode = int(exc.code) if isinstance(exc.code, int) else 0
        return FlattenResult(
            returncode=returncode,
            stdout=stdout_buf.getvalue(),
            stderr=stderr_buf.getvalue(),
        )

    yield _run


@pytest.fixture
def make_stub_cpp_tools(tmp_path: Path) -> Callable[..., Path]:
    """Write a tiny ``competitive.sh`` stub exposing the requested zsh functions.

    Example::

        stub = make_stub_cpp_tools(cpphelp='print -- "ok"')
        runner = CppToolsRunner(stub, tmp_path, default_timeout=5)
    """

    def _build(**functions: str) -> Path:
        if not functions:
            functions = {"cpphelp": 'print -- "stub-help-valid"'}
        body = "\n\n".join(f"{name}() {{\n  {body}\n}}" for name, body in functions.items())
        stub = tmp_path / "competitive_stub.sh"
        stub.write_text(body + "\n", encoding="utf-8")
        return stub

    return _build


@pytest.fixture
def cli_argv(monkeypatch: pytest.MonkeyPatch) -> Callable[[Sequence[str]], None]:
    """Set ``sys.argv`` to ``[script_name, *args]`` for in-process CLI tests."""

    def _set(args: Sequence[str], *, script_name: str = "test_cli") -> None:
        monkeypatch.setattr(sys, "argv", [script_name, *args])

    return _set
