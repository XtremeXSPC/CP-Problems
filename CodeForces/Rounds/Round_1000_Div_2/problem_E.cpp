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

  Vec2D<I32> adj(n + 1);
  FOR(n - 1) {
    INT(u, v);
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  if (n == 1) {
    OUT(0);
    return;
  }

  VecI32 parent(n + 1, 0);
  VecI32 order;
  order.reserve(n);
  VecI32 stack;
  stack.push_back(1);
  parent[1] = -1;

  while (!stack.empty()) {
    const I32 u = stack.back();
    stack.pop_back();
    order.push_back(u);

    for (const I32 v : adj[u]) {
      if (v == parent[u]) continue;
      parent[v] = u;
      stack.push_back(v);
    }
  }

  VecI32 subtree_size(n + 1, 1);
  VecI32 height(n + 1, 0);
  VecI32 heavy_child(n + 1, -1);

  for (I32 idx = as<I32>(order.size()) - 1; idx >= 0; --idx) {
    const I32 u = order[idx];
    I32 best_height = -1;

    subtree_size[u] = 1;
    for (const I32 v : adj[u]) {
      if (v == parent[u]) continue;
      subtree_size[u] += subtree_size[v];

      if (height[v] > best_height) {
        best_height = height[v];
        heavy_child[u] = v;
      }
    }

    height[u] = best_height + 1;
  }

  Vec<Deque<I32>> suffix_counts(n + 1);
  I64 sum_two_min = 0;

  for (I32 idx = as<I32>(order.size()) - 1; idx >= 0; --idx) {
    const I32 u = order[idx];

    if (heavy_child[u] == -1) {
      suffix_counts[u].push_back(1);
      continue;
    }

    suffix_counts[u] = std::move(suffix_counts[heavy_child[u]]);
    suffix_counts[u].push_front(subtree_size[u]);

    for (const I32 v : adj[u]) {
      if (v == parent[u] || v == heavy_child[u]) continue;

      auto& current = suffix_counts[u];
      auto& light   = suffix_counts[v];

      for (I32 j = 0; j < as<I32>(light.size()); ++j) {
        const I64 cnt_new = light[j];
        const I64 cnt_acc = current[j + 1];
        sum_two_min += 2LL * cnt_new * cnt_acc;
        current[j + 1] += light[j];
      }

      Deque<I32>().swap(light);
    }
  }

  const I64 total_pairs = as<I64>(n) * (n - 1) / 2;
  I64 ancestor_pairs = 0;
  FOR(u, 1, n + 1) ancestor_pairs += subtree_size[u] - 1;
  const I64 good_pairs = total_pairs - ancestor_pairs;

  OUT(sum_two_min - good_pairs);
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
