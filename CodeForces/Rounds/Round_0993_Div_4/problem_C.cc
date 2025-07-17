//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Round 993 (Div. 4)
 * @author: Costantino Lombardi
 * @file: problem_C.cc
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  while (t--) {
    long long m, a, b, c;
    cin >> m >> a >> b >> c;

    long long row1     = min(a, m);
    long long row2     = min(b, m);
    long long leftover = 2 * m - (row1 + row2);

    // Quanto possiamo piazzare di c?
    long long seatC = min(c, leftover);

    cout << row1 + row2 + seatC << "\n";
  }

  return 0;
}

//===---------------------------------------------------------------------===//
