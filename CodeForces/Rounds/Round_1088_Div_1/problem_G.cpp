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

void solve() {
  INT(n);

  VecI64 a(n);
  I64 total_sum = 0;
  FOR(i, n) {
    IN(a[i]);
    total_sum += a[i];
  }

  if (n <= 2) {
    YES();
    return;
  }

  I64 prefix_sum = 0;
  I64 prev_dist  = 0;

  bool has_neg          = false;
  bool has_adj_positive = false;

  FOR(i, n - 1) {
    prefix_sum += a[i];
    const I64 index = i + 1;
    const I64 curr_dist = as<I64>(n) * prefix_sum - index * total_sum;

    if (curr_dist < 0) {
      has_neg = true;
    }
    if (i > 0 && prev_dist > 0 && curr_dist > 0) {
      has_adj_positive = true;
    }

    prev_dist = curr_dist;
  }

  OUT(has_neg || !has_adj_positive ? "Yes" : "No");
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
