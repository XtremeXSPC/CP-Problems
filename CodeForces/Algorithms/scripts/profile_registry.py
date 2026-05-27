#!/usr/bin/env python3
"""Typed loader for ``templates/profiles.toml`` (uses :mod:`tomllib`).

Parses the central profile registry into immutable dataclasses exposing:
``config_defaults`` (strict vs relaxed CP_* values), ``io_profiles`` (per
``CP_IO_PROFILE_*`` ``NEED_*`` lists + extra defines), and scaffold
recipes. Consumers (``flattener_pipeline.macros``, ``gen_config``,
``gen_scaffold``) reach the registry exclusively through this loader so
schema changes stay in one place.
"""

from __future__ import annotations

import tomllib
from collections.abc import Mapping
from dataclasses import dataclass
from functools import lru_cache
from pathlib import Path
from typing import Any

SCRIPTS_DIR = Path(__file__).resolve().parent
TEMPLATES_DIR = SCRIPTS_DIR.parent / "templates"
DEFAULT_PROFILES_PATH = TEMPLATES_DIR / "profiles.toml"


@dataclass(frozen=True, slots=True)
class IOProfile:
    name: str
    needs: frozenset[str]
    defines: Mapping[str, int]


@dataclass(frozen=True, slots=True)
class ScaffoldProfile:
    name: str
    description: str
    needs: frozenset[str]
    io_profile: str
    header_doc: bool
    advanced: bool


@dataclass(frozen=True, slots=True)
class ConfigDefaults:
    base: Mapping[str, int]
    strict_overrides: Mapping[str, int]


@dataclass(frozen=True, slots=True)
class ProfileRegistry:
    schema_version: int
    io_profiles: Mapping[str, IOProfile]
    scaffolds: Mapping[str, ScaffoldProfile]
    defaults: ConfigDefaults

    def expand_io(self, name: str) -> tuple[frozenset[str], dict[str, int]]:
        profile = self.io_profiles[name]
        return profile.needs, dict(profile.defines)

    def expand_scaffold(self, name: str) -> tuple[frozenset[str], dict[str, int]]:
        scaffold = self.scaffolds[name]
        io_needs, io_defines = self.expand_io(scaffold.io_profile)
        return scaffold.needs | io_needs, dict(io_defines)

    def all_scaffold_names(self) -> tuple[str, ...]:
        return tuple(self.scaffolds.keys())

    def config_defaults_as_dict(self, *, strict: bool) -> dict[str, int]:
        values = dict(self.defaults.base)
        if strict:
            values.update(self.defaults.strict_overrides)
        return values

    def io_profile_to_needs(self) -> dict[str, tuple[str, ...]]:
        return {
            f"CP_IO_PROFILE_{name.upper()}": tuple(sorted(p.needs))
            for name, p in self.io_profiles.items()
        }


def _int_map(payload: object, *, where: str) -> dict[str, int]:
    if not isinstance(payload, dict):
        raise ValueError(f"{where}: expected a table")
    out: dict[str, int] = {}
    for k, v in payload.items():
        if not isinstance(k, str) or isinstance(v, bool) or not isinstance(v, int):
            raise ValueError(f"{where}.{k}: expected int")
        out[k] = v
    return out


def _io(name: str, payload: dict[str, Any]) -> IOProfile:
    needs = payload.get("needs", [])
    if not isinstance(needs, list) or not all(isinstance(n, str) for n in needs):
        raise ValueError(f"io_profile.{name}.needs must be list[str]")
    return IOProfile(
        name=name,
        needs=frozenset(needs),
        defines=_int_map(payload.get("defines", {}) or {}, where=f"io_profile.{name}.defines"),
    )


def _scaffold(name: str, payload: dict[str, Any], known_io: frozenset[str]) -> ScaffoldProfile:
    needs = payload.get("needs", [])
    io_profile = payload.get("io_profile", "")
    if not isinstance(needs, list) or not all(isinstance(n, str) for n in needs):
        raise ValueError(f"scaffold.{name}.needs must be list[str]")
    if not isinstance(io_profile, str) or io_profile not in known_io:
        raise ValueError(f"scaffold.{name}.io_profile must reference a known io_profile")
    return ScaffoldProfile(
        name=name,
        description=str(payload.get("description", "")),
        needs=frozenset(needs),
        io_profile=io_profile,
        header_doc=bool(payload.get("header_doc", False)),
        advanced=bool(payload.get("advanced", False)),
    )


def _build(payload: dict[str, Any]) -> ProfileRegistry:
    schema = payload.get("schema_version", 0)
    if schema != 1:
        raise ValueError(f"unsupported schema_version: {schema}")

    defaults_payload = dict(payload.get("defaults", {}) or {})
    strict = defaults_payload.pop("strict_overrides", {}) or {}
    defaults = ConfigDefaults(
        base=_int_map(defaults_payload, where="defaults"),
        strict_overrides=_int_map(strict, where="defaults.strict_overrides"),
    )

    io_payload = payload.get("io_profile", {}) or {}
    io_profiles = {n: _io(n, b) for n, b in io_payload.items()}
    known_io = frozenset(io_profiles.keys())

    scaffold_payload = payload.get("scaffold", {}) or {}
    scaffolds = {n: _scaffold(n, b, known_io) for n, b in scaffold_payload.items()}

    return ProfileRegistry(
        schema_version=schema,
        io_profiles=io_profiles,
        scaffolds=scaffolds,
        defaults=defaults,
    )


@lru_cache(maxsize=4)
def load_registry(path: str | None = None) -> ProfileRegistry:
    resolved = Path(path) if path else DEFAULT_PROFILES_PATH
    with open(resolved, "rb") as f:
        return _build(tomllib.load(f))


def reset_cache() -> None:
    load_registry.cache_clear()
