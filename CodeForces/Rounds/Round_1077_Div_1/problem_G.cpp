#define NEED_CORE
#define NEED_FAST_IO

#include "templates/Base.hpp"

using namespace std;

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n);
  VLL a(n);
  IN(a);
  STR(s);

  const I32 KMAX = min<I32>(650, n);

  VLL dp(KMAX + 2, INF64), nxt(KMAX + 2, INF64);
  dp[0] = 0;
  I32 max_k = 0;

  VLL answer;
  answer.reserve(n);

  FOR(i, n) {
    const bool has_key = (s[i] == '1');
    const I32 next_max_k = min<I32>(KMAX, max_k + (has_key ? 1 : 0));

    FOR(k, next_max_k + 1) {
      I64 best = INF64;

      // State after possible key pick-up at room i+1.
      I64 hk = dp[k];
      if (has_key && k > 0) hk = min(hk, dp[k - 1]);

      // Option 1: wait for door i+1 to open naturally.
      if (hk < INF64 / 2) {
        I64 t_wait = max(hk, a[i]) + 1 + 2LL * max<I32>(0, k - 1);
        best = min(best, t_wait);
      }

      // Option 2: open door i+1 with a key.
      if (k + 1 <= next_max_k) {
        I64 hk_plus = dp[k + 1];
        if (has_key) hk_plus = min(hk_plus, dp[k]);
        if (hk_plus < INF64 / 2) {
          I64 t_use = hk_plus + 1 + 2LL * k;
          best = min(best, t_use);
        }
      }

      nxt[k] = best;
    }

    FOR(k, next_max_k + 1, KMAX + 2) nxt[k] = INF64;
    swap(dp, nxt);
    max_k = next_max_k;

    answer.push_back(dp[0]);
  }

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
