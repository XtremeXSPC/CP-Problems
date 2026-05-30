#ifndef CP_TEMPLATE_PROFILE_STRICT
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_TIMER
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  I32 n, x, s;
  String p;
  IN(n, x, s, p);

  I32 Op  = 0;
  I64 Cap = 0;
  I32 A_joiners = 0;

  I32 ans = 0;

  for (char c : p) {
    if (c == 'I') {
      if (Op < x) {
        Op++;
        Cap += s - 1;
        ans++;
      }
    } else if (c == 'E') {
      if (Cap > 0) {
        Cap--;
        ans++;
      } else if (A_joiners > 0 && Op < x) {
        A_joiners--;
        Op++;
        Cap += s - 1;
        ans++;
      }
    } else if (c == 'A') {
      if (Cap > 0) {
        Cap--;
        A_joiners++;
        ans++;
      } else if (Op < x) {
        Op++;
        Cap += s - 1;
        ans++;
      }
    }
  }

  OUT(ans);
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
