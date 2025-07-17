//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Global Round 28
 * @author: Costantino Lombardi
 * @file: problem_D_v1.cc
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  while (t--) {
    int n, m;
    cin >> n >> m;
    vector<long long> a(n), b(m);
    for (int i = 0; i < n; i++)
      cin >> a[i];
    for (int j = 0; j < m; j++)
      cin >> b[j];

    long long a1           = a[0];
    int       strong_count = 0;

    for (int i = 0; i < n; i++) {
      if (a[i] > a1)
        strong_count++;
    }

    sort(b.begin(), b.end());

    int S = (int)(upper_bound(b.begin(), b.end(), a1) - b.begin());

    for (int k = 1; k <= m; k++) {
      int C = m / k;
      if (C == 0) {
        cout << 0 << " ";
        continue;
      }
      long long need = (long long)C * k;
      if (S >= need) {
        cout << C << " ";
      } else {
        long long chosen_U = need - S;
        long long x        = (chosen_U + k - 1) / k;
        long long sum_rank = C + (long long)strong_count * x;
        cout << sum_rank << " ";
      }
    }
    cout << "\n";
  }

  return 0;
}

//===---------------------------------------------------------------------===//
