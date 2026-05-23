#ifndef CP_TEMPLATE_PROFILE_STRICT
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_MACROS
#define NEED_TIMER
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  STR(s);
  I32 right13 = 0;
  for (char c : s) {
    if (c == '1' || c == '3') right13++;
  }

  I32 left2 = 0;
  I32 max_kept = right13;

  for (char c : s) {
    if (c == '1' || c == '3') {
      right13--;
    } else if (c == '2') {
      left2++;
    }
    max_kept = max(max_kept, left2 + right13);
  }

  OUT(sz(s) - max_kept);
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Stopwatch timer;
#endif

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
