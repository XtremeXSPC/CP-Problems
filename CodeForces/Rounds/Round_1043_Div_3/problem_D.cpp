//===----------------------------------------------------------------------===//
/**
 * @file: problem_D.cpp
 * @brief: Codeforces Round 1043 (Div. 3) - Problem D
 * @author: Costantino Lombardi
 *
 * @status: PASSED
 */
//===----------------------------------------------------------------------===//
/* Included library and Macros */

// clang-format off
// Compiler optimizations:
#ifdef __GNUC__
  #pragma GCC optimize("Ofast,unroll-loops,fast-math,O3")
  // x86_64 optimizations:
  #ifdef __x86_64__
    #pragma GCC target("avx,avx2,fma")
  #endif
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

// Precomputed powers of 10 for digit DP calculations
vector<ull> ten_powers;

void build_power_table() {
  ten_powers.resize(20, 1);
  for (int i = 1; i < 20; ++i) {
    ten_powers[i] = ten_powers[i - 1] * 10ULL;
  }
}

// Calculates sum of digits for numbers in [1, n] using a Digit DP approach.
ull sumDigitsToN(ull n) {
  if (n < 1)
    return 0;
  string s     = to_string(n);
  int    len   = s.length();
  ull    total = 0;

  // Sum for all numbers with fewer digits than n
  for (int d = 1; d < len; ++d) {
    ull count = 9 * ten_powers[d - 1];
    // Sum of first digits (1-9)
    total += 45 * ten_powers[d - 1];
    // Sum of other d-1 digits
    if (d > 1) {
      total += (ull)(d - 1) * 45 * count / 10;
    }
  }

  // Sum for numbers with same length as n, but smaller than n
  ull prefix_val = 0;
  for (int i = 0; i < len; ++i) {
    int digit   = s[i] - '0';
    int rem_len = len - 1 - i;

    for (int d = (i == 0 ? 1 : 0); d < digit; ++d) {
      // Contribution from the fixed prefix
      total += prefix_val * ten_powers[rem_len];
      // Contribution from the current digit d
      total += (ull)d * ten_powers[rem_len];
      // Contribution from the remaining digits (00... to 99...)
      if (rem_len > 0) {
        total += (ull)rem_len * 45 * ten_powers[rem_len - 1];
      }
    }
    prefix_val += digit;
  }

  // Add sum of digits of n itself
  total += prefix_val;

  return total;
}

// Counts total digits in the sequence 123...n using a closed-form formula.
ull countDigitsToN(ull n) {
  if (n < 1)
    return 0;
  string s   = to_string(n);
  int    len = s.length();
  return (ull)len * (n + 1) - (ten_powers[len] - 1) / 9;
}

void solve() {
  ull k;
  cin >> k;

  // Binary search for the number `N` containing the k-th digit.
  ull low = 1, high = 3e14, target_num = 0;
  while (low <= high) {
    ull mid = low + (high - low) / 2;
    if (mid == 0) {
      low = 1;
      continue;
    }
    if (countDigitsToN(mid) >= k) {
      target_num = mid;
      high       = mid - 1;
    } else {
      low = mid + 1;
    }
  }

  // Sum of digits for all numbers strictly before `target_num`.
  ull final_sum = sumDigitsToN(target_num - 1);

  // Number of digits from numbers before `target_num`.
  ull digits_before = countDigitsToN(target_num - 1);

  // Digits to take from `target_num`.
  ull remaining_len = k - digits_before;

  string num_str = to_string(target_num);
  for (ull i = 0; i < remaining_len; ++i) {
    final_sum += (num_str[i] - '0');
  }

  cout << final_sum << "\n";
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
