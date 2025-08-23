//===----------------------------------------------------------------------===//
/**
 * @file: problem_E.cpp
 * @brief: Codeforces Round 1042 (Div. 3) - Problem E
 * @author: Costantino Lombardi
 *
 * @status:
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

// Class to encapsulate the transformation logic.
class Transformer {
private:
  int         n;
  const vui&  source;
  const vui&  target;
  vui         diffs_needed;
  vector<int> memo; // Memoization table: -1=uncomputed, 0=false, 1=true

  // Recursively checks if the suffix starting at `idx` can be transformed.
  bool can_transform_from(int idx) {
    // Base case: If we've processed all modifiable elements, we succeeded.
    if (idx >= n - 1) {
      return true;
    }

    // Return cached result if already computed.
    if (memo[idx] != -1) {
      return memo[idx] == 1;
    }

    // If current element already matches, move to the next.
    if (diffs_needed[idx] == 0) {
      memo[idx] = can_transform_from(idx + 1) ? 1 : 0;
      return memo[idx];
    }

    // A difference at `idx` requires a chain of operations.
    // We now simulate this chain to find a valid termination point.
    unsigned int current_diff_propagation = diffs_needed[idx];
    for (int j = idx; j < n - 1; ++j) {
      // Check for a valid termination at index `j`.
      // The chain can end if the propagated difference matches `source[j+1]`.
      if (current_diff_propagation == source[j + 1]) {
        // If chain terminates, recursively solve for the rest of the array.
        if (can_transform_from(j + 1)) {
          memo[idx] = 1;
          return memo[idx];
        }
      }

      // If it doesn't terminate, check if the chain can propagate to j+1.
      // If we are at the last element, propagation is not an option.
      if (j + 1 >= n - 1) {
        break;
      }

      // The required difference at j+1 must match what this chain would produce.
      unsigned int expected_next_diff = current_diff_propagation ^ source[j + 1];
      if (diffs_needed[j + 1] != expected_next_diff) {
        // The chain is broken. No solution is possible starting from `idx`.
        memo[idx] = 0;
        return memo[idx];
      }

      // The chain continues, update the propagated difference.
      current_diff_propagation = diffs_needed[j + 1];
    }

    // If the loop finishes without finding a valid termination, it's impossible.
    memo[idx] = 0;
    return memo[idx];
  }

public:
  Transformer(int size, const vui& a, const vui& b) : n(size), source(a), target(b) {}

  bool solve() {
    if (source.back() != target.back()) {
      return false;
    }

    diffs_needed.resize(n - 1);
    for (int i = 0; i < n - 1; ++i) {
      diffs_needed[i] = source[i] ^ target[i];
    }

    memo.assign(n, -1);
    return can_transform_from(0);
  }
};

void solve_wrapper() {
  int n;
  cin >> n;
  vui a(n), b(n);
  for (int i = 0; i < n; i++)
    cin >> a[i];
  for (int i = 0; i < n; i++)
    cin >> b[i];

  Transformer solver(n, a, b);
  if (solver.solve()) {
    cout << "YES\n";
  } else {
    cout << "NO\n";
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
    solve_wrapper();
  }

  return 0;
}

//===----------------------------------------------------------------------===//
