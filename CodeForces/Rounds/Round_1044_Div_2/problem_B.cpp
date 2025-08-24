//===----------------------------------------------------------------------===//
/**
 * @file: problem_B.cpp
 * @brief Codeforces Round 1044 (Div. 2) - Problem B
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

// Function to solve a single test case
void solve() {
  int num_villagers;
  cin >> num_villagers;

  vll grumpiness_levels(num_villagers);
  for (ll& level : grumpiness_levels) {
    cin >> level;
  }

  // Sort grumpiness levels for optimal strategy
  std::ranges::sort(grumpiness_levels);

  // Sum elements with stride 2, offset based on parity
  int  offset          = (num_villagers % 2 == 0);
  auto elements_to_sum = grumpiness_levels | std::views::drop(offset) | std::views::stride(2);
  ll   total_emeralds  = std::accumulate(elements_to_sum.begin(), elements_to_sum.end(), 0LL);

  cout << total_emeralds << endl;
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
