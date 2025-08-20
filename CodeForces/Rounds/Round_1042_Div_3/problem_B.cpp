//===----------------------------------------------------------------------===//
/**
 * @file: problem_B.cpp
 * @brief Codeforces Round 1042 (Div. 3) - Problem B
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

  for (int i = 0; i < n; ++i) {
    if (i % 2 == 0) {
      cout << -1;
    } else {
      // i is odd => 1-indexed even position
      if (i == n - 1)
        cout << 2; // last element when n is even
      else
        cout << 3; // internal even positions need >=3
    }
    if (i < n - 1)
      cout << ' ';
  }
  cout << '\n';
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
