"""Submission-mode post-pass: strip comments, collapse blanks, re-indent.

Used only by ``FlattenerMode.SUBMISSION`` to turn the flattened source into the
artifact pasted to an online judge: comments are stripped and blank runs
collapsed to fit size limits, then the shared clang-format pass re-indents the
result (see :mod:`flattener_pipeline.formatting`). Compact/auto modes share the
same re-indent step but keep comments.
"""

from __future__ import annotations

from flattener_core.lexer import (
    collapse_redundant_blank_lines,
    strip_comments,
    trim_outer_blank_lines,
)
from flattener_pipeline.formatting import reindent_with_clang_format


def prepare_submission_output(flattened_source: str) -> str:
    """Minimize and re-indent flattened source for online judge submission."""

    stripped = strip_comments(flattened_source)
    compacted = collapse_redundant_blank_lines(stripped)
    trimmed = trim_outer_blank_lines(compacted) + "\n"
    return reindent_with_clang_format(trimmed)
