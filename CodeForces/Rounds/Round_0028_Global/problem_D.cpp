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

using namespace std;

void solve() {
  INT(n, m);

  VecI32 a(as<Size>(n));
  VecI32 b(as<Size>(m));
  FOR(i, n) IN(a[i]);
  FOR(j, m) IN(b[j]);

  const I32 kevin = a[0];
  sort(a.begin(), a.end());

  VecI32 hard;
  hard.reserve(as<Size>(m));
  for (const I32 x : b) {
    if (x > kevin) hard.push_back(x);
  }
  sort(hard.begin(), hard.end());

  const I32 hard_cnt = as<I32>(hard.size());
  VecI32 penalty(as<Size>(hard_cnt));
  FOR (i, hard_cnt) {
    const auto it = lower_bound(a.begin(), a.end(), hard[i]);
    penalty[i] = as<I32>(a.end() - it);
  }

  VecI64 answer(as<Size>(m));
  FOR (k, 1, m + 1) {
    const I32 contests = m / k;
    const I32 discard = m % k;
    I64 total = contests;

    FOR (idx, discard, hard_cnt, k) {
      total += penalty[idx];
    }

    answer[k - 1] = total;
  }

  FOR (i, m) {
    cout << answer[i] << (i + 1 == m ? '\n' : ' ');
  }
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
