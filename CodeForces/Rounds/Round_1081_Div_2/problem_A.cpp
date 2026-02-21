#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n);
  STR(s);

  I32 best = 0;
  FOR(shift, n) {
    I32 blocks = 1;
    FOR(i, 1, n) {
      const I32 prev = (shift + i - 1) % n;
      const I32 cur = (shift + i) % n;
      if (s[cur] != s[prev]) ++blocks;
    }
    chmax(best, blocks);
  }

  OUT(best);
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
