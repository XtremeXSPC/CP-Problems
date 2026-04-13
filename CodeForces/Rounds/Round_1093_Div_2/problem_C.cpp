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
  I64 P, Q;
  IN(P, Q);

  const I64 K = 2 * P + 4 * Q + 1;

  for (I64 D = 3; D * D <= K; D += 2) {
    if (K % D != 0) continue;

    const I64 n = (D - 1) / 2;
    const I64 m = (K / D - 1) / 2;
    if (Q <= n * m + min(n, m)) {
      OUT(n, m);
      return;
    }
  }

  OUT(-1);
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
