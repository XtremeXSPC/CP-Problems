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
  INT(n, k);

#ifdef LOCAL
  my_assert(k == 1);
#endif

  VecI32 a(n);
  IN(a);

  INT(p);
  --p;

  const I32 x = a[p];

  I32 left = 0;
  I32 cur  = 0;
  FOR(i, p) {
    const I32 need = a[i] ^ x;
    if ((need ^ cur) == 1) {
      ++left;
      cur ^= 1;
    }
  }

  I32 right = 0;
  cur = 0;
  FOR_R(i, p + 1, n) {
    const I32 need = a[i] ^ x;
    if ((need ^ cur) == 1) {
      ++right;
      cur ^= 1;
    }
  }

  I32 ans = std::max(left, right);
  if (ans % 2 == 1) ++ans;

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
