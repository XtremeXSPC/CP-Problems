#ifndef CP_TEMPLATE_PROFILE_STRICT
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_MACROS
#define NEED_TIMER
#define NEED_CORE
#define NEED_IO
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n);
  VecI32 A(n), B(n);
  FOR(i, n) IN(A[i]);
  FOR(i, n) IN(B[i]);

  auto OK = [&](I32 x) -> bool {
    I32 twos = 0;
    I32 bad  = 0;
    bool in_block = false;
    bool has_zero = false;

    auto close_block = [&]() {
      if (in_block && has_zero) ++bad;
      in_block = false;
      has_zero = false;
    };

    FOR(i, n) {
      const I32 v = (A[i] >= x) + (B[i] >= x);
      if (v == 2) {
        ++twos;
        close_block();
      } else {
        in_block = true;
        has_zero = has_zero || (v == 0);
      }
    }
    close_block();

    return twos > bad;
  };

  I32 lo  = 1;
  I32 hi  = 2 * n;
  I32 ans = 1;

  while (lo <= hi) {
    const I32 mid = (lo + hi) >> 1;
    if (OK(mid)) {
      ans = mid;
      lo  = mid + 1;
    } else {
      hi  = mid - 1;
    }
  }

  OUT(ans);
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
