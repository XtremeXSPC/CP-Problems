#ifndef CP_TEMPLATE_PROFILE_STRICT
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_MACROS
#define NEED_TIMER
#define NEED_CORE
#define NEED_IO
#define NEED_CONSTANTS
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"
#include "modules/data_structures/FenwickTree.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n);

  Vec2D<I32> ADJ(n + 1);
  VecI32 DEG(n + 1, 0);
  FOR(n - 1) {
    INT(u, v);
    ADJ[u].push_back(v);
    ADJ[v].push_back(u);
    ++DEG[u];
    ++DEG[v];
  }

  I32 start = 1;
  FOR(i, 1, n + 1) {
    if (DEG[i] == 1) start = i;
  }

  VecI32 parent(n + 1, 0);
  VecI32 order;
  order.reserve(n);

  VecI32 st{n};
  parent[n] = -1;
  while (!st.empty()) {
    const I32 u = st.back();
    st.pop_back();
    order.push_back(u);

    for (const I32 v : ADJ[u]) {
      if (v == parent[u]) continue;
      parent[v] = u;
      st.push_back(v);
    }
  }

  VecI32 sub_max(n + 1, 0), need(n + 1, 0);
  for (auto it = order.rbegin(); it != order.rend(); ++it) {
    const I32 u = *it;
    I32 mx1 = 0;
    I32 mx2 = 0;
    sub_max[u] = u;

    for (const I32 v : ADJ[u]) {
      if (parent[v] != u) continue;
      sub_max[u] = std::max(sub_max[u], sub_max[v]);
      if (sub_max[v] > mx1) {
        mx2 = mx1;
        mx1 = sub_max[v];
      } else if (sub_max[v] > mx2) {
        mx2 = sub_max[v];
      }
    }

    need[u] = (u == n ? mx2 : mx1);
  }

  FenwickTree<I64> bit(n + 1);
  VecI64 DP(n + 1, 0);
  DP[start] = 1;
  bit.add(start, 1);

  auto pref = [&](I32 idx) -> I64 {
    if (idx <= 0) return 0;
    return bit.sum(idx) % MOD2;
  };

  FOR(i, start + 1, n + 1) {
    const I32 l = need[i] + 1;
    const I32 r = i - 1;
    if (l <= r) {
      DP[i] = (pref(r) - pref(l - 1) + MOD2) % MOD2;
      bit.add(i, DP[i]);
    }
  }

  OUT(DP[n] % MOD2);
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Stopwatch timer;
#endif

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
