#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

using namespace std;

void solve() {
  INT(n, m);

  if (m > 2 * n - 1) {
    OUT("NO");
    return;
  }

  OUT("YES");

  FOR (i, n) {
    FOR (j, m) {
      const I32 color = (i + j) % n + 1;
      cout << color << (j + 1 == m ? '\n' : ' ');
    }
  }

  FOR (i, n) {
    FOR (j, m) {
      const I32 color = (j < n) ? ((i + j) % n + 1) : ((i + j + 1) % n + 1);
      cout << color << (j + 1 == m ? '\n' : ' ');
    }
  }
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
