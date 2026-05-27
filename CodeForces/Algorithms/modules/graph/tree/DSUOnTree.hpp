#ifndef CP_MODULES_GRAPH_TREE_DSU_ON_TREE_HPP
#define CP_MODULES_GRAPH_TREE_DSU_ON_TREE_HPP

#include "../_Common.hpp"

/// @brief Small-to-large subtree traversal helper for offline subtree queries.
///
/// `add(v)` inserts one vertex, `query(v)` answers for the current subtree of v,
/// and `clear()` resets all inserted state after a light subtree.
template <typename Add, typename Query, typename Clear>
void dsu_on_tree(const Vec2D<I32>& tree, I32 root, Add add, Query query, Clear clear) {
  const I32 n = isz(tree);
  VecI32 parent(n, -1), sub(n, 1), heavy(n, -1), order;
  order.reserve(n);

  VecI32 st{root};
  parent[root] = root;
  while (!st.empty()) {
    const I32 v = st.back();
    st.pop_back();
    order.push_back(v);
    for (const I32 to : tree[v]) {
      if (to == parent[v]) continue;
      parent[to] = v;
      st.push_back(to);
    }
  }
  FOR_R(i, isz(order)) {
    const I32 v = order[i];
    I32 best_size = 0;
    for (const I32 to : tree[v]) {
      if (to == parent[v]) continue;
      sub[v] += sub[to];
      if (sub[to] > best_size) {
        best_size = sub[to];
        heavy[v] = to;
      }
    }
  }

  auto add_subtree = [&](I32 start) {
    VecI32 stack{start};
    while (!stack.empty()) {
      const I32 v = stack.back();
      stack.pop_back();
      add(v);
      for (const I32 to : tree[v]) {
        if (to != parent[v]) stack.push_back(to);
      }
    }
  };

  auto dfs = [&](auto&& self, I32 v, bool keep) -> void {
    for (const I32 to : tree[v]) {
      if (to != parent[v] && to != heavy[v]) self(self, to, false);
    }
    if (heavy[v] != -1) self(self, heavy[v], true);
    for (const I32 to : tree[v]) {
      if (to != parent[v] && to != heavy[v]) add_subtree(to);
    }
    add(v);
    query(v);
    if (!keep) clear();
  };

  dfs(dfs, root, true);
}

#endif
