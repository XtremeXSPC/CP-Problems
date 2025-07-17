/******************************************************************************
 * Codeforces Round 995 Div. 3                                                *
 * Author: Costantino Lombardi                                                *
 * File: problem_A.cc                                                         *
 *****************************************************************************/

/* ========================================================================= */
/* Included library */

#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "algo/debug.h"
#else
#define debug(...) 42
#endif

/* ========================================================================= */
/* Main function */

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;

    vector<int> a(n), b(n);
    for (int i = 0; i < n; i++)
      cin >> a[i];

    for (int i = 0; i < n; i++)
      cin >> b[i];

    int max_diff = 0;

    for (int i = 0; i < n - 1; i++) {
      int delta = a[i] - b[i + 1];
      if (delta > 0) {
        max_diff += delta;
      }
    }

    max_diff += a[n - 1];
    cout << max_diff << "\n";
  }

  return 0;
}

/* ========================================================================= */