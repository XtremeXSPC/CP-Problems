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
  VecI32 a(n + 1);
  FOR(i, 1, n + 1) IN(a[i]);

  VecI32 next_bad(n + 2, n + 1);
  for (I32 i = n - 1; i >= 1; --i) {
    next_bad[i] = (a[i + 1] > a[i] + 1) ? i + 1 : next_bad[i + 1];
  }

  VecI32 nxt(n + 2, n + 1);
  VecI32 st;
  st.reserve(n);

  for (I32 i = n; i >= 1; --i) {
    while (!st.empty() && a[st.back()] > a[i]) {
      st.pop_back();
    }

    const I32 next_smaller_or_equal = st.empty() ? n + 1 : st.back();
    nxt[i] = std::min(next_bad[i], next_smaller_or_equal);
    st.eb(i);
  }

  VecI64 dp(n + 2, 0);
  I64 ans = 0;
  for (I32 i = n; i >= 1; --i) {
    dp[i] = as<I64>(n - i + 1) + dp[nxt[i]];
    ans += dp[i];
  }

  OUT(ans);
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
