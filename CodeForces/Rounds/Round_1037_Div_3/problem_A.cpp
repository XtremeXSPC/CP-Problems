//===----------------------------------------------------------------------===//
/**
 * @brief Codeforces Round 1037 Div. 3
 * @author: Costantino Lombardi
 * @file: problem_A.cc
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
/* Data Types and Function Definitions */

// Fast I/O
void setup_io() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
}

// Function that solves a single test case
void solve() {
  int x;
  cin >> x;

  // If the number x has only one digit, the answer is x itself.
  if (x < 10) {
    cout << x << "\n";
    return;
  }

  // For numbers with two or more digits, find the minimum digit.
  // This will be the answer, as it is the smallest non-negative y
  // that shares a digit with x.
  int min_digit = 9;
  int temp_x    = x;
  while (temp_x > 0) {
    int digit = temp_x % 10;
    if (digit < min_digit) {
      min_digit = digit;
    }
    // If we find a 0, it is the smallest possible digit,
    // so we can stop searching.
    if (min_digit == 0) {
      break;
    }
    temp_x /= 10;
  }

  cout << min_digit << "\n";
}

//===----------------------------------------------------------------------===//
/* Main function */

int main() {
  setup_io();

  int t;
  cin >> t;
  while (t--) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//