#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n);

  Vec<I32> l(n + 1), r(n + 1);
  FOR(i, 1, n + 1) {
    IN(l[i], r[i]);
  }

  Vec<I32> order;
  order.reserve(n);
  Vec<I32> st;
  st.reserve(n);
  st.pb(1);
  while (!st.empty()) {
    const I32 v = st.back();
    st.pop_back();
    order.pb(v);
    if (l[v] != 0) st.pb(l[v]);
    if (r[v] != 0) st.pb(r[v]);
  }

  Vec<I64> up(n + 1, 0), tau(n + 1, 0);

  FOR_R(i, sz(order)) {
    const I32 v = order[i];
    if (l[v] == 0) {
      up[v] = 1;
    } else {
      up[v] = (up[l[v]] + up[r[v]] + 3) % MOD;
    }
  }

  tau[1] = up[1];
  st.clear();
  st.pb(1);
  while (!st.empty()) {
    const I32 v = st.back();
    st.pop_back();

    if (l[v] != 0) {
      tau[l[v]] = (tau[v] + up[l[v]]) % MOD;
      st.pb(l[v]);
    }
    if (r[v] != 0) {
      tau[r[v]] = (tau[v] + up[r[v]]) % MOD;
      st.pb(r[v]);
    }
  }

  Vec<I64> ans(n);
  FOR(i, n) ans[i] = tau[i + 1];
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
