#ifndef CP_MODULES_GRAPH_FLOYD_WARSHALL_HPP
#define CP_MODULES_GRAPH_FLOYD_WARSHALL_HPP

#include "_Common.hpp"

/**
 * @brief Floyd-Warshall all-pairs shortest paths.
 * @tparam Weight Edge weight type.
 * @param n Number of vertices.
 * @param edges Directed weighted edges.
 * @return n x n distance matrix.
 *
 * Complexity: O(V^3).
 */
template <typename Weight = I64>
Vec2<Weight> floyd_warshall(I32 n, const Vec<std::tuple<I32, I32, Weight>>& edges) {
  Vec2<Weight> dist(n, Vec<Weight>(n, infinity<Weight>));

  FOR(i, n) dist[i][i] = 0;

  for (const auto& [u, v, w] : edges) {
    chmin(dist[u][v], w);
  }

  FOR(k, n) {
    FOR(i, n) {
      FOR(j, n) {
        if (dist[i][k] != infinity<Weight> && dist[k][j] != infinity<Weight>) {
          chmin(dist[i][j], dist[i][k] + dist[k][j]);
        }
      }
    }
  }

  return dist;
}

#endif
