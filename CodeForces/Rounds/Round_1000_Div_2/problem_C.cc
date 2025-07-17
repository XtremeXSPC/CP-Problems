//===---------------------------------------------------------------------===//
/* Codeforces Round 1000 Div. 2
 * Author: Costantino Lombardi
 * File: problem_C.cc
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

static const int MAXN = 200000;
static const int K    = 50;

//===---------------------------------------------------------------------===//
/* Main function */

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;

  static int lastVisit[MAXN + 1];
  memset(lastVisit, 0, sizeof(lastVisit));
  int currentTime = 0;

  while (t--) {
    int n;
    cin >> n;

    vector<vector<int>> adj(n + 1);
    vector<int>         deg(n + 1, 0);

    for (int i = 0; i < n - 1; i++) {
      int u, v;
      cin >> u >> v;
      adj[u].push_back(v);
      adj[v].push_back(u);
      deg[u]++;
      deg[v]++;
    }

    if (n == 2) {
      cout << 0 << "\n";
      continue;
    }

    vector<int> vertices(n);
    iota(vertices.begin(), vertices.end(), 1);

    sort(vertices.begin(), vertices.end(), [&](int a, int b) { return deg[a] > deg[b]; });

    int limit = min(n, K);
    int best  = 0;

    for (int i = 0; i < limit; i++) {
      int v = vertices[i];
      currentTime++;
      lastVisit[v] = currentTime;

      for (int w : adj[v]) {
        lastVisit[w] = currentTime;
      }

      for (int j = i + 1; j < limit; j++) {
        int u      = vertices[j];
        int sumDeg = deg[v] + deg[u];
        if (lastVisit[u] == currentTime) {
          best = max(best, sumDeg - 2);
        } else {
          best = max(best, sumDeg - 1);
        }
      }
    }

    cout << best << "\n";
  }

  return 0;
}

//===---------------------------------------------------------------------===//