#ifndef CP_MODULES_GRAPH_LCA_RMQ_HPP
#define CP_MODULES_GRAPH_LCA_RMQ_HPP

#include "_Common.hpp"
#include "Graph.hpp"

/**
 * @brief O(1) Lowest Common Ancestor via Euler Tour + Sparse Table RMQ.
 * @tparam Weight Graph edge weight type.
 *
 * Builds in O(n log n), answers LCA queries in O(1).
 * Preferred over binary-lifting LCA when query count greatly exceeds n.
 */
template <typename Weight = I64>
struct LCA_RMQ {
  I32 n;
  VI first_occ;
  VI euler;
  VI euler_dep;
  VI depth;
  VI parent;
  VI component;
  Vec2<I32> sparse;
  VI lg;

  LCA_RMQ(const Graph<Weight>& g, I32 root = 0)
      : n(g.n), first_occ(n, -1), depth(n, 0), parent(n, -1), component(n, -1) {
    if (n == 0) return;
    euler.reserve(2 * n);
    euler_dep.reserve(2 * n);

    // Iterative DFS building full Euler tour for each connected component.
    VB visited(n, false);
    auto build_component = [&](I32 start) {
      Stack<Pair<I32, I32>> stk;
      stk.push({start, 0});
      while (!stk.empty()) {
        auto [v, ei] = stk.top();
        stk.pop();

        if (!visited[v]) {
          visited[v] = true;
          component[v] = start;
        }

        euler.push_back(v);
        euler_dep.push_back(depth[v]);
        if (first_occ[v] == -1) first_occ[v] = sz(euler) - 1;

        while (ei < sz(g.adj[v])) {
          I32 to = g.adj[v][ei].to;
          ++ei;
          if (to == parent[v] || visited[to]) continue;
          parent[to] = v;
          depth[to] = depth[v] + 1;
          component[to] = start;
          stk.push({v, ei});
          stk.push({to, 0});
          break;
        }
      }
    };

    if (0 <= root && root < n && !visited[root]) build_component(root);
    FOR(i, n) {
      if (!visited[i]) {
        depth[i] = 0;
        build_component(i);
      }
    }

    // Build sparse table on euler_dep for range-minimum-index queries.
    I32 m = sz(euler);
    I32 max_log = 1;
    while ((1 << max_log) <= m) ++max_log;

    lg.assign(m + 1, 0);
    FOR(i, 2, m + 1) lg[i] = lg[i / 2] + 1;

    sparse.assign(max_log, VI(m));
    FOR(i, m) sparse[0][i] = i;

    FOR(k, 1, max_log) {
      FOR(i, m - (1 << k) + 1) {
        I32 a = sparse[k - 1][i];
        I32 b = sparse[k - 1][i + (1 << (k - 1))];
        sparse[k][i] = (euler_dep[a] <= euler_dep[b]) ? a : b;
      }
    }
  }

  /// @brief LCA query in O(1).
  I32 query(I32 u, I32 v) const {
    if (component[u] != component[v]) return -1;
    I32 l = first_occ[u], r = first_occ[v];
    if (l > r) std::swap(l, r);
    I32 k = lg[r - l + 1];
    I32 a = sparse[k][l], b = sparse[k][r - (1 << k) + 1];
    return euler[euler_dep[a] <= euler_dep[b] ? a : b];
  }

  /// @brief Tree distance in edges between u and v.
  I32 distance(I32 u, I32 v) const {
    I32 w = query(u, v);
    if (w == -1) return -1;
    return depth[u] + depth[v] - 2 * depth[w];
  }
};

#endif
