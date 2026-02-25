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
  INT(n);
  Vec<I32> a(n);
  IN(a);

  const I32 S = as<I32>(std::sqrt(as<F64>(n)));
  I64 ans = 0;

  // Pairs where the left element a[i] <= S.
  FOR(j, n) {
    const I32 aj = a[j];
    FOR(x, 1, S + 1) {
      const I64 i = as<I64>(j) - as<I64>(aj) * x;
      if (i < 0) break;
      if (a[as<I32>(i)] == x) ++ans;
    }
  }

  // Pairs where the left element a[i] > S and the right element a[j] <= S.
  FOR(i, n) {
    const I32 ai = a[i];
    if (ai <= S) continue;
    FOR(x, 1, S + 1) {
      const I64 j = as<I64>(i) + as<I64>(ai) * x;
      if (j >= n) break;
      if (a[as<I32>(j)] == x) ++ans;
    }
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
