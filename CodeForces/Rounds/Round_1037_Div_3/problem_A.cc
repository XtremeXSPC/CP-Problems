//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Round 1037 Div. 3
 * @author: Costantino Lombardi
 * @file: problem_A.cc
 *
 * @status: PASSED
 */
//===---------------------------------------------------------------------===//
/* Included library */

#include <bits/stdc++.h>
#include <iostream>

using namespace std;

#ifdef LOCAL
#include "algo/debug.h"
#else
#define debug(...) 42
#endif

//===---------------------------------------------------------------------===//
/* Main function */

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

int main() {
  setup_io();

  int t;
  cin >> t;
  while (t--) {
    solve();
  }

  return 0;
}

//===---------------------------------------------------------------------===//