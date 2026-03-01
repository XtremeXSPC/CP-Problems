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

void solve() {
  STR(s);
  const I32 n = sz(s);

  const I32 l1 = 1;
  const I32 r1 = n;

  I32 first_zero = -1;
  FOR(i, n) {
    if (s[i] == '0') {
      first_zero = i;
      break;
    }
  }

  if (first_zero == -1) {
    OUT(l1, r1, 1, 1);
    return;
  }

  const I32 len = n - first_zero;
  I32 best_start = 0;

  const auto is_better = [&](const I32 lhs_start, const I32 rhs_start) -> bool {
    FOR(k, len) {
      const bool lhs_bit = (s[first_zero + k] != s[lhs_start + k]);
      const bool rhs_bit = (s[first_zero + k] != s[rhs_start + k]);
      if (lhs_bit != rhs_bit) return lhs_bit;
    }
    return false;
  };

  FOR(start, 1, first_zero + 1) {
    if (is_better(start, best_start)) best_start = start;
  }

  OUT(l1, r1, best_start + 1, best_start + len);
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
