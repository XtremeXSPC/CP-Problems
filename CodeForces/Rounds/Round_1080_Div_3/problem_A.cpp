#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  I64 a, b;
  std::cin >> a >> b;
  std::cout << (a + b) << '\n';
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
