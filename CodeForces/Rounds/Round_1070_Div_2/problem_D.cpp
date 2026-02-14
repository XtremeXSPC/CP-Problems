#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

const I64 MAXVAL = I64(1e18);

void solve() {
  INT(n, m);
  VEC(I64, a, n);

  Vec<std::unordered_map<I64, VI>> adj(n);
  VPII edges(m);
  FOR(i, m) {
    INT(u, v); u--; v--;
    edges[i] = {u, v};
    adj[u][a[v]].pb(v);
  }

  Vec<std::unordered_map<I64, I64>> dp(n);

  auto ext = [&](auto& self, I32 v, I64 need) -> I64 {
    if (need > MAXVAL) return 0;
    auto it = dp[v].find(need);
    if (it != dp[v].end()) return it->second;
    auto jt = adj[v].find(need);
    if (jt == adj[v].end()) return dp[v][need] = 0;
    I64 res = 0;
    for (I32 w : jt->second)
      res = (res + 1 + self(self, w, a[v] + need)) % MOD2;
    return dp[v][need] = res;
  };

  I64 ans = 0;
  for (auto [u, v] : edges)
    ans = (ans + 1 + ext(ext, v, a[u] + a[v])) % MOD2;

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
