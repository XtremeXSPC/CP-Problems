/**
 *    author:  tourist
 *    created: 15.12.2024 06:39:11
**/
#include <bits/stdc++.h>
 
using namespace std;
 
#ifdef LOCAL
#include "algo/debug.h"
#else
#define debug(...) 42
#endif
 
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int tt;
  cin >> tt;
  while (tt--) {
    int64_t k, l1, r1, l2, r2;
    cin >> k >> l1 >> r1 >> l2 >> r2;
    int64_t r = 1;
    int64_t ans = 0;
    while (r <= r2 / l1) {
      auto low = max(l1, (l2 + r - 1) / r);
      auto high = min(r1, r2 / r);
      if (low <= high) {
        ans += high - low + 1;
      }
      r *= k;
    }
    cout << ans << '\n';
  }
  return 0;
}