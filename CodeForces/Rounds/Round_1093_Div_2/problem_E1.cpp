#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_MATH
#define NEED_TIMER
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"
#include "templates/Constants.hpp"

/* THIS SOLUTION REACHES A TIME LIMIT EXCEEDED */

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(N);
  STR(S);

  Vec2D<I32> adj(N + 1), black_adj(N + 1);
  FOR(_, N - 1) {
    INT(U, V);
    adj[U].eb(V);
    adj[V].eb(U);
  }

  VecBool is_black(N + 1, false);
  FOR(U, 1, N + 1) is_black[U] = (S[U - 1] == '0');

  VecI32 base_red(N + 1, 0);
  I32 black_cnt = 0;
  FOR(U, 1, N + 1) {
    if (!is_black[U]) continue;
    ++black_cnt;
    for (I32 V : adj[U]) {
      if (is_black[V]) {
        black_adj[U].eb(V);
      } else {
        ++base_red[U];
      }
    }
  }

  if (black_cnt == 0) {
    cout << 0.0L << '\n';
    return;
  }

  VecI32 parent(N + 1, -1), roots, order;
  order.reserve(black_cnt);
  FOR(U, 1, N + 1) {
    if (!is_black[U] || parent[U] != -1) continue;
    roots.eb(U);
    parent[U] = 0;
    VecI32 stk;
    stk.eb(U);
    while (!stk.empty()) {
      const I32 X = stk.back();
      stk.pop_back();
      order.eb(X);
      for (I32 Y : black_adj[X]) {
        if (Y == parent[X]) continue;
        parent[Y] = X;
        stk.eb(Y);
      }
    }
  }

  Vec<Array<F80, 2>> dp(N + 1, {INF64, INF64});
  FOR_R(I, sz(order)) {
    const I32 U = order[I];
    Vec<F80> cur(1, 0.0L);
    for (I32 V : black_adj[U]) {
      if (V == parent[U]) continue;
      Vec<F80> nxt(cur.size() + 1, INF64);
      FOR(J, sz(cur)) {
        chmin(nxt[J], cur[J] + dp[V][1]);
        chmin(nxt[J + 1], cur[J] + dp[V][0]);
      }
      cur.swap(nxt);
    }

    const F80 deg = as<F80>(sz(adj[U]));
    FOR(K, sz(cur)) {
      if (base_red[U] + K > 0) {
        chmin(dp[U][0], cur[K] + deg / as<F80>(base_red[U] + K));
      }
      chmin(dp[U][1], cur[K] + deg / as<F80>(base_red[U] + 1 + K));
    }
  }

  F80 ans = 0.0L;
  for (I32 U : roots) ans += dp[U][0];
  cout << ans << '\n';
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  cout << fixed << setprecision(15);

  I32 T;
  if (!(cin >> T)) return 0;
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
