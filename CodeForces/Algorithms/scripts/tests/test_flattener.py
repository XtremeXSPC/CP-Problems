"""Focused audit tests for flattener correctness, safety, and robustness."""

from __future__ import annotations

import os
import subprocess
import sys
import tempfile
import textwrap
import unittest
from pathlib import Path

SCRIPTS_DIR = Path(__file__).resolve().parents[1]
FLATTENER_SCRIPT = SCRIPTS_DIR / "flattener.py"

if str(SCRIPTS_DIR) not in sys.path:
    sys.path.insert(0, str(SCRIPTS_DIR))

from flattener import extract_macro_values_from_source, extract_prefix_before_base_include  # noqa: E402
from flattener_core.flattener_helpers import (  # noqa: E402
    fold_simple_preprocessor_conditionals,
    resolve_project_include,
)
from need_resolver import extract_need_macros_from_source, load_need_mapping  # noqa: E402


class FlattenerAuditTests(unittest.TestCase):
    def test_extract_need_macros_handles_values_undef_and_comments(self) -> None:
        source = textwrap.dedent(
            """\
            /*
            #define NEED_IO
            */
            #if 0
            #define NEED_IO
            #endif
            #define NEED_IO 1
            #define NEED_FAST_IO 1 // inline comment
            #undef NEED_IO
            #include "templates/Base.hpp"
            #define NEED_IO
            """
        )
        known = {"NEED_CORE", "NEED_IO", "NEED_FAST_IO"}

        found = extract_need_macros_from_source(source, known)

        self.assertEqual(found, {"NEED_CORE", "NEED_FAST_IO"})

    def test_load_need_mapping_ignores_else_branch_of_need_guard(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            base = Path(tmpdir) / "Base.hpp"
            base.write_text(
                textwrap.dedent(
                    """\
                    #ifdef NEED_X
                    #include "A.hpp"
                    #else
                    #include "B.hpp"
                    #endif
                    """
                ),
                encoding="utf-8",
            )

            mapping = load_need_mapping(base)
            self.assertEqual(mapping.get("NEED_X"), ["A.hpp"])

    def test_extract_need_macros_ignores_directives_inside_raw_string(self) -> None:
        source = textwrap.dedent(
            """\
            const char* payload = R"RAW(
            #define NEED_IO
            #include "templates/Base.hpp"
            )RAW";
            #define NEED_FAST_IO 1
            #include "templates/Base.hpp"
            """
        )
        known = {"NEED_CORE", "NEED_IO", "NEED_FAST_IO"}

        found = extract_need_macros_from_source(source, known)

        self.assertEqual(found, {"NEED_CORE", "NEED_FAST_IO"})

    def test_extract_macro_values_stops_at_base_include(self) -> None:
        source = textwrap.dedent(
            """\
            /* #define CP_ENABLE_LEGACY_IO_VEC_MACROS 1 */
            #define CP_ENABLE_LEGACY_IO_VEC_MACROS 0
            #include "templates/Base.hpp"
            #define CP_ENABLE_LEGACY_IO_VEC_MACROS 1
            """
        )
        prefix = extract_prefix_before_base_include(source)
        values = extract_macro_values_from_source(
            prefix, strict_profile_enabled=False, relaxed_profile_enabled=False
        )

        self.assertEqual(values.get("CP_ENABLE_LEGACY_IO_VEC_MACROS"), 0)

    def test_fold_simple_conditionals_supports_numeric_literals(self) -> None:
        content = textwrap.dedent(
            """\
            #if 0
            DROP_A
            #endif
            #if 1
            KEEP_B
            #endif
            #if !0
            KEEP_C
            #endif
            #if !1
            DROP_D
            #endif
            """
        )

        folded = fold_simple_preprocessor_conditionals(content, {})

        self.assertIn("KEEP_B", folded)
        self.assertIn("KEEP_C", folded)
        self.assertNotIn("DROP_A", folded)
        self.assertNotIn("DROP_D", folded)
        self.assertNotIn("#if", folded)
        self.assertNotIn("#endif", folded)

    def test_fold_unknown_conditional_does_not_leak_macro_state(self) -> None:
        content = textwrap.dedent(
            """\
            #if FOO + BAR
            #define X 1
            #endif
            #if X
            int yes;
            #endif
            """
        )

        folded = fold_simple_preprocessor_conditionals(content, {})

        self.assertIn("#if FOO + BAR", folded)
        self.assertIn("#if X", folded)
        self.assertIn("#define X 1", folded)
        self.assertIn("int yes;", folded)
        self.assertNotIn("\n#endif\nint yes;\n", folded)

    def test_resolve_project_include_rejects_parent_escape(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            tmp = Path(tmpdir)
            project_root = tmp / "project"
            project_root.mkdir()
            inside = project_root / "inside.hpp"
            inside.write_text("// in\n", encoding="utf-8")
            including_file = project_root / "sub" / "main.hpp"
            including_file.parent.mkdir()
            including_file.write_text('#include "../inside.hpp"\n', encoding="utf-8")

            outside = tmp / "outside.hpp"
            outside.write_text("// out\n", encoding="utf-8")

            resolved_inside = resolve_project_include(
                project_root, including_file, "../inside.hpp"
            )
            resolved_escape = resolve_project_include(
                project_root, including_file, "../../outside.hpp"
            )

            self.assertEqual(resolved_inside, inside.resolve())
            self.assertIsNone(resolved_escape)

    def test_flattener_accepts_need_macro_with_value(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            src = Path(tmpdir) / "probe.cpp"
            src.write_text(
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
                encoding="utf-8",
            )

            result = subprocess.run(
                [sys.executable, str(FLATTENER_SCRIPT), str(src)],
                capture_output=True,
                text=True,
                check=False,
                env={**os.environ, "CP_FLATTENER_MODE": "safe"},
            )

            self.assertEqual(result.returncode, 0, msg=result.stderr)
            self.assertIn("#define INT(", result.stdout)
            self.assertIn("#define OUT(", result.stdout)

    def test_flattener_expands_base_include_with_trailing_comment(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            src = Path(tmpdir) / "probe.cpp"
            src.write_text(
                textwrap.dedent(
                    """\
                    #define NEED_CORE
                    #include "templates/Base.hpp" // keep this comment style
                    auto main() -> int { return 0; }
                    """
                ),
                encoding="utf-8",
            )

            result = subprocess.run(
                [sys.executable, str(FLATTENER_SCRIPT), str(src)],
                capture_output=True,
                text=True,
                check=False,
                env={**os.environ, "CP_FLATTENER_MODE": "safe"},
            )

            self.assertEqual(result.returncode, 0, msg=result.stderr)
            self.assertNotIn('#include "templates/Base.hpp"', result.stdout)
            self.assertIn("<bits/stdc++.h>", result.stdout)

    def test_flattener_output_avoids_triple_newlines(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            src = Path(tmpdir) / "probe.cpp"
            src.write_text(
                textwrap.dedent(
                    """\
                    #define CP_TEMPLATE_PROFILE_RELAXED
                    #define NEED_CORE
                    #define NEED_IO
                    #include "templates/Base.hpp"
                    auto main() -> int { return 0; }
                    """
                ),
                encoding="utf-8",
            )

            result = subprocess.run(
                [sys.executable, str(FLATTENER_SCRIPT), str(src)],
                capture_output=True,
                text=True,
                check=False,
                env={**os.environ, "CP_FLATTENER_MODE": "safe"},
            )

            self.assertEqual(result.returncode, 0, msg=result.stderr)
            self.assertNotIn("\n\n\n", result.stdout)

    def test_flattener_mode_safe_keeps_unreferenced_need_headers(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            src = Path(tmpdir) / "probe.cpp"
            src.write_text(
                textwrap.dedent(
                    """\
                    #define NEED_IO
                    #include "templates/Base.hpp"
                    auto main() -> int { return 0; }
                    """
                ),
                encoding="utf-8",
            )

            compact = subprocess.run(
                [sys.executable, str(FLATTENER_SCRIPT), str(src)],
                capture_output=True,
                text=True,
                check=False,
                env={**os.environ, "CP_FLATTENER_MODE": "compact"},
            )
            safe = subprocess.run(
                [sys.executable, str(FLATTENER_SCRIPT), str(src)],
                capture_output=True,
                text=True,
                check=False,
                env={**os.environ, "CP_FLATTENER_MODE": "safe"},
            )

            self.assertEqual(compact.returncode, 0, msg=compact.stderr)
            self.assertEqual(safe.returncode, 0, msg=safe.stderr)
            self.assertNotIn("#define OUT(", compact.stdout)
            self.assertIn("#define OUT(", safe.stdout)

    def test_flattener_mode_auto_fallbacks_to_safe_on_compact_failure(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            src = Path(tmpdir) / "probe.cpp"
            src.write_text(
                textwrap.dedent(
                    """\
                    #define NEED_IO
                    #include "templates/Base.hpp"
                    auto main() -> int { return 0; }
                    """
                ),
                encoding="utf-8",
            )

            fake_compiler = Path(tmpdir) / "fake_compiler.sh"
            fake_compiler.write_text(
                textwrap.dedent(
                    """\
                    #!/bin/sh
                    grep -q "#define OUT(" && exit 0
                    exit 1
                    """
                ),
                encoding="utf-8",
            )
            fake_compiler.chmod(0o755)

            result = subprocess.run(
                [sys.executable, str(FLATTENER_SCRIPT), str(src)],
                capture_output=True,
                text=True,
                check=False,
                env={
                    **os.environ,
                    "CP_FLATTENER_MODE": "auto",
                    "CP_FLATTENER_VALIDATION_COMPILER": str(fake_compiler),
                },
            )

            self.assertEqual(result.returncode, 0, msg=result.stderr)
            self.assertIn("#define OUT(", result.stdout)

    def test_nested_template_section_header_has_blank_line_separator(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            src = Path(tmpdir) / "probe.cpp"
            src.write_text(
                textwrap.dedent(
                    """\
                    #define NEED_CORE
                    #define NEED_MATH
                    #include "templates/Base.hpp"
                    auto main() -> int { return 0; }
                    """
                ),
                encoding="utf-8",
            )

            result = subprocess.run(
                [sys.executable, str(FLATTENER_SCRIPT), str(src)],
                capture_output=True,
                text=True,
                check=False,
                env={**os.environ, "CP_FLATTENER_MODE": "safe"},
            )

            self.assertEqual(result.returncode, 0, msg=result.stderr)
            self.assertIn(
                "} // namespace cp\n\n//===----------------------------------------------------------------------===//\n/* Lightweight Stopwatch Utility */",
                result.stdout,
            )

    def test_fast_io_internal_define_not_emitted_when_need_fast_io(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            src = Path(tmpdir) / "probe.cpp"
            src.write_text(
                textwrap.dedent(
                    """\
                    #define NEED_CORE
                    #define NEED_FAST_IO
                    #include "templates/Base.hpp"
                    auto main() -> int { return 0; }
                    """
                ),
                encoding="utf-8",
            )

            result = subprocess.run(
                [sys.executable, str(FLATTENER_SCRIPT), str(src)],
                capture_output=True,
                text=True,
                check=False,
                env={**os.environ, "CP_FLATTENER_MODE": "safe"},
            )

            self.assertEqual(result.returncode, 0, msg=result.stderr)
            self.assertNotIn("#define CP_FAST_IO_NAMESPACE_DEFINED", result.stdout)
            self.assertIn("namespace fast_io {", result.stdout)

    def test_fast_io_macro_state_applies_to_io_header_when_included(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            src = Path(tmpdir) / "probe.cpp"
            src.write_text(
                textwrap.dedent(
                    """\
                    #define NEED_CORE
                    #define NEED_FAST_IO
                    #include "templates/Base.hpp"
                    #include "templates/IO.hpp"
                    auto main() -> int { return 0; }
                    """
                ),
                encoding="utf-8",
            )

            result = subprocess.run(
                [sys.executable, str(FLATTENER_SCRIPT), str(src)],
                capture_output=True,
                text=True,
                check=False,
                env={**os.environ, "CP_FLATTENER_MODE": "safe"},
            )

            self.assertEqual(result.returncode, 0, msg=result.stderr)
            self.assertNotIn("#define CP_FAST_IO_NAMESPACE_DEFINED", result.stdout)
            self.assertIn(
                "#define CP_IO_IMPL_READ(...) fast_io::read(__VA_ARGS__)",
                result.stdout,
            )


if __name__ == "__main__":
    unittest.main()
