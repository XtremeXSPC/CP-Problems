//===----------------------------------------------------------------------===//
/**
 * @file: problem_F1.cpp
 * @brief Codeforces Round #XXX (Div. X) - Problem Y
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

/**
 * Ask the judge with a single-word article of length x.
 * Protocol: print "? 1 x\n", flush, then read the integer reply.
 * Returns:
 *   1  -> x <= W  (fits on one line)
 *   0  -> x >  W  (editor cannot display the article)
 *  -1  -> invalid / closed stream: must exit immediately.
 */
static int ask_one(int x) {
  cout << "? 1 " << x << '\n' << flush;
  int r;
  if (!(cin >> r)) {
    // Stream closed unexpectedly; exit as per statement.
    std::exit(0);
  }
  if (r == -1) {
    std::exit(0);
  }
  return r;
}

//===----------------------------------------------------------------------===//
/* Core solving routine (one test case) */

/**
 * Strategy:
 *  - Binary search W in [1, 100000] using only single-word queries.
 *  - Monotonic predicate: P(x) := (x <= W) <=> ask_one(x) == 1.
 *  - The interactor may be adaptive but must remain consistent; the
 *    final 'lo' we obtain is a valid W satisfying all answers so far.
 *
 * Complexity:
 *  - At most ceil(log2(1e5)) ≈ 17 queries per test case.
 */
void solve() {
  int lo = 1, hi = 100000;
  while (lo < hi) {
    int mid = lo + (hi - lo + 1) / 2;
    int r   = ask_one(mid);
    if (r == 1) {
      lo = mid; // mid <= W
    } else {
      hi = mid - 1; // mid  > W
    }
  }
  // Report the discovered W
  cout << "! " << lo << '\n' << flush;
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
