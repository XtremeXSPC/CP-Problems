#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

using namespace std;

void solve() {
  INT(n);
  LL(h, k);
  Vec<I64> a(n);
  IN(a);

  Vec<I64> pref_sum(n), pref_min(n), suff_max(n);
  pref_sum[0] = a[0];
  pref_min[0] = a[0];
  FOR(i, 1, n) {
    pref_sum[i] = pref_sum[i - 1] + a[i];
    pref_min[i] = min(pref_min[i - 1], a[i]);
  }

  suff_max[n - 1] = a[n - 1];
  FOR_R(i, 0, n - 1) {
    suff_max[i] = max(suff_max[i + 1], a[i]);
  }

  const I64 total = pref_sum[n - 1];
  I64 ans = std::numeric_limits<I64>::max();

  FOR(r, 1, n + 1) {
    const I32 idx = r - 1;
    const I64 prefix_damage = pref_sum[idx];

    I64 gain = 0;
    if (r < n) gain = max<I64>(0, suff_max[r] - pref_min[idx]);

    const I64 best_partial = prefix_damage + gain;
    const I64 full_magazines = (h <= best_partial) ? 0 : (h - best_partial + total - 1) / total;

    const I64 time = full_magazines * (n + k) + r;
    chmin(ans, time);
  }

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
