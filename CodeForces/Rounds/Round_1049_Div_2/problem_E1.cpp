//===----------------------------------------------------------------------===//
/**
 * @file: problem_E1.cpp
 * @brief Codeforces Round 1049 (Div. 2) - Problem E1
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

// Computes (base^exp) % MOD.
static constexpr ll power(ll base, ll exp) {
  ll res = 1;
  base %= MOD;
  while (exp > 0) {
    if (exp % 2 == 1) res = (res * base) % MOD;
    base = (base * base) % MOD;
    exp /= 2;
  }
  return res;
}

// Removes i-th bit (1-indexed) from mask.
static constexpr unsigned get_next_mask(unsigned current_mask, int i) {
  unsigned lower_bits  = current_mask & ((1u << (i - 1)) - 1);
  unsigned higher_bits = current_mask >> i;
  return lower_bits | (higher_bits << (i - 1));
}

// Function to solve a single test case.
void solve() {
  int n, m, k;
  cin >> n >> m >> k;
  V_i initial_moves(k);
  for (int& move : initial_moves) cin >> move;

  if (m == 1) {  // Edge case for E2.
    cout << 1 << '\n';
    return;
  }

  // Precompute valid moves per game size.
  VV_i moves_by_size(n + 1);
  for (int p = 1; p <= n; ++p) {
    for (int move : initial_moves) {
      if (move <= p) {
        moves_by_size[p].push_back(move);
      }
    }
  }

  // DP: dp[p][S] = Alice wins from size p with config S.
  V_b dp_prev_level = {false, true};

  for (int p = 2; p <= n; ++p) {
    V_b         dp_current_level(1u << p);
    const bool  is_alice_turn   = ((n - p) % 2 == 0);
    const auto& available_moves = moves_by_size[p];

    for (unsigned S = 0; S < (1u << p); ++S) {
      auto outcomes_view = available_moves | views::transform([&](int move) {
                             unsigned next_mask = get_next_mask(S, move);
                             return dp_prev_level[next_mask];
                           });

      if (is_alice_turn) {
        // Alice wins if ANY move leads to win.
        dp_current_level[S] = ranges::any_of(outcomes_view, [](bool v) { return v; });
      } else {
        // Alice wins only if ALL Bob's moves lead to win.
        dp_current_level[S] = ranges::all_of(outcomes_view, [](bool v) { return v; });
      }
    }
    dp_prev_level.swap(dp_current_level);
  }

  ll alice_win_count = ranges::count(dp_prev_level, true);
  ll total_configs   = power(2, n);
  ll final_answer    = (total_configs + alice_win_count) % MOD;

  cout << final_answer << '\n';
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
