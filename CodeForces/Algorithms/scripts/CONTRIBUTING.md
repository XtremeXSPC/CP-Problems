# Contributing to `Algorithms/scripts/`

This directory hosts the Python tooling that wraps the centralized CP build
(flattener, workflow manager, module tester/verifier, generators). The code
here is **not** packaged or pip-installed — scripts run directly via
`python3 scripts/<name>.py` or, for top-level workflows, through the
[`Makefile`](./Makefile).

## Required toolchain

- **Python ≥ 3.11** (uses `tomllib`, structural pattern matching, PEP 604
  unions, `StrEnum`, `typing.Self`).
- **ruff** for lint/format. Install: `brew install ruff` or
  `pipx install ruff`.
- **mypy** for static type checking in strict mode. Install:
  `brew install mypy` or `pipx install mypy`.
- **zsh** + the user's local `cpp-tools` install for the workflow manager
  runtime path (`competitive.sh`).

## CI gate

```sh
make check          # Runs lint + typecheck + test
```

Each of these is also available as a standalone target. Any of them failing
must block the PR.

## Coding conventions

- **Type annotations everywhere** that mypy can verify in strict mode.
  Use `from __future__ import annotations` to keep forward references cheap.
- **Dataclasses with `frozen=True, slots=True`** for value objects; default
  to immutable.
- **`enum.StrEnum`** for closed sets of CLI values (`BuildType`, `Compiler`,
  preset names, …). Never hand-roll `("foo", "bar")` tuples for argparse
  choices.
- **`@functools.cache` / `@functools.cached_property`** for values that are
  expensive once and stable thereafter (e.g. discovered toolchains, git
  toplevel lookups).
- **Subprocess calls go through `scripts/_lib/process.run_capture`**.
  Direct `subprocess.run` is forbidden outside `_lib/` (enforced by a
  `grep`-style test).
- **Path safety**: every path coming from a manifest or CLI argument must be
  resolved and `Path.relative_to`-checked against the expected root.

## Workflow

1. `make lint` early and often.
2. New CLI flag or behavior? Add a focused unittest in `tests/` —
   `test_scripts_regressions.py` is the canonical home for cross-script
   regressions.
3. Before pushing: `make check`. If `make typecheck` complains about
   stdlib stubs, document the `# type: ignore[<code>]` reason inline.

## File map

See [reports/Audits/2026-05-26_Scripts_Audit.md](../reports/Audits/2026-05-26_Scripts_Audit.md)
§5 for an up-to-date inventory.
