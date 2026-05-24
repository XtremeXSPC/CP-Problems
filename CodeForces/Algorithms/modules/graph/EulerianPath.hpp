#ifndef CP_MODULES_GRAPH_EULERIAN_PATH_HPP
#define CP_MODULES_GRAPH_EULERIAN_PATH_HPP

#include "_Common.hpp"

/**
 * @brief Iterative Hierholzer algorithm for Eulerian path/cycle.
 *
 * @details APIs below work on explicit edge lists:
 * - undirected: each edge is treated as one undirected edge
 * - directed: each edge is treated as one directed edge
 *
 * @returns std::nullopt when Eulerian path/cycle does not exist.
 * @Complexity: O(V + E).
 */
inline Optional<VecI32> eulerian_path_undirected(I32 n, const Vec<PairI32>& edges) {
  if (n < 0) return std::nullopt;
  if (n == 0) return VecI32{};

  const I32 m = isz(edges);
  Vec2D<PairI32> adj(n);  // {to, edge_id}
  VecI32 deg(n, 0);

  FOR(id, m) {
    auto [u, v] = edges[id];
    if (u < 0 || u >= n || v < 0 || v >= n) return std::nullopt;
    adj[u].push_back({v, id});
    adj[v].push_back({u, id});
    ++deg[u];
    ++deg[v];
  }

  VecI32 odd;
  odd.reserve(2);
  FOR(v, n) {
    if (deg[v] & 1) odd.push_back(v);
  }
  if (!(odd.empty() || odd.size() == 2)) return std::nullopt;

  I32 start = -1;
  if (!odd.empty()) {
    start = odd[0];
  } else {
    FOR(v, n) {
      if (deg[v] > 0) {
        start = v;
        break;
      }
    }
    if (start == -1) return VecI32{0};
  }

  // Connectivity check on vertices with deg > 0.
  VecBool vis(n, false);
  Stack<I32> dfs;
  dfs.push(start);
  vis[start] = true;
  while (!dfs.empty()) {
    I32 v = dfs.top();
    dfs.pop();
    for (const auto& [to, id] : adj[v]) {
      (void)id;
      if (!vis[to]) {
        vis[to] = true;
        dfs.push(to);
      }
    }
  }
  FOR(v, n) {
    if (deg[v] > 0 && !vis[v]) {
      return std::nullopt;
    }
  }

  VecBool used(m, false);
  VecI32 it(n, 0);
  VecI32 path;
  path.reserve((m + 1));

  Stack<I32> st;
  st.push(start);
  while (!st.empty()) {
    I32 v = st.top();
    auto& idx = it[v];
    while (idx < isz(adj[v]) &&
           used[adj[v][idx].second]) {
      ++idx;
    }
    if (idx == isz(adj[v])) {
      path.push_back(v);
      st.pop();
      continue;
    }

    auto [to, id] = adj[v][idx++];
    if (used[id]) continue;
    used[id] = true;
    st.push(to);
  }

  if (isz(path) != m + 1) return std::nullopt;
  std::reverse(all(path));
  return path;
}

inline Optional<VecI32> eulerian_path_directed(I32 n, const Vec<PairI32>& edges) {
  if (n < 0) return std::nullopt;
  if (n == 0) return VecI32{};

  const I32 m = isz(edges);
  Vec2D<PairI32> adj(n);      // {to, edge_id}
  Vec2D<I32> und_adj(n);        // weak connectivity check
  VecI32 in_deg(n, 0);
  VecI32 out_deg(n, 0);

  FOR(id, m) {
    auto [u, v] = edges[id];
    if (u < 0 || u >= n || v < 0 || v >= n) return std::nullopt;
    adj[u].push_back({v, id});
    und_adj[u].push_back(v);
    und_adj[v].push_back(u);
    ++out_deg[u];
    ++in_deg[v];
  }

  I32 start = -1;
  I32 finish = -1;
  FOR(v, n) {
    I32 diff = out_deg[v] - in_deg[v];
    if (diff == 1) {
      if (start != -1) return std::nullopt;
      start = v;
    } else if (diff == -1) {
      if (finish != -1) return std::nullopt;
      finish = v;
    } else if (diff != 0) {
      return std::nullopt;
    }
  }
  if ((start == -1) != (finish == -1)) return std::nullopt;

  if (start == -1) {
    FOR(v, n) {
      if (out_deg[v] > 0) {
        start = v;
        break;
      }
    }
    if (start == -1) return VecI32{0};
  }

  /// @brief Weak connectivity check on active vertices (in_deg + out_deg > 0).
  VecBool vis(n, false);
  Stack<I32> dfs;
  dfs.push(start);
  vis[start] = true;
  while (!dfs.empty()) {
    I32 v = dfs.top();
    dfs.pop();
    for (I32 to : und_adj[v]) {
      if (!vis[to]) {
        vis[to] = true;
        dfs.push(to);
      }
    }
  }
  FOR(v, n) {
    if (in_deg[v] + out_deg[v] > 0 &&
        !vis[v]) {
      return std::nullopt;
    }
  }

  VecBool used(m, false);
  VecI32 it(n, 0);
  VecI32 path;
  path.reserve((m + 1));

  Stack<I32> st;
  st.push(start);
  while (!st.empty()) {
    I32 v = st.top();
    auto& idx = it[v];
    while (idx < isz(adj[v]) &&
           used[adj[v][idx].second]) {
      ++idx;
    }
    if (idx == isz(adj[v])) {
      path.push_back(v);
      st.pop();
      continue;
    }

    auto [to, id] = adj[v][idx++];
    if (used[id]) continue;
    used[id] = true;
    st.push(to);
  }

  if (isz(path) != m + 1) return std::nullopt;
  std::reverse(all(path));
  return path;
}

#endif
