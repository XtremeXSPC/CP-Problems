//===----------------------------------------------------------------------===//
/* PCH_Workspace.h — Diagnostic PCH for the Algorithms Workspace
 *
 * Used exclusively by the workspace probe build (see CMakeLists.txt at the
 * Algorithms root). Pulls in just the standard library headers that most
 * modules transitively need, so that compiling 150+ header probes and 38
 * verify probes amortizes the cost of <vector>, <string>, <algorithm>, etc.
 *
 * Why this file is NOT PCH.h / PCH_Wrapper.h:
 *   The contest PCHs apply `#pragma GCC optimize("O3,unroll-loops,fast-math,...")`
 *   and `#pragma GCC target(...)` at PCH-compile time. CMake's PCH facility
 *   injects the resulting .gch via `-include-pch`, which means consumer TUs
 *   never see those pragmas at file scope — GCC then refuses to inline
 *   always_inline std functions with "target specific option mismatch".
 *
 *   The workspace is purely diagnostic: it indexes headers for clangd, runs
 *   verify probes, and validates that public APIs compile cleanly. No contest
 *   tuning is needed, and shipping a thin PCH avoids the pragma conflict.
 *
 * For contest builds, use PCH.h (Clang/MSVC) or PCH_Wrapper.h (GCC) instead.
 */
//===----------------------------------------------------------------------===//
// clang-format off

#ifndef CP_ALGORITHMS_PCH_WORKSPACE_H
#define CP_ALGORITHMS_PCH_WORKSPACE_H

#if defined(__GNUC__) && !defined(__clang__)
  // GCC: bits/stdc++.h is the canonical contest-style aggregate; no pragmas.
  #include <bits/stdc++.h>
#else
  // Clang/AppleClang/MSVC: delegate to the canonical portable header to keep
  // the workspace PCH surface in lockstep with what contest builds see on the
  // same compiler family.
  #include "templates/core/PortableStdHeaders.hpp"
#endif

#endif  // CP_ALGORITHMS_PCH_WORKSPACE_H
//===----------------------------------------------------------------------===//
