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
inline Optional<VI> eulerian_path_undirected(I32 n, const Vec<PII>& edges) {
  if (n < 0) return std::nullopt;
  if (n == 0) return VI{};

  const I32 m = static_cast<I32>(edges.size());
  Vec<Vec<PII>> adj(static_cast<Size>(n));  // {to, edge_id}
  VI deg(static_cast<Size>(n), 0);

  FOR(id, m) {
    auto [u, v] = edges[static_cast<Size>(id)];
    if (u < 0 || u >= n || v < 0 || v >= n) return std::nullopt;
    adj[static_cast<Size>(u)].push_back({v, id});
    adj[static_cast<Size>(v)].push_back({u, id});
    ++deg[static_cast<Size>(u)];
    ++deg[static_cast<Size>(v)];
  }

  VI odd;
  odd.reserve(2);
  FOR(v, n) {
    if (deg[static_cast<Size>(v)] & 1) odd.push_back(v);
  }
  if (!(odd.empty() || odd.size() == 2)) return std::nullopt;

  I32 start = -1;
  if (!odd.empty()) {
    start = odd[0];
  } else {
    FOR(v, n) {
      if (deg[static_cast<Size>(v)] > 0) {
        start = v;
        break;
      }
    }
    if (start == -1) return VI{0};
  }

  // Connectivity check on vertices with deg > 0.
  VB vis(static_cast<Size>(n), false);
  Stack<I32> dfs;
  dfs.push(start);
  vis[static_cast<Size>(start)] = true;
  while (!dfs.empty()) {
    I32 v = dfs.top();
    dfs.pop();
    for (const auto& [to, id] : adj[static_cast<Size>(v)]) {
      (void)id;
      if (!vis[static_cast<Size>(to)]) {
        vis[static_cast<Size>(to)] = true;
        dfs.push(to);
      }
    }
  }
  FOR(v, n) {
    if (deg[static_cast<Size>(v)] > 0 && !vis[static_cast<Size>(v)]) {
      return std::nullopt;
    }
  }

  VB used(static_cast<Size>(m), false);
  VI it(static_cast<Size>(n), 0);
  VI path;
  path.reserve(static_cast<Size>(m + 1));

  Stack<I32> st;
  st.push(start);
  while (!st.empty()) {
    I32 v = st.top();
    auto& idx = it[static_cast<Size>(v)];
    while (idx < static_cast<I32>(adj[static_cast<Size>(v)].size()) &&
           used[static_cast<Size>(
               adj[static_cast<Size>(v)][static_cast<Size>(idx)].second)]) {
      ++idx;
    }
    if (idx == static_cast<I32>(adj[static_cast<Size>(v)].size())) {
      path.push_back(v);
      st.pop();
      continue;
    }

    auto [to, id] = adj[static_cast<Size>(v)][static_cast<Size>(idx++)];
    if (used[static_cast<Size>(id)]) continue;
    used[static_cast<Size>(id)] = true;
    st.push(to);
  }

  if (static_cast<I32>(path.size()) != m + 1) return std::nullopt;
  std::reverse(all(path));
  return path;
}

inline Optional<VI> eulerian_path_directed(I32 n, const Vec<PII>& edges) {
  if (n < 0) return std::nullopt;
  if (n == 0) return VI{};

  const I32 m = static_cast<I32>(edges.size());
  Vec<Vec<PII>> adj(static_cast<Size>(n));      // {to, edge_id}
  Vec<VI> und_adj(static_cast<Size>(n));        // weak connectivity check
  VI in_deg(static_cast<Size>(n), 0);
  VI out_deg(static_cast<Size>(n), 0);

  FOR(id, m) {
    auto [u, v] = edges[static_cast<Size>(id)];
    if (u < 0 || u >= n || v < 0 || v >= n) return std::nullopt;
    adj[static_cast<Size>(u)].push_back({v, id});
    und_adj[static_cast<Size>(u)].push_back(v);
    und_adj[static_cast<Size>(v)].push_back(u);
    ++out_deg[static_cast<Size>(u)];
    ++in_deg[static_cast<Size>(v)];
  }

  I32 start = -1;
  I32 finish = -1;
  FOR(v, n) {
    I32 diff = out_deg[static_cast<Size>(v)] - in_deg[static_cast<Size>(v)];
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
      if (out_deg[static_cast<Size>(v)] > 0) {
        start = v;
        break;
      }
    }
    if (start == -1) return VI{0};
  }

  // Weak connectivity check on active vertices (in_deg + out_deg > 0).
  VB vis(static_cast<Size>(n), false);
  Stack<I32> dfs;
  dfs.push(start);
  vis[static_cast<Size>(start)] = true;
  while (!dfs.empty()) {
    I32 v = dfs.top();
    dfs.pop();
    for (I32 to : und_adj[static_cast<Size>(v)]) {
      if (!vis[static_cast<Size>(to)]) {
        vis[static_cast<Size>(to)] = true;
        dfs.push(to);
      }
    }
  }
  FOR(v, n) {
    if (in_deg[static_cast<Size>(v)] + out_deg[static_cast<Size>(v)] > 0 &&
        !vis[static_cast<Size>(v)]) {
      return std::nullopt;
    }
  }

  VB used(static_cast<Size>(m), false);
  VI it(static_cast<Size>(n), 0);
  VI path;
  path.reserve(static_cast<Size>(m + 1));

  Stack<I32> st;
  st.push(start);
  while (!st.empty()) {
    I32 v = st.top();
    auto& idx = it[static_cast<Size>(v)];
    while (idx < static_cast<I32>(adj[static_cast<Size>(v)].size()) &&
           used[static_cast<Size>(
               adj[static_cast<Size>(v)][static_cast<Size>(idx)].second)]) {
      ++idx;
    }
    if (idx == static_cast<I32>(adj[static_cast<Size>(v)].size())) {
      path.push_back(v);
      st.pop();
      continue;
    }

    auto [to, id] = adj[static_cast<Size>(v)][static_cast<Size>(idx++)];
    if (used[static_cast<Size>(id)]) continue;
    used[static_cast<Size>(id)] = true;
    st.push(to);
  }

  if (static_cast<I32>(path.size()) != m + 1) return std::nullopt;
  std::reverse(all(path));
  return path;
}

#endif
