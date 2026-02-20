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

  const I32 m = as<I32>(edges.size());
  Vec<Vec<PII>> adj(as<Size>(n));  // {to, edge_id}
  VI deg(as<Size>(n), 0);

  FOR(id, m) {
    auto [u, v] = edges[as<Size>(id)];
    if (u < 0 || u >= n || v < 0 || v >= n) return std::nullopt;
    adj[as<Size>(u)].push_back({v, id});
    adj[as<Size>(v)].push_back({u, id});
    ++deg[as<Size>(u)];
    ++deg[as<Size>(v)];
  }

  VI odd;
  odd.reserve(2);
  FOR(v, n) {
    if (deg[as<Size>(v)] & 1) odd.push_back(v);
  }
  if (!(odd.empty() || odd.size() == 2)) return std::nullopt;

  I32 start = -1;
  if (!odd.empty()) {
    start = odd[0];
  } else {
    FOR(v, n) {
      if (deg[as<Size>(v)] > 0) {
        start = v;
        break;
      }
    }
    if (start == -1) return VI{0};
  }

  // Connectivity check on vertices with deg > 0.
  VB vis(as<Size>(n), false);
  Stack<I32> dfs;
  dfs.push(start);
  vis[as<Size>(start)] = true;
  while (!dfs.empty()) {
    I32 v = dfs.top();
    dfs.pop();
    for (const auto& [to, id] : adj[as<Size>(v)]) {
      (void)id;
      if (!vis[as<Size>(to)]) {
        vis[as<Size>(to)] = true;
        dfs.push(to);
      }
    }
  }
  FOR(v, n) {
    if (deg[as<Size>(v)] > 0 && !vis[as<Size>(v)]) {
      return std::nullopt;
    }
  }

  VB used(as<Size>(m), false);
  VI it(as<Size>(n), 0);
  VI path;
  path.reserve(as<Size>(m + 1));

  Stack<I32> st;
  st.push(start);
  while (!st.empty()) {
    I32 v = st.top();
    auto& idx = it[as<Size>(v)];
    while (idx < as<I32>(adj[as<Size>(v)].size()) &&
           used[as<Size>(
               adj[as<Size>(v)][as<Size>(idx)].second)]) {
      ++idx;
    }
    if (idx == as<I32>(adj[as<Size>(v)].size())) {
      path.push_back(v);
      st.pop();
      continue;
    }

    auto [to, id] = adj[as<Size>(v)][as<Size>(idx++)];
    if (used[as<Size>(id)]) continue;
    used[as<Size>(id)] = true;
    st.push(to);
  }

  if (as<I32>(path.size()) != m + 1) return std::nullopt;
  std::reverse(all(path));
  return path;
}

inline Optional<VI> eulerian_path_directed(I32 n, const Vec<PII>& edges) {
  if (n < 0) return std::nullopt;
  if (n == 0) return VI{};

  const I32 m = as<I32>(edges.size());
  Vec<Vec<PII>> adj(as<Size>(n));      // {to, edge_id}
  Vec<VI> und_adj(as<Size>(n));        // weak connectivity check
  VI in_deg(as<Size>(n), 0);
  VI out_deg(as<Size>(n), 0);

  FOR(id, m) {
    auto [u, v] = edges[as<Size>(id)];
    if (u < 0 || u >= n || v < 0 || v >= n) return std::nullopt;
    adj[as<Size>(u)].push_back({v, id});
    und_adj[as<Size>(u)].push_back(v);
    und_adj[as<Size>(v)].push_back(u);
    ++out_deg[as<Size>(u)];
    ++in_deg[as<Size>(v)];
  }

  I32 start = -1;
  I32 finish = -1;
  FOR(v, n) {
    I32 diff = out_deg[as<Size>(v)] - in_deg[as<Size>(v)];
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
      if (out_deg[as<Size>(v)] > 0) {
        start = v;
        break;
      }
    }
    if (start == -1) return VI{0};
  }

  /// @brief Weak connectivity check on active vertices (in_deg + out_deg > 0).
  VB vis(as<Size>(n), false);
  Stack<I32> dfs;
  dfs.push(start);
  vis[as<Size>(start)] = true;
  while (!dfs.empty()) {
    I32 v = dfs.top();
    dfs.pop();
    for (I32 to : und_adj[as<Size>(v)]) {
      if (!vis[as<Size>(to)]) {
        vis[as<Size>(to)] = true;
        dfs.push(to);
      }
    }
  }
  FOR(v, n) {
    if (in_deg[as<Size>(v)] + out_deg[as<Size>(v)] > 0 &&
        !vis[as<Size>(v)]) {
      return std::nullopt;
    }
  }

  VB used(as<Size>(m), false);
  VI it(as<Size>(n), 0);
  VI path;
  path.reserve(as<Size>(m + 1));

  Stack<I32> st;
  st.push(start);
  while (!st.empty()) {
    I32 v = st.top();
    auto& idx = it[as<Size>(v)];
    while (idx < as<I32>(adj[as<Size>(v)].size()) &&
           used[as<Size>( adj[as<Size>(v)][as<Size>(idx)].second)]) {
      ++idx;
    }
    if (idx == as<I32>(adj[as<Size>(v)].size())) {
      path.push_back(v);
      st.pop();
      continue;
    }

    auto [to, id] = adj[as<Size>(v)][as<Size>(idx++)];
    if (used[as<Size>(id)]) continue;
    used[as<Size>(id)] = true;
    st.push(to);
  }

  if (as<I32>(path.size()) != m + 1) return std::nullopt;
  std::reverse(all(path));
  return path;
}

#endif
