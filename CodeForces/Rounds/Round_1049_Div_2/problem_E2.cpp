//===----------------------------------------------------------------------===//
/**
 * @file: problem_E2.cpp
 * @brief Codeforces Round 1049 (Div. 2) - Problem E2
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
using V_c      = std::vector<char>;
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

constexpr int MAXN_FACTORIAL = 25;

using namespace std;

//===----------------------------------------------------------------------===//
/* Precomputed Combinatorics & Math Helpers */

// Factorials and Inverse Factorials storage.
V_ll fact, inv_fact;

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

// Modular inverse using Fermat's little theorem.
ll mod_inverse(ll n) {
  return power(n, MOD - 2);
}

// Precompute factorials and inverse factorials.
void precompute_factorials(int n) {
  fact.resize(n + 1);
  inv_fact.resize(n + 1);
  fact[0] = inv_fact[0] = 1;
  for (int i = 1; i <= n; i++) {
    fact[i]     = (fact[i - 1] * i) % MOD;
    inv_fact[i] = mod_inverse(fact[i]);
  }
}

// Binomial coefficient C(n,k).
ll combinations(int n, int k) {
  if (k < 0 || k > n) return 0;
  return (((fact[n] * inv_fact[k]) % MOD) * inv_fact[n - k]) % MOD;
}

// Sum of i^p for i=0 to M using Lagrange interpolation.
ll sum_powers(ll M, int p) {
  if (M < 0) return 0;
  M %= MOD;
  int  k = p + 1;
  V_ll y(k + 1, 0);
  y[0] = (p == 0);
  for (int i = 1; i <= k; ++i) {
    y[i] = (y[i - 1] + power(i, p)) % MOD;
  }
  if (M <= k) return y[M];

  // Build prefix and suffix products for interpolation.
  V_ll pref(k + 2, 1), suff(k + 2, 1);
  for (int i = 0; i <= k; ++i) pref[i + 1] = (pref[i] * (M - i + MOD)) % MOD;
  for (int i = k; i >= 0; --i) suff[i] = (suff[i + 1] * (M - i + MOD)) % MOD;

  ll total_sum = 0;
  for (int i = 0; i <= k; ++i) {
    ll numerator   = (pref[i] * suff[i + 1]) % MOD;
    ll denominator = (inv_fact[i] * inv_fact[k - i]) % MOD;
    ll term        = (y[i] * numerator) % MOD;
    term           = (term * denominator) % MOD;
    if ((k - i) % 2 == 1)
      total_sum = (total_sum - term + MOD) % MOD;
    else
      total_sum = (total_sum + term) % MOD;
  }
  return total_sum;
}

// Remove i-th bit from mask (1-indexed).
static constexpr unsigned get_next_mask(unsigned current_mask, int i) {
  unsigned lower_bits  = current_mask & ((1u << (i - 1)) - 1);
  unsigned higher_bits = current_mask >> i;
  return lower_bits | (higher_bits << (i - 1));
}

// Function to solve a single test case.
void solve() {
  int n;
  ll  m;
  int k_count;
  cin >> n >> m >> k_count;
  V_i good_indices(k_count);
  for (int& idx : good_indices) cin >> idx;

  if (m == 1) {
    cout << 1 << '\n';
    return;
  }

  // Precompute available moves for each pile size.
  VV_i moves_by_size(n + 1);
  for (int p = 1; p <= n; ++p) {
    for (int move : good_indices)
      if (move <= p) moves_by_size[p].push_back(move);
  }

  // DP to determine winning/losing states.
  V_c dp_prev_level = {0, 1};
  for (int p = 2; p <= n; ++p) {
    V_c         dp_current_level(1u << p);
    const bool  is_alice_turn   = ((n - p) % 2 == 0);
    const auto& available_moves = moves_by_size[p];

    for (unsigned S = 0; S < (1u << p); ++S) {
      auto outcomes_view = available_moves | views::transform([&](int move) {
                             return (bool)dp_prev_level[get_next_mask(S, move)];
                           });
      if (is_alice_turn)
        dp_current_level[S] = ranges::any_of(outcomes_view, identity{});
      else
        dp_current_level[S] = ranges::all_of(outcomes_view, identity{});
    }
    dp_prev_level.swap(dp_current_level);
  }
  const V_c& winning_masks = dp_prev_level;

  // Count winning masks by population count.
  V_ll winning_masks_by_popcount(n + 1, 0);
  for (unsigned S = 0; S < (1u << n); ++S) {
    if (winning_masks[S]) {
      winning_masks_by_popcount[__builtin_popcount(S)]++;
    }
  }

  // Precompute power sums and powers of 'm'.
  V_ll S_powers(n + 1);
  for (int r = 0; r <= n; ++r) S_powers[r] = sum_powers(m - 1, r);
  V_ll m_powers(n + 1, 1);
  for (int e = 1; e <= n; ++e) m_powers[e] = (m_powers[e - 1] * (m % MOD)) % MOD;

  // Calculate final answer using inclusion-exclusion.
  ll final_answer = 0;
  for (int s = 0; s <= n; ++s) {  // s = piles Alice can choose from.
    if (winning_masks_by_popcount[s] == 0) continue;

    ll  inner_sum = 0;
    int d         = n - s;  // d = piles Bob can choose from.
    for (int j = 0; j <= s; ++j) {
      ll term = (combinations(s, j) * m_powers[s - j]) % MOD;
      term    = (term * S_powers[j + d]) % MOD;
      if (j % 2 == 1)
        inner_sum = (inner_sum - term + MOD) % MOD;
      else
        inner_sum = (inner_sum + term) % MOD;
    }
    final_answer = (final_answer + winning_masks_by_popcount[s] * inner_sum) % MOD;
  }

  cout << final_answer << '\n';
}

//===----------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  precompute_factorials(MAXN_FACTORIAL);

  int T = 1;
  cin >> T;
  for ([[maybe_unused]] auto _ : views::iota(0, T)) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//
