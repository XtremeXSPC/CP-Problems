# Centralized Round Build System

This directory is the single source of truth for round-level CMake logic.

## Phase 1 Layout

- `CPRoundBootstrap.cmake`: entrypoint included by each round `CMakeLists.txt`.
- `CPRoundConfig.cmake`: global build configuration entrypoint (RPATH, TBB, clangd include discovery, project/cache setup).
- `CPRoundTargets.cmake`: target definitions (`cp_add_problem`), auto-discovery, utility targets (`symlink_clangd`, `all_problems`, `rebuild`).
- `CPRoundSummary.cmake`: configure-time summary output.
- `ClangdAssist.cmake`: compiler include-path detection helpers.
- `CPRoundFeatureTiming.cmake`: optional compilation-timing flags.
- `CPRoundFeatureLTO.cmake`: optional Link-Time Optimization configuration.
- `CPRoundFeatureSanitizers.cmake`: sanitizer compiler checks/policy.
- `CPRoundFeaturePCH.cmake`: PCH detection and rebuild policy.

## Round Integration

Round `CMakeLists.txt` should stay thin:

```cmake
cmake_minimum_required(VERSION 3.20)
project(competitive_programming LANGUAGES CXX)
include("${CMAKE_CURRENT_SOURCE_DIR}/algorithms/cmake/CPRoundBootstrap.cmake")
```

This keeps all round build behavior inherited from one centralized implementation.

Round `CMakePresets.json` can also stay thin and include centralized presets:

```json
{
  "version": 6,
  "include": ["algorithms/cmake/CPRoundPresets.json"]
}
```

## Compatibility Notes

- Round algorithms links should point to `CodeForces/Algorithms/libs/*` (no root-level compatibility stubs).
- `ClangdAssist.cmake` includes stale-cache recovery for GCC on macOS when target triples change across upgrades.
- Toolchain files are centralized in `CodeForces/Algorithms/` and are selected via `cppconf` (`-DCMAKE_TOOLCHAIN_FILE=...`).
- Round-level `gcc-toolchain.cmake` / `clang-toolchain.cmake` are intentionally removed to avoid symlink portability issues.
- Legacy monolithic `CodeForces/Algorithms/CMakeLists.txt` is removed; round builds are driven by centralized bootstrap/presets.
- You can purge legacy round-local toolchain files with:
  `CodeForces/Algorithms/scripts/migrate_round_toolchains.py`.
- You can write/update thin round-level preset wrappers with:
  `CodeForces/Algorithms/scripts/migrate_round_presets.py`.

## Unified Build Entry

Centralized presets live in:

- `CodeForces/Algorithms/cmake/CPRoundPresets.json`

Useful commands from a round directory:

```bash
# Configure with centralized preset
CP_ALGORITHMS_DIR=/absolute/path/to/CodeForces/Algorithms cmake --preset cp-debug-gcc

# Build with centralized preset
cmake --build --preset cp-build-debug-gcc
```

`CP_ALGORITHMS_DIR` is required because presets intentionally do not depend on
round-local toolchain files.

Equivalent centralized commands via workflow manager:

```bash
python3 /Volumes/LCS.Data/CP-Problems/CodeForces/Algorithms/scripts/workflow_manager.py \
  preset-conf --preset cp-debug-gcc --fresh

python3 /Volumes/LCS.Data/CP-Problems/CodeForces/Algorithms/scripts/workflow_manager.py \
  preset-build --preset cp-build-debug-gcc
```

## Next Phase (recommended)

- Update `cpp-tools` template generation so new rounds are created directly with the thin `CMakeLists.txt`.
- Add a reproducible benchmark script for first-build vs incremental-build impact (`PCH on/off`) to guide default preset tuning.
