#ifndef CP_MODULES_GRAPH_PRIM_HPP
#define CP_MODULES_GRAPH_PRIM_HPP

#include "_Common.hpp"
#include "Graph.hpp"

/**
 * @brief Prim minimum spanning forest for undirected weighted graph.
 *
 * @details Returns total weight and selected edges (u, v) for all components.
 * For connected graphs, selected edge count is n-1.
 * @Complexity: O((V + E) log V).
 */
template <typename Weight = I64>
Pair<Weight, Vec<PairI32>> prim_mst(const Graph<Weight>& g) {
  Weight total = 0;
  Vec<PairI32> used_edges;
  if (g.n == 0) return {total, used_edges};

  VecBool used(as<Size>(g.n), false);

  using State = std::tuple<Weight, I32, I32>;  // w, to, parent
  MinPriorityQueue<State> pq;

  FOR(start, g.n) {
    if (used[as<Size>(start)]) continue;
    pq.push({0, start, -1});

    while (!pq.empty()) {
      auto [w, v, p] = pq.top();
      pq.pop();
      if (used[as<Size>(v)]) continue;

      used[as<Size>(v)] = true;
      if (p != -1) {
        total += w;
        used_edges.push_back({p, v});
      }

      for (const auto& e : g.adj[as<Size>(v)]) {
        if (!used[as<Size>(e.to)]) {
          pq.push({e.weight, e.to, v});
        }
      }
    }
  }

  return {total, used_edges};
}

#endif
