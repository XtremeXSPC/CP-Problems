# Algorithms Workspace CMake

Date: 2026-05-24

`CodeForces/Algorithms` is a library workspace, not a contest round.

Its root CMake configuration is therefore different from the thin round-oriented
bootstrap used under `CodeForces/Rounds/*`.

## 1. Goal

This workspace CMake layer exists primarily to:

- generate a high-quality `compile_commands.json` for clangd and other language servers
- give the language server real target context (include paths, C++ standard, flags)
- register reusable verification probes under `verify/modules/`
- register template scaffold sources under `templates/cpp/`
- generate header probes so every public header appears in the compilation database

## 2. Configure

```bash
cmake --preset algorithms-workspace-debug
```

This writes the compilation database to:

```path
CodeForces/Algorithms/build/workspace/debug/compile_commands.json
```

The default preset pins Homebrew GNU C++ (`/opt/homebrew/bin/g++-15`) on purpose
because the template surface relies on GNU-style headers such as `bits/stdc++.h`.

A symlink at the workspace root already points to it:

```path
CodeForces/Algorithms/compile_commands.json -> build/workspace/debug/compile_commands.json
```

Configure clangd to consume it by keeping `.clangd` at the workspace root. No
manual symlink management is needed beyond the initial `cmake --preset`.

## 3. Build presets

All probe targets are `EXCLUDE_FROM_ALL` — nothing builds unless explicitly
requested.

Module verification probes:

```bash
cmake --build --preset algorithms-workspace-verify
```

Header probes (compile-tests one per public header):

```bash
cmake --build --preset algorithms-workspace-headers
```

Template scaffold sources:

```bash
cmake --build --preset algorithms-workspace-scaffolds
```

All probe families at once:

```bash
cmake --build --preset algorithms-workspace-all-probes
```

Or via the Makefile shorthand from the workspace root:

```bash
make all-probes
```

## 4. Probe families

| Family    | CMake property                   | Source pattern            | Purpose                              |
| --------- | -------------------------------- | ------------------------- | ------------------------------------ |
| verify    | `CP_ALGORITHMS_VERIFY_TARGETS`   | `verify/modules/**/*.cpp` | compile + run module smoke tests     |
| headers   | `CP_ALGORITHMS_HEADER_TARGETS`   | all public `.hpp`/`.h`    | ensure every header resolves cleanly |
| scaffolds | `CP_ALGORITHMS_SCAFFOLD_TARGETS` | `templates/cpp/*.cpp`     | index template sources in clangd     |

## 5. Configure-time summary

After `cmake --preset algorithms-workspace-debug`, the configure step prints:

```example
-- CP Algorithms Workspace configured:
--   Verify probes   : <N>
--   Header probes   : <N>
--   Scaffold probes : <N>
```

Use these counts to verify that new headers and verify sources are being picked
up correctly after reorganizations.

## 6. Options

All three probe families can be disabled independently:

```bash
cmake --preset algorithms-workspace-debug \
  -DCP_ALGORITHMS_ENABLE_VERIFY_PROBES=OFF \
  -DCP_ALGORITHMS_ENABLE_HEADER_PROBES=OFF \
  -DCP_ALGORITHMS_ENABLE_TEMPLATE_SCAFFOLDS=OFF
```

## 7. Design notes

The workspace INTERFACE library `cp_algorithms_workspace` owns the include paths
and compiler flags. Every probe executable links against it privately, inheriting
the same flags that a contest solution would see.

This file is intentionally separate from the contest round bootstrap
(`cmake/CPRoundBootstrap.cmake`). Round builds are driven by thin per-round
`CMakeLists.txt` files; the workspace build is a permanent fixture of the
library repository.
