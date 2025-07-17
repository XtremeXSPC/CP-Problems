//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Round 1000 (Div. 2)
 * @author: Costantino Lombardi
 * @file: problem_A.cc
 *
 * @status: PASSED
 */
//===---------------------------------------------------------------------===//
/* Included library */

#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "algo/debug.h"
#else
#define debug(...) 42
#endif

//===---------------------------------------------------------------------===//
/* Main function */

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  while (t--) {
    long long l, r;
    cin >> l >> r;

    long long answer = 0;
    if (l <= 1 && r >= 1) {
      answer += 1;
    }

    long long start = max(l, 2LL);
    if (start <= r - 1) {
      answer += (r - start);
    }

    cout << answer << "\n";
  }

  return 0;
}

//===---------------------------------------------------------------------===//
