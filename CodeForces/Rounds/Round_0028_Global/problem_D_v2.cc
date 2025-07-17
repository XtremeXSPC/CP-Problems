//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Global Round 28
 * @author: Costantino Lombardi
 * @file: problem_D_v2.cc
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

    long long a1 = a[0];

    sort(a.begin(), a.end());

    auto count_ge = [&](long long d) {
      int pos = (int)(lower_bound(a.begin(), a.end(), d) - a.begin());
      return n - pos;
    };

    sort(b.begin(), b.end());
    int S = (int)(upper_bound(b.begin(), b.end(), a1) - b.begin());

    int               U = m - S;
    vector<long long> Uproblems;
    for (int i = S; i < m; i++) {
      Uproblems.push_back(b[i]);
    }

    reverse(Uproblems.begin(), Uproblems.end());

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
        int       chosen_U    = (int)(need - S);
        int       x           = (chosen_U + k - 1) / k;
        long long surpass_sum = 0;

        for (int i = 1; i <= x; i++) {
          int idx_min = i * k - 1;
          if (idx_min >= chosen_U)
            idx_min = chosen_U - 1;
          long long diff_min = Uproblems[idx_min];
          long long cnt      = count_ge(diff_min);
          surpass_sum += cnt;
        }

        long long sum_rank = C + surpass_sum;
        cout << sum_rank << " ";
      }
    }
    cout << "\n";
  }

  return 0;
}

//===---------------------------------------------------------------------===//
