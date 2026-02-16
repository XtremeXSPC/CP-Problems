#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

auto odd_part(I32 x) -> I32 {
  while ((x & 1) == 0) x >>= 1;
  return x;
}

void solve() {
  INT(n);
  VEC(I32, a, n);

  bool answer = true;
  FOR(i, n) {
    if (odd_part(i + 1) != odd_part(a[i])) {
      answer = false;
      break;
    }
  }

  YES(answer);
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
