#pragma once
#include "../templates/Types.hpp"
#include "../templates/Macros.hpp"
#include "../templates/Constants.hpp"

/**
 * @file Graph.hpp
 * @brief Graph data structures and algorithms for competitive programming.
 */

//===----------------------------------------------------------------------===//
//========================= Graph Algorithms Module ==========================//

/**
 * @brief Adjacency-list graph with common traversal and connectivity routines.
 * @tparam Weight Edge weight type.
 *
 * Supports directed or undirected graphs. In undirected mode each edge is
 * stored in both directions in @c adj and in @c edges for consistency.
 */
template <typename Weight = I64>
struct Graph {
  /**
   * @brief Edge record stored in adjacency lists and in the global edge list.
   */
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

  /**
   * @brief Adds an edge to the graph.
   * @param from Source vertex.
   * @param to Destination vertex.
   * @param weight Edge weight (default 1).
   *
   * Complexity: O(1).
   */
  void add_edge(I32 from, I32 to, Weight weight = 1) {
    adj[from].eb(from, to, weight, m);
    edges.eb(from, to, weight, m);
    if (!directed) {
      adj[to].eb(to, from, weight, m);
      edges.eb(to, from, weight, m);
    }
    m++;
  }

  /**
   * @brief Unweighted shortest-path distances from a source via BFS.
   * @param source Source vertex.
   * @return Distance array where unreachable nodes are -1.
   *
   * Complexity: O(V + E).
   */
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

