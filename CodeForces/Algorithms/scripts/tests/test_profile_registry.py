"""Tests for profile_registry: load behavior, validation, and registry queries."""

from __future__ import annotations

import sys
import tempfile
import textwrap
import unittest
from pathlib import Path

SCRIPTS_DIR = Path(__file__).resolve().parents[1]
if str(SCRIPTS_DIR) not in sys.path:
    sys.path.insert(0, str(SCRIPTS_DIR))

import profile_registry  # noqa: E402


def _write_toml(body: str) -> Path:
    tmp = Path(tempfile.mkstemp(suffix=".toml")[1])
    tmp.write_text(textwrap.dedent(body), encoding="utf-8")
    return tmp


class ProfileRegistryTests(unittest.TestCase):
    def setUp(self) -> None:
        profile_registry.reset_cache()

    def test_loads_default_profiles_toml(self) -> None:
        registry = profile_registry.load_registry()
        self.assertEqual(registry.schema_version, 1)
        self.assertIn("simple", registry.io_profiles)
        self.assertIn("fast_extended", registry.io_profiles)
        self.assertEqual(
            set(registry.all_scaffold_names()),
            {"base", "default", "pbds", "advanced"},
        )

    def test_expand_scaffold_unions_io_needs(self) -> None:
        registry = profile_registry.load_registry()
        needs, defines = registry.expand_scaffold("advanced")
        self.assertIn("NEED_FAST_IO", needs)
        self.assertIn("NEED_MOD_INT", needs)
        self.assertIn("NEED_BIT_OPS", needs)
        self.assertEqual(defines.get("CP_FAST_IO_ENABLE_MODINT"), 1)

    def test_config_defaults_apply_strict_overrides(self) -> None:
        registry = profile_registry.load_registry()
        relaxed = registry.config_defaults_as_dict(strict=False)
        strict = registry.config_defaults_as_dict(strict=True)
        self.assertEqual(relaxed["CP_USE_GLOBAL_STD_NAMESPACE"], 1)
        self.assertEqual(strict["CP_USE_GLOBAL_STD_NAMESPACE"], 0)
        self.assertEqual(strict["CP_STRICT_TEMPLATE_NEEDS"], 1)

    def test_io_profile_to_needs_uses_macro_names(self) -> None:
        registry = profile_registry.load_registry()
        mapping = registry.io_profile_to_needs()
        self.assertIn("CP_IO_PROFILE_SIMPLE", mapping)
        self.assertEqual(mapping["CP_IO_PROFILE_SIMPLE"], ("NEED_IO",))

    def test_rejects_unsupported_schema_version(self) -> None:
        path = _write_toml("schema_version = 0\n")
        try:
            with self.assertRaisesRegex(ValueError, "schema_version"):
                profile_registry.load_registry(str(path))
        finally:
            path.unlink(missing_ok=True)

    def test_rejects_scaffold_referencing_unknown_io_profile(self) -> None:
        path = _write_toml(
            """\
            schema_version = 1

            [io_profile.simple]
            needs = ["NEED_IO"]

            [scaffold.broken]
            needs = ["NEED_MACROS"]
            io_profile = "does_not_exist"
            """
        )
        try:
            with self.assertRaisesRegex(ValueError, "io_profile"):
                profile_registry.load_registry(str(path))
        finally:
            path.unlink(missing_ok=True)

    def test_rejects_non_int_default(self) -> None:
        path = _write_toml(
            """\
            schema_version = 1

            [defaults]
            CP_BAD = "not-an-int"
            """
        )
        try:
            with self.assertRaisesRegex(ValueError, "expected int"):
                profile_registry.load_registry(str(path))
        finally:
            path.unlink(missing_ok=True)

    def test_rejects_non_list_needs(self) -> None:
        path = _write_toml(
            """\
            schema_version = 1

            [io_profile.broken]
            needs = "NEED_IO"
            """
        )
        try:
            with self.assertRaisesRegex(ValueError, "needs"):
                profile_registry.load_registry(str(path))
        finally:
            path.unlink(missing_ok=True)


class GeneratedArtefactStalenessTests(unittest.TestCase):
    """Regenerate the committed outputs and assert nothing drifts from the spec."""

    def setUp(self) -> None:
        profile_registry.reset_cache()
        self.templates_dir = SCRIPTS_DIR.parent / "templates"

    def _backup_and_run(self, generator_module_name: str, output_paths: list[Path]) -> None:
        import importlib

        module = importlib.import_module(generator_module_name)
        backups = {p: p.read_text(encoding="utf-8") for p in output_paths if p.is_file()}
        try:
            rc = module.main()
            self.assertEqual(rc, 0)
            for path in output_paths:
                self.assertTrue(path.is_file(), f"{path} missing after regenerate")
                current = path.read_text(encoding="utf-8")
                self.assertEqual(
                    current,
                    backups.get(path, current),
                    f"{path} drifted from profiles.toml; run scripts/{generator_module_name}.py",
                )
        finally:
            for path, content in backups.items():
                path.write_text(content, encoding="utf-8")

    def test_gen_config_outputs_match_committed_files(self) -> None:
        self._backup_and_run(
            "gen_config",
            [
                self.templates_dir / "Config_defaults.hpp",
                self.templates_dir / "Base_profiles.hpp",
            ],
        )

    def test_gen_scaffold_outputs_match_committed_files(self) -> None:
        registry = profile_registry.load_registry()
        scaffold_paths = [
            self.templates_dir / "cpp" / f"{name}.cpp"
            for name in registry.all_scaffold_names()
        ]
        self._backup_and_run("gen_scaffold", scaffold_paths)


if __name__ == "__main__":
    unittest.main()
