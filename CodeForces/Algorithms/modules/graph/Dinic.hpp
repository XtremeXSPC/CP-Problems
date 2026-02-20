#ifndef CP_MODULES_GRAPH_DINIC_HPP
#define CP_MODULES_GRAPH_DINIC_HPP

#include "_Common.hpp"

/**
 * @brief Dinic maximum flow implementation.
 * @tparam Cap Capacity type.
 *
 * Uses BFS level graph + DFS blocking flow.
 * Complexity: O(V^2 E) in general, O(E sqrt(V)) for bipartite matching cases.
 */
template <typename Cap = I64>
struct Dinic {
  /**
   * @brief Residual edge representation.
   */
  struct Edge {
    I32 to;
    I32 rev;
    Cap cap;
  };

  I32 n;
  Vec<Vec<Edge>> g;
  Vec<I32> level;
  Vec<I32> ptr;

  Dinic(I32 n) : n(n), g(n), level(n), ptr(n) {}

  /**
   * @brief Adds a directed edge with capacity.
   * @param from Source vertex.
   * @param to Destination vertex.
   * @param cap Capacity of forward edge.
   */
  void add_edge(I32 from, I32 to, Cap cap) {
    Edge fwd{to, static_cast<I32>(g[to].size()), cap};
    Edge rev{from, static_cast<I32>(g[from].size()), 0};
    g[from].pb(fwd);
    g[to].pb(rev);
  }

  /**
   * @brief Builds BFS level graph from source.
   * @return true if sink is reachable in residual graph.
   */
  bool build_level_graph(I32 source, I32 sink) {
    std::fill(all(level), -1);
    Queue<I32> q;
    level[source] = 0;
    q.push(source);

    while (!q.empty()) {
      I32 v = q.front();
      q.pop();
      for (const auto& e : g[v]) {
        if (e.cap <= 0 || level[e.to] != -1) continue;
        level[e.to] = level[v] + 1;
        q.push(e.to);
      }
    }
    return level[sink] != -1;
  }

  /**
   * @brief DFS blocking-flow augmentation on level graph.
   * @return Amount of pushed flow.
   */
  Cap dfs(I32 v, I32 sink, Cap pushed) {
    if (pushed == 0 || v == sink) return pushed;
    for (I32& cid = ptr[v]; cid < sz(g[v]); ++cid) {
      Edge& e = g[v][cid];
      if (e.cap <= 0 || level[e.to] != level[v] + 1) continue;
      Cap flow = dfs(e.to, sink, std::min(pushed, e.cap));
      if (flow == 0) continue;
      e.cap -= flow;
      g[e.to][e.rev].cap += flow;
      return flow;
    }
    return 0;
  }

  /**
   * @brief Computes max flow between source and sink.
   * @return Maximum s-t flow value.
   */
  Cap max_flow(I32 source, I32 sink) {
    if (source == sink) return 0;
    Cap flow = 0;
    while (build_level_graph(source, sink)) {
      std::fill(all(ptr), 0);
      while (Cap pushed = dfs(source, sink, std::numeric_limits<Cap>::max() / 4)) {
        flow += pushed;
      }
    }
    return flow;
  }
};

#endif
