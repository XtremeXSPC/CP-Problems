#ifndef CP_MODULES_GRAPH_DIJKSTRA_SPARSE_HPP
#define CP_MODULES_GRAPH_DIJKSTRA_SPARSE_HPP

#include "_Common.hpp"
#include "Graph.hpp"

/**
 * @brief Dijkstra on sparse graphs with path restoration.
 *
 * @details Works for non-negative edge weights.
 * @Complexity: O((V + E) log V).
 */
template <typename Weight = I64>
struct DijkstraResult {
  Vec<Weight> dist;
  VecI32 parent;
};

template <typename Weight = I64>
DijkstraResult<Weight> dijkstra_sparse(const Graph<Weight>& g, I32 source) {
  DijkstraResult<Weight> res{
      Vec<Weight>(as<Size>(g.n), infinity<Weight>),
      VI(as<Size>(g.n), -1)};
  if (source < 0 || source >= g.n) return res;

  using State = Pair<Weight, I32>;
  MinPriorityQueue<State> pq;
  res.dist[as<Size>(source)] = 0;
  pq.push({0, source});

  while (!pq.empty()) {
    auto [d, v] = pq.top();
    pq.pop();
    if (d != res.dist[as<Size>(v)]) continue;

    for (const auto& e : g.adj[as<Size>(v)]) {
      if (e.weight < 0) {
        #ifndef NDEBUG
          assert(false && "dijkstra_sparse requires non-negative edge weights");
        #endif
        continue;
      }

      Weight nd = d + e.weight;
      if (nd < res.dist[as<Size>(e.to)]) {
        res.dist[as<Size>(e.to)] = nd;
        res.parent[as<Size>(e.to)] = v;
        pq.push({nd, e.to});
      }
    }
  }

  return res;
}

/// @brief Restores source->target path from Dijkstra parent array.
inline VecI32 restore_path(I32 target, const VecI32& parent) {
  VecI32 path;
  if (target < 0 || target >= as<I32>(parent.size())) return path;
  for (I32 v = target; v != -1; v = parent[as<Size>(v)]) {
    path.push_back(v);
  }
  std::reverse(all(path));
  return path;
}

#endif
