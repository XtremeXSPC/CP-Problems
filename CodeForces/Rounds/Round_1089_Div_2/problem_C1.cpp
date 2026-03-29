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
  VecI64 a(n), b(n);
  IN(a);
  IN(b);

  if (!std::ranges::equal(a, b)) {
    OUT(0);
    return;
  }

  VecI64 g(n - 1);
  FOR(i, n - 1) g[i] = std::gcd(a[i], a[i + 1]);

  VecI64 need(n);
  need[0] = g[0];
  need[n - 1] = g[n - 2];
  FOR(i, 1, n - 1) need[i] = g[i - 1] / std::gcd(g[i - 1], g[i]) * g[i];

  I32 ans = 0;
  FOR(i, n) ans += (need[i] < a[i]);
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
