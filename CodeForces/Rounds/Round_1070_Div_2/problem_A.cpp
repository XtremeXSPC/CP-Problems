#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n);
  VC<I32> a(n);
  IN(a);

  I32 pref_max = a[0];
  I32 operations = 0;

  FOR(i, 1, n) {
    if (pref_max > a[i]) ++operations;
    chmax(pref_max, a[i]);
  }

  OUT(operations);
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
