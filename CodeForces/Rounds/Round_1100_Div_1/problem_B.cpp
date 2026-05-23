#ifndef CP_TEMPLATE_PROFILE_STRICT
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_IO
#define NEED_MINMAX
#define NEED_TIMER

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n);
  VecI64 A(n);
  FOR(i, n) cin >> A[i];
  VecI64 B(n);
  FOR(i, n) cin >> B[i];

  I64 sum_U = 0;
  I64 max_V = 0;
  FOR(i, n) {
    I64 U = max(A[i], B[i]);
    I64 V = min(A[i], B[i]);
    sum_U += U;
    chmax(max_V, V);
  }

  OUT(sum_U + max_V);
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
