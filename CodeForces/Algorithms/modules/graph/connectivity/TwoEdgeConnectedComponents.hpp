#ifndef CP_MODULES_GRAPH_TWO_EDGE_CONNECTED_COMPONENTS_HPP
#define CP_MODULES_GRAPH_TWO_EDGE_CONNECTED_COMPONENTS_HPP

#include "LowLink.hpp"

/// @brief 2-edge-connected components and the bridge forest.
struct TwoEdgeConnectedComponents {
  LowLink lowlink;
  I32 comp_count = 0;
  VecI32 comp;
  Vec2D<I32> vertices;
  Vec2D<I32> tree;

  TwoEdgeConnectedComponents() = default;

  explicit TwoEdgeConnectedComponents(I32 n) : lowlink(n) {}

  /// @brief Adds one undirected edge before build().
  auto add_edge(I32 u, I32 v) -> I32 { return lowlink.add_edge(u, v); }

  /// @brief Builds components; bridge edges connect different components.
  void build() {
    lowlink.build();
    comp.assign(lowlink.n, -1);
    vertices.clear();
    comp_count = 0;
    FOR(v, lowlink.n) {
      if (comp[v] == -1) {
        vertices.push_back({});
        dfs_component(v, comp_count++);
      }
    }
    tree.assign(comp_count, {});
    FOR(id, lowlink.m) {
      if (!lowlink.is_bridge[id]) continue;
      auto [u, v] = lowlink.edges[id];
      const I32 a = comp[u];
      const I32 b = comp[v];
      tree[a].push_back(b);
      tree[b].push_back(a);
    }
  }

private:
  void dfs_component(I32 root, I32 id) {
    VecI32 st{root};
    comp[root] = id;
    while (!st.empty()) {
      const I32 v = st.back();
      st.pop_back();
      vertices[id].push_back(v);
      for (const auto& e : lowlink.adj[v]) {
        if (lowlink.is_bridge[e.id] || comp[e.to] != -1) continue;
        comp[e.to] = id;
        st.push_back(e.to);
      }
    }
  }
};

#endif
