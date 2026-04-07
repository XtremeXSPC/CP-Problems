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

auto count_k(I32 a, I32 lim) -> I64 {
  if (lim < 0) return 0;

  I64 less = 0;
  I64 same = 1;

  for (I32 bit = 19; bit >= 0; --bit) {
    const I32 abit = (a >> bit) & 1;
    const I32 lbit = (lim >> bit) & 1;

    I64 next_less = 0;
    I64 next_same = 0;

    if (less > 0) {
      if (abit == 0) {
        next_less += less * 2;
      } else {
        next_less += less;
      }
    }

    if (same > 0) {
      if (abit == 0) {
        if (lbit == 0) {
          next_same += same;
        } else {
          next_less += same;
          next_same += same;
        }
      } else {
        if (lbit == 0) {
          next_same += same;
        } else {
          next_less += same;
        }
      }
    }

    less = next_less;
    same = next_same;
  }

  return less + same;
}

auto count_bad(I32 a, I32 x2) -> I64 {
  const I32 lim = (x2 - 1 - a) / 2;
  if (x2 - 1 < a) return 0;
  return count_k(a, lim) << std::popcount(static_cast<U32>(a));
}

void solve() {
  INT(x1, x2);

  I64 best = Limits<I64>::max();
  I32 ans  = 0;

  FOR(a, x1) {
    const I64 cur = count_bad(a, x2);
    if (cur < best) {
      best = cur;
      ans  = a;
      if (best == 0) break;
    }
  }

  OUT(ans + 1, x1);
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
