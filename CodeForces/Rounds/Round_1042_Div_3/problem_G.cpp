//===----------------------------------------------------------------------===//
/**
 * @file: problem_G.cpp
 * @brief: Codeforces Round 1042 (Div. 3) - Problem G
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

// Simulates the score calculation using a state machine approach.
class ProcessSimulator {
private:
  ll         ops_remaining;
  ll         score_product;
  vector<ll> milestones;
  size_t     next_milestone_idx;
  ll         current_gap;

  // Pre-computed values for the H(g) function.
  vector<ll> full_clear_scores;

  // Computes (base^exp) % MOD using a recursive approach.
  ll power_recursive(ll base, ll exp) {
    if (exp == 0)
      return 1;
    if (exp == 1)
      return base % MOD;
    ll half = power_recursive(base, exp / 2);
    ll res  = (half * half) % MOD;
    if (exp % 2 == 1) {
      res = (res * base) % MOD;
    }
    return res;
  }

  // Calculates score for a partial gap clear, iterating by powers of two.
  ll score_for_partial_gap_clear(ll op_count) {
    if (op_count == 0)
      return 1;
    ll product = 1;
    for (ll term = 1, multiplier = 1; term <= op_count; term <<= 1, ++multiplier) {
      ll num_occurrences = (op_count / term + 1) / 2;
      if (num_occurrences > 0) {
        product = (product * power_recursive(multiplier, num_occurrences)) % MOD;
      }
    }
    return product;
  }

  // Pre-computes H(g) values iteratively up to a safe limit.
  void precompute_full_clear_scores(int limit) {
    full_clear_scores.assign(limit + 1, 1);
    for (int g = 2; g <= limit; ++g) {
      ll prev_h            = full_clear_scores[g - 1];
      full_clear_scores[g] = (ll)(g - 1) * prev_h % MOD * prev_h % MOD;
    }
  }

public:
  ProcessSimulator(ll k, const set<ll>& initial_elements) : ops_remaining(k), score_product(1), next_milestone_idx(0) {
    precompute_full_clear_scores(60); // Pre-calculate H for all relevant gap sizes.

    current_gap = 1;
    while (initial_elements.count(current_gap)) {
      current_gap++;
    }

    for (ll val : initial_elements) {
      if (val > current_gap) {
        milestones.push_back(val);
      }
    }
    sort(milestones.begin(), milestones.end());
  }

  ll run() {
    while (ops_remaining > 0) {
      // STATE 1: CLEARING_GAP
      if (current_gap > 1) {
        ll ops_to_clear_gap = -1;
        if (current_gap - 1 < 60) {
          ops_to_clear_gap = (1LL << (current_gap - 1)) - 1;
        }

        if (ops_to_clear_gap != -1 && ops_remaining >= ops_to_clear_gap) {
          // We have enough ops to complete the entire phase.
          ops_remaining -= ops_to_clear_gap;
          score_product = (score_product * full_clear_scores[current_gap]) % MOD;
        } else {
          // Not enough ops. Spend the rest here and terminate.
          score_product = (score_product * score_for_partial_gap_clear(ops_remaining)) % MOD;
          ops_remaining = 0;
          break;
        }
      }

      if (ops_remaining == 0)
        break;

      // STATE 2: EXTRACTING_MINIMUM
      if (next_milestone_idx < milestones.size()) {
        ll next_val   = milestones[next_milestone_idx];
        score_product = (score_product * next_val) % MOD;
        ops_remaining--;
        // The new gap is the value we just conceptually extracted.
        current_gap = next_val;
        next_milestone_idx++;
      } else {
        // No more milestones. Spend all remaining ops in the final gap-clearing state.
        score_product = (score_product * score_for_partial_gap_clear(ops_remaining)) % MOD;
        ops_remaining = 0;
        break;
      }
    }
    return score_product;
  }
};

// Function to solve a single test case.
void solve() {
  int n;
  ll  k;
  cin >> n >> k;
  set<ll> initial_elements;
  for (int i = 0; i < n; ++i) {
    ll val;
    cin >> val;
    initial_elements.insert(val);
  }

  ProcessSimulator sim(k, initial_elements);
  cout << sim.run() << "\n";
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