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
  INT(n);

#ifdef LOCAL
  STR(hidden);
  VecI32 pref_ones(n + 1, 0);
  FOR(i, n) pref_ones[i + 1] = pref_ones[i] + (hidden[i] == '1');
  F80 cost = 0;
#endif

  auto ask = [&](I32 l, I32 r) -> I32 {
#ifdef LOCAL
    const I32 m = r - l + 1;
    const I32 ones = pref_ones[r] - pref_ones[l - 1];
    cost += as<F80>(n) / m;
    return m / std::gcd(m, ones);
#else
    OUT("?", l, r);
    FLUSH();
    INT(res);
    if (res == -1) std::exit(0);
    return res;
#endif
  };

  VecI32 parent(n + 1), sz_dsu(n + 1, 1);
  std::iota(all(parent), 0);

  auto find = [&](this auto&& self, I32 x) -> I32 {
    if (parent[x] == x) return x;
    return parent[x] = self(parent[x]);
  };

  auto unite = [&](I32 a, I32 b) -> bool {
    a = find(a);
    b = find(b);
    if (a == b) return false;
    if (sz_dsu[a] < sz_dsu[b]) std::swap(a, b);
    parent[b] = a;
    sz_dsu[a] += sz_dsu[b];
    return true;
  };

  VecPairI32 edges;
  edges.reserve(n - 1);
  for (I32 len = n; len >= 2; --len) {
    if (len % 4 != 2) continue;
    FOR(u, n - len + 1) {
      const I32 v = u + len;
      if (!unite(u, v)) continue;
      edges.eb(u, v);
      if (sz(edges) == n - 1) break;
    }
    if (sz(edges) == n - 1) break;
  }

  Vec<VecPairI32> adj(n + 1);
  for (auto [u, v] : edges) {
    const I32 parity = 1 - (ask(u + 1, v) % 2);
    adj[u].eb(v, parity);
    adj[v].eb(u, parity);
  }

  VecI32 pref(n + 1, -1);
  for (I32 root : {0, 1}) {
    pref[root] = 0;
    VecI32 st = {root};
    while (!st.empty()) {
      const I32 u = st.back();
      st.pop_back();
      for (auto [v, w] : adj[u]) {
        if (pref[v] != -1) continue;
        pref[v] = pref[u] ^ w;
        st.eb(v);
      }
    }
  }

  String ans;
  ans.reserve(n);
  FOR(i, 1, n + 1) ans += as<char>('0' + (pref[i] ^ pref[i - 1]));

#ifdef LOCAL
  my_assert(cost <= std::max<F80>(30.0L, 3.0L * n) + 1e-12L);
  if (ans != hidden) {
    for (char& c : ans) c = (c == '0' ? '1' : '0');
  }
  OUT(ans);
#else
  OUT("!", ans);
  FLUSH();
  INT(T);
  if (T == -1) std::exit(0);
  if (T == 1) return;

  for (char& c : ans) c = (c == '0' ? '1' : '0');
  OUT("!", ans);
  FLUSH();
  INT(T2);
  if (T2 == -1) std::exit(0);
#endif
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
