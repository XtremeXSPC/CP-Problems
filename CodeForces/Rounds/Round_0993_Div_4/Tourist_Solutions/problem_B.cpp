/**
 *    author:  tourist
 *    created: 15.12.2024 06:35:49
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
    string s;
    cin >> s;
    reverse(s.begin(), s.end());
    for (char& c : s) {
      if (c == 'p') c = 'q'; else
      if (c == 'q') c = 'p';
    }
    cout << s << '\n';
  }
  return 0;
}