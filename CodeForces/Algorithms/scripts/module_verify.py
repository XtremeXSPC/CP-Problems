#!/usr/bin/env python3
"""
Manifest-driven verification harness for algorithm modules.

This complements `module_tester.py`:
- `module_tester.py` validates template-level `NEED_*` combinations
- `module_verify.py` compiles and optionally runs focused module probes
"""

from __future__ import annotations

import argparse
import json
import subprocess
import tempfile
import time
from dataclasses import dataclass, field
from datetime import datetime
from pathlib import Path
from typing import Literal, TypedDict

from module_tester import _build_compiler_flags, _load_template_config, _select_compiler

VerificationKind = Literal["compile", "run"]


class VerificationCasePayload(TypedDict, total=False):
    """Validated subset of one verification case entry."""

    name: str
    kind: VerificationKind
    source: str
    stdin: str
    expected_stdout: str
    expected_exit_code: int
    timeout_seconds: float


class VerificationManifestPayload(TypedDict, total=False):
    """Validated subset of one module verification manifest."""

    name: str
    header: str
    category: str
    tags: list[str]
    cases: list[VerificationCasePayload]


@dataclass(frozen=True, slots=True)
class VerificationCase:
    """One compile/run probe discovered from a module manifest."""

    module_name: str
    header: str
    category: str
    tags: tuple[str, ...]
    name: str
    kind: VerificationKind
    source: Path
    stdin: str = ""
    expected_stdout: str | None = None
    expected_exit_code: int = 0
    timeout_seconds: float = 15.0

    @property
    def display_label(self) -> str:
        """Return the console label used when printing one verification case."""

        return f"{self.module_name} :: {self.name}"


@dataclass(frozen=True, slots=True)
class VerificationRecord:
    """Serializable outcome for one verification case."""

    module_name: str
    header: str
    category: str
    tags: tuple[str, ...]
    case_name: str
    kind: VerificationKind
    source: str
    success: bool
    elapsed_seconds: float
    compile_error: str | None = None
    runtime_error: str | None = None
    stdout: str | None = None
    stderr: str | None = None
    timestamp: str = field(default_factory=lambda: datetime.now().isoformat())

    def to_dict(self) -> dict[str, object]:
        """Serialize one record into a JSON-friendly dictionary."""

        payload: dict[str, object] = {
            "module_name": self.module_name,
            "header": self.header,
            "category": self.category,
            "tags": list(self.tags),
            "case_name": self.case_name,
            "kind": self.kind,
            "source": self.source,
            "success": self.success,
            "elapsed_seconds": self.elapsed_seconds,
            "timestamp": self.timestamp,
        }
        if self.compile_error is not None:
            payload["compile_error"] = self.compile_error
        if self.runtime_error is not None:
            payload["runtime_error"] = self.runtime_error
        if self.stdout is not None:
            payload["stdout"] = self.stdout
        if self.stderr is not None:
            payload["stderr"] = self.stderr
        return payload


