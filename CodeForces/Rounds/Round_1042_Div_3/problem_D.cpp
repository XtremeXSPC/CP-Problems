//===----------------------------------------------------------------------===//
/**
 * @file: problem_D.cpp
 * @brief: Codeforces Round 1042 (Div. 3) - Problem D
 * @author: Costantino Lombardi
 *
 * @status: PASSED
 */
//===----------------------------------------------------------------------===//
/* Included library and Macros */

// clang-format off
// Compiler optimizations:
#ifdef __GNUC__
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
using ll     = long long;
using ull    = unsigned long long;
using ld     = long double;
using pii    = std::pair<int, int>;
using pll    = std::pair<long long, long long>;
using vi     = std::vector<int>;
using vui    = std::vector<unsigned int>;
using vll    = std::vector<long long>;
using vvi    = std::vector<std::vector<int>>;
using vvll   = std::vector<std::vector<long long>>;
using vs     = std::vector<std::string>;
using vpii   = std::vector<std::pair<int, int>>;
using vpll   = std::vector<std::pair<long long, long long>>;
using map_ll = std::map<long long, long long>;

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
/* Data Types and Function Definitions */

// Solver class encapsulating the logic for the tree contraction.
class TreeContractionSolver {
private:
  int num_nodes;
  vvi tree_adj;
  vi  node_degree;

  // DP arrays
  vi  subtree_node_count;
  vi  subtree_leaves;
  vll ops_for_subtree;

  // Result variable
  ll min_operations_found;

  // First DFS to compute subtree statistics
  void dfs1_calculate_subtree_stats(int current, int parent) {
    subtree_node_count[current] = 1;
    // A node is a leaf if its degree in the original graph is 1.
    subtree_leaves[current]  = (node_degree[current] == 1);
    ops_for_subtree[current] = 0;

    for (int neighbor : tree_adj[current]) {
      if (neighbor == parent)
        continue;

      dfs1_calculate_subtree_stats(neighbor, current);

      // Aggregate results from child's subtree
      subtree_node_count[current] += subtree_node_count[neighbor];
      subtree_leaves[current] += subtree_leaves[neighbor];

      // An operation is needed for a child's branch only if it's not a single leaf.
      // The cost of the operation equals the number of leaves in that branch.
      if (subtree_node_count[neighbor] > 1) {
        ops_for_subtree[current] += subtree_leaves[neighbor];
      }
    }
  }

  // Second DFS to find the optimal root by considering the "rest of the tree".
  void dfs2_find_optimal_root(int current, int parent) {
    // The "rest of the tree" is the component connected via the parent.
    // We calculate the number of leaves and nodes in this "parent component".
    ll total_leaves_in_tree = subtree_leaves[1];

    ll leaves_in_parent_component = total_leaves_in_tree - subtree_leaves[current];
    ll nodes_in_parent_component  = num_nodes - subtree_node_count[current];

    ll ops_for_parent_component = 0;
    // The parent component needs flattening only if it contains more than one node.
    if (nodes_in_parent_component > 1) {
      ops_for_parent_component = leaves_in_parent_component;
    }

    ll total_ops_for_current_root = ops_for_subtree[current] + ops_for_parent_component;
    min_operations_found          = min(min_operations_found, total_ops_for_current_root);

    // Continue traversal to evaluate all nodes.
    for (int neighbor : tree_adj[current]) {
      if (neighbor != parent) {
        dfs2_find_optimal_root(neighbor, current);
      }
    }
  }

public:
  // Constructs the solver for a given tree.
  explicit TreeContractionSolver(int n, const vector<pair<int, int>>& edges) : num_nodes(n) {
    tree_adj.assign(n + 1, vi());
    node_degree.assign(n + 1, 0);
    for (const auto& edge : edges) {
      int u = edge.first;
      int v = edge.second;
      tree_adj[u].push_back(v);
      tree_adj[v].push_back(u);
      node_degree[u]++;
      node_degree[v]++;
    }
  }

  // Executes the algorithm and returns the minimum number of operations.
  ll solve() {
    if (num_nodes <= 2) {
      return 0;
    }

    // Allocate memory for DP arrays.
    subtree_node_count.assign(num_nodes + 1, 0);
    subtree_leaves.assign(num_nodes + 1, 0);
    ops_for_subtree.assign(num_nodes + 1, 0);

    // Run DFS from an arbitrary root (e.g., node 1) to compute base stats.
    dfs1_calculate_subtree_stats(1, 0);

    // Initialize the minimum with the result for root 1, then
    // traverse the tree again to find the true optimal root.
    min_operations_found = ops_for_subtree[1];
    dfs2_find_optimal_root(1, 0);

    return min_operations_found;
  }
};

// Main function to handle test cases
void run_test_case() {
  int n;
  cin >> n;
  vector<pair<int, int>> edges(n - 1);
  for (int i = 0; i < n - 1; ++i) {
    cin >> edges[i].first >> edges[i].second;
  }

  TreeContractionSolver solver(n, edges);
  cout << solver.solve() << '\n';
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
    run_test_case();
  }

  return 0;
}

//===----------------------------------------------------------------------===//