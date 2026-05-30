#ifndef CP_TEMPLATE_PROFILE_STRICT
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
  I32 n; IN(n);
  VecI32 a(n);
  for (auto& x : a) IN(x);

  I64 sum = 0;
  I64 ans = 2e18;
  VecI64 res(n);

  FOR(i, n) {
    sum += a[i];
    ans = min(ans, sum / (i + 1));
    res[i] = ans;
  }

  FOR(i, n) {
    cout << res[i] << (i + 1 == n ? '\n' : ' ');
  }
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Stopwatch timer;
#endif

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
