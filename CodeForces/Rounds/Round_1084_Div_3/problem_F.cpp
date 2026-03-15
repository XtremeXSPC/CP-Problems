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
#include "modules/data_structures/SegmentTree.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

using namespace std;

struct SegNode {
  I32 cnt = 0;
  I64 sum = 0;
};

struct SegNodeOp {
  auto operator()(const SegNode& lhs, const SegNode& rhs) const -> SegNode {
    return SegNode{lhs.cnt + rhs.cnt, lhs.sum + rhs.sum};
  }
};

auto query_top_k_dfs(const SegmentTree<SegNode, SegNodeOp>& seg, const I32 node, I32& need, I64& acc) -> void {
  if (need <= 0) return;
  const SegNode cur = seg.tree[node];
  if (cur.cnt == 0) return;

  if (cur.cnt <= need) {
    acc  += cur.sum;
    need -= cur.cnt;
    return;
  }

  if (node >= seg.n) {
    const I64 value = cur.sum / cur.cnt;
    acc += value * as<I64>(need);
    need = 0;
    return;
  }

  query_top_k_dfs(seg, node << 1 | 1, need, acc);
  query_top_k_dfs(seg, node << 1, need, acc);
}

auto query_top_k(const SegmentTree<SegNode, SegNodeOp>& seg, const I32 k) -> I64 {
  if (k <= 0) return 0;
  I32 need = k;
  I64 acc  = 0;
  query_top_k_dfs(seg, 1, need, acc);
  return acc;
}

auto solve() -> void {
  INT(n, m);

  VecPair<I64, I32> base(n);
  VecI64 xs;
  xs.reserve(n);

  FOR(i, 0, n) {
    LL(x);
    INT(y);
    base[i] = {x, y};
    xs.push_back(x);
  }

  VecPair<I64, I32> shop(m);
  FOR(i, 0, m) {
    LL(x);
    INT(y);
    shop[i] = {x, y};
  }

  sort(xs.begin(), xs.end());
  xs.erase(unique(xs.begin(), xs.end()), xs.end());

  SegmentTree<SegNode, SegNodeOp> seg(as<I32>(xs.size()), SegNode{}, SegNodeOp{});
  VecI32 leaf_cnt(as<I32>(xs.size()), 0);
  VecI64 leaf_sum(as<I32>(xs.size()), 0);

  Vec2D<I32> by_y(n + 1);
  for (const auto& [x, y] : base) {
    const I32 idx = as<I32>(lower_bound(xs.begin(), xs.end(), x) - xs.begin());
    by_y[y].push_back(idx);
    ++leaf_cnt[idx];
    leaf_sum[idx] += x;
    seg.update(idx, SegNode{leaf_cnt[idx], leaf_sum[idx]});
  }

  const I64 NEG_INF  = -(1LL << 60);
  I64 best_base_only = 0;
  VecI64 pref(n + 2, NEG_INF);
  pref[0] = NEG_INF;

  FOR(k, 1, n + 2) {
    if (k >= 2) {
      for (const I32 idx : by_y[k - 2]) {
        --leaf_cnt[idx];
        leaf_sum[idx] -= xs[idx];
        seg.update(idx, SegNode{leaf_cnt[idx], leaf_sum[idx]});
      }
    }

    const I32 active = seg.tree[1].cnt;

    I64 base_exact = NEG_INF;
    if (active >= k) base_exact = query_top_k(seg, as<I32>(k));
    best_base_only = max(best_base_only, base_exact);

    I64 with_shop_exact = NEG_INF;
    if (active >= k - 1) with_shop_exact = query_top_k(seg, as<I32>(k - 1));

    pref[k] = max(pref[k - 1], with_shop_exact);
  }

  VecI64 answer(m);
  FOR(i, 0, m) {
    const auto [x, y] = shop[i];
    const I32 kmax = min<I32>(n + 1, y + 1);
    answer[i] = max(best_base_only, x + pref[kmax]);
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
