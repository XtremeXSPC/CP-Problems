#ifndef CP_MODULES_GRAPH_EULER_TOUR_HPP
#define CP_MODULES_GRAPH_EULER_TOUR_HPP

#include "_Common.hpp"
#include "Graph.hpp"

/**
 * @brief Euler Tour decomposition of a rooted tree.
 *
 * @details Iterative DFS to avoid stack overflow on large trees (n up to 10^6).
 * Provides discovery/finish times, DFS order, depths, and parent array.
 */
template <typename Weight = I64>
struct EulerTour {
  I32 n, timer;
  VI tin, tout, order, depth, parent;

  /// @brief Builds Euler Tour from the given tree rooted at root.
  EulerTour(const Graph<Weight>& g, I32 root = 0)
      : n(g.n), timer(0), tin(n, -1), tout(n, -1), depth(n, 0), parent(n, -1) {
    order.reserve(n);
    // Iterative DFS using explicit stack of (vertex, edge_index).
    Stack<PII> stk;
    stk.push({root, 0});
    tin[root] = timer++;
    order.pb(root);

    while (!stk.empty()) {
      auto& [v, ei] = stk.top();
      bool found = false;
      while (ei < sz(g.adj[v])) {
        I32 to = g.adj[v][ei].to;
        ++ei;
        if (to == parent[v]) continue;
        parent[to] = v;
        depth[to] = depth[v] + 1;
        tin[to] = timer++;
        order.pb(to);
        stk.push({to, 0});
        found = true;
        break;
      }
      if (!found) {
        tout[v] = timer;
        stk.pop();
      }
    }
  }

  /// @brief Checks if u is an ancestor of v (inclusive).
  bool is_ancestor(I32 u, I32 v) const {
    return tin[u] <= tin[v] && tout[v] <= tout[u];
  }

  /// @brief Returns half-open subtree range [tin[v], tout[v]) in DFS order.
  PII subtree_range(I32 v) const {
    return {tin[v], tout[v]};
  }
};

#endif
