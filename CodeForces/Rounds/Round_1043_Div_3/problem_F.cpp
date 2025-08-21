//===----------------------------------------------------------------------===//
/**
 * @file: problem_F.cpp
 * @brief Codeforces Round 1043 (Div. 3) - Problem F
 * @author: Costantino Lombardi
 *
 * @status: PASSED
 */
//===----------------------------------------------------------------------===//
/* Included library */

// clang-format off
// Sanitaze macro:
#ifdef USE_CLANG_SANITIZE
  #include "PCH.h"
#else
  #include <bits/stdc++.h>
#endif

using namespace std;

//===----------------------------------------------------------------------===//
/* Macros and Type Aliases */

// Debug macro:
#ifdef LOCAL
  #include "debug.h"
#else
  #define debug(...) 42
#endif
// clang-format on

// Type aliases
using ll   = long long;
using ull  = unsigned long long;
using ld   = long double;
using pii  = std::pair<int, int>;
using pll  = std::pair<long long, long long>;
using vi   = std::vector<int>;
using vll  = std::vector<long long>;
using vvi  = std::vector<std::vector<int>>;
using vvll = std::vector<std::vector<long long>>;
using vs   = std::vector<std::string>;
using vpii = std::vector<std::pair<int, int>>;
using vpll = std::vector<std::pair<long long, long long>>;

// Mathematical constants
constexpr long double PI   = 3.141592653589793238462643383279502884L;
constexpr long double E    = 2.718281828459045235360287471352662498L;
constexpr long double EPS  = 1e-9L;
constexpr int         INF  = 0x3f3f3f3f;
constexpr long long   LINF = 0x3f3f3f3f3f3f3f3fLL;
constexpr int         MOD  = 1000000007;
constexpr int         MOD2 = 998244353;

//===----------------------------------------------------------------------===//
/* Data Types and Function Definitions */

const int MAXN = 200005;

// Graph representation and bridge finding variables
vector<pii>  adj[MAXN];
int          n_nodes, m_lanes;
vi           discovery_time, lowest_time;
vector<bool> is_bridge;
int          timer;

// Biconnected components variables
vi  component_id;
int current_component_id;

// Condensed graph (tree) variables
vpii condensed_adj[MAXN];
vi   parent_node, parent_edge_idx;

// Lane endpoints
vi lane_u, lane_v;

void find_bridges_dfs(int u, int p_edge_idx) {
  discovery_time[u] = lowest_time[u] = ++timer;
  for (auto& edge : adj[u]) {
    int v        = edge.first;
    int edge_idx = edge.second;
    if (edge_idx == p_edge_idx)
      continue;
    if (discovery_time[v] != -1) {
      lowest_time[u] = min(lowest_time[u], discovery_time[v]);
    } else {
      find_bridges_dfs(v, edge_idx);
      lowest_time[u] = min(lowest_time[u], lowest_time[v]);
      if (lowest_time[v] > discovery_time[u]) {
        is_bridge[edge_idx] = true;
      }
    }
  }
}

void find_components_dfs(int u) {
  component_id[u] = current_component_id;
  for (auto& edge : adj[u]) {
    int v        = edge.first;
    int edge_idx = edge.second;
    if (!is_bridge[edge_idx] && component_id[v] == 0) {
      find_components_dfs(v);
    }
  }
}

void find_path_on_tree_dfs(int u, int p, int edge_p_idx) {
  parent_node[u]     = p;
  parent_edge_idx[u] = edge_p_idx;
  for (auto& edge : condensed_adj[u]) {
    int v = edge.first;
    if (v != p) {
      find_path_on_tree_dfs(v, u, edge.second);
    }
  }
}

