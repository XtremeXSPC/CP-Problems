#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n);
  bool ok = false;
  FOR(n) {
    INT(x);
    if (x == 67) ok = true;
  }
  YES(ok);
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
