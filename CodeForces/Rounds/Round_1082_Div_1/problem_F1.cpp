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
  INT(n, q);

  VecI32 row_deg(n + 1, 0);
  VecI32 col_deg(n + 1, 0);
  VecI32 d_star(n + 1, 0);

  I64 sum_col_sq = 0;
  I64 sum_d_sq   = 0;

  FOR(_, q) {
    INT(r, c);

    sum_col_sq += 2LL * col_deg[c] + 1;
    ++col_deg[c];

    const I32 next_row_deg = row_deg[r] + 1;
    sum_d_sq += 2LL * d_star[next_row_deg] + 1;
    ++d_star[next_row_deg];
    row_deg[r] = next_row_deg;

    OUT(sum_col_sq == sum_d_sq ? "YES" : "NO");
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
