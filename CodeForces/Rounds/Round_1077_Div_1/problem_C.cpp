#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n, m);

  VecI32 parent(n, 0);
  FOR(i, n - 1) parent[i] = i + 1;

  FOR(m) {
    INT(u, v);
    --u;
    --v;
    parent[u] = std::max(parent[u], v);
  }

  Vec2D<I32> tree(n);
  FOR(i, n - 1) {
    tree[parent[i]].push_back(i);
  }

  const I32 root = n - 1;

  VecI32 depth(n, 0);
  VecI32 stack;
  stack.push_back(root);
  while (!stack.empty()) {
    const I32 u = stack.back();
    stack.pop_back();
    for (const I32 v : tree[u]) {
      depth[v] = depth[u] + 1;
      stack.push_back(v);
    }
  }

  VecI32 subtree_size(n, 1);
  Vec<Map<I32, I32>> depth_count(n);
  I64 ans = 0;

  FOR(u, n) {
    I32 best_child = -1;
    Size best_size = 0;

    for (const I32 v : tree[u]) {
      subtree_size[u] += subtree_size[v];
      if (depth_count[v].size() > best_size) {
        best_size = depth_count[v].size();
        best_child = v;
      }
    }

    if (best_child != -1) depth_count[u].swap(depth_count[best_child]);

    for (const I32 v : tree[u]) {
      if (v == best_child) continue;
      for (const auto& [d, cnt] : depth_count[v]) {
        ans += 1LL * cnt * depth_count[u][d] * (d - depth[u]);
        depth_count[u][d] += cnt;
      }
    }

    depth_count[u][depth[u]] += 1;
  }

  FOR(u, n) {
    I64 pairs_with_lca_u = 1LL * subtree_size[u] * (subtree_size[u] - 1) / 2;
    for (const I32 v : tree[u]) {
      pairs_with_lca_u -= 1LL * subtree_size[v] * (subtree_size[v] - 1) / 2;
    }
    ans -= 1LL * depth[u] * pairs_with_lca_u;
  }

  VecI32 sorted_depth = depth;
  std::ranges::sort(sorted_depth);
  FOR(i, n) ans += 1LL * sorted_depth[i] * (n - i - 1);

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
