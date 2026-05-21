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
  INT(n, d);
  I32 D = d - 1;

  Vec2D<I32> adj(n + 1);
  FOR(n - 1) {
    INT(u, v);
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  VecI32 par(n + 1), order;
  order.reserve(n);
  VecI32 st = {1};
  par[1] = -1;
  while (!st.empty()) {
    I32 u = st.back();
    st.pop_back();
    order.push_back(u);
    for (I32 v : adj[u]) {
      if (v == par[u]) continue;
      par[v] = u;
      st.push_back(v);
    }
  }

  Vec2D<I64> in(n + 1, VecI64(D + 1));
  Vec2D<I64> out(n + 1, VecI64(D + 1));

  for (auto it = order.rbegin(); it != order.rend(); ++it) {
    I32 u = *it;
    in[u][0] = 1;
    for (I32 v : adj[u]) {
      if (v == par[u]) continue;
      FOR(dist, 1, D + 1) in[u][dist] += in[v][dist - 1];
    }
  }

  for (I32 u : order) {
    for (I32 v : adj[u]) {
      if (v == par[u]) continue;
      FOR(dist, 1, D + 1) {
        out[v][dist] = out[u][dist - 1] + in[u][dist - 1];
        if (dist >= 2) out[v][dist] -= in[v][dist - 2];
      }
    }
  }

  I64 ans = 0;
  VecI64 e1(D + 1), e2(D + 1), e3(D + 1);

  FOR(u, 1, n + 1) {
    fill(all(e1), 0);
    fill(all(e2), 0);
    fill(all(e3), 0);

    for (I32 v : adj[u]) {
      if (v == par[u]) continue;

      Vec<PairI32> cur;
      FOR(dist, 1, D + 1) {
        I64 cnt = in[v][dist - 1];
        if (cnt > 0) cur.emplace_back(dist, cnt);
      }

      FOR(sum, 1, D + 1) {
        if (e2[sum] == 0) continue;
        for (auto [dist, cnt] : cur) {
          if (sum + dist > D) break;
          e3[sum + dist] += e2[sum] * cnt;
        }
      }

      FOR(sum, 1, D + 1) {
        if (e1[sum] == 0) continue;
        for (auto [dist, cnt] : cur) {
          if (sum + dist > D) break;
          e2[sum + dist] += e1[sum] * cnt;
        }
      }

      for (auto [dist, cnt] : cur) e1[dist] += cnt;
    }

    ans += e3[D];
    ans += e2[D];

    FOR(sum, 1, D) {
      I64 up = out[u][D - sum];
      ans += e2[sum] * up;
      ans += e1[sum] * up;
    }
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
