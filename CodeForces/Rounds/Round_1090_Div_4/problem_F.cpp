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
  I32 X, Y;
  IN(X, Y);

  if (X > Y || (X == 0 && Y % 2 == 0)) {
    NO();
    return;
  }

  YES();

  I32 TotN = X + Y;
  I32 A = 0;
  I32 B = 0;

  if (TotN % 2 == 0) {
    B = X - 1;
    A = Y - B;
  } else {
    B = X;
    A = Y - B - 1;
  }

  I32 CN = 2;

  FOR(A) {
    OUT(1, CN);
    CN++;
  }

  FOR(B) {
    OUT(1, CN);
    OUT(CN, CN + 1);
    CN += 2;
  }
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
