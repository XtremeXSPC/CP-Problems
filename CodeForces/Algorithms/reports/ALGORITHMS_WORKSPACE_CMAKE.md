# Algorithms Workspace CMake

Date: 2026-05-24

`CodeForces/Algorithms` is a library workspace, not a contest round.

Its root CMake configuration is therefore different from the thin round-oriented
bootstrap used under `CodeForces/Rounds/*`.

## 1. Goal

This workspace CMake layer exists primarily to:

- generate a high-quality `compile_commands.json`
- give clangd/language-server tooling real target context
- register reusable verification probes under `verify/modules/`
- register template scaffold sources under `templates/cpp/`
- generate header probes so public headers inherit realistic compile flags

## 2. Configure

Use:

```bash
cmake --preset algorithms-workspace-debug
```

This writes the compilation database to:

- `CodeForces/Algorithms/build/workspace/debug/compile_commands.json`

The default preset pins Homebrew GNU C++ (`/opt/homebrew/bin/g++-15`) on purpose
because the template surface relies on GNU-style headers such as `bits/stdc++.h`.

Recommended symlink targets for editor tooling:

- `CodeForces/Algorithms/compile_commands.json`
- `CodeForces/Algorithms/.ide-configs/compile_commands.json`

## 3. Build presets

Verification probes:

```bash
cmake --build --preset algorithms-workspace-verify
```

Header probes:

```bash
cmake --build --preset algorithms-workspace-headers
```

Template scaffold sources:

```bash
cmake --build --preset algorithms-workspace-scaffolds
```

## 4. Design notes

This workspace now registers three target families:

- verification probes from `verify/modules/*.cpp`
- scaffold sources from `templates/cpp/*.cpp`
- generated header probes for public headers

The generated header probes are mainly for compile database coverage and editor
intelligence; they are not meant to replace focused runtime verification.
