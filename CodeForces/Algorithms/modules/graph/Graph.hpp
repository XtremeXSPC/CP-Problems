#ifndef CP_MODULES_GRAPH_CORE_HPP
#define CP_MODULES_GRAPH_CORE_HPP

#include "_Common.hpp"

/**
 * @brief Adjacency-list graph with common traversal and connectivity routines.
 *
 * @details Supports directed or undirected graphs. In undirected mode each edge is
 * stored in both directions in @c adj and in @c edges for consistency.
 */
template <typename Weight = I64>
struct Graph {
  /// @brief Edge record stored in adjacency lists and in the global edge list.
  struct Edge {
    I32 from;
    I32 to;
    Weight weight;
    I32 id;

    Edge(I32 f, I32 t, Weight w = 1, I32 i = -1) : from(f), to(t), weight(w), id(i) {}
    bool operator<(const Edge& other) const { return weight < other.weight; }
  };

  I32 n, m;
  Vec<Vec<Edge>> adj;
  Vec<Edge> edges;
  bool directed;

  Graph(I32 n, bool directed = false) : n(n), m(0), adj(n), directed(directed) {}

  /// @brief Adds an edge from 'from' to 'to' with given weight (default 1).
  void add_edge(I32 from, I32 to, Weight weight = 1) {
    adj[from].eb(from, to, weight, m);
    edges.eb(from, to, weight, m);
    if (!directed) {
      adj[to].eb(to, from, weight, m);
      edges.eb(to, from, weight, m);
    }
    m++;
  }

  /// @brief Unweighted shortest-path distances from a source via BFS.
  Vec<I32> bfs(I32 source) const {
    Vec<I32> dist(n, -1);
    Queue<I32> q;
    dist[source] = 0;
    q.push(source);

    while (!q.empty()) {
      I32 u = q.front();
      q.pop();
      for (const auto& e : adj[u]) {
        if (dist[e.to] != -1) continue;
        dist[e.to] = dist[u] + 1;
        q.push(e.to);
      }
    }

    return dist;
  }

  /// @brief Dijkstra shortest paths for non-negative edge weights.
  Vec<Weight> dijkstra(I32 source) const {
    Vec<Weight> dist(n, infinity<Weight>);
    MinPriorityQueue<Pair<Weight, I32>> pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
      auto [d, u] = pq.top();
      pq.pop();

      if (d > dist[u]) continue;

      for (const auto& e : adj[u]) {
        Weight new_dist = dist[u] + e.weight;
        if (new_dist < dist[e.to]) {
          dist[e.to] = new_dist;
          pq.push({new_dist, e.to});
        }
      }
    }

    return dist;
  }

  /// @brief Bellman-Ford shortest paths with negative-cycle detection.
  Pair<bool, Vec<Weight>> bellman_ford(I32 source) const {
    Vec<Weight> dist(n, infinity<Weight>);
    dist[source] = 0;

    // Relax edges n-1 times.
    FOR(i, n - 1) {
      bool updated = false;
      FOR(u, n) {
        if (dist[u] == infinity<Weight>) continue;
        for (const auto& e : adj[u]) {
          if (dist[u] + e.weight < dist[e.to]) {
            dist[e.to] = dist[u] + e.weight;
            updated = true;
          }
        }
      }
      if (!updated) break;
    }

    // Check for negative cycle.
    FOR(u, n) {
      if (dist[u] == infinity<Weight>) continue;
      for (const auto& e : adj[u]) {
        if (dist[u] + e.weight < dist[e.to]) {
          return {true, dist}; // Negative cycle exists
        }
      }
    }

    return {false, dist};
  }

  /// @brief Topological sort of a DAG. Returns vertices in topologically sorted order.
  Vec<I32> topological_sort() const {
    Vec<I32> result;
    Vec<bool> visited(n, false);

    std::function<void(I32)> dfs = [&](I32 u) {
      visited[u] = true;
      for (const auto& e : adj[u]) {
        if (!visited[e.to]) dfs(e.to);
      }
      result.push_back(u);
    };

    FOR(i, n) {
      if (!visited[i]) dfs(i);
    }

    std::reverse(all(result));
    return result;
  }

  /// @brief Strongly connected components via Kosaraju algorithm.
  Vec<I32> find_scc() const {
    Vec<I32> order;
    Vec<bool> visited(n, false);

    // First DFS to find finish times.
    std::function<void(I32)> dfs1 = [&](I32 u) {
      visited[u] = true;
      for (const auto& e : adj[u]) {
        if (!visited[e.to]) dfs1(e.to);
      }
      order.push_back(u);
    };

    FOR(i, n) {
      if (!visited[i]) dfs1(i);
    }

    // Build reverse graph.
    Vec<Vec<I32>> rev_adj(n);
    FOR(u, n) {
      for (const auto& e : adj[u]) {
        rev_adj[e.to].push_back(u);
      }
    }

    // Second DFS on reverse graph.
    Vec<I32> component(n, -1);
    I32 comp_count = 0;

    std::function<void(I32, I32)> dfs2 = [&](I32 u, I32 comp) {
      component[u] = comp;
      for (I32 v : rev_adj[u]) {
        if (component[v] == -1) dfs2(v, comp);
      }
    };

    std::reverse(all(order));
    for (I32 u : order) {
      if (component[u] == -1) {
        dfs2(u, comp_count++);
      }
    }

    return component;
  }

  ///@brief Finds all bridges (cut edges) in an undirected graph.
  Vec<PairI32> find_bridges() const {
    Vec<PairI32> bridges;
    Vec<I32> disc(n, -1), low(n, -1);
    I32 timer = 0;

    std::function<void(I32, I32)> dfs = [&](I32 u, I32 parent_edge) {
      disc[u] = low[u] = timer++;

      for (const auto& e : adj[u]) {
        I32 v = e.to;
        if (disc[v] == -1) {
          dfs(v, e.id);
          low[u] = std::min(low[u], low[v]);

          if (low[v] > disc[u]) {
            bridges.push_back({std::min(u, v), std::max(u, v)});
          }
        } else if (e.id != parent_edge) {
          low[u] = std::min(low[u], disc[v]);
        }
      }
    };

    FOR(i, n) {
      if (disc[i] == -1) dfs(i, -1);
    }

    return bridges;
  }

  /// @brief Finds articulation points (cut vertices) in an undirected graph.
  Vec<I32> find_articulation_points() const {
    Vec<I32> disc(n, -1), low(n, -1);
    Vec<bool> is_articulation(n, false);
    I32 timer = 0;

    std::function<void(I32, I32)> dfs = [&](I32 u, I32 parent_edge) {
      disc[u] = low[u] = timer++;
      I32 child_count = 0;

      for (const auto& e : adj[u]) {
        I32 v = e.to;
        if (disc[v] == -1) {
          child_count++;
          dfs(v, e.id);
          low[u] = std::min(low[u], low[v]);

          if (parent_edge != -1 && low[v] >= disc[u]) {
            is_articulation[u] = true;
          }
        } else if (e.id != parent_edge) {
          low[u] = std::min(low[u], disc[v]);
        }
      }

      if (parent_edge == -1 && child_count > 1) {
        is_articulation[u] = true;
      }
    };

    FOR(i, n) {
      if (disc[i] == -1) dfs(i, -1);
    }

    Vec<I32> result;
    FOR(i, n) {
      if (is_articulation[i]) result.push_back(i);
    }
    return result;
  }
};

#endif
