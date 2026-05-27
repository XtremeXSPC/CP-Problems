"""Tests for the manifest-driven module verification harness.

Exercises the discovery, compile, and run paths of ``module_verify``: how
``.verify.toml`` manifests are parsed, how probes under
``verify/modules/<name>.{compile,runtime}.cpp`` are matched to module
names with word-boundary precision, and how filters / categories shape
the test set. Compile probes use stub manifests rather than real headers
so the suite stays decoupled from the C++ template surface.
"""

from __future__ import annotations

import contextlib
import io
import json
import shutil
import tempfile
import unittest
from pathlib import Path

SCRIPTS_DIR = Path(__file__).resolve().parents[1]

import module_verify  # noqa: E402


class ModuleVerifyTests(unittest.TestCase):
    def _write_manifest_case(self, root: Path, *, module_name: str = "DemoModule") -> Path:
        """Create a minimal manifest + probe pair for verifier unit tests."""

        (root / "templates").mkdir(parents=True, exist_ok=True)
        (root / "modules" / "demo").mkdir(parents=True, exist_ok=True)
        (root / "verify" / "modules").mkdir(parents=True, exist_ok=True)

        (root / "modules" / "demo" / "Demo.hpp").write_text(
            "#pragma once\ninline int demo_answer() { return 42; }\n",
            encoding="utf-8",
        )
        (root / "verify" / "modules" / f"{module_name}.runtime.cpp").write_text(
            '#include "modules/demo/Demo.hpp"\n'
            "#include <iostream>\n"
            'int main() { std::cout << demo_answer() << "\\n"; return 0; }\n',
            encoding="utf-8",
        )
        manifest = {
            "name": module_name,
            "header": "modules/demo/Demo.hpp",
            "category": "demo",
            "tags": ["smoke"],
            "cases": [
                {
                    "name": "basic-runtime",
                    "kind": "run",
                    "source": f"verify/modules/{module_name}.runtime.cpp",
                }
            ],
        }
        manifest_path = root / "verify" / "modules" / f"{module_name}.json"
        manifest_path.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
        return manifest_path

    @unittest.skipIf(shutil.which("c++") is None, "c++ is required for verification harness tests")
    def test_run_all_executes_manifest_runtime_case(self) -> None:
        """Runtime probes should compile, execute, and store their stdout."""

        with tempfile.TemporaryDirectory() as tmpdir:
            root = Path(tmpdir)
            self._write_manifest_case(root)
            verifier = module_verify.ModuleVerifier(root)

            with contextlib.redirect_stdout(io.StringIO()):
                success = verifier.run_all()

            self.assertTrue(success)
            self.assertEqual(len(verifier.results), 1)
            self.assertTrue(verifier.results[0].success)
            self.assertEqual(verifier.results[0].stdout, "42\n")

    @unittest.skipIf(shutil.which("c++") is None, "c++ is required for verification harness tests")
    def test_module_filter_limits_discovered_cases(self) -> None:
        """Module filters should exclude manifests with non-matching names."""

        with tempfile.TemporaryDirectory() as tmpdir:
            root = Path(tmpdir)
            self._write_manifest_case(root, module_name="KeepMe")
            verifier = module_verify.ModuleVerifier(root, ("DropMe",))

            self.assertEqual(verifier.discover_cases(), [])

    def test_invalid_manifest_raises_value_error(self) -> None:
        """Malformed manifests should raise a validation error."""

        with tempfile.TemporaryDirectory() as tmpdir:
            root = Path(tmpdir)
            (root / "templates").mkdir(parents=True, exist_ok=True)
            (root / "verify" / "modules").mkdir(parents=True, exist_ok=True)
            manifest_path = root / "verify" / "modules" / "bad.json"
            manifest_path.write_text('{"name": "Broken"}\n', encoding="utf-8")

            verifier = module_verify.ModuleVerifier(root)
            with self.assertRaises(ValueError):
                verifier.discover_cases()

    def test_rejects_source_path_outside_algorithms_root(self) -> None:
        """Manifest sources must stay inside the Algorithms workspace."""

        with tempfile.TemporaryDirectory() as tmpdir:
            root = Path(tmpdir)
            (root / "templates").mkdir(parents=True, exist_ok=True)
            (root / "modules" / "demo").mkdir(parents=True, exist_ok=True)
            (root / "verify" / "modules").mkdir(parents=True, exist_ok=True)
            (root / "modules" / "demo" / "Demo.hpp").write_text("#pragma once\n", encoding="utf-8")
            manifest = {
                "name": "EscapeCase",
                "header": "modules/demo/Demo.hpp",
                "cases": [
                    {
                        "name": "escape",
                        "kind": "compile",
                        "source": "../outside.cpp",
                    }
                ],
            }
            (root / "verify" / "modules" / "EscapeCase.json").write_text(
                json.dumps(manifest, indent=2) + "\n",
                encoding="utf-8",
            )

            verifier = module_verify.ModuleVerifier(root)
            with self.assertRaisesRegex(ValueError, "escapes Algorithms root"):
                verifier.discover_cases()

    def test_rejects_probe_name_without_module_prefix(self) -> None:
        """Probe filenames must start with the module name for consistent reporting."""

        with tempfile.TemporaryDirectory() as tmpdir:
            root = Path(tmpdir)
            (root / "templates").mkdir(parents=True, exist_ok=True)
            (root / "modules" / "demo").mkdir(parents=True, exist_ok=True)
            (root / "verify" / "modules").mkdir(parents=True, exist_ok=True)
            (root / "modules" / "demo" / "Demo.hpp").write_text("#pragma once\n", encoding="utf-8")
            (root / "verify" / "modules" / "wrong_name.cpp").write_text(
                '#include "modules/demo/Demo.hpp"\nint main() { return 0; }\n',
                encoding="utf-8",
            )
            manifest = {
                "name": "DemoModule",
                "header": "modules/demo/Demo.hpp",
                "cases": [
                    {
                        "name": "compile",
                        "kind": "compile",
                        "source": "verify/modules/wrong_name.cpp",
                    }
                ],
            }
            (root / "verify" / "modules" / "DemoModule.json").write_text(
                json.dumps(manifest, indent=2) + "\n",
                encoding="utf-8",
            )

            verifier = module_verify.ModuleVerifier(root)
            with self.assertRaisesRegex(ValueError, "must start with module name"):
                verifier.discover_cases()


class ProbeNameMatchingTests(unittest.TestCase):
    """Direct unit coverage for the probe-name word-boundary guard."""

    def test_exact_module_name_matches(self) -> None:
        self.assertTrue(module_verify._probe_name_matches_module("Foo", "Foo"))

    def test_dot_separator_matches(self) -> None:
        self.assertTrue(module_verify._probe_name_matches_module("Foo.runtime.cpp", "Foo"))

    def test_underscore_separator_matches(self) -> None:
        self.assertTrue(module_verify._probe_name_matches_module("Foo_extra.cpp", "Foo"))

    def test_letter_extension_does_not_match(self) -> None:
        """`Foo` should NOT match `Foobar.cpp` even though it is a prefix."""

        self.assertFalse(module_verify._probe_name_matches_module("Foobar.cpp", "Foo"))

    def test_unrelated_prefix_does_not_match(self) -> None:
        self.assertFalse(module_verify._probe_name_matches_module("Bar.cpp", "Foo"))
