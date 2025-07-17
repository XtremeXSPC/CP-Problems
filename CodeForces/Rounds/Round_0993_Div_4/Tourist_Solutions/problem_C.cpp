/**
 *    author:  tourist
 *    created: 15.12.2024 06:36:30
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
    int m, a, b, c;
    cin >> m >> a >> b >> c;
    int x = m;
    int y = m;
    int ans = 0;
    int take = min(x, a);
    x -= take;
    ans += take;
    take = min(y, b);
    y -= take;
    ans += take;
    take = min(x + y, c);
    ans += take;
    cout << ans << '\n';
  }
  return 0;
}