#ifndef CP_MODULES_GRAPH_LOW_LINK_HPP
#define CP_MODULES_GRAPH_LOW_LINK_HPP

#include "../_Common.hpp"

/// @brief Low-link decomposition for undirected graphs.
struct LowLink {
  struct Edge {
    I32 to = 0;
    I32 id = -1;
  };

  I32 n = 0;
  I32 m = 0;
  Vec2D<Edge> adj;
  Vec<PairI32> edges;
  VecI32 ord;
  VecI32 low;
  Vec<char> is_articulation;
  Vec<char> is_bridge;
  Vec<PairI32> bridges;

  LowLink() = default;

  explicit LowLink(I32 n_) { build_empty(n_); }

  /// @brief Initializes an empty graph with @p n_ vertices.
  void build_empty(I32 n_) {
    my_assert(n_ >= 0);
    n = n_;
    m = 0;
    adj.assign(n, {});
    edges.clear();
    clear_result();
  }

  /// @brief Adds one undirected edge and returns its edge id.
  auto add_edge(I32 u, I32 v) -> I32 {
    my_assert(0 <= u && u < n);
    my_assert(0 <= v && v < n);
    const I32 id = m++;
    adj[u].push_back({v, id});
    adj[v].push_back({u, id});
    edges.push_back({u, v});
    return id;
  }

  /// @brief Computes discovery order, low-link values, articulation points, and bridges.
  void build() {
    clear_result();
    ord.assign(n, -1);
    low.assign(n, -1);
    is_articulation.assign(n, false);
    is_bridge.assign(m, false);
    I32 timer = 0;
    FOR(v, n) {
      if (ord[v] == -1) dfs(v, -1, timer);
    }
  }

private:
  void clear_result() {
    ord.clear();
    low.clear();
    is_articulation.clear();
    is_bridge.clear();
    bridges.clear();
  }

  void dfs(I32 v, I32 parent_edge, I32& timer) {
    ord[v] = low[v] = timer++;
    I32 child_count = 0;
    for (const Edge& e : adj[v]) {
      if (e.id == parent_edge) continue;
      const I32 to = e.to;
      if (ord[to] == -1) {
        ++child_count;
        dfs(to, e.id, timer);
        low[v] = std::min(low[v], low[to]);
        if (parent_edge != -1 && low[to] >= ord[v]) is_articulation[v] = true;
        if (low[to] > ord[v]) {
          is_bridge[e.id] = true;
          bridges.push_back(edges[e.id]);
        }
      } else {
        low[v] = std::min(low[v], ord[to]);
      }
    }
    if (parent_edge == -1 && child_count >= 2) is_articulation[v] = true;
  }
};

#endif
