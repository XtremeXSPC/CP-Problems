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

  VecI32 p(n), d(n);
  IN(p);
  IN(d);

  VecI32 pos(n + 1);
  FOR(i, n) pos[p[i]] = i;

  VecI32 ord;
  ord.reserve(n);
  FOR_R(val, 1, n + 1) ord.eb(pos[val]);

  VecI32 seq;
  seq.reserve(n);

  for (I32 i : ord) {
    const I32 need = d[i];
    I32 cnt = 0;
    I32 at  = sz(seq);

    for (I32 k = sz(seq) - 1; k >= 0; --k) {
      if (cnt == need) break;
      if (seq[k] > i) ++cnt;
      at = k;
    }

    if (cnt < need) {
      OUT(-1);
      return;
    }

    seq.insert(seq.begin() + at, i);
  }

  VecI32 q(n);
  FOR(i, n) q[seq[i]] = i + 1;
  OUT(q);
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
