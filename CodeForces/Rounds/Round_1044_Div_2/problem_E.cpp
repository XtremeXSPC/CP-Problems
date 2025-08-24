//===----------------------------------------------------------------------===//
/**
 * @file: problem_E.cpp
 * @brief Codeforces Round 1044 (Div. 2) - Problem E
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

// Helper struct for tree operations
struct TreeProcessor {
  int                 n;     // Number of nodes
  vector<vector<int>> adj;   // Adjacency list
  vpii                moves; // Generated moves (operation, node)

  TreeProcessor(int size) : n(size), adj(size + 1) {}

  // Add undirected edge between nodes a and b
  void addEdge(int a, int b) {
    adj[a].push_back(b);
    adj[b].push_back(a);
  }

  // Generate path traversal for linear trees
  void generatePathMoves() {
    vector<int> degree(n + 1, 0);
    for (int node = 1; node <= n; ++node)
      degree[node] = static_cast<int>(adj[node].size());

    // Find leaf node as starting point
    int start = 1;
    for (int node = 1; node <= n; ++node) {
      if (degree[node] == 1) {
        start = node;
        break;
      }
    }

    // Traverse path from leaf to leaf
    int prev = 0, curr = start;
    for (int step = 0; step < n; ++step) {
      moves.push_back({1, curr});
      int next = 0;
      for (int neighbor : adj[curr]) {
        if (neighbor != prev) {
          next = neighbor;
          break;
        }
      }
      prev = curr;
      curr = next;
      if (!curr)
        break;
    }
  }

  // Check if tree is a path (all nodes have degree <= 2)
  bool isLinearPath() const {
    for (int node = 1; node <= n; ++node) {
      if (static_cast<int>(adj[node].size()) > 2)
        return false;
    }
    return true;
  }
};

// Dynamic programming state for tree decomposition
struct DPState {
  vector<ll>          exclude;  // DP: Node not in selected set
  vector<ll>          include0; // DP: Node in set, no parent restriction
  vector<ll>          include1; // DP: Node in set, with parent restriction
  vector<int>         parent;
  vector<int>         bfsOrder;
  vector<vector<int>> children;
  vector<int>         selected;

  DPState(int n) : exclude(n + 1), include0(n + 1), include1(n + 1), parent(n + 1), selected(n + 1) { children.resize(n + 1); }

  // Build rooted tree from adjacency list using BFS
  void buildTree(const vector<vector<int>>& adj, int root) {
    bfsOrder.push_back(root);
    parent[root] = -1;

    for (size_t idx = 0; idx < bfsOrder.size(); ++idx) {
      int curr = bfsOrder[idx];
      for (int neighbor : adj[curr]) {
        if (neighbor != parent[curr]) {
          parent[neighbor] = curr;
          bfsOrder.push_back(neighbor);
        }
      }
    }

    // Build children lists
    for (int node = 2; node <= static_cast<int>(parent.size()) - 1; ++node) {
      if (parent[node] != 0)
        children[parent[node]].push_back(node);
    }
  }

  // Compute DP values bottom-up
  void computeDP() {
    for (int idx = static_cast<int>(bfsOrder.size()) - 1; idx >= 0; --idx) {
      int node = bfsOrder[idx];

      // DP for excluding current node
      ll sumMax = 0;
      for (int child : children[node]) {
        sumMax += max(exclude[child], include0[child]);
      }
      exclude[node] = sumMax;

      // DP for including current node
      ll baseSum = 0;
      for (int child : children[node]) {
        baseSum += exclude[child];
      }

      // Find best gains from including children
      ll best1 = 0, best2 = 0;
      for (int child : children[node]) {
        ll gain = include1[child] - exclude[child];
        if (gain > best1) {
          best2 = best1;
          best1 = gain;
        } else if (gain > best2) {
          best2 = gain;
        }
      }

      include1[node] = 1 + baseSum + max(0LL, best1);
      include0[node] = 1 + baseSum + max(0LL, best1) + max(0LL, best2);
    }
  }

  // Reconstruct optimal selection from DP values
  void reconstructSelection(int node, int parentIncluded, int mustInclude) {
    selected[node] = mustInclude;

    if (!mustInclude) {
      // Node not selected, choose best children
      for (int child : children[node]) {
        if (include0[child] >= exclude[child]) {
          reconstructSelection(child, 0, 1);
        } else {
          reconstructSelection(child, 0, 0);
        }
      }
    } else {
      // Node selected, choose best children respecting constraints
      vector<pair<ll, int>> childGains;
      for (int child : children[node]) {
        childGains.push_back({include1[child] - exclude[child], child});
      }

      ranges::sort(childGains, greater<>());

      int      maxSelect = parentIncluded ? 1 : 2;
      set<int> chosenChildren;

      for (int i = 0; i < min(maxSelect, static_cast<int>(childGains.size())); ++i) {
        if (childGains[i].first > 0) {
          chosenChildren.insert(childGains[i].second);
        }
      }

      for (int child : children[node]) {
        if (chosenChildren.count(child)) {
          reconstructSelection(child, 1, 1);
        } else {
          reconstructSelection(child, 1, 0);
        }
      }
    }
  }
};

// Function to solve a single test case
void solve() {
  int n;
  cin >> n;

  TreeProcessor tree(n);

  // Read tree edges
  for (int i = 0; i < n - 1; ++i) {
    int a, b;
    cin >> a >> b;
    tree.addEdge(a, b);
  }

  // Handle base case n=2
  if (n == 2) {
    cout << 2 << endl;
    cout << "1 1" << endl;
    cout << "1 2" << endl;
    return;
  }

  // Handle linear trees (n=3 or path)
  if (n == 3 || tree.isLinearPath()) {
    tree.generatePathMoves();
    cout << tree.moves.size() << endl;
    for (const auto& [op, node] : tree.moves) {
      cout << op << " " << node << endl;
    }
    return;
  }

  // General tree case using DP
  DPState dp(n);
  dp.buildTree(tree.adj, 1);
  dp.computeDP();

  // Choose optimal root decision
  if (dp.include0[1] >= dp.exclude[1]) {
    dp.reconstructSelection(1, 0, 1);
  } else {
    dp.reconstructSelection(1, 0, 0);
  }

  vpii operations;

  // Remove non-selected nodes
  for (int node = 1; node <= n; ++node) {
    if (!dp.selected[node]) {
      operations.push_back({1, node});
      operations.push_back({2, node});
    }
  }

  // Process selected components
  vector<int> componentDegree(n + 1, 0);
  vector<int> visited(n + 1, 0);

  // Calculate degrees within selected subgraph
  for (int node = 1; node <= n; ++node) {
    if (dp.selected[node]) {
      for (int neighbor : tree.adj[node]) {
        if (dp.selected[neighbor]) {
          ++componentDegree[node];
        }
      }
    }
  }

  // Process paths in selected components
  for (int node = 1; node <= n; ++node) {
    if (dp.selected[node] && componentDegree[node] <= 1 && !visited[node]) {
      int prev = 0, curr = node;
      while (curr) {
        visited[curr] = 1;
        operations.push_back({1, curr});

        int next = 0;
        for (int neighbor : tree.adj[curr]) {
          if (dp.selected[neighbor] && neighbor != prev) {
            next = neighbor;
            break;
          }
        }
        prev = curr;
        curr = next;
      }
    }
  }

  // Fallback to path traversal if operations exceed threshold
  ll threshold = (5LL * n) / 4;
  if (static_cast<ll>(operations.size()) > threshold) {
    tree.moves.clear();
    tree.generatePathMoves();
    operations = tree.moves;
  }

  // Output result
  cout << operations.size() << endl;
  for (const auto& [op, node] : operations) {
    cout << op << " " << node << endl;
  }
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
