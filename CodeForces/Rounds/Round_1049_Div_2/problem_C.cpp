//===----------------------------------------------------------------------===//
/**
 * @file: problem_C.cpp
 * @brief Codeforces Round 1049 (Div. 2) - Problem C
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

// Type aliases:
using ll       = long long;
using ull      = unsigned long long;
using ld       = long double;
using P_ii     = std::pair<int, int>;
using P_ll     = std::pair<long long, long long>;
using V_b      = std::vector<bool>;
using V_i      = std::vector<int>;
using V_ui     = std::vector<unsigned int>;
using V_ll     = std::vector<long long>;
using VV_i     = std::vector<std::vector<int>>;
using VV_ll    = std::vector<std::vector<long long>>;
using V_s      = std::vector<std::string>;
using VV_s     = std::vector<std::vector<std::string>>;
using VP_ii    = std::vector<std::pair<int, int>>;
using VVP_ii   = std::vector<std::vector<std::pair<int, int>>>;
using VP_ll    = std::vector<std::pair<long long, long long>>;
using VT_iii   = std::vector<std::tuple<int, int, int>>;
using Map_ll   = std::map<long long, long long>;
using VUMap_il = std::vector<std::unordered_map<int, ll>>;

// Mathematical constants:
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
  int n;
  cin >> n;

  V_ll sequence(n);
  for (auto& value : sequence) {
    cin >> value;
  }

  // Base score if Alice makes no swaps.
  const auto baseline_score = ranges::fold_left(views::iota(0, n), 0LL, [&](ll accumulator, int i) {
    return accumulator + (i % 2 == 0 ? sequence[i] : -sequence[i]);
  });

  ll max_swap_benefit = 0LL;

  // Case 1: Swap between indices with same parity.
  if (n > 1) {
    max_swap_benefit = (n % 2 != 0) ? (ll)n - 1 : (ll)n - 2;
  }

  // Case 2: Swap between indices with different parity.
  optional<ll> min_even_prefix_metric;
  optional<ll> max_odd_prefix_metric;

  auto even_idx_metric = [](ll val, ll idx) { return 2 * val + idx; };
  auto odd_idx_metric  = [](ll val, ll idx) { return 2 * val - idx; };

  for (const auto& [idx, val] : sequence | views::enumerate) {
    if (idx % 2 == 0) {  // Even index.
      // Can be `r` for a previous odd `l`.
      if (max_odd_prefix_metric.has_value()) {
        const ll current_benefit = *max_odd_prefix_metric - odd_idx_metric(val, idx);
        max_swap_benefit         = max(max_swap_benefit, current_benefit);
      }

      // Can be a future even `l`.
      const ll current_metric = even_idx_metric(val, idx);
      min_even_prefix_metric = min(min_even_prefix_metric.value_or(current_metric), current_metric);

    } else {  // Odd index.
      // Can be `r` for a previous even `l`.
      if (min_even_prefix_metric.has_value()) {
        const ll current_benefit = even_idx_metric(val, idx) - *min_even_prefix_metric;
        max_swap_benefit         = max(max_swap_benefit, current_benefit);
      }

      // Can be a future odd `l`.
      const ll current_metric = odd_idx_metric(val, idx);
      max_odd_prefix_metric   = max(max_odd_prefix_metric.value_or(current_metric), current_metric);
    }
  }

  cout << baseline_score + max_swap_benefit << "\n";
}

//===----------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int T = 1;
  cin >> T;
  for ([[maybe_unused]] auto _ : views::iota(0, T)) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//
