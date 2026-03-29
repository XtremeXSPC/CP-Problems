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
  VecI32 p(n);
  IN(p);

  VecI32 pos(n + 1);
  FOR(i, n) pos[p[i]] = i + 1;

  VecI32 pref(n + 1, 0);
  FOR(x, 1, n + 1) pref[x] = pref[x - 1] + (pos[x] >= x);

  I32 ans = pref[n];
  FOR(j, 1, n + 1) {
    if (pos[j] < j) ans = std::max(ans, pref[j - 1]);
  }
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
