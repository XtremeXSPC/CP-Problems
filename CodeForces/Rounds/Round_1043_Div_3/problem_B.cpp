//===----------------------------------------------------------------------===//
/**
 * @file: problem_B.cpp
 * @brief Codeforces Round 1043 (Div. 3) - Problem B
 * @author: Costantino Lombardi
 *
 * @status: PASSED
 */
//===----------------------------------------------------------------------===//
/* Included library */

// clang-format off
// Sanitaze macro:
#ifdef USE_CLANG_SANITIZE
  #include "PCH.h"
#else
  #include <bits/stdc++.h>
#endif

using namespace std;

//===----------------------------------------------------------------------===//
/* Macros and Type Aliases */

// Debug macro:
#ifdef LOCAL
  #include "debug.h"
#else
  #define debug(...) 42
#endif
// clang-format on

// Type aliases
using ll   = long long;
using ull  = unsigned long long;
using ld   = long double;
using pii  = std::pair<int, int>;
using pll  = std::pair<long long, long long>;
using vi   = std::vector<int>;
using vll  = std::vector<long long>;
using vvi  = std::vector<std::vector<int>>;
using vvll = std::vector<std::vector<long long>>;
using vs   = std::vector<std::string>;
using vpii = std::vector<std::pair<int, int>>;
using vpll = std::vector<std::pair<long long, long long>>;

// Mathematical constants
constexpr long double PI   = 3.141592653589793238462643383279502884L;
constexpr long double E    = 2.718281828459045235360287471352662498L;
constexpr long double EPS  = 1e-9L;
constexpr int         INF  = 0x3f3f3f3f;
constexpr long long   LINF = 0x3f3f3f3f3f3f3f3fLL;
constexpr int         MOD  = 1000000007;
constexpr int         MOD2 = 998244353;

//===----------------------------------------------------------------------===//
/* Data Types and Function Definitions */

// Function to solve a single test case
void solve() {
  ll n;
  cin >> n;

  vll solutions;
  ll  power_of_10 = 10;

  // k is the number of appended zeros. We can iterate on k.
  for (int k = 1; k <= 18; ++k) {
    ll divisor = power_of_10 + 1;

    // If the divisor becomes larger than n, x would be less than 1.
    if (divisor > n) {
      break;
    }

    if (n % divisor == 0) {
      solutions.push_back(n / divisor);
    }

    if (power_of_10 > LINF / 10) {
      break;
    }
    power_of_10 *= 10;
  }

  if (solutions.empty()) {
    cout << 0 << "\n";
  } else {
    sort(solutions.begin(), solutions.end());
    cout << solutions.size() << "\n";
    for (size_t i = 0; i < solutions.size(); ++i) {
      cout << solutions[i] << (i == solutions.size() - 1 ? "" : " ");
    }
    cout << "\n";
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
