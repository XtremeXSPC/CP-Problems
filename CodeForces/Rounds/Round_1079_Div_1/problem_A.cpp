#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  LL(p, q);

  // Bob wins iff he can force the diagonal strategy to reach (2k, 3k).
  const bool bob_wins = (p < q) && (2 * q <= 3 * p);
  OUT(bob_wins ? "Bob" : "Alice");
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
