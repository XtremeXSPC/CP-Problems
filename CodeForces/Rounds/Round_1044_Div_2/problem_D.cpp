//===----------------------------------------------------------------------===//
/**
 * @file: problem_D.cpp
 * @brief Codeforces Round 1044 (Div. 2) - Problem D
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

// Function to solve a single test case
void solve() {
  int n;
  cin >> n;
  vll mob_healths(n);
  ll  total_health = 0LL;
  for (int i = 0; i < n; ++i) {
    cin >> mob_healths[i];
    total_health += mob_healths[i];
  }

  // DP for max weight independent set
  ll max_savings_at_prev      = 0; // dp[i-1]
  ll max_savings_at_prev_prev = 0; // dp[i-2]

  for (int i = 1; i < n; ++i) {
    ll mobs_below     = i;
    ll current_health = mob_healths[i];

    // Calculate extra saving potential
    ll potential_extra_saving = std::max(0LL, std::min(mobs_below, current_health) - 2);

    // DP transition: dp[i] = max(dp[i-1], dp[i-2] + weight[i])
    ll current_max_savings = std::max(max_savings_at_prev, max_savings_at_prev_prev + potential_extra_saving);

    // Update DP state
    max_savings_at_prev_prev = max_savings_at_prev;
    max_savings_at_prev      = current_max_savings;
  }

  ll total_extra_savings = max_savings_at_prev;
  ll base_savings        = n - 1;
  ll min_attacks         = total_health - base_savings - total_extra_savings;
  cout << min_attacks << "\n";
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
