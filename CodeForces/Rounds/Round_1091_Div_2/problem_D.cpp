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

using namespace std;

void solve() {
  INT(n, k);

  VecI32 a(n);
  IN(a);

  VecI32 p(k);
  IN(p);

  const I32 x = a[p[0] - 1];

  I32 sum   = 0;
  I32 mx    = 0;
  I32 cur   = 0;
  I32 prev  = 0;
  I32 ptr   = 0;

  FOR(i, n) {
    const I32 need = a[i] ^ x;
    cur += prev ^ need;
    prev = need;

    if (ptr < k && p[ptr] == i + 1) {
      sum += cur;
      mx  = max(mx, cur);
      cur = 0;
      ++ptr;
    }
  }

  cur += prev;
  sum += cur;
  mx = max(mx, cur);

  const I32 ans = max(sum / 2, mx);
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
