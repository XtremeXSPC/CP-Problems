//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Round 993 (Div. 4)
 * @author: Costantino Lombardi
 * @file: problem_A.cc
 *
 * @status: PASSED
 */
//===---------------------------------------------------------------------===//
/* Included library */

#include <bits/stdc++.h>
#include <iostream>

#ifdef LOCAL
#include "algo/debug.h"
#else
#define debug(...) 42
#endif

using namespace std;

//===---------------------------------------------------------------------===//
/* Main function */

int main() {
  int t;
  cin >> t;

  for (int i = 0; i < t; i++) {
    int n;
    cin >> n;
    // Il numero di coppie (a, b) è semplicemente n - 1
    cout << n - 1 << "\n";
  }

  return 0;
}

//===---------------------------------------------------------------------===//