// Function to solve a single test case
void solve() {
  cin >> n_nodes >> m_lanes;

  // Clear data structures for the new test case
  for (int i = 1; i <= n_nodes; ++i)
    adj[i].clear();
  lane_u.assign(m_lanes + 1, 0);
  lane_v.assign(m_lanes + 1, 0);

  for (int i = 1; i <= m_lanes; ++i) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back({v, i});
    adj[v].push_back({u, i});
    lane_u[i] = u;
    lane_v[i] = v;
  }

  // Find bridges
  discovery_time.assign(n_nodes + 1, -1);
  lowest_time.assign(n_nodes + 1, 0);
  is_bridge.assign(m_lanes + 1, false);
  timer = 0;
  find_bridges_dfs(1, -1);

  // Find biconnected components
  component_id.assign(n_nodes + 1, 0);
  current_component_id = 0;
  for (int i = 1; i <= n_nodes; ++i) {
    if (component_id[i] == 0) {
      current_component_id++;
      find_components_dfs(i);
    }
  }

  // Build the condensed graph (tree of components)
  for (int i = 1; i <= current_component_id; ++i)
    condensed_adj[i].clear();
  for (int i = 1; i <= m_lanes; ++i) {
    if (is_bridge[i]) {
      int u_comp = component_id[lane_u[i]];
      int v_comp = component_id[lane_v[i]];
      condensed_adj[u_comp].push_back({v_comp, i});
      condensed_adj[v_comp].push_back({u_comp, i});
    }
  }

  // Find the path of essential bridges in the component tree
  int start_comp = component_id[1];
  int end_comp   = component_id[n_nodes];

  parent_node.assign(current_component_id + 1, -1);
  parent_edge_idx.assign(current_component_id + 1, -1);

  if (start_comp != end_comp) {
    find_path_on_tree_dfs(start_comp, -1, -1);
  }

  vi essential_lanes;
  if (start_comp != end_comp) {
    int curr = end_comp;
    while (curr != start_comp) {
      essential_lanes.push_back(parent_edge_idx[curr]);
      curr = parent_node[curr];
    }
  }

  int q_count;
  cin >> q_count;

  if (essential_lanes.empty()) {
    for (int i = 0; i < q_count; ++i) {
      int c;
      cin >> c;
      cout << -1 << (i == q_count - 1 ? "" : " ");
    }
    cout << "\n";
    return;
  }

  // Multi-source BFS to find nearest essential lane
  vi         min_distance(n_nodes + 1, INF);
  vi         nearest_lane_idx(n_nodes + 1, INF);
  queue<int> q;

  for (int lane_idx : essential_lanes) {
    int u = lane_u[lane_idx];
    int v = lane_v[lane_idx];

    if (min_distance[u] == INF) {
      min_distance[u]     = 0;
      nearest_lane_idx[u] = lane_idx;
      q.push(u);
    } else if (lane_idx < nearest_lane_idx[u]) {
      nearest_lane_idx[u] = lane_idx;
    }

    if (min_distance[v] == INF) {
      min_distance[v]     = 0;
      nearest_lane_idx[v] = lane_idx;
      q.push(v);
    } else if (lane_idx < nearest_lane_idx[v]) {
      nearest_lane_idx[v] = lane_idx;
    }
  }

  while (!q.empty()) {
    int u = q.front();
    q.pop();

    for (auto& edge : adj[u]) {
      int v = edge.first;
      if (min_distance[v] > min_distance[u] + 1) {
        min_distance[v]     = min_distance[u] + 1;
        nearest_lane_idx[v] = nearest_lane_idx[u];
        q.push(v);
      } else if (min_distance[v] == min_distance[u] + 1) {
        nearest_lane_idx[v] = min(nearest_lane_idx[v], nearest_lane_idx[u]);
      }
    }
  }

  // Answer queries
  for (int i = 0; i < q_count; ++i) {
    int c;
    cin >> c;
    cout << nearest_lane_idx[c] << (i == q_count - 1 ? "" : " ");
  }
  cout << "\n";
}

//===----------------------------------------------------------------------===//
/* Main function */

int main() {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int t = 1;
  cin >> t;
  while (t--) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//
