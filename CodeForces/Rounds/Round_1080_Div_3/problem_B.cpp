#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  I64 n;
  std::cin >> n;
  std::cout << (n * n) << '\n';
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  int T;
  std::cin >> T;
  while (T--) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
