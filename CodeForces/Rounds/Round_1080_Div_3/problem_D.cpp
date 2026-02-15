#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n);

  Vec<I64> f(n + 1);
  FOR(i, 1, n + 1) IN(f[i]);

  // f(1) + f(n) = (n - 1) * sum(a[i]).
  const I64 total = (f[1] + f[n]) / (n - 1);

  Vec<I64> a(n + 1, 0);
  if (n == 2) {
    const I64 g1 = f[2] - f[1];
    a[1] = (total + g1) / 2;
    a[2] = total - a[1];
  } else {
    FOR(i, 2, n) {
      const I64 g_prev = f[i] - f[i - 1];
      const I64 g_curr = f[i + 1] - f[i];
      a[i] = (g_curr - g_prev) / 2;
    }

    const I64 g1 = f[2] - f[1];
    const I64 g_last = f[n] - f[n - 1];
    a[1] = (total + g1) / 2;
    a[n] = (total - g_last) / 2;
  }

  Vec<I64> ans(n);
  FOR(i, n) ans[i] = a[i + 1];
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
