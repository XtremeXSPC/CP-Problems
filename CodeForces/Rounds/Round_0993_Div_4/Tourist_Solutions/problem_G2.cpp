/**
 *    author:  tourist
 *    created: 15.12.2024 06:44:58
**/
#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "algo/debug.h"
#else
#define debug(...) 42
#endif

class dsu {
 public:
  vector<int> p;
  int n;

  dsu(int _n) : n(_n) {
    p.resize(n);
    iota(p.begin(), p.end(), 0);
  }

  inline int get(int x) {
    return (x == p[x] ? x : (p[x] = get(p[x])));
  }

  inline bool unite(int x, int y) {
    x = get(x);
    y = get(y);
    if (x != y) {
      p[x] = y;
      return true;
    }
    return false;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int tt;
  cin >> tt;
  while (tt--) {
    int n;
    cin >> n;
    vector<int> r(n);
    vector<vector<int>> g(n);
    for (int i = 0; i < n; i++) {
      cin >> r[i];
      --r[i];
      g[r[i]].push_back(i);
    }
    dsu d(n);
    vector<bool> on_cycle(n, false);
    auto Dfs = [&](auto&& self, int v) -> int {
      int h = 0;
      for (int u : g[v]) {
        if (on_cycle[u]) {
          continue;
        }
        if (on_cycle[v]) {
          h = max(h, self(self, u) + 1);
        } else {
          h = h + self(self, u) + 1;
        }
      }
      return h;
    };
    int ans = 0;
    for (int i = 0; i < n; i++) {
      if (!d.unite(i, r[i])) {
        int x = i;
        vector<int> cycle;
        while (true) {
          cycle.push_back(x);
          on_cycle[x] = true;
          x = r[x];
          if (x == i) {
            break;
          }
        }
        for (int v : cycle) {
          ans = max(ans, Dfs(Dfs, v));
        }
      }
    }
    cout << ans + 2 << '\n';
  }
  return 0;
}
