"""Shared clang-format re-indentation pass for flattened output.

The conditional folder removes ``#if``/``#endif`` lines and leaves their bodies
carrying the indentation of the now-absent nesting. Every mode that emits a
final artifact (compact, auto, submission) runs the output through clang-format
to restore correct directive indentation and collapse the redundant blank runs
the fold can leave behind.

Best-effort by design: if clang-format is missing or errors, the (still valid)
unformatted source is returned so flattening never breaks on a bare machine.
"""

from __future__ import annotations

import shutil
import sys

from _lib.process import ProcessRequest, run_capture

# Self-contained, gentle style for the generated artifact. The repo's own
# ``.clang-format`` is ``DisableFormat: true`` (sources are hand-laid-out), so
# the flattened output cannot use ``-style=file`` and carries its own policy
# instead: re-indent preprocessor directives by nesting depth (the actual fix)
# without reflowing the crafted ``constexpr`` tables / long lines
# (``ColumnLimit: 0``) or reordering the ordered template includes
# (``SortIncludes: false``); ``MaxEmptyLinesToKeep: 1`` collapses fold leftovers.
_CLANG_FORMAT_STYLE = (
    "{BasedOnStyle: LLVM, IndentWidth: 2, ColumnLimit: 0, "
    "IndentPPDirectives: BeforeHash, SortIncludes: false, MaxEmptyLinesToKeep: 1, "
    "AlignConsecutiveAssignments: true, PointerAlignment: Left }"
)


def reindent_with_clang_format(source: str) -> str:
    """Re-indent ``source`` via clang-format; return it unchanged on any failure."""

    executable = shutil.which("clang-format")
    if executable is None:
        return source
    result = run_capture(
        ProcessRequest(
            argv=[
                executable,
                f"-style={_CLANG_FORMAT_STYLE}",
                "--assume-filename=submission.cpp",
            ],
            stdin=source,
            timeout=30.0,
        )
    )
    if result.failed or not result.stdout:
        sys.stderr.write(
            "warning: clang-format failed, emitting unformatted output "
            f"(exit {result.returncode})\n"
        )
        return source
    return result.stdout
