#pragma once
#include "../templates/Types.hpp"
#include "../templates/Macros.hpp"
#include "../templates/Constants.hpp"

//===----------------------------------------------------------------------===//
//========================= Graph Algorithms Module ==========================//

// Graph representation using adjacency list.
template <typename Weight = I64>
struct Graph {
  struct Edge {
    I32 to;
    Weight weight;
    I32 id;
    
    Edge(I32 t, Weight w = 1, I32 i = -1) : to(t), weight(w), id(i) {}
    bool operator<(const Edge& other) const { return weight < other.weight; }
  };
  
  I32 n, m;
  VC<VC<Edge>> adj;
  VC<Edge> edges;
  bool directed;
  
  Graph(I32 n, bool directed = false) : n(n), m(0), adj(n), directed(directed) {}
  
  void add_edge(I32 from, I32 to, Weight weight = 1) {
    adj[from].eb(to, weight, m);
    if (!directed) adj[to].eb(from, weight, m);
    edges.eb(to, weight, m);
    m++;
  }
  
  // Dijkstra's shortest path algorithm.
  VC<Weight> dijkstra(I32 source) const {
    VC<Weight> dist(n, infinity<Weight>);
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
  
  // Bellman-Ford algorithm for detecting negative cycles.
  P<bool, VC<Weight>> bellman_ford(I32 source) const {
    VC<Weight> dist(n, infinity<Weight>);
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
  
  // Topological sort using DFS (for DAGs).
  VC<I32> topological_sort() const {
    VC<I32> result;
    VC<bool> visited(n, false);
    
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
  
  // Find strongly connected components using Kosaraju's algorithm.
  VC<I32> find_scc() const {
    VC<I32> order;
    VC<bool> visited(n, false);
    
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
    VC<VC<I32>> rev_adj(n);
    FOR(u, n) {
      for (const auto& e : adj[u]) {
        rev_adj[e.to].pb(u);
      }
    }
    
    // Second DFS on reverse graph.
    VC<I32> component(n, -1);
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
  
  // Find bridges in the graph
  VC<PII> find_bridges() const {
    VC<PII> bridges;
    VC<I32> disc(n, -1), low(n, -1);
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
};

// Disjoint Set Union (DSU) / Union-Find.
struct DSU {
  VC<I32> parent, rank, size;
  I32 components;
  
  DSU(I32 n) : parent(n), rank(n, 0), size(n, 1), components(n) {
    std::iota(all(parent), 0);
  }
  
  I32 find(I32 x) {
    if (parent[x] != x) parent[x] = find(parent[x]);
    return parent[x];
  }
  
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
  
  bool connected(I32 x, I32 y) { return find(x) == find(y); }
  I32 component_size(I32 x) { return size[find(x)]; }
  I32 num_components() const { return components; }
};

// Kruskal's minimum Spanning Tree.
template <typename Weight = I64>
P<Weight, VC<I32>> kruskal_mst(I32 n, VC<std::tuple<I32, I32, Weight>>& edges) {
  std::sort(all(edges), [](const auto& a, const auto& b) {
    return std::get<2>(a) < std::get<2>(b);
  });
  
  DSU dsu(n);
  Weight total_weight = 0;
  VC<I32> mst_edges;
  
  FOR(i, sz(edges)) {
    auto [u, v, w] = edges[i];
    if (dsu.unite(u, v)) {
      total_weight += w;
      mst_edges.pb(i);
    }
  }
  
  return {total_weight, mst_edges};
}

// Floyd-Warshall all-pairs shortest path.
template <typename Weight = I64>
VVC<Weight> floyd_warshall(I32 n, const VC<std::tuple<I32, I32, Weight>>& edges) {
  VVC<Weight> dist(n, VC<Weight>(n, infinity<Weight>));
  
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

// LCA (Lowest Common Ancestor) using Binary Lifting.
struct LCA {
  I32 n, log_n;
  VVI parent;
  VI depth;
  
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
  
  I32 distance(I32 u, I32 v) {
    return depth[u] + depth[v] - 2 * depth[query(u, v)];
  }
};

//===----------------------------------------------------------------------===//
