//===----------------------------------------------------------------------===//
/**
 * @file: problem_A.cpp
 * @brief Codeforces Round 1042 (Div. 3) - Problem A
 * @author: Costantino Lombardi
 *
 * @status: PASSED
 */
//===----------------------------------------------------------------------===//
/* Included library */

#include <bits/stdc++.h>
#include <ctime>
using namespace std;

//===----------------------------------------------------------------------===//
/* Macros and Type Aliases */

// Debug macro:
#ifdef LOCAL
#include "debug.h"
#else
#define debug(...) 42
#endif

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
  Timer timer;

  size_t n;
  cin >> n;
  vi a(n), b(n);
  ll total_excess = 0;

  for (size_t i = 0; i < n; i++) {
    cin >> a[i];
  }

  for (size_t i = 0; i < n; i++) {
    cin >> b[i];
  }

  for (size_t i = 0; i < n; i++) {
    if (a[i] > b[i]) {
      total_excess += a[i] - b[i];
    }
  }

  cout << total_excess + 1 << endl;
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
