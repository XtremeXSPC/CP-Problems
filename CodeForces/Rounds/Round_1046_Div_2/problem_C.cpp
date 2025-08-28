//===----------------------------------------------------------------------===//
/**
 * @file: problem_C.cpp
 * @brief Codeforces Round 1046 (Div. 2) - Problem C
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

// Structure to represent a block candidate.
struct BlockCandidate {
  int start_idx;
  int end_idx;
  int length;
};

// Function to solve a single test case.
void solve() {
  int n;
  cin >> n;
  vi  a(n);
  vvi positions(n + 1);
  for (int i = 0; i < n; ++i) {
    cin >> a[i];
    if (a[i] <= n) {
      positions[a[i]].push_back(i);
    }
  }

  // Find all possible blocks for Weighted Interval Scheduling.
  vector<BlockCandidate> candidates;
  for (int val = 1; val <= n; ++val) {
    if (positions[val].size() < static_cast<size_t>(val)) {
      continue;
    }
    for (size_t i = 0; i <= positions[val].size() - val; ++i) {
      candidates.push_back({.start_idx = positions[val][i], .end_idx = positions[val][i + val - 1], .length = val});
    }
  }

  if (candidates.empty()) {
    cout << 0 << "\n";
    return;
  }

  // Sort candidates by end index.
  ranges::sort(candidates, {}, &BlockCandidate::end_idx);

  // DP: dp[i] = maximum length using first i+1 candidates.
  vll dp(candidates.size());
  dp[0] = candidates[0].length;

  for (size_t i = 1; i < candidates.size(); ++i) {
    const auto& current = candidates[i];

    // Include current block + best non-overlapping previous.
    ll length_if_included = current.length;

    // Find last candidate ending before current starts.
    auto it = ranges::upper_bound(candidates.begin(), candidates.begin() + i, current.start_idx - 1, {}, &BlockCandidate::end_idx);

    if (it != candidates.begin()) {
      int prev_idx = distance(candidates.begin(), prev(it));
      length_if_included += dp[prev_idx];
    }

    // Exclude current block.
    ll length_if_excluded = dp[i - 1];

    dp[i] = max(length_if_included, length_if_excluded);
  }

  cout << dp.back() << "\n";
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
