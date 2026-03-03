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

using namespace std;

void solve() {
  INT(n, l, r);
  VecI64 a(n);
  IN(a);

  const I32 left_idx  = l - 1;
  const I32 right_idx = r - 1;
  const I32 k = right_idx - left_idx + 1;

  VecI64 left_values(a.begin(), a.begin() + right_idx + 1);
  VecI64 right_values(a.begin() + left_idx, a.end());

  ranges::sort(left_values);
  ranges::sort(right_values);

  I64 left_sum {0};
  I64 right_sum {0};
  FOR(i, k) {
    left_sum  += left_values[i];
    right_sum += right_values[i];
  }

  OUT(min(left_sum, right_sum));
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
