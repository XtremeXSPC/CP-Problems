//===----------------------------------------------------------------------===//
/**
 * @file: problem_B.cpp
 * @brief Codeforces Round 1046 (Div. 2) - Problem B
 * @author: Costantino Lombardi
 *
 * @status: PASSED
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

// Function to solve a single test case.
void solve() {
  int    n;
  ll     k;
  string s;
  cin >> n >> k >> s;

  // Check if there's a contiguous block of k or more '1's.
  int  consecutive_ones = 0;
  bool possible         = true;
  for (char bit : s) {
    if (bit == '1') {
      consecutive_ones++;
    } else {
      consecutive_ones = 0;
    }
    if (consecutive_ones >= k) {
      possible = false;
      break;
    }
  }

  if (!possible) {
    cout << "NO\n";
    return;
  }

  cout << "YES\n";

  // Assign high values to '0' positions and low values to '1' positions.
  vi p(n);
  vi indices(n);
  iota(indices.begin(), indices.end(), 0);

  // Partition indices: '0's first, then '1's.
  auto partition_point = partition(indices.begin(), indices.end(), [&](int index) { return s[index] == '0'; });

  // Assign permutation values.
  int current_value = n;

  for (auto it = indices.begin(); it != partition_point; ++it) {
    p[*it] = current_value--;
  }
  for (auto it = partition_point; it != indices.end(); ++it) {
    p[*it] = current_value--;
  }

  // Print the resulting permutation.
  if (n > 0) {
    cout << p[0];
    for (int i = 1; i < n; ++i) {
      cout << ' ' << p[i];
    }
  }
  cout << '\n';
}

//===----------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int t = 1;
  cin >> t;

  for ([[maybe_unused]] auto _ : views::iota(0, t)) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//
