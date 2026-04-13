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
  I32 N;
  I64 M;
  IN(N, M);

  VecI64 A(N), D(N);
  IN(A);

  FOR(i, N) D[i] = (M - (A[i] + i + 1) % M) % M;

  auto in_seg = [&](I64 s, I64 e, I64 x) -> bool {
    if (s <= e) return s <= x && x <= e;
    return x >= s || x <= e;
  };

  I64 s = (D[0] + 1) % M;
  FOR(i, N - 1) {
    const I64 e = (D[i] + M - 1) % M;
    if (D[i + 1] != e) s = (D[i] + 1) % M;

    const I64 x = (D[i + 1] + 1) % M;
    if (in_seg(s, e, x)) {
      s = x;
      continue;
    }
    if (s == D[i + 1]) {
      NO();
      return;
    }
  }

  YES();
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
