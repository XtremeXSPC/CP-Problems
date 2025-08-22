//===----------------------------------------------------------------------===//
/**
 * @file: problem_C1.cpp
 * @brief: Codeforces Round 1043 (Div. 3) - Problem C1
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

// A table to store powers of 3, pre-calculated for efficiency.
// A size of 40 is sufficient for numbers up to 10^18.
vector<ull> ternary_powers;

void build_power_table() {
  ternary_powers.resize(40, 1);
  for (int i = 1; i < 40; ++i) {
    ternary_powers[i] = ternary_powers[i - 1] * 3ull;
  }
}

// Function to solve a single test case
void solve() {
  ll melons_to_buy_ll;
  cin >> melons_to_buy_ll;

  ull melons_to_process = melons_to_buy_ll;
  ull final_price       = 0;
  int x                 = 0;

  while (melons_to_process > 0) {
    int ternary_coeff = melons_to_process % 3;

    // If the coefficient for this power of 3 is not zero, add its cost
    if (ternary_coeff != 0) {
      ull cost_per_single_deal;

      // Calculate cost C(x) = 3^(x+1) + x * 3^(x-1)
      if (x == 0) {
        // For x=0, the second term is 0. C(0) = 3^1 = 3.
        cost_per_single_deal = 3;
      } else {
        cost_per_single_deal = ternary_powers[x + 1] + (ull)x * ternary_powers[x - 1];
      }

      final_price += (ull)ternary_coeff * cost_per_single_deal;
    }

    // Move to the next digit in base 3
    melons_to_process /= 3;
    x++;
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
