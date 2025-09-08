//===----------------------------------------------------------------------===//
/**
 * @file: problem_E1.cpp
 * @brief Codeforces Round 1048 (Div. 2) - Problem E1
 * @author: Costantino Lombardi
 *
 * @status: In Progress
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

// Type aliases:
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
using VVV_i    = std::vector<std::vector<std::vector<int>>>;
using VV_ll    = std::vector<std::vector<long long>>;
using V_s      = std::vector<std::string>;
using VV_s     = std::vector<std::vector<std::string>>;
using VP_ii    = std::vector<std::pair<int, int>>;
using VVP_ii   = std::vector<std::vector<std::pair<int, int>>>;
using VP_ll    = std::vector<std::pair<long long, long long>>;
using VT_iii   = std::vector<std::tuple<int, int, int>>;
using Map_ll   = std::map<long long, long long>;
using VUMap_il = std::vector<std::unordered_map<int, ll>>;

// Mathematical constants:
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
/* Data Types and Function Definitions */

// Solver class encapsulating the logic for computing maximum beauty.
class TreeBeautySolver {
private:
  int n, num_zeros;
  VV_i adj;
  VVV_i dp_costs;

  // Compute minimum costs using post-order DFS.
  void compute_monochromatic_costs_dfs(int u, int target_beauty) {
    // Base case: leaf node.
    if (adj[u].empty()) {
      dp_costs[u][0][0] = 0;
      if (target_beauty >= 1) {
        dp_costs[u][1][1] = 1;
      }
      return;
    }

    // Initialize for node u.
    dp_costs[u][0][0] = 0;

    // Recursively process children.
    for (int v : adj[u]) {
      compute_monochromatic_costs_dfs(v, target_beauty);
    }

    for (int len = 1; len <= target_beauty; ++len) {
      // Calculate cost if 'u' is NOT labeled with target color.
      ll cost_without_label = 0;
      for (int v : adj[u]) {
        cost_without_label += min(dp_costs[v][0][len], dp_costs[v][1][len]);
      }
      dp_costs[u][0][len] = min((ll)INF, cost_without_label);

      // Calculate cost if 'u' IS labeled with target color.
      ll cost_with_label = 1;
      for (int v : adj[u]) {
        cost_with_label += min(dp_costs[v][0][len - 1], dp_costs[v][1][len - 1]);
      }
      dp_costs[u][1][len] = min((ll)INF, cost_with_label);
    }
  }

  // Check if given beauty is achievable.
  bool is_beauty_achievable(int beauty) {
    if (beauty == 0) return true;
    if (beauty > n) return false;

    // Initialize DP table.
    dp_costs.assign(n + 1, VVV_i::value_type(2, V_i(beauty + 1, INF)));
    compute_monochromatic_costs_dfs(1, beauty);

    const auto& costs_at_root = dp_costs[1];
    int num_ones = n - num_zeros;

    // Try all possible distributions of zeros and ones.
    for (int zeros_in_lcs = 0; zeros_in_lcs <= beauty; ++zeros_in_lcs) {
      int ones_in_lcs = beauty - zeros_in_lcs;

      // Scenario 1: root is labeled '0'.
      ll cost_zeros = costs_at_root[1][zeros_in_lcs];
      ll cost_ones = costs_at_root[0][ones_in_lcs];
      if (cost_zeros <= num_zeros && cost_ones <= num_ones) {
        return true;
      }

      // Scenario 2: root is labeled '1'.
      cost_zeros = costs_at_root[0][zeros_in_lcs];
      cost_ones = costs_at_root[1][ones_in_lcs];
      if (cost_zeros <= num_zeros && cost_ones <= num_ones) {
        return true;
      }
    }

    return false;
  }

public:
  TreeBeautySolver(int nodes, int zeros) : n(nodes), num_zeros(zeros), adj(nodes + 1) {}

  // Build tree from input.
  void build_tree() {
    // Read parent of each node (2 to n).
    for (int i = 2; i <= n; ++i) {
      int parent;
      cin >> parent;
      adj[parent].push_back(i);
    }
  }

  // Binary search to find maximum beauty.
  int find_maximum_beauty() {
    // Binary search for maximum achievable beauty.
    int low = 0, high = n, max_beauty = 0;

    while (low <= high) {
      int mid_beauty = low + (high - low) / 2;
      if (is_beauty_achievable(mid_beauty)) {
        max_beauty = mid_beauty;
        low = mid_beauty + 1;
      } else {
        high = mid_beauty - 1;
      }
    }
    return max_beauty;
  }
};

// Function to solve a single test case.
void solve() {
  int n, k;
  cin >> n >> k;

  TreeBeautySolver solver(n, k);
  solver.build_tree();
  cout << solver.find_maximum_beauty() << "\n";
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
