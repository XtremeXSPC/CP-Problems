#ifndef CP_MODULES_GRAPH_LCA_HPP
#define CP_MODULES_GRAPH_LCA_HPP

#include "_Common.hpp"
#include "Graph.hpp"

/// @brief Lowest Common Ancestor via binary lifting.
struct LCA {
  I32 n, log_n;
  Vec2D<I32> parent;
  VecI32 depth, component;

  /// @brief Builds lifting table from a rooted tree.
  LCA(const Graph<>& g, I32 root = 0) : n(g.n) {
    log_n = 0;
    while ((1 << log_n) < n) log_n++;

    parent.assign(log_n + 1, VecI32(n, -1));
    depth.assign(n, 0);
    component.assign(n, -1);
    VecBool visited(n, false);

    // DFS to set up parent[0], depth and component id.
    std::function<void(I32, I32, I32)> dfs = [&](I32 u, I32 p, I32 comp_id) {
      visited[u] = true;
      parent[0][u] = p;
      component[u] = comp_id;
      for (const auto& e : g.adj[u]) {
        if (e.to != p && !visited[e.to]) {
          depth[e.to] = depth[u] + 1;
          dfs(e.to, u, comp_id);
        }
      }
    };

    if (0 <= root && root < n && !visited[root]) dfs(root, -1, root);
    FOR(i, n) {
      if (!visited[i]) {
        depth[i] = 0;
        dfs(i, -1, i);
      }
    }

    // Binary lifting preprocessing.
    FOR(j, 1, log_n + 1) {
      FOR(i, n) {
        if (parent[j - 1][i] != -1) {
          parent[j][i] = parent[j - 1][parent[j - 1][i]];
        }
      }
    }
  }

  /// @brief Returns LCA of u and v.
  I32 query(I32 u, I32 v) const {
    if (component[u] != component[v]) return -1;
    if (depth[u] < depth[v]) std::swap(u, v);

    // Bring 'u' to the same level as 'v'.
    I32 diff = depth[u] - depth[v];
    FOR(i, log_n + 1) {
      if ((diff >> i) & 1) u = parent[i][u];
    }

    if (u == v) return u;

    // Binary search for LCA.
    FOR_R(i, log_n + 1) {
      if (parent[i][u] != parent[i][v]) {
        u = parent[i][u];
        v = parent[i][v];
      }
    }

    return parent[0][u];
  }

  /// @brief Tree distance in edges between u and v.
  I32 distance(I32 u, I32 v) const {
    I32 w = query(u, v);
    if (w == -1) return -1;
    return depth[u] + depth[v] - 2 * depth[w];
  }
};

#endif
