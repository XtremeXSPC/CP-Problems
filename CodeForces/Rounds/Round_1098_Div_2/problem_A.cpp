#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
#define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
#define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_TIMER
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(N);
  I32 c0 = 0, c1 = 0, c2 = 0;
  FOR(N) {
    INT(W);
    if (W == 0)
      ++c0;
    else if (W == 1)
      ++c1;
    else
      ++c2;
  }
  I32 pairs = min(c1, c2);
  I32 rem   = abs(c1 - c2);

  I32 ans = c0 + pairs + rem / 3;
  OUT(ans);
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
