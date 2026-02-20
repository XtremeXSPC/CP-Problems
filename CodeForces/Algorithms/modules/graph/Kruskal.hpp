#ifndef CP_MODULES_GRAPH_KRUSKAL_HPP
#define CP_MODULES_GRAPH_KRUSKAL_HPP

#include "_Common.hpp"
#include "DSU.hpp"

/**
 * @brief Kruskal minimum spanning tree.
 *
 * @details Returns total weight and list of edge indices in the MST.
 * Edges should be provided in (u, v, w) format.
 * Complexity: O(E log E).
 */
template <typename Weight = I64>
TP<Weight, Vec<I32>> kruskal_mst(I32 n, Vec<std::tuple<I32, I32, Weight>>& edges) {
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
