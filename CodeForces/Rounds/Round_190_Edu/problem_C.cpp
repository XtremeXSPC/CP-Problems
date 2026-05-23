#ifndef CP_TEMPLATE_PROFILE_STRICT
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_MACROS
#define NEED_TIMER
#define NEED_CONTAINERS
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n);
  VecI64 C(n);
  FOR (i, n) {
    IN(C[i]);
  }

  I64 major_sum = 0;
  I64 minor_count = 0;
  I64 m = 0;

  for (I64 x : C) {
    if (x >= 2) {
      major_sum += x;
      m++;
    } else if (x == 1) {
      minor_count++;
    }
  }

  if (m == 0) {
    OUT(0);
    return;
  }

  I64 slots = 0;
  if (m == 1) {
    slots = major_sum / 2;
  } else {
    for (I64 x : C) {
      if (x >= 2) {
        slots += (x - 2) / 2;
      }
    }
  }

  I64 ans = major_sum + min(minor_count, slots);
  if (ans < 3) {
    OUT(0);
  } else {
    OUT(ans);
  }
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
