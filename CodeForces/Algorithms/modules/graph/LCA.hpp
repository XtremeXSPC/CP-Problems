#ifndef CP_MODULES_GRAPH_LCA_HPP
#define CP_MODULES_GRAPH_LCA_HPP

#include "_Common.hpp"
#include "Graph.hpp"

/**
 * @brief Lowest Common Ancestor via binary lifting.
 */
struct LCA {
  I32 n, log_n;
  VVI parent;
  VI depth;

  /**
   * @brief Builds lifting table from a rooted tree.
   * @param g Input tree.
   * @param root Chosen root.
   */
  LCA(const Graph<>& g, I32 root = 0) : n(g.n) {
    log_n = 0;
    while ((1 << log_n) < n) log_n++;

    parent.assign(log_n + 1, VI(n, -1));
    depth.assign(n, 0);
    VB visited(n, false);

    // DFS to set up parent[0] and depth.
    std::function<void(I32, I32)> dfs = [&](I32 u, I32 p) {
      visited[u] = true;
      parent[0][u] = p;
      for (const auto& e : g.adj[u]) {
        if (e.to != p && !visited[e.to]) {
          depth[e.to] = depth[u] + 1;
          dfs(e.to, u);
        }
      }
    };

    dfs(root, -1);

    // Binary lifting preprocessing.
    FOR(j, 1, log_n + 1) {
      FOR(i, n) {
        if (parent[j - 1][i] != -1) {
          parent[j][i] = parent[j - 1][parent[j - 1][i]];
        }
      }
    }
  }

  /**
   * @brief Returns LCA of u and v.
   */
  I32 query(I32 u, I32 v) {
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

  /**
   * @brief Tree distance in edges between u and v.
   */
  I32 distance(I32 u, I32 v) {
    return depth[u] + depth[v] - 2 * depth[query(u, v)];
  }
};

#endif
