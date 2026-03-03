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
  INT(n, m);
  VecI64 a(n), b(m);
  IN(a, b);

  ranges::sort(a);
  ranges::sort(b);

  const I32 max_pairs_a = n / 2;
  const I32 max_pairs_b = m / 2;

  VecI64 pref_a(max_pairs_a + 1, 0);
  VecI64 pref_b(max_pairs_b + 1, 0);

  FOR (i, 1, max_pairs_a + 1) {
    pref_a[i] = pref_a[i - 1] + (a[n - i] - a[i - 1]);
  }

  FOR (i, 1, max_pairs_b + 1) {
    pref_b[i] = pref_b[i - 1] + (b[m - i] - b[i - 1]);
  }

  VecI64 answers;
  answers.reserve((n + m) / 3);

  for (I32 k {1}; ; ++k) {
    I32 left  = max<I32>(0, 2 * k - m);
    I32 right = min<I32>(k, n - k);
    left  = max(left, k - max_pairs_b);
    right = min(right, max_pairs_a);

    if (left > right) break;

    auto score = [&](const I32 x) -> I64 {
      return pref_a[x] + pref_b[k - x];
    };

    I32 lo = left;
    I32 hi = right;
    while (hi - lo > 3) {
      const I32 mid1 = lo + (hi - lo) / 3;
      const I32 mid2 = hi - (hi - lo) / 3;

      if (score(mid1) <= score(mid2)) {
        lo = mid1;
      } else {
        hi = mid2;
      }
    }

    I64 best = 0;
    for (I32 x {lo}; x <= hi; ++x) {
      best = max(best, score(x));
    }
    answers.push_back(best);
  }

  OUT(sz(answers));
  if (!answers.empty()) OUT(answers);
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
