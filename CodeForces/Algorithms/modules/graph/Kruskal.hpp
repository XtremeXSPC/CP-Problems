#ifndef CP_MODULES_GRAPH_KRUSKAL_HPP
#define CP_MODULES_GRAPH_KRUSKAL_HPP

#include "_Common.hpp"
#include "DSU.hpp"

/**
 * @brief Kruskal minimum spanning tree.
 * @tparam Weight Edge weight type.
 * @param n Number of vertices.
 * @param edges Edge list as (u, v, w).
 * @return Pair {total_weight, indices_of_edges_used_from_input_array}.
 *
 * Complexity: O(E log E).
 */
template <typename Weight = I64>
P<Weight, Vec<I32>> kruskal_mst(I32 n, Vec<std::tuple<I32, I32, Weight>>& edges) {
  std::sort(all(edges), [](const auto& a, const auto& b) {
    return std::get<2>(a) < std::get<2>(b);
  });

  DSU dsu(n);
  Weight total_weight = 0;
  Vec<I32> mst_edges;

  FOR(i, sz(edges)) {
    auto [u, v, w] = edges[i];
    if (dsu.unite(u, v)) {
      total_weight += w;
      mst_edges.pb(i);
    }
  }

  return {total_weight, mst_edges};
}

#endif
