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
  INT(n);
  VecI64 a(n);
  FOR(i, n) IN(a[i]);

  I64 L = 0;
  bool sorted = true;
  FOR(i, n - 1) {
    if (a[i] > a[i + 1]) {
      sorted = false;
      L = max(L, a[i] - a[i + 1]);
    }
  }

  if (sorted) {
    YES();
    return;
  }

  bool dp0 = true;
  bool dp1 = true;
  FOR(i, n - 1) {
    bool next_dp0 = false;
    bool next_dp1 = false;

    if (dp0) {
      if (a[i] <= a[i + 1]) next_dp0     = true;
      if (a[i] <= a[i + 1] + L) next_dp1 = true;
    }
    if (dp1) {
      if (a[i] + L <= a[i + 1]) next_dp0     = true;
      if (a[i] + L <= a[i + 1] + L) next_dp1 = true;
    }

    dp0 = next_dp0;
    dp1 = next_dp1;

    if (!dp0 && !dp1) {
      NO();
      return;
    }
  }
  YES();
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
