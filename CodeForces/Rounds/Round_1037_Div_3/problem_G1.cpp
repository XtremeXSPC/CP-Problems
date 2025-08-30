//===----------------------------------------------------------------------===//
/**
 * @file: problem_G1.cpp
 * @brief Codeforces Round 1037 Div. 3 - Problem G1
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

// Type aliases
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
using VP_ii    = std::vector<std::pair<int, int>>;
using VVP_ii   = std::vector<std::vector<std::pair<int, int>>>;
using VP_ll    = std::vector<std::pair<long long, long long>>;
using VT_iii   = std::vector<std::tuple<int, int, int>>;
using Map_ll   = std::map<long long, long long>;
using VUMap_il = std::vector<std::unordered_map<int, ll>>;

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
/* Fast I/O Setup */

using namespace std;

// Fast I/O setup:
struct FastIOSetup {
  FastIOSetup() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::cout << std::fixed << std::setprecision(10);
  }
} fast_io_setup;

//===----------------------------------------------------------------------===//
/* Main Solver Function */

// Function to solve a single test case
void solve() {
  int n;
  cin >> n;
  V_i a(n);
  for (int i = 0; i < n; ++i) {
    cin >> a[i];
  }

  // Limits based on problem constraints.
  int min_val = 101;
  int max_val = 0;

  // Read input array and find global min and max values
  for (int i = 0; i < n; ++i) {
    if (a[i] < min_val) {
      min_val = a[i];
    }
    if (a[i] > max_val) {
      max_val = a[i];
    }
  }

  int max_ans = 0;

  // Candidate 1: Max difference between any adjacent elements.
  // This covers optimal subarrays of length 2.
  for (int i = 1; i < n; ++i) {
    if (a[i - 1] > a[i]) {
      if (a[i - 1] - a[i] > max_ans) {
        max_ans = a[i - 1] - a[i];
      }
    } else {
      if (a[i] - a[i - 1] > max_ans) {
        max_ans = a[i] - a[i - 1];
      }
    }
  }

  // Candidate 2: Subarray whose median is the global max_val.
  // We check the difference with the very first element.
  if (max_val - a[0] > max_ans) {
    max_ans = max_val - a[0];
  }

  // Candidate 3: Subarray whose min is the global min_val.
  // We check the difference with the very last element.
  if (a[n - 1] - min_val > max_ans) {
    max_ans = a[n - 1] - min_val;
  }

  // Also check max_val vs min_val over the whole array, as the array itself is a subarray
  // this check is needed because max_val might be at a[0] and min_val at a[n-1].
  if (max_val - min_val > max_ans) {
    max_ans = max_val - min_val;
  }

  std::cout << max_ans << std::endl;
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
