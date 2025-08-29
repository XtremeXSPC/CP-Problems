//===----------------------------------------------------------------------===//
/**
 * @brief Codeforces Round 1037 Div. 3
 * @author: Costantino Lombardi
 * @file: problem_E.cc
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

using namespace std;

//===----------------------------------------------------------------------===//
/* Function Definitions */

// Function to compute Least Common Multiple - LCM.
long long lcm(long long a, long long b) {
  if (a == 0 || b == 0)
    return 0;
  return abs(a * b) / gcd(a, b);
}

// Function to solve a single test case.
void solve() {
  int n;
  cin >> n;
  vector<long long> p(n), s(n);
  for (int i = 0; i < n; ++i)
    cin >> p[i];
  for (int i = 0; i < n; ++i)
    cin >> s[i];

  bool possible = true;

  // "Condition 1: p_n must be equal to s_1 (0-indexed: p[n-1] == s[0])".
  if (p[n - 1] != s[0]) {
    possible = false;
  }

  // "Condition 2: p[i] must divide p[i-1] (0-indexed: p[i+1] must divide p[i])".
  if (possible) {
    for (int i = 0; i < n - 1; ++i) {
      if (p[i] % p[i + 1] != 0) {
        possible = false;
        break;
      }
    }
  }

  // "Condition 3: s[i] must divide s[i+1] (0-indexed: s[i] must divide s[i+1])".
  if (possible) {
    for (int i = 0; i < n - 1; ++i) {
      if (s[i + 1] % s[i] != 0) {
        possible = false;
        break;
      }
    }
  }

  // "Condition 4: gcd(p[i], s[i+1]) must be equal to p[n-1]".
  if (possible) {
    for (int i = 0; i < n - 1; ++i) {
      if (gcd(p[i], s[i + 1]) != p[n - 1]) {
        possible = false;
        break;
      }
    }
  }

  if (possible) {
    cout << "YES\n";
  } else {
    cout << "NO\n";
  }
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
