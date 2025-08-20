//===----------------------------------------------------------------------===//
/**
 * @file: problem_E.cpp
 * @brief Codeforces Round 1042 (Div. 3) - Problem E
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
using vui  = vector<unsigned int>;

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
  vui a(n), b(n);
  for (int i = 0; i < n; i++)
    cin >> a[i];
  for (int i = 0; i < n; i++)
    cin >> b[i];

  if (a[n - 1] != b[n - 1]) {
    cout << "NO\n";
    return;
  }

  vui d(n - 1);
  for (int i = 0; i < n - 1; i++) {
    d[i] = a[i] ^ b[i];
  }

  bool ok = true;
  for (int i = 0; i < n - 1 && ok;) {
    if (d[i] == 0) {
      ++i;
      continue;
    }
    int j = i;
    while (true) {
      if (d[j] == a[j + 1]) {
        i = j + 1;
        break;
      }
      if (j == n - 2) {
        ok = false;
        break;
      }
      if (d[j + 1] != (d[j] ^ a[j + 1])) {
        ok = false;
        break;
      }
      ++j;
    }
  }
  cout << (ok ? "YES" : "NO") << '\n';
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
