#ifndef CP_MODULES_GRAPH_CENTROID_DECOMPOSITION_HPP
#define CP_MODULES_GRAPH_CENTROID_DECOMPOSITION_HPP

#include "_Common.hpp"
#include "Graph.hpp"

/**
 * @brief Centroid decomposition of a tree/forest.
 * @tparam Weight Graph edge weight type.
 *
 * Builds centroid-parent tree in @c parent.
 */
template <typename Weight = I64>
struct CentroidDecomposition {
  I32 n;
  Vec<Vec<I32>> tree;
  VI parent, subtree_size;
  VB used;

  /// @brief Constructs centroid decomposition for each connected component.
  explicit CentroidDecomposition(const Graph<Weight>& g)
      : n(g.n), tree(n), parent(n, -1), subtree_size(n, 0), used(n, false) {
    FOR(u, n) {
      for (const auto& e : g.adj[u]) {
        tree[u].push_back(e.to);
      }
    }

    FOR(i, n) {
      if (!used[i]) build(i, -1);
    }
  }

private:
  I32 calc_subtree_size(I32 v, I32 p) {
    subtree_size[v] = 1;
    for (I32 to : tree[v]) {
      if (to == p || used[to]) continue;
      subtree_size[v] += calc_subtree_size(to, v);
    }
    return subtree_size[v];
  }

  I32 find_centroid(I32 v, I32 p, I32 total_size) {
    for (I32 to : tree[v]) {
      if (to == p || used[to]) continue;
      if (subtree_size[to] > total_size / 2) {
        return find_centroid(to, v, total_size);
      }
    }
    return v;
  }

  I32 build(I32 entry, I32 p) {
    I32 total_size = calc_subtree_size(entry, -1);
    I32 centroid = find_centroid(entry, -1, total_size);
    used[centroid] = true;
    parent[centroid] = p;

    for (I32 to : tree[centroid]) {
      if (used[to]) continue;
      build(to, centroid);
    }
    return centroid;
  }
};

#endif
