//===---------------------------------------------------------------------===//
/* Codeforces Round 1000 Div. 2
 * Author: Costantino Lombardi
 * File: problem_B.cc
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
    long long n, l, r;
    cin >> n >> l >> r;

    vector<long long> a(n + 1);
    for (int i = 1; i <= n; i++) {
      cin >> a[i];
    }

    long long S = 0;
    for (int i = l; i <= r; i++) {
      S += a[i];
    }

    vector<int> I;
    I.reserve(r - l + 1);
    for (int i = l; i <= r; i++) {
      I.push_back(i);
    }

    vector<int> O;
    O.reserve((l - 1) + (n - r));

    for (int j = n; j >= r + 1; j--) {
      O.push_back(j);
    }
    for (int j = l - 1; j >= 1; j--) {
      O.push_back(j);
    }

    int lenI = (int)I.size();
    int lenO = (int)O.size();

    vector<long long> dp(lenO + 1, 0LL), newdp(lenO + 1, 0LL);

    for (int iIdx = 0; iIdx < lenI; iIdx++) {
      for (int o = 0; o <= lenO; o++) {
        newdp[o] = dp[o];
      }

      for (int o = 1; o <= lenO; o++) {
        newdp[o] = max(newdp[o], newdp[o - 1]);

        long long valI = a[I[iIdx]];
        long long valO = a[O[o - 1]];
        if (valI > valO) {
          long long gain = valI - valO;
          newdp[o]       = max(newdp[o], dp[o - 1] + gain);
        }
      }

      dp = newdp;
    }

    long long bestGain = dp[lenO];
    long long ans      = S - bestGain;
    cout << ans << "\n";
  }

  return 0;
}

//===---------------------------------------------------------------------===//