  /**
   * @brief Dijkstra shortest paths for non-negative edge weights.
   * @param source Source vertex.
   * @return Distance array with @c infinity<Weight> for unreachable nodes.
   *
   * Complexity: O((V + E) log V).
   */
  Vec<Weight> dijkstra(I32 source) const {
    Vec<Weight> dist(n, infinity<Weight>);
    MinPriorityQueue<P<Weight, I32>> pq;

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

  /**
   * @brief Bellman-Ford shortest paths with negative-cycle detection.
   * @param source Source vertex.
   * @return Pair {has_negative_cycle, distances}.
   *
   * Complexity: O(VE).
   */
  P<bool, Vec<Weight>> bellman_ford(I32 source) const {
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

  /**
   * @brief Topological ordering via DFS postorder.
   * @return A topological order for DAG inputs.
   *
   * Complexity: O(V + E).
   * @note For non-DAG graphs, returned order is not guaranteed to be valid.
   */
  Vec<I32> topological_sort() const {
    Vec<I32> result;
    Vec<bool> visited(n, false);

    std::function<void(I32)> dfs = [&](I32 u) {
      visited[u] = true;
      for (const auto& e : adj[u]) {
        if (!visited[e.to]) dfs(e.to);
      }
      result.pb(u);
    };

    FOR(i, n) {
      if (!visited[i]) dfs(i);
    }

    std::reverse(all(result));
    return result;
  }

  /**
   * @brief Strongly connected components via Kosaraju algorithm.
   * @return Component index for each vertex.
   *
   * Complexity: O(V + E).
   */
  Vec<I32> find_scc() const {
    Vec<I32> order;
    Vec<bool> visited(n, false);

    // First DFS to find finish times.
    std::function<void(I32)> dfs1 = [&](I32 u) {
      visited[u] = true;
      for (const auto& e : adj[u]) {
        if (!visited[e.to]) dfs1(e.to);
      }
      order.pb(u);
    };

    FOR(i, n) {
      if (!visited[i]) dfs1(i);
    }

    // Build reverse graph.
    Vec<Vec<I32>> rev_adj(n);
    FOR(u, n) {
      for (const auto& e : adj[u]) {
        rev_adj[e.to].pb(u);
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

  /**
   * @brief Finds all bridges (cut edges) in an undirected graph.
   * @return List of bridge edges as sorted endpoint pairs.
   *
   * Complexity: O(V + E).
   */
  Vec<PII> find_bridges() const {
    Vec<PII> bridges;
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
            bridges.pb({std::min(u, v), std::max(u, v)});
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

  /**
   * @brief Finds articulation points (cut vertices) in an undirected graph.
   * @return List of articulation vertex indices.
   *
   * Complexity: O(V + E).
   */
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
      if (is_articulation[i]) result.pb(i);
    }
    return result;
  }
};

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

/**
 * @brief Min-Cost Max-Flow via Successive Shortest Paths with SPFA.
 * @tparam Cap Capacity type.
 * @tparam Cost Cost type.
 *
 * Handles negative edge costs correctly via Bellman-Ford (SPFA).
 */
template <typename Cap = I64, typename Cost = I64>
struct MCMF {
  struct Edge {
    I32 to, rev;
    Cap cap;
    Cost cost;
  };

  I32 n;
  Vec<Vec<Edge>> g;
  Vec<Cost> dist;
  VI prev_v, prev_e;
  VB in_queue;

  MCMF(I32 n) : n(n), g(n), dist(n), prev_v(n), prev_e(n), in_queue(n) {}

  /**
   * @brief Adds a directed edge with capacity and cost (and reverse with 0 cap, -cost).
   */
  void add_edge(I32 from, I32 to, Cap cap, Cost cost) {
    g[from].push_back({to, (I32)g[to].size(), cap, cost});
    g[to].push_back({from, (I32)g[from].size() - 1, 0, -cost});
  }

  /**
   * @brief SPFA shortest path from s to t in the residual graph.
   */
  bool spfa(I32 s, I32 t) {
    std::fill(all(dist), infinity<Cost>);
    std::fill(all(in_queue), false);
    dist[s] = 0;
    Queue<I32> que;
    que.push(s);
    in_queue[s] = true;

    while (!que.empty()) {
      I32 v = que.front(); que.pop();
      in_queue[v] = false;
      FOR(i, (I64)g[v].size()) {
        auto& e = g[v][i];
        if (e.cap > 0 && dist[v] + e.cost < dist[e.to]) {
          dist[e.to] = dist[v] + e.cost;
          prev_v[e.to] = v;
          prev_e[e.to] = (I32)i;
          if (!in_queue[e.to]) {
            que.push(e.to);
            in_queue[e.to] = true;
          }
        }
      }
    }
    return dist[t] < infinity<Cost>;
  }

  /**
   * @brief Computes min-cost flow up to max_flow units from s to t.
   * @return {total_flow, total_cost}.
   */
  P<Cap, Cost> min_cost_flow(I32 s, I32 t, Cap max_flow) {
    Cap flow = 0;
    Cost cost = 0;
    while (flow < max_flow && spfa(s, t)) {
      // Find bottleneck along shortest path
      Cap d = max_flow - flow;
      for (I32 v = t; v != s; v = prev_v[v]) {
        d = std::min(d, g[prev_v[v]][prev_e[v]].cap);
      }
      // Augment along path
      for (I32 v = t; v != s; v = prev_v[v]) {
        auto& e = g[prev_v[v]][prev_e[v]];
        e.cap -= d;
        g[v][e.rev].cap += d;
      }
      flow += d;
      cost += d * dist[t];
    }
    return {flow, cost};
  }
};

/**
 * @brief Hopcroft-Karp maximum bipartite matching.
 *
 * O(E * sqrt(V)) complexity.
 * Left vertices: [0, left_size), Right vertices: [0, right_size).
 */
struct BipartiteMatching {
  I32 left_sz, right_sz;
  Vec<VI> adj;
  VI match_left, match_right, dist_left;

  BipartiteMatching(I32 left_size, I32 right_size)
      : left_sz(left_size), right_sz(right_size), adj(left_size),
        match_left(left_size, -1), match_right(right_size, -1), dist_left(left_size) {}

  /**
   * @brief Adds an edge from left vertex u to right vertex v.
   */
  void add_edge(I32 u, I32 v) { adj[u].push_back(v); }

  /**
   * @brief BFS phase: builds layered graph of free left vertices.
   */
  bool bfs() {
    Queue<I32> que;
    FOR(u, left_sz) {
      if (match_left[u] == -1) {
        dist_left[u] = 0;
        que.push(u);
      } else {
        dist_left[u] = infinity<I32>;
      }
    }
    bool found = false;
    while (!que.empty()) {
      I32 u = que.front(); que.pop();
      for (I32 v : adj[u]) {
        I32 w = match_right[v];
        if (w == -1) {
          found = true;
        } else if (dist_left[w] == infinity<I32>) {
          dist_left[w] = dist_left[u] + 1;
          que.push(w);
        }
      }
    }
    return found;
  }

  /**
   * @brief DFS phase: finds augmenting path from left vertex u.
   */
  bool dfs(I32 u) {
    for (I32 v : adj[u]) {
      I32 w = match_right[v];
      if (w == -1 || (dist_left[w] == dist_left[u] + 1 && dfs(w))) {
        match_left[u] = v;
        match_right[v] = u;
        return true;
      }
    }
    dist_left[u] = infinity<I32>;
    return false;
  }

  /**
   * @brief Computes maximum matching size.
   */
  I32 max_matching() {
    I32 result = 0;
    while (bfs()) {
      FOR(u, left_sz) {
        if (match_left[u] == -1 && dfs(u)) ++result;
      }
    }
    return result;
  }
};

/**
 * @brief Disjoint Set Union (Union-Find) with union by rank + path compression.
 */
struct DSU {
  Vec<I32> parent, rank, size;
  I32 components;

  DSU(I32 n) : parent(n), rank(n, 0), size(n, 1), components(n) {
    std::iota(all(parent), 0);
  }

  /**
   * @brief Finds representative of the set containing x.
   */
  I32 find(I32 x) {
    if (parent[x] != x) parent[x] = find(parent[x]);
    return parent[x];
  }

  /**
   * @brief Unites sets containing x and y.
   * @return True if merge happened; false if already in same set.
   */
  bool unite(I32 x, I32 y) {
    x = find(x);
    y = find(y);

    if (x == y) return false;

    if (rank[x] < rank[y]) std::swap(x, y);
    parent[y] = x;
    size[x] += size[y];

    if (rank[x] == rank[y]) rank[x]++;

    components--;
    return true;
  }

  /**
   * @brief Checks whether x and y are in the same connected component.
   */
  bool connected(I32 x, I32 y) { return find(x) == find(y); }
  /**
   * @brief Returns size of component containing x.
   */
  I32 component_size(I32 x) { return size[find(x)]; }
  /**
   * @brief Returns current number of components.
   */
  I32 num_components() const { return components; }
};

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

/**
 * @brief Lowest Common Ancestor via binary lifting.
 */
struct LCA {
  I32 n, log_n;
  VVI parent;
  VI depth;

  /**
   * @brief Builds lifting table from a rooted tree.
   * @param g Input tree.
   * @param root Chosen root.
   */
  LCA(const Graph<>& g, I32 root = 0) : n(g.n) {
    log_n = 0;
    while ((1 << log_n) < n) log_n++;

    parent.assign(log_n + 1, VI(n, -1));
    depth.assign(n, 0);
    VB visited(n, false);

    // DFS to set up parent[0] and depth.
    std::function<void(I32, I32)> dfs = [&](I32 u, I32 p) {
      visited[u] = true;
      parent[0][u] = p;
      for (const auto& e : g.adj[u]) {
        if (e.to != p && !visited[e.to]) {
          depth[e.to] = depth[u] + 1;
          dfs(e.to, u);
        }
      }
    };

    dfs(root, -1);

    // Binary lifting preprocessing.
    FOR(j, 1, log_n + 1) {
      FOR(i, n) {
        if (parent[j - 1][i] != -1) {
          parent[j][i] = parent[j - 1][parent[j - 1][i]];
        }
      }
    }
  }

  /**
   * @brief Returns LCA of u and v.
   */
  I32 query(I32 u, I32 v) {
    if (depth[u] < depth[v]) std::swap(u, v);

    // Bring 'u' to the same level as 'v'.
    I32 diff = depth[u] - depth[v];
    FOR(i, log_n + 1) {
      if ((diff >> i) & 1) u = parent[i][u];
    }

    if (u == v) return u;

    // Binary search for LCA.
    FOR_R(i, log_n + 1) {
      if (parent[i][u] != parent[i][v]) {
        u = parent[i][u];
        v = parent[i][v];
      }
    }

    return parent[0][u];
  }

  /**
   * @brief Tree distance in edges between u and v.
   */
  I32 distance(I32 u, I32 v) {
    return depth[u] + depth[v] - 2 * depth[query(u, v)];
  }
};

/**
 * @brief Euler Tour decomposition of a rooted tree.
 * @tparam Weight Edge weight type.
 *
 * Iterative DFS to avoid stack overflow on large trees (n up to 10^6).
 * Provides discovery/finish times, DFS order, depths, and parent array.
 */
template <typename Weight = I64>
struct EulerTour {
  I32 n, timer;
  VI tin, tout, order, depth, parent;

  /**
   * @brief Builds Euler Tour from the given tree rooted at root.
   * @param g Input tree.
   * @param root Root vertex (default 0).
   */
  EulerTour(const Graph<Weight>& g, I32 root = 0)
      : n(g.n), timer(0), tin(n, -1), tout(n, -1), depth(n, 0), parent(n, -1) {
    order.reserve(n);
    // Iterative DFS using explicit stack of (vertex, edge_index).
    Stack<PII> stk;
    stk.push({root, 0});
    tin[root] = timer++;
    order.push_back(root);

    while (!stk.empty()) {
      auto& [v, ei] = stk.top();
      bool found = false;
      while (ei < (I32)g.adj[v].size()) {
        I32 to = g.adj[v][ei].to;
        ++ei;
        if (to == parent[v]) continue;
        parent[to] = v;
        depth[to] = depth[v] + 1;
        tin[to] = timer++;
        order.push_back(to);
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

  /**
   * @brief Checks if u is an ancestor of v (inclusive).
   */
  bool is_ancestor(I32 u, I32 v) const {
    return tin[u] <= tin[v] && tout[v] <= tout[u];
  }

  /**
   * @brief Returns half-open subtree range [tin[v], tout[v]) in DFS order.
   */
  PII subtree_range(I32 v) const {
    return {tin[v], tout[v]};
  }
};

/**
 * @brief Heavy-Light Decomposition for tree path/subtree queries.
 * @tparam Weight Graph edge weight type.
 *
 * Maps tree vertices to a base array so any path can be decomposed into
 * O(log V) contiguous segments.
 */
template <typename Weight = I64>
struct HLD {
  I32 n;
  const Graph<Weight>& g;
  VI parent, depth, heavy, head, pos, subtree_size;
  I32 cur_pos;

  /**
   * @brief Builds decomposition for every connected component.
   * @param g Input forest/tree.
   */
  HLD(const Graph<Weight>& g) : n(g.n), g(g), parent(n, -1), depth(n, 0), heavy(n, -1),
                                head(n, 0), pos(n, -1), subtree_size(n, 0), cur_pos(0) {
    auto dfs = [&](auto&& self, I32 v, I32 p) -> I32 {
      parent[v] = p;
      subtree_size[v] = 1;
      I32 max_subtree = 0;
      for (const auto& e : g.adj[v]) {
        if (e.to == p) continue;
        depth[e.to] = depth[v] + 1;
        I32 child_size = self(self, e.to, v);
        subtree_size[v] += child_size;
        if (child_size > max_subtree) {
          max_subtree = child_size;
          heavy[v] = e.to;
        }
      }
      return subtree_size[v];
    };

    auto decompose = [&](auto&& self, I32 v, I32 h, I32 p) -> void {
      head[v] = h;
      pos[v] = cur_pos++;
      if (heavy[v] != -1) {
        self(self, heavy[v], h, v);
      }
      for (const auto& e : g.adj[v]) {
        if (e.to == p || e.to == heavy[v]) continue;
        self(self, e.to, e.to, v);
      }
    };

    FOR(i, n) {
      if (parent[i] != -1) continue;
      depth[i] = 0;
      dfs(dfs, i, -1);
      decompose(decompose, i, i, -1);
    }
  }

  /**
   * @brief LCA query using chain heads.
   */
  I32 lca(I32 u, I32 v) const {
    while (head[u] != head[v]) {
      if (depth[head[u]] > depth[head[v]]) {
        u = parent[head[u]];
      } else {
        v = parent[head[v]];
      }
    }
    return depth[u] < depth[v] ? u : v;
  }

  /**
   * @brief Iterates path(u, v) as O(log V) base-array segments.
   * @tparam F Callable signature void(I32 l, I32 r), inclusive segment.
   * @param u Path endpoint.
   * @param v Path endpoint.
   * @param op Callback for each segment in decomposition order.
   * @param edge_mode If true, excludes LCA node from final segment.
   */
  template <class F>
  void process_path(I32 u, I32 v, F&& op, bool edge_mode = false) const {
    while (head[u] != head[v]) {
      if (depth[head[u]] < depth[head[v]]) std::swap(u, v);
      op(pos[head[u]], pos[u]);
      u = parent[head[u]];
    }
    if (depth[u] > depth[v]) std::swap(u, v);
    I32 l = pos[u] + static_cast<I32>(edge_mode);
    I32 r = pos[v];
    if (l <= r) op(l, r);
  }

  /**
   * @brief Returns subtree interval in base-array coordinates.
   * @param v Root of subtree.
   * @return Inclusive range [l, r].
   */
  P<I32, I32> subtree_range(I32 v) const {
    return {pos[v], pos[v] + subtree_size[v] - 1};
  }
};

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

  /**
   * @brief Constructs centroid decomposition for each connected component.
   */
  explicit CentroidDecomposition(const Graph<Weight>& g)
      : n(g.n), tree(n), parent(n, -1), subtree_size(n, 0), used(n, false) {
    FOR(u, n) {
      for (const auto& e : g.adj[u]) {
        tree[u].pb(e.to);
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

//===----------------------------------------------------------------------===//
