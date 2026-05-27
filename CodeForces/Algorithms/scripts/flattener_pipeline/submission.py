"""Submission-mode post-pass: strip comments and collapse blank lines.

Used only by ``FlattenerMode.SUBMISSION`` to trim the flattened source
before printing it to stdout, so the output fits within online-judge size
limits without losing meaningful semantics.
"""

from __future__ import annotations

from flattener_core.lexer import (
    collapse_redundant_blank_lines,
    strip_comments,
    trim_outer_blank_lines,
)


def prepare_submission_output(flattened_source: str) -> str:
    """Minimize flattened source for online judge submission."""

    stripped = strip_comments(flattened_source)
    compacted = collapse_redundant_blank_lines(stripped)
    return trim_outer_blank_lines(compacted) + "\n"
