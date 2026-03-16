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
  I32 n;
  I64 k;
  IN(n, k);

  if (k < n || k > 2LL * n - 1) {
    NO();
    return;
  }

  const I32 wasted = as<I32>(k - n);
  VecI32 a(2 * n);

  if (wasted == 0) {
    for (I32 i = 0; i < n; ++i) {
      a[2 * i] = i + 1;
      a[2 * i + 1] = i + 1;
    }
  } else {
    a[0] = 1;
    a[1] = 2;

    for (I32 i = 1; i < wasted; ++i) {
      a[2 * i] = i + 2;
      a[2 * i + 1] = i + 1;
    }

    a[2 * wasted] = 1;
    a[2 * wasted + 1] = wasted + 1;

    for (I32 i = wasted + 1; i < n; ++i) {
      a[2 * i] = i + 1;
      a[2 * i + 1] = i + 1;
    }
  }

  YES();
  OUT(a);
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
