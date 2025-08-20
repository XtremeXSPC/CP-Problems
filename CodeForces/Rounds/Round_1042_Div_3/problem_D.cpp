//===----------------------------------------------------------------------===//
/**
 * @file: problem_D.cpp
 * @brief: Codeforces Round 1042 (Div. 3) - Problem D
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
using vi   = vector<int>;
using pii  = pair<int, int>;
using vll  = vector<ll>;
using vpii = vector<pii>;

// Constants
constexpr int MOD  = 1000000007;
constexpr int INF  = 1e9;
constexpr ll  LINF = 1e18;

//===----------------------------------------------------------------------===//
/* Data Types and Function Definitions */

// Function to solve a single test case
void solve() {
  int n;
  cin >> n;
  vector<vi> adj(n + 1);
  vi         deg(n + 1, 0);
  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
    deg[u]++;
    deg[v]++;
  }

  bool is_star = false;
  for (int i = 1; i <= n; ++i) {
    if (deg[i] == n - 1) {
      is_star = true;
      break;
    }
  }
  if (is_star) {
    cout << 0 << '\n';
    return;
  }

  vi  sz(n + 1);
  vi  leaves_down(n + 1);
  vll ops_down(n + 1, 0);

  // DFS to calculate properties for subtrees rooted at 1
  function<void(int, int)> dfs1 = [&](int u, int p) {
    sz[u]          = 1;
    leaves_down[u] = (deg[u] == 1);
    ops_down[u]    = 0;

    for (int v : adj[u]) {
      if (v == p)
        continue;
      dfs1(v, u);
      sz[u] += sz[v];
      leaves_down[u] += leaves_down[v];
      if (sz[v] > 1) {
        ops_down[u] += leaves_down[v];
      }
    }
  };

  dfs1(1, 0);

  ll min_ops      = -1;
  ll total_leaves = leaves_down[1];

  // Re-rooting DP: calculate the cost for each node as the root
  for (int i = 1; i <= n; ++i) {
    ll current_ops = ops_down[i];
    if (i != 1) {
      ll parent_comp_size = n - sz[i];
      if (parent_comp_size > 1) {
        current_ops += (total_leaves - leaves_down[i]);
      }
    }
    if (min_ops == -1 || current_ops < min_ops) {
      min_ops = current_ops;
    }
  }

  cout << min_ops << '\n';
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