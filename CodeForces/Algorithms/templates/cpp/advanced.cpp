//===----------------------------------------------------------------------===//
/**
 * @file: __FILE_NAME__
 * @brief __PROBLEM_BRIEF__
 * @author: Costantino Lombardi
 *
 * @status: In Progress
 */
//===----------------------------------------------------------------------===//

// Strict profile is enabled by default to keep generated output lean/safe.
// Define CP_TEMPLATE_PROFILE_RELAXED to disable strict defaults.
#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_FAST_IO
#define NEED_PBDS
#define NEED_BIT_OPS
#define NEED_MOD_INT
#define NEED_CONTAINERS
#define NEED_TYPE_SAFETY

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  // Optimized solution here
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
#endif

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
