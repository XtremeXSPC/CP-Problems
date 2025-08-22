//===----------------------------------------------------------------------===//
/**
 * @file: problem_C2.cpp
 * @brief: Codeforces Round 1043 (Div. 3) - Problem C2
 * @author: Costantino Lombardi
 *
 * @status: PASSED
 */
//===----------------------------------------------------------------------===//
/* Included library and Macros */

// clang-format off
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

// A table to store powers of 3, pre-calculated for efficiency
vector<ull> ternary_powers;

void build_power_table() {
  ternary_powers.resize(40, 1);
  for (int i = 1; i < 40; ++i) {
    ternary_powers[i] = ternary_powers[i - 1] * 3ull;
  }
}

// Function to solve a single test case
void solve() {
  ll melons_to_buy_ll, max_deals_ll;
  cin >> melons_to_buy_ll >> max_deals_ll;

  // Initial configuration
  vll deal_counts(40, 0);
  ll  min_deals_needed  = 0;
  ll  temp_n            = melons_to_buy_ll;
  int highest_deal_type = 0;

  for (int i = 0; i < 40 && temp_n > 0; ++i) {
    deal_counts[i] = temp_n % 3;
    min_deals_needed += deal_counts[i];
    if (deal_counts[i] > 0) {
      highest_deal_type = i;
    }
    temp_n /= 3;
  }

  // Determin the target deal count based on the maximum allowed deals
  ll target_deal_count = max_deals_ll;
  if ((max_deals_ll % 2) != (min_deals_needed % 2)) {
    target_deal_count--;
  }

  // Verify if the target deal count is achievable
  if (target_deal_count < min_deals_needed) {
    cout << -1 << "\n";
    return;
  }

  // Greedily perform breakdowns to reach the target deal count
  ll breakdowns_to_perform = (target_deal_count - min_deals_needed) / 2;
  for (int i = highest_deal_type; i >= 1 && breakdowns_to_perform > 0; --i) {
    ll possible_breakdowns = deal_counts[i];
    ll actual_breakdowns   = min(breakdowns_to_perform, possible_breakdowns);

    if (actual_breakdowns > 0) {
      deal_counts[i] -= actual_breakdowns;
      deal_counts[i - 1] += actual_breakdowns * 3;
      breakdowns_to_perform -= actual_breakdowns;
    }
  }

  // Calculate the final price based on the optimized configuration
  ull final_price = 0;
  for (int deal_type_index = 0; deal_type_index < 40; ++deal_type_index) {
    if (deal_counts[deal_type_index] > 0) {
      ull cost_per_single_deal;
      if (deal_type_index == 0) {
        cost_per_single_deal = 3;
      } else {
        cost_per_single_deal = ternary_powers[deal_type_index + 1] + (ull)deal_type_index * ternary_powers[deal_type_index - 1];
      }
      final_price += (ull)deal_counts[deal_type_index] * cost_per_single_deal;
    }
  }

  cout << final_price << "\n";
}

//===----------------------------------------------------------------------===//
/* Main function */

int main() {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  build_power_table();

  int t = 1;
  cin >> t;
  while (t--) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//
