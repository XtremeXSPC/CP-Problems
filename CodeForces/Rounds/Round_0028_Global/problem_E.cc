//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Global Round 28
 * @author: Costantino Lombardi
 * @file: problem_E.cc
 *
 * @status: NOT PASSED
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

#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  while (t--) {
    int n, m;
    cin >> n >> m;
    if (m > n) {
      // Non esiste soluzione
      cout << "No\n";
    } else {
      cout << "Yes\n";
      // Costruzione della soluzione
      for (int i = 1; i <= 2 * n; i++) {
        for (int j = 1; j <= m; j++) {
          int color = ((i - 1) + (j - 1)) % n + 1;
          cout << color << (j == m ? '\n' : ' ');
        }
      }
    }
  }

  return 0;
}

//===---------------------------------------------------------------------===//
