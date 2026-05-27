"""Immutable input/output dataclasses and mode enums for the pipeline.

``FlattenContext`` freezes everything the pipeline needs about the user
source (paths, contents, masked lines, used identifiers, macro state) so
each stage can be a pure function of its inputs. ``FlattenedTemplateBundle``
carries the rendered preamble and template sections between the bundle and
rewrite stages, decoupling them from CLI plumbing.

``FlattenerMode`` and ``ValidationStatus`` are the user-facing enums used by
``cli`` and ``validation`` for dispatch.
"""

from __future__ import annotations

from dataclasses import dataclass
from enum import Enum, auto
from pathlib import Path

from flattener_core.macros import MacroValueMap
from flattener_core.symbols import ModuleLeafTokenMap


class FlattenerMode(Enum):
    """Supported output strategies for flattened source generation."""

    COMPACT = "compact"
    SAFE = "safe"
    AUTO = "auto"
    SUBMISSION = "submission"


class ValidationStatus(Enum):
    """Availability and result of compile-check validation."""

    PASSED = auto()
    FAILED = auto()
    UNAVAILABLE = auto()


@dataclass(frozen=True, slots=True)
class FlattenContext:
    """Immutable input context shared across flattening pipeline stages."""

    source_file: Path
    source_content: str
    source_lines: tuple[str, ...]
    source_masked_lines: tuple[str, ...]
    project_root: Path
    templates_dir: Path
    preamble_path: Path
    preamble_resolved: Path
    need_mapping: dict[str, list[str]]
    used_identifiers: frozenset[str]
    source_module_includes: tuple[Path, ...]
    module_leaf_tokens: ModuleLeafTokenMap | None
    strip_module_docs: bool
    strip_template_docs: bool
    macro_values: MacroValueMap


@dataclass(frozen=True, slots=True)
class FlattenedTemplateBundle:
    """Rendered template section content plus effective macro state."""

    preamble_content: str
    template_sections: tuple[str, ...]
    effective_macro_values: MacroValueMap
    inlined_headers: frozenset[Path]
