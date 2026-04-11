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
  I32 BC;
  IN(BC);

  I32 CMin = 1;
  I32 CMed = BC + 1;

  VecI32 Res;
  Res.reserve(BC * 3);

  FOR (i, BC) {
    I32 CMax = CMed + 1;
    Res.eb(CMin);
    Res.eb(CMed);
    Res.eb(CMax);

    CMin++;
    CMed += 2;
  }

  OUT(Res);
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
