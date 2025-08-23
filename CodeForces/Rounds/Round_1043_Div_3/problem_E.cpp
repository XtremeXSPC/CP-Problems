//===----------------------------------------------------------------------===//
/**
 * @file: problem_E.cpp
 * @brief: Codeforces Round 1043 (Div. 3) - Problem E
 * @author: Costantino Lombardi
 *
 * @status: PASSED
 */
//===----------------------------------------------------------------------===//
/* Included library and Macros */

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
  // Read the number of cards for Vadim, Kostya, and the number of queries.
  ll n, m, q;
  cin >> n >> m >> q;

  vll vadim_cards(n);
  for (int i = 0; i < n; ++i) {
    cin >> vadim_cards[i];
  }

  vll kostya_cards(m);
  for (int i = 0; i < m; ++i) {
    cin >> kostya_cards[i];
  }

  // Sort both card lists in descending order to maximize the sum of the top cards.
  sort(vadim_cards.rbegin(), vadim_cards.rend());
  sort(kostya_cards.rbegin(), kostya_cards.rend());

  // Precompute prefix sums to quickly get the sum of the top k cards.
  vll vadim_prefix_sum(n + 1, 0);
  for (int i = 0; i < n; ++i) {
    vadim_prefix_sum[i + 1] = vadim_prefix_sum[i] + vadim_cards[i];
  }

  vll kostya_prefix_sum(m + 1, 0);
  for (int i = 0; i < m; ++i) {
    kostya_prefix_sum[i + 1] = kostya_prefix_sum[i] + kostya_cards[i];
  }

  // Process each query online.
  for (int i = 0; i < q; ++i) {
    ll x, y, z;
    cin >> x >> y >> z;

    // Determine the valid range [min_vadim_cards, max_vadim_cards] for the
    // number of cards Vadim can pick.
    ll min_vadim_cards = max({0LL, z - m, z - y});
    ll max_vadim_cards = min({n, x, z});

    if (min_vadim_cards > max_vadim_cards) {
      cout << 0 << "\n";
      continue;
    }

    // The function sum(k_a) = vadim_prefix_sum[k_a] +
    // kostya_prefix_sum[z - k_a] is concave. We can find its maximum over
    // the valid range using binary search on its derivative.
    ll low                 = min_vadim_cards;
    ll high                = max_vadim_cards;
    ll optimal_vadim_cards = min_vadim_cards;

    // Binary search for the optimal number of cards from Vadim.
    while (low <= high) {
      ll mid = low + (high - low) / 2;

      bool can_improve = true;
      if (mid == 0) {
        // Taking 0 from Vadim. It's almost always better to take more if possible.
        can_improve = true;
      } else if (mid > z) {
        can_improve = false;
      } else if (z - mid >= m) {
        // Kostya's pool is full, must take from Vadim if available.
        can_improve = true;
      } else {
        // Compare the card we would add from Vadim with the one we would give
        // up from Kostya.
        can_improve = (vadim_cards[mid - 1] > kostya_cards[z - mid]);
      }

      if (can_improve) {
        // If it's profitable to take 'mid' cards from Vadim, we try to take even more.
        optimal_vadim_cards = mid;
        low                 = mid + 1;
      } else {
        // If it is not profitable, 'mid' is too high.
        high = mid - 1;
      }
    }

    ll total_sum = vadim_prefix_sum[optimal_vadim_cards] + kostya_prefix_sum[z - optimal_vadim_cards];
    cout << total_sum << "\n";
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
