"""Function-style flattener tests that drive the CLI through ``flatten_inproc``.

Each test takes the shared ``write_source`` + ``flatten_inproc`` fixtures
from ``conftest.py``, writes a small ``.cpp`` probe, and asserts on the
flattened output without spawning a separate Python interpreter — real
tracebacks survive failures and the per-test runtime drops sharply.

Acts as the canonical pattern for new flattener tests: prefer this style
over the subprocess scaffolding still present in ``test_flattener.py``.
"""

from __future__ import annotations

import textwrap
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from collections.abc import Callable
    from pathlib import Path

    from .conftest import FlattenResult


def test_flattener_accepts_need_macro_with_value(
    clean_cp_env: None,
    write_source: Callable[[str, str], Path],
    flatten_inproc: Callable[..., FlattenResult],
) -> None:
    """``#define NEED_IO 1`` (value form) must enable the IO section."""

    source = write_source(
        "probe.cpp",
        textwrap.dedent(
            """\
            #define NEED_IO 1
            #include "templates/Base.hpp"
            auto main() -> int {
              INT(x);
              OUT(x);
              return 0;
            }
            """
        ),
    )

    result = flatten_inproc(source, env={"CP_FLATTENER_MODE": "safe"})

    assert result.returncode == 0, result.stderr
    assert "#define INT(" in result.stdout
    assert "#define OUT(" in result.stdout


def test_flattener_expands_base_include_with_trailing_comment(
    clean_cp_env: None,
    write_source: Callable[[str, str], Path],
    flatten_inproc: Callable[..., FlattenResult],
) -> None:
    """A trailing line comment after ``Base.hpp`` must not prevent inlining."""

    source = write_source(
        "probe.cpp",
        textwrap.dedent(
            """\
            #define NEED_CORE
            #include "templates/Base.hpp" // keep this comment style
            auto main() -> int { return 0; }
            """
        ),
    )

    result = flatten_inproc(source, env={"CP_FLATTENER_MODE": "safe"})

    assert result.returncode == 0, result.stderr
    assert '#include "templates/Base.hpp"' not in result.stdout
    assert "<bits/stdc++.h>" in result.stdout


def test_flattener_keeps_reverse_loop_macros_from_need_core(
    clean_cp_env: None,
    write_source: Callable[[str, str], Path],
    flatten_inproc: Callable[..., FlattenResult],
) -> None:
    """``NEED_CORE`` must keep the reverse-loop helper macros in the output."""

    source = write_source(
        "probe.cpp",
        textwrap.dedent(
            """\
            #define NEED_CORE
            #include "templates/Base.hpp"
            auto main() -> int { return 0; }
            """
        ),
    )

    result = flatten_inproc(source, env={"CP_FLATTENER_MODE": "safe"})

    assert result.returncode == 0, result.stderr
    assert "#define ROF(" in result.stdout
    assert "#define FORD(" in result.stdout
