#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n);
  VEC(I32, a, n);

  constexpr I32 INF = std::numeric_limits<I32>::max() / 4;
  std::array<I32, 7> dp{};
  std::array<I32, 7> ndp{};

  FOR(v, 1, 7) dp[v] = (a[0] != v);

  FOR(i, 1, n) {
    FOR(v, 1, 7) ndp[v] = INF;

    FOR(cur, 1, 7) {
      const I32 cost = (a[i] != cur);
      FOR(prev, 1, 7) {
        if (cur == prev || cur == 7 - prev) continue;
        ndp[cur] = std::min(ndp[cur], dp[prev] + cost);
      }
    }

    dp = ndp;
  }

  I32 ans = INF;
  FOR(v, 1, 7) ans = std::min(ans, dp[v]);
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
