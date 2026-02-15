#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n, q);

  Vec<I32> l(n + 1), r(n + 1), par(n + 1, 0);
  FOR(i, 1, n + 1) {
    IN(l[i], r[i]);
    if (l[i] != 0) par[l[i]] = i;
    if (r[i] != 0) par[r[i]] = i;
  }

  Vec<I32> order;
  order.reserve(n);
  Vec<I32> st;
  st.reserve(n);
  st.pb(1);

  while (!st.empty()) {
    const I32 u = st.back();
    st.pop_back();
    order.pb(u);
    if (l[u] != 0) {
      st.pb(l[u]);
      st.pb(r[u]);
    }
  }

  Vec<I32> sub(n + 1, 1);
  FOR_R(i, sz(order)) {
    const I32 u = order[i];
    if (l[u] != 0) {
      sub[u] = 1 + sub[l[u]] + sub[r[u]];
    }
  }

  Vec<I64> d(n + 1, 1);
  FOR(i, 1, n + 1) {
    d[i] = (I64)2 * (sub[i] - 1) + 1;
  }

  Vec<I32> tin(n + 1, -1), e;
  e.reserve((I64)2 * n - 1);

  Vec<std::pair<I32, I32>> dfs_st;
  dfs_st.reserve(n);
  dfs_st.pb({1, 0});

  while (!dfs_st.empty()) {
    auto& [u, state] = dfs_st.back();
    const bool leaf = (l[u] == 0);

    if (state == 0) {
      if (tin[u] == -1) tin[u] = sz(e);
      e.pb(u);
      if (leaf) {
        dfs_st.pop_back();
      } else {
        state = 1;
        dfs_st.pb({l[u], 0});
      }
    } else if (state == 1) {
      e.pb(u);
      state = 2;
      dfs_st.pb({r[u], 0});
    } else {
      e.pb(u);
      dfs_st.pop_back();
    }
  }

  I32 lg = 1;
  while ((1LL << lg) <= n) ++lg;

  Vec<Vec<I32>> up(lg, Vec<I32>(n + 1, 0));
  Vec<Vec<I64>> jump_sum(lg, Vec<I64>(n + 1, 0));

  FOR(u, 1, n + 1) {
    up[0][u] = par[u];
    jump_sum[0][u] = d[u];
  }

  FOR(j, 1, lg) {
    FOR(u, 1, n + 1) {
      const I32 mid = up[j - 1][u];
      up[j][u] = up[j - 1][mid];
      jump_sum[j][u] = jump_sum[j - 1][u] + jump_sum[j - 1][mid];
    }
  }

  Vec<I32> ans(q);
  FOR(i, q) {
    I32 v;
    I64 k;
    IN(v, k);

    I32 cur = v;
    I64 rem = k;

    for (I32 j = lg - 1; j >= 0; --j) {
      const I32 nxt = up[j][cur];
      if (nxt != 0 && jump_sum[j][cur] <= rem) {
        rem -= jump_sum[j][cur];
        cur = nxt;
      }
    }

    ans[i] = e[tin[cur] + rem];
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
