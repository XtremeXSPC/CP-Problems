"""Submission-mode post-pass: strip comments, collapse blanks, re-indent.

Used only by ``FlattenerMode.SUBMISSION`` to turn the flattened source into the
artifact pasted to an online judge: comments are stripped and blank runs
collapsed to fit size limits, then ``clang-format`` re-indents the result so the
preprocessor directives left dangling by the conditional folder (which removes
``#if``/``#endif`` lines) line up again. The format step is best-effort: if
``clang-format`` is missing or errors, the still-valid unformatted source is
emitted so flattening never breaks on a bare machine.
"""

from __future__ import annotations

import shutil
import sys

from _lib.process import ProcessRequest, run_capture
from flattener_core.lexer import (
    collapse_redundant_blank_lines,
    strip_comments,
    trim_outer_blank_lines,
)

# Self-contained, gentle style for the generated submission. The repo's own
# ``.clang-format`` is ``DisableFormat: true`` (sources are hand-laid-out), so
# the submission cannot use ``-style=file`` and carries its own policy instead:
# re-indent preprocessor directives by nesting depth (the actual fix) without
# reflowing the crafted ``constexpr`` tables / long lines (``ColumnLimit: 0``)
# or reordering the ordered template includes (``SortIncludes: false``).
_SUBMISSION_CLANG_FORMAT_STYLE = (
    "{BasedOnStyle: LLVM, IndentWidth: 2, ColumnLimit: 0, "
    "IndentPPDirectives: BeforeHash, SortIncludes: false, MaxEmptyLinesToKeep: 1}"
)


def _reindent_with_clang_format(source: str) -> str:
    """Re-indent ``source`` via clang-format; return it unchanged on any failure."""

    executable = shutil.which("clang-format")
    if executable is None:
        return source
    result = run_capture(
        ProcessRequest(
            argv=[
                executable,
                f"-style={_SUBMISSION_CLANG_FORMAT_STYLE}",
                "--assume-filename=submission.cpp",
            ],
            stdin=source,
            timeout=30.0,
        )
    )
    if result.failed or not result.stdout:
        sys.stderr.write(
            "warning: clang-format failed, emitting unformatted submission "
            f"(exit {result.returncode})\n"
        )
        return source
    return result.stdout


def prepare_submission_output(flattened_source: str) -> str:
    """Minimize and re-indent flattened source for online judge submission."""

    stripped = strip_comments(flattened_source)
    compacted = collapse_redundant_blank_lines(stripped)
    trimmed = trim_outer_blank_lines(compacted) + "\n"
    return _reindent_with_clang_format(trimmed)