class ModuleVerifier:
    """Discover module manifests, compile probes, and run focused checks."""

    def __init__(self, algorithms_dir: Path, module_filters: tuple[str, ...] = ()) -> None:
        """Initialize repository paths, compiler selection, and optional filters."""

        self.algorithms_dir = algorithms_dir.expanduser().resolve()
        self.templates_dir = self.algorithms_dir / "templates"
        self.verify_dir = self.algorithms_dir / "verify" / "modules"
        self.config = _load_template_config(self.templates_dir)
        self.compiler = _select_compiler(self.config)
        self.compile_only_flags = _build_compiler_flags(self.config)
        self.runtime_flags = self._build_runtime_flags(self.compile_only_flags)
        self.module_filters = tuple(filter(None, module_filters))
        self.results: list[VerificationRecord] = []

    @staticmethod
    def _build_runtime_flags(flags: tuple[str, ...]) -> tuple[str, ...]:
        """Convert compile-only flags into executable-build flags for runtime probes."""

        runtime_flags = [flag for flag in flags if flag != "-fsyntax-only"]
        if not any(flag.startswith("-O") for flag in runtime_flags):
            runtime_flags.append("-O2")
        return tuple(runtime_flags)

    def discover_cases(self) -> list[VerificationCase]:
        """Discover and validate every manifest-driven verification case."""

        manifests = sorted(self.verify_dir.rglob("*.json"))
        cases: list[VerificationCase] = []
        for manifest_path in manifests:
            cases.extend(self._load_manifest(manifest_path))
        return cases

    def _resolve_repo_path(self, raw_path: str, *, role: str, manifest_path: Path) -> Path:
        """Resolve one manifest path while preventing absolute or escaping paths."""

        relative_path = Path(raw_path)
        if relative_path.is_absolute():
            raise ValueError(f"{role} must be repository-relative in {manifest_path}: {raw_path}")

        resolved_path = (self.algorithms_dir / relative_path).resolve()
        try:
            resolved_path.relative_to(self.algorithms_dir)
        except ValueError as exc:
            raise ValueError(
                f"{role} escapes Algorithms root in {manifest_path}: {raw_path}"
            ) from exc
        return resolved_path

    def _load_manifest(self, manifest_path: Path) -> list[VerificationCase]:
        """Load, validate, and expand one manifest into executable verification cases."""

        try:
            payload = json.loads(manifest_path.read_text(encoding="utf-8"))
        except (OSError, json.JSONDecodeError) as exc:
            raise ValueError(f"Unable to read manifest {manifest_path}: {exc}") from exc

        if not isinstance(payload, dict):
            raise ValueError(f"Manifest must be a JSON object: {manifest_path}")

        name = payload.get("name")
        header = payload.get("header")
        cases_payload = payload.get("cases")
        if not isinstance(name, str) or not name.strip():
            raise ValueError(f"Manifest missing valid 'name': {manifest_path}")
        if not isinstance(header, str) or not header.strip():
            raise ValueError(f"Manifest missing valid 'header': {manifest_path}")
        if not isinstance(cases_payload, list) or not cases_payload:
            raise ValueError(f"Manifest missing non-empty 'cases': {manifest_path}")
        if manifest_path.stem != name:
            raise ValueError(
                f"Manifest filename must match module name in {manifest_path}: expected {name}.json"
            )

        if self.module_filters and name not in self.module_filters:
            return []

        category = payload.get("category", "uncategorized")
        tags_payload = payload.get("tags", [])
        tags = tuple(tag for tag in tags_payload if isinstance(tag, str))

        header_path = self._resolve_repo_path(header, role="header", manifest_path=manifest_path)
        if not header_path.is_file():
            raise ValueError(f"Header not found for manifest {manifest_path}: {header_path}")

        cases: list[VerificationCase] = []
        for case_payload in cases_payload:
            if not isinstance(case_payload, dict):
                raise ValueError(f"Invalid case entry in {manifest_path}")
            case_name = case_payload.get("name")
            source_rel = case_payload.get("source")
            if not isinstance(case_name, str) or not case_name.strip():
                raise ValueError(f"Case missing valid 'name' in {manifest_path}")
            if not isinstance(source_rel, str) or not source_rel.strip():
                raise ValueError(f"Case missing valid 'source' in {manifest_path}")
            kind = case_payload.get("kind", "run")
            if kind not in {"compile", "run"}:
                raise ValueError(f"Unsupported case kind '{kind}' in {manifest_path}")
            source_path = self._resolve_repo_path(
                source_rel,
                role="source",
                manifest_path=manifest_path,
            )
            if not source_path.is_file():
                raise ValueError(f"Source not found for case {case_name}: {source_path}")
            if not source_path.name.startswith(name):
                raise ValueError(
                    f"Source filename must start with module name '{name}' in {manifest_path}: "
                    f"{source_path.name}"
                )
            expected_stdout = case_payload.get("expected_stdout")
            if expected_stdout is not None and not isinstance(expected_stdout, str):
                raise ValueError(f"expected_stdout must be a string in {manifest_path}")
            stdin = case_payload.get("stdin", "")
            if not isinstance(stdin, str):
                raise ValueError(f"stdin must be a string in {manifest_path}")
            expected_exit_code = case_payload.get("expected_exit_code", 0)
            if not isinstance(expected_exit_code, int):
                raise ValueError(f"expected_exit_code must be an int in {manifest_path}")
            timeout_seconds = case_payload.get("timeout_seconds", 15.0)
            if not isinstance(timeout_seconds, (int, float)):
                raise ValueError(f"timeout_seconds must be numeric in {manifest_path}")
            cases.append(
                VerificationCase(
                    module_name=name,
                    header=header,
                    category=category if isinstance(category, str) else "uncategorized",
                    tags=tags,
                    name=case_name,
                    kind=kind,
                    source=source_path,
                    stdin=stdin,
                    expected_stdout=expected_stdout,
                    expected_exit_code=expected_exit_code,
                    timeout_seconds=float(timeout_seconds),
                )
            )
        return cases

    def _compile_binary(self, case: VerificationCase, output_path: Path) -> tuple[bool, str | None]:
        """Compile one runtime probe to an executable binary."""

        command = [
            self.compiler,
            *self.runtime_flags,
            f"-I{self.algorithms_dir}",
            str(case.source),
            "-o",
            str(output_path),
        ]
        try:
            result = subprocess.run(
                command,
                capture_output=True,
                text=True,
                timeout=case.timeout_seconds,
                check=False,
            )
        except subprocess.TimeoutExpired:
            return False, "Compilation timeout"
        except OSError as exc:
            return False, f"Unable to execute compiler '{self.compiler}': {exc}"
        if result.returncode == 0:
            return True, None
        return False, result.stderr

    def _compile_only(self, case: VerificationCase) -> tuple[bool, str | None]:
        """Run a syntax-only compilation pass for one compile-smoke probe."""

        command = [
            self.compiler,
            *self.compile_only_flags,
            f"-I{self.algorithms_dir}",
            str(case.source),
        ]
        try:
            result = subprocess.run(
                command,
                capture_output=True,
                text=True,
                timeout=case.timeout_seconds,
                check=False,
            )
        except subprocess.TimeoutExpired:
            return False, "Compilation timeout"
        except OSError as exc:
            return False, f"Unable to execute compiler '{self.compiler}': {exc}"
        if result.returncode == 0:
            return True, None
        return False, result.stderr

    def run_case(self, case: VerificationCase) -> VerificationRecord:
        """Execute one verification case and normalize its outcome."""

        start = time.perf_counter()
        if case.kind == "compile":
            success, compile_error = self._compile_only(case)
            elapsed = time.perf_counter() - start
            return VerificationRecord(
                module_name=case.module_name,
                header=case.header,
                category=case.category,
                tags=case.tags,
                case_name=case.name,
                kind=case.kind,
                source=str(case.source),
                success=success,
                elapsed_seconds=elapsed,
                compile_error=compile_error if not success else None,
            )

        with tempfile.TemporaryDirectory() as tmpdir:
            binary_path = Path(tmpdir) / "probe.out"
            compiled, compile_error = self._compile_binary(case, binary_path)
            if not compiled:
                elapsed = time.perf_counter() - start
                return VerificationRecord(
                    module_name=case.module_name,
                    header=case.header,
                    category=case.category,
                    tags=case.tags,
                    case_name=case.name,
                    kind=case.kind,
                    source=str(case.source),
                    success=False,
                    elapsed_seconds=elapsed,
                    compile_error=compile_error,
                )

            try:
                result = subprocess.run(
                    [str(binary_path)],
                    input=case.stdin,
                    capture_output=True,
                    text=True,
                    timeout=case.timeout_seconds,
                    check=False,
                )
            except subprocess.TimeoutExpired:
                elapsed = time.perf_counter() - start
                return VerificationRecord(
                    module_name=case.module_name,
                    header=case.header,
                    category=case.category,
                    tags=case.tags,
                    case_name=case.name,
                    kind=case.kind,
                    source=str(case.source),
                    success=False,
                    elapsed_seconds=elapsed,
                    runtime_error="Runtime timeout",
                )

            elapsed = time.perf_counter() - start
            success = result.returncode == case.expected_exit_code
            runtime_error = None
            if not success:
                runtime_error = (
                    f"Exit code mismatch: expected {case.expected_exit_code}, got {result.returncode}"
                )
            if case.expected_stdout is not None and result.stdout != case.expected_stdout:
                success = False
                runtime_error = "stdout mismatch"

            return VerificationRecord(
                module_name=case.module_name,
                header=case.header,
                category=case.category,
                tags=case.tags,
                case_name=case.name,
                kind=case.kind,
                source=str(case.source),
                success=success,
                elapsed_seconds=elapsed,
                runtime_error=runtime_error,
                stdout=result.stdout,
                stderr=result.stderr,
            )

    def run_all(self) -> bool:
        """Run all discovered verification cases and print a concise summary."""

        print("=" * 50)
        print("MODULE VERIFICATION HARNESS")
        print("=" * 50)

        cases = self.discover_cases()
        if not cases:
            print("No module verification cases discovered.")
            return False

        for case in cases:
            record = self.run_case(case)
            self.results.append(record)
            status = "PASS" if record.success else "FAIL"
            print(f"{case.display_label:45} {status} {record.elapsed_seconds:.3f}s")
            if record.compile_error:
                print(f"  Compile error: {record.compile_error[:240]}...")
            if record.runtime_error:
                print(f"  Runtime error: {record.runtime_error}")

        passed = sum(1 for record in self.results if record.success)
        total = len(self.results)
        print("\n" + "=" * 50)
        print(f"SUMMARY: {passed}/{total} verification cases passed")
        print("=" * 50)
        return passed == total

    def generate_report(self, output_file: Path) -> None:
        """Persist verification results as a JSON report on disk."""

        report = {
            "timestamp": datetime.now().isoformat(),
            "algorithms_dir": str(self.algorithms_dir),
            "compiler": self.compiler,
            "compile_only_flags": list(self.compile_only_flags),
            "runtime_flags": list(self.runtime_flags),
            "module_filters": list(self.module_filters),
            "results": [record.to_dict() for record in self.results],
        }
        output_file.parent.mkdir(parents=True, exist_ok=True)
        output_file.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
        print(f"\nVerification report saved to: {output_file}")


