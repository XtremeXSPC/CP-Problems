#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
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

auto solve() -> void {
  INT(n);
  VecI32 a(n);
  IN(a);

  VecBool can_win(n, false);

  FOR(start, 0, n) {
    auto cur = a;
    I32 remaining = 0;
    for (const I32 x : cur) remaining += x;

    I32 pos = start;
    I32 winner = -1;
    while (remaining > 0) {
      if (cur[pos] > 0) {
        --cur[pos];
        --remaining;
        winner = pos;
      }
      pos = (pos + 1) % n;
    }

    can_win[winner] = true;
  }

  OUT(std::count(can_win.begin(), can_win.end(), true));
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
