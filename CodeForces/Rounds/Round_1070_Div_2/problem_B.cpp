#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n);
  STR(s);

  std::string doubled = s + s;
  I32 best = 0;
  I32 cur = 0;

  for (char c : doubled) {
    if (c == '0') {
      ++cur;
      chmax(best, cur);
    } else {
      cur = 0;
    }
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
