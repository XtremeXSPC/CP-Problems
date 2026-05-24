# Module Verification Workflow

Date: 2026-05-24

This document defines the first dedicated verification workflow for reusable
algorithm modules under `CodeForces/Algorithms`.

It complements the existing template-level checks in
`scripts/module_tester.py`.

## 1. Two layers of validation

### Template layer

Use:

```bash
python3 CodeForces/Algorithms/scripts/module_tester.py
```

Purpose:

- validate `NEED_*` combinations
- catch template integration breakage
- smoke-check compile surface of the starter stack

### Module layer

Use:

```bash
python3 CodeForces/Algorithms/scripts/module_verify.py
```

Purpose:

- validate concrete reusable modules
- compile and optionally run focused probes
- scale one imported module at a time

## 2. Manifest-driven design

Module verification cases live under:

- `CodeForces/Algorithms/verify/modules/`

Each module gets a JSON manifest plus one or more `.cpp` probes.
Modules should be grouped by category under subdirectories, for example:

- `verify/modules/data_structures/`
- `verify/modules/graph/`
- `verify/modules/strings/`

Current imported examples:

- `SlidingWindowAggregation`
- `PotentializedUnionFind`

## 3. Manifest format

Minimal shape:

```json
{
  "name": "ModuleName",
  "header": "modules/path/Header.hpp",
  "category": "data_structures",
  "tags": ["smoke"],
  "cases": [
    {
      "name": "runtime",
      "kind": "run",
      "source": "verify/modules/data_structures/ModuleName.runtime.cpp"
    },
    {
      "name": "compile",
      "kind": "compile",
      "source": "verify/modules/data_structures/ModuleName.compile.cpp"
    }
  ]
}
```

Supported case fields:

- `name`
- `kind`: `compile` or `run`
- `source`
- `stdin`
- `expected_stdout`
- `expected_exit_code`
- `timeout_seconds`

Naming and safety rules:

- manifest filename must match `name`, e.g. `SlidingWindowAggregation.json`
- probe filenames must start with the same module name
- `header` and `source` must be repository-relative paths under
  `CodeForces/Algorithms`
- manifests are rejected if they try to escape the workspace via `..` or
  absolute paths

## 4. Why both `compile` and `runtime`

The two case kinds serve different purposes.

`compile`:

- validates that the module's public API integrates cleanly
- catches include/dependency regressions
- is a cheap smoke-check for template-heavy surfaces

`runtime`:

- validates actual behavior on a small but meaningful scenario
- catches logical bugs that a syntax-only check cannot see

Recommended baseline for each imported module:

- one `compile` case
- one `runtime` case

## 5. Design goals

This system is intentionally different from a loose pile of ad-hoc test files.

Goals:

- keep each module's verification self-describing
- support both compile-only and runtime checks
- make future imports from Maspy easy to register
- produce machine-readable JSON reports
- stay compatible with the existing local compiler/template setup

## 6. Recommended workflow for new imports

When importing a new module:

1. adapt the header to local style and local core aliases
2. add one runtime probe covering normal behavior
3. add one compile-smoke probe for template/API shape
4. register both in a manifest under `verify/modules/`
5. run `module_verify.py` filtered to that module

Example:

```bash
python3 CodeForces/Algorithms/scripts/module_verify.py --module SlidingWindowAggregation
python3 CodeForces/Algorithms/scripts/module_verify.py --module PotentializedUnionFind
```

## 7. Why this is a good fit for this repository

The local library is centered on:

- centralized templates
- curated reusable modules
- explicit workflows and reproducible tooling

A manifest-driven verifier matches that philosophy better than a purely
personal shell-based setup.

It also gives us a clean bridge from Maspy-inspired imports to a local
long-term maintenance model.
