//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Round 1037 Div. 3
 * @author: Costantino Lombardi
 * @file: problem_G1.cc
 *
 * @status: NOT PASSED
 */
//===---------------------------------------------------------------------===//
/* Included library */

#include <bits/stdc++.h>
#include <iostream>

using namespace std;

#ifdef LOCAL
#include "../../Algorithms/debug.h"
#else
#define debug(...) 42
#endif

//===---------------------------------------------------------------------===//
/* Function Definitions */

// Fast I/O
void setup_io() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
}

// Function to solve a single test case
void solve() {
  int n;
  std::cin >> n;
  std::vector<int> a(n);
  int              min_val = 101; // Constraints are 1 <= a_i <= min(n, 100)
  int              max_val = 0;

  // Read input array and find global min and max values
  for (int i = 0; i < n; ++i) {
    std::cin >> a[i];
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

//===---------------------------------------------------------------------===//
/* Main function */

int main() {

  setup_io();
  int t;
  std::cin >> t;
  while (t--) {
    solve();
  }

  return 0;
}

//===---------------------------------------------------------------------===//
