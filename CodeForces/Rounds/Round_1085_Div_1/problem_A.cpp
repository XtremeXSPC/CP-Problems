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
  STR(s);

  I64 min_ones = 0;
  I64 max_ones = 0;

  for (I32 l = 0; l < n;) {
    I32 r = l;
    while (r + 1 < n && !(s[r] == '0' && s[r + 1] == '0')) ++r;

    I32 first_one = -1;
    I32 last_one  = -1;
    FOR(i, l, r + 1) {
      if (s[i] == '1') {
        if (first_one == -1) first_one = i;
        last_one = i;
      }
    }

    if (first_one != -1) {
      const I64 span = last_one - first_one + 1;
      max_ones += span;
      min_ones += (span + 2) / 2;
    }

    l = r + 1;
  }

  OUT(min_ones, max_ones);
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
