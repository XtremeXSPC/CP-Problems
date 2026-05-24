# PCH Benchmark

## Last recorded result

Date: 2026-02-15
Target: `problem_A` in `Round_1047_Div_3`
Compiler: GCC (`cp-debug-gcc` preset)

Raw numbers (`reports/PCHBenchmarkResult.json`):

| Scenario               | PCH ON     | PCH OFF    |
| ---------------------- | ---------- | ---------- |
| Configure              | 1.188s     | 1.070s     |
| Cold build             | 4.730s     | 2.304s     |
| **Incremental build**  | **0.867s** | **2.428s** |
| Reconfigure            | 0.104s     | 0.110s     |
| Post-reconfigure build | 4.834s     | 1.651s     |

## Interpretation

**Incremental build** (the dominant workflow during a round) improves by
**1.561s** with PCH ON — from 2.428s to 0.867s. This is the gain that matters
for iteration speed.

**Cold build** (first build after `cppconf` or `--fresh`) is **2.426s slower**
with PCH ON because the precompiled header must be compiled first. This penalty
occurs once per configure cycle.

**Post-reconfigure build** (after `cppconf` changes flags, e.g. `pch=off` then
back `pch=on`) is **3.183s slower** with PCH ON for the same reason.

## Decision

Keep PCH ON by default for Debug builds (`cp-debug-gcc` preset).

The incremental gain (+1.561s per build) outweighs the cold penalty given that
a round involves many incremental builds after the initial configure.

**Exception:** disable PCH temporarily when creating many new problem files at
once (`cppbatch`), since each new target triggers a cold build:

```bash
cppconf Debug gcc pch=off
# create many problems / first-pass builds
cppconf Debug gcc pch=on
```

PCH is always OFF for Release and Sanitize builds (no iteration value there).

## How to re-run

The benchmark script takes a single-target round as input:

```bash
python3 scripts/benchmark_pch.py \
  --round-dir /path/to/Rounds/Round_XXXX \
  --target problem_A \
  --compiler gcc \
  --algorithms-dir /path/to/CodeForces/Algorithms \
  --json-out reports/PCHBenchmarkResult.json
```

Re-run after major module additions or library reorganizations, as a larger
include surface may shift the incremental gain.

## Scope of the existing PCH

The current PCH (`libs/PCH.h` for Clang, `libs/PCH_Wrapper.h` for GCC) covers
the standard library only (`bits/stdc++.h` equivalent). Module headers are not
precompiled.

Adding module headers to the PCH is only worthwhile if a specific module appears
in every problem of a round. That is a per-round decision — not a library-level
one — and should be measured before committing to it.
