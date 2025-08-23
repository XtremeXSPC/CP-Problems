//===----------------------------------------------------------------------===//
/**
 * @file: problem_F.cpp
 * @brief: Codeforces Round 1042 (Div. 3) - Problem F
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

// Information needed for each y to speed up calculations.
struct YInfo {
  int y;       // The original index y (1-based)
  ll  db;      // The value 2 * pb1[y] - y
  ll  pb1_val; // The value pb1[y]
};

// Function to solve a single test case
void solve() {
  int n;
  cin >> n;
  string a, b;
  cin >> a >> b;

  // Calculate prefix sums of 1s for string a
  vll pa1(n + 1, 0);
  for (int i = 0; i < n; ++i) {
    pa1[i + 1] = pa1[i] + (a[i] - '0');
  }

  // Calculate prefix sums of 1s for string b
  vll pb1(n + 1, 0);
  for (int i = 0; i < n; ++i) {
    pb1[i + 1] = pb1[i] + (b[i] - '0');
  }

  vector<YInfo> y_infos(n);
  for (int y = 1; y <= n; ++y) {
    y_infos[y - 1] = {y, 2 * pb1[y] - y, pb1[y]};
  }

  // Sort y_infos based on the d_b value
  sort(y_infos.begin(), y_infos.end(), [](const YInfo& l, const YInfo& r) { return l.db < r.db; });

  // Precompute prefix sums over the sorted y_infos
  vll pref_y(n + 1, 0);
  vll pref_pb1(n + 1, 0);
  vll sorted_dbs(n);
  for (int i = 0; i < n; ++i) {
    pref_y[i + 1]   = pref_y[i] + y_infos[i].y;
    pref_pb1[i + 1] = pref_pb1[i] + y_infos[i].pb1_val;
    sorted_dbs[i]   = y_infos[i].db;
  }

  ll total_sum = 0;

  // Iterate through all possible x
  for (int x = 1; x <= n; ++x) {
    // Condition is d_b(y) <= x - 2*pa1[x]
    ll da_val = (ll)x - 2 * pa1[x];

    // Binary search to find how many y's satisfy the condition
    auto it = upper_bound(sorted_dbs.begin(), sorted_dbs.end(), da_val);
    auto k  = distance(sorted_dbs.begin(), it);

    // Sum for y in Y1 (where condition holds)
    // sum is k * pa1[x] + sum(pb1[y] for y in Y1)
    ll sum_y1 = (ll)k * pa1[x] + pref_pb1[k];

    // Sum for y in Y2 (where condition does not hold)
    // sum is (n-k)*(x - pa1[x]) + sum(y - pb1[y] for y in Y2)
    ll sum_y2_y   = pref_y[n] - pref_y[k];
    ll sum_y2_pb1 = pref_pb1[n] - pref_pb1[k];
    ll sum_y2     = (ll)(n - k) * (x - pa1[x]) + sum_y2_y - sum_y2_pb1;

    total_sum += sum_y1 + sum_y2;
  }

  cout << total_sum << "\n";
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
