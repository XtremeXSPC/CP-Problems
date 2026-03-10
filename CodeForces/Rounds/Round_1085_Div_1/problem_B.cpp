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

auto solve() -> void {
  LL(n, m, l);
  VecI64 a(n);
  IN(a);

  const I64 last_flash = a.back();
  const I64 initial_sz = min<I64>(m, n + 1);

  MultiSet<I64> cur;
  FOR(_, 0, initial_sz) cur.insert(0);

  I64 idx = 0;
  FOR(t, 1, last_flash + 1) {
    auto it_min = cur.begin();
    const I64 boosted = *it_min + 1;
    cur.erase(it_min);
    cur.insert(boosted);

    if (idx < n && t == a[idx]) {
      auto it_max = prev(cur.end());
      cur.erase(it_max);

      ++idx;
      const I64 remaining_slots = n - idx + 1;
      const I64 target_sz = min<I64>(m, remaining_slots);
      if ((I64)cur.size() < target_sz) cur.insert(0);
    }
  }

  const I64 answer = *cur.rbegin() + (l - last_flash);
  OUT(answer);
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
