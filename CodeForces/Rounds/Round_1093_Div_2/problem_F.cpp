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
#include "modules/data_structures/FenwickTree.hpp"
#include "modules/data_structures/SegmentTree.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

struct MinOp {
  auto operator()(const I32 A, const I32 B) const -> I32 { return min(A, B); }
};

struct MexTree {
  I32 lim;
  SegmentTree<I32, MinOp> seg;

  explicit MexTree(I32 size)
    : lim(size),
      seg(VecI32(size, 0), Limits<I32>::max(), MinOp{}) {}

  void set(I32 X, I32 val) { seg.update(X, val); }

  auto first_zero_dfs(I32 node, I32 L, I32 R, I32 start) const -> I32 {
    if (R <= start || L >= lim || seg.tree[node] > 0) return -1;
    if (node >= seg.n) return L;

    const I32 mid  = (L + R) / 2;
    const I32 left = first_zero_dfs(node * 2, L, mid, start);
    if (left != -1) return left;
    return first_zero_dfs(node * 2 + 1, mid, R, start);
  }

  auto first_zero(I32 start) const -> I32 {
    const I32 Res = first_zero_dfs(1, 0, seg.n, start);
    my_assert(Res != -1);
    return Res;
  }
};

void solve() {
  INT(N);

  VecI32 P(N + 1);
  FOR(U, 1, N + 1) IN(P[U]);

  Vec2D<I32> adj(N + 1);
  FOR(_, N - 1) {
    INT(U, V);
    adj[U].eb(V);
    adj[V].eb(U);
  }

  VecI32 parent(N + 1, 0), tin(N + 1), tout(N + 1), F(N + 1), G(N + 1);
  VecI32 ord;
  ord.reserve(N);

  MexTree mex_tree(N + 2);
  Vec<Array<I32, 3>> stk = {{1, 0, 0}};
  I32 timer = 0;
  I64 base  = 0;

  while (!stk.empty()) {
    const auto [U, Par, typ] = stk.back();
    stk.pop_back();

    if (typ == 0) {
      parent[U] = Par;
      mex_tree.set(P[U], 1);

      F[U] = mex_tree.first_zero(0);
      G[U] = mex_tree.first_zero(F[U] + 1);
      tin[U] = timer++;
      ord.eb(U);
      base += F[U];

      stk.eb(Array<I32, 3>{U, Par, 1});
      for (I32 I = sz(adj[U]) - 1; I >= 0; --I) {
        const I32 V = adj[U][I];
        if (V == Par) continue;
        stk.eb(Array<I32, 3>{V, U, 0});
      }
    } else {
      tout[U] = timer - 1;
      mex_tree.set(P[U], 0);
    }
  }

  Vec<VecI32> grp(N + 1);
  FOR(U, 1, N + 1) grp[F[U]].eb(U);

  VecI64 delta(N + 1, 0);
  FenwickTree<I64> bit_cnt(N), bit_sum(N);

  FOR(K, N + 1) {
    auto& nodes = grp[K];
    if (nodes.empty()) continue;

    VecI32 pos;
    pos.reserve(sz(nodes));
    for (I32 U : nodes) pos.eb(tin[U]);
    ranges::sort(pos);

    VecI32 by_g = nodes;
    ranges::sort(by_g, [&](const I32 A, const I32 B) {
      return G[A] < G[B];
    });

    VecI32 by_p = nodes;
    ranges::sort(by_p, [&](const I32 A, const I32 B) {
      return P[A] < P[B];
    });

    I32 ptr = 0;
    for (I32 V : by_p) {
      while (ptr < sz(by_g) && G[by_g[ptr]] <= P[V]) {
        const I32 U = by_g[ptr++];
        bit_cnt.add(tin[U], 1);
        bit_sum.add(tin[U], G[U]);
      }

      const I32 total = as<I32>(
        ranges::upper_bound(pos, tout[V]) - ranges::lower_bound(pos, tin[V])
      );
      const I64 cnt_small = bit_cnt.range_sum(tin[V], tout[V]);
      const I64 sum_small = bit_sum.range_sum(tin[V], tout[V]);

      delta[V] += sum_small
        + (as<I64>(total) - cnt_small) * P[V]
        - as<I64>(total) * F[V];
    }

    FOR(I, ptr) {
      const I32 U = by_g[I];
      bit_cnt.add(tin[U], -1);
      bit_sum.add(tin[U], -G[U]);
    }
  }

  VecI32 nodes = ord;
  ranges::sort(nodes, [&](const I32 A, const I32 B) {
    return F[A] > F[B];
  });

  VecI32 queries = ord;
  ranges::sort(queries, [&](const I32 A, const I32 B) {
    return max(F[A], P[A]) > max(F[B], P[B]);
  });

  FenwickTree<I64> bit_cnt_hi(N), bit_sum_hi(N);
  I32 ptr = 0;
  for (I32 V : queries) {
    const I32 need = max(F[V], P[V]);
    while (ptr < N && F[nodes[ptr]] > need) {
      const I32 U = nodes[ptr++];
      bit_cnt_hi.add(tin[U], 1);
      bit_sum_hi.add(tin[U], F[U]);
    }

    const I64 cnt_hi = bit_cnt_hi.range_sum(tin[V], tout[V]);
    const I64 sum_hi = bit_sum_hi.range_sum(tin[V], tout[V]);
    delta[V] -= sum_hi - cnt_hi * P[V];
  }

  I64 ans = base;
  FOR(U, 1, N + 1) ans = max(ans, base + delta[U]);
  cout << ans << '\n';
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  I32 T;
  if (!(cin >> T)) return 0;
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
