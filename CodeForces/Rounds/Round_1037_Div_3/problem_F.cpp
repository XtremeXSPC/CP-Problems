//===----------------------------------------------------------------------===//
/**
 * @brief Codeforces Round 1037 Div. 3
 * @author: Costantino Lombardi
 * @file: problem_F.cc
 *
 * @status: In progress
 */
//===----------------------------------------------------------------------===//
/* Included library */

// clang-format off
// Compiler optimizations:
#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC optimize("Ofast,unroll-loops,fast-math,O3")
  // Apple Silicon optimizations:
  #ifdef __aarch64__
    #pragma GCC target("+simd")
  #endif
#endif

#ifdef __clang__
  #pragma clang optimize on
#endif

// Sanitaze macro:
#ifdef USE_CLANG_SANITIZE
  #include "PCH.h"
#else
  #include <bits/stdc++.h>
#endif

// Debug macro:
#ifdef LOCAL
  #include "debug.h"
#else
  #define debug(...) 42
#endif
// clang-format on

//===----------------------------------------------------------------------===//
/* Type Aliases and Constants */

// Type aliases
using ll       = long long;
using ull      = unsigned long long;
using ld       = long double;
using P_ii     = std::pair<int, int>;
using P_ll     = std::pair<long long, long long>;
using V_b      = std::vector<bool>;
using V_i      = std::vector<int>;
using V_ui     = std::vector<unsigned int>;
using V_ll     = std::vector<long long>;
using VV_i     = std::vector<std::vector<int>>;
using VV_ll    = std::vector<std::vector<long long>>;
using V_s      = std::vector<std::string>;
using VP_ii    = std::vector<std::pair<int, int>>;
using VVP_ii   = std::vector<std::vector<std::pair<int, int>>>;
using VP_ll    = std::vector<std::pair<long long, long long>>;
using VT_iii   = std::vector<std::tuple<int, int, int>>;
using Map_ll   = std::map<long long, long long>;
using VUMap_il = std::vector<std::unordered_map<int, ll>>;

// Mathematical constants
constexpr long double PI   = 3.141592653589793238462643383279502884L;
constexpr long double E    = 2.718281828459045235360287471352662498L;
constexpr long double EPS  = 1e-9L;
constexpr int         INF  = 0x3f3f3f3f;
constexpr long long   LINF = 0x3f3f3f3f3f3f3f3fLL;
constexpr int         LIM  = 1000000 + 5;
constexpr int         MOD  = 1000000007;
constexpr int         MOD2 = 998244353;

using namespace std;

//===----------------------------------------------------------------------===//
/* Function Definitions */

// Graph representation.
using AdjacencyList = VVP_ii; // pair<neighbor, weight>.

// State needed by the DFS.
struct DfsState {
  AdjacencyList& adj;
  V_i&           colors;
  V_i&           parent;
  V_i&           parent_weight;
  VUMap_il&      children_color_weights;
};

// DFS to build the rooted tree and populate the frequency maps.
void dfs(int u, int p, DfsState& state) {
  state.parent[u] = p;
  for (const auto& [v, w] : state.adj[u]) {
    if (v == p)
      continue;
    state.parent_weight[v] = w;
    // The parent 'u' accumulates the weights of its children 'v' by color.
    state.children_color_weights[u][state.colors[v]] += w;
    dfs(v, u, state);
  }
}

// Function to solve a single test case.
void solve() {
  int n, q;
  cin >> n >> q;

  V_i colors(n);
  for (int i = 0; i < n; ++i) {
    cin >> colors[i];
  }

  AdjacencyList adj(n);
  VT_iii        edges;
  for (int i = 0; i < n - 1; ++i) {
    int u, v, c;
    cin >> u >> v >> c;
    u--;
    v--;
    adj[u].push_back({v, c});
    adj[v].push_back({u, c});
    edges.emplace_back(u, v, c);
  }

  // Calculate initial total cost.
  ll total_cost = 0;
  for (const auto& [u, v, c] : edges) {
    if (colors[u] != colors[v]) {
      total_cost += c;
    }
  }

  // Build rooted tree with parent pointers and children color weights.
  V_i      parent(n, -1);
  V_i      parent_weight(n, 0);
  VUMap_il children_color_weights(n);

  DfsState state{adj, colors, parent, parent_weight, children_color_weights};
  dfs(0, -1, state);

  // Process queries
  for (int k = 0; k < q; ++k) {
    int u, new_color;
    cin >> u >> new_color;
    u--;

    int old_color = colors[u];
    if (old_color == new_color) {
      cout << total_cost << "\n";
      continue;
    }

    // Update cost for parent edge
    int p = parent[u];
    if (p != -1) {
      int p_color  = colors[p];
      int p_weight = parent_weight[u];

      if (p_color == old_color)
        total_cost += p_weight;
      if (p_color == new_color)
        total_cost -= p_weight;

      // Update parent's frequency map.
      children_color_weights[p][old_color] -= p_weight;
      children_color_weights[p][new_color] += p_weight;
    }

    // Update cost for children edges.
    total_cost += children_color_weights[u][old_color];
    total_cost -= children_color_weights[u][new_color];

    colors[u] = new_color;
    cout << total_cost << "\n";
  }
}

//===----------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int T = 1;
  cin >> T;
  for ([[maybe_unused]] auto _ : views::iota(0, T)) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//