def build_parser() -> argparse.ArgumentParser:
    """Build the command-line interface for module verification runs."""

    parser = argparse.ArgumentParser(
        description="Compile/run manifest-driven verification cases for algorithm modules."
    )
    parser.add_argument(
        "--algorithms-dir",
        type=Path,
        default=None,
        help="path to CodeForces/Algorithms (default: auto-detect)",
    )
    parser.add_argument(
        "--module",
        action="append",
        default=[],
        help="limit verification to the given module name (repeatable)",
    )
    parser.add_argument(
        "--report",
        type=Path,
        default=None,
        help="optional output path for JSON report",
    )
    return parser


def main() -> int:
    """Run the verifier CLI and return a CI-friendly process exit code."""

    parser = build_parser()
    args = parser.parse_args()

    if args.algorithms_dir is not None:
        algorithms_dir = args.algorithms_dir.expanduser().resolve()
    else:
        algorithms_dir = Path(__file__).resolve().parent.parent

    if not algorithms_dir.is_dir():
        print(f"Error: Algorithms directory not found: {algorithms_dir}")
        return 1

    verifier = ModuleVerifier(algorithms_dir, tuple(args.module))
    success = verifier.run_all()

    if args.report is not None:
        verifier.generate_report(args.report.expanduser())

    return 0 if success else 1


if __name__ == "__main__":
    raise SystemExit(main())
