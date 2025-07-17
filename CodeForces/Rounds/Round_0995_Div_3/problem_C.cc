//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Round 995 (Div. 3)
 * @author: Costantino Lombardi
 * @file: problem_C.cc
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
    int n, m, k;
    cin >> n >> m >> k;

    vector<int> a(m);
    for (int i = 0; i < m; i++) {
      cin >> a[i];
    }

    vector<bool> known(n + 1, false);
    for (int i = 0; i < k; i++) {
      int q;
      cin >> q;
      known[q] = true;
    }

    int unknownCount    = n - k;
    int unknownQuestion = -1;
    if (unknownCount == 1) {
      for (int i = 1; i <= n; i++) {
        if (!known[i]) {
          unknownQuestion = i;
          break;
        }
      }
    }

    if (unknownCount == 0) {
      for (int i = 0; i < m; i++) {
        cout << '1';
      }
    } else if (unknownCount == 1) {
      for (int i = 0; i < m; i++) {
        cout << (a[i] == unknownQuestion ? '1' : '0');
      }
    } else {
      for (int i = 0; i < m; i++) {
        cout << '0';
      }
    }
    cout << "\n";
  }

  return 0;
}

//===---------------------------------------------------------------------===//
