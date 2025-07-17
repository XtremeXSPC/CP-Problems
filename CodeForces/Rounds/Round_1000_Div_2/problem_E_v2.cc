//===---------------------------------------------------------------------===//
/* Codeforces Round 1000 Div. 2
 * Author: Costantino Lombardi
 * File: problem_E_v2.cc
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
/* Global variable and functions */

static const int MAXN = 300000;

vector<int> adj[MAXN + 1];
int         depth[MAXN + 1];
long long   ans;

struct DS {
  vector<int>       arr;
  vector<long long> pref;
};

static void buildPrefix(DS& ds) {
  ds.pref.resize(ds.arr.size() + 1);
  ds.pref[0] = 0LL;
  for (size_t i = 0; i < ds.arr.size(); i++) {
    ds.pref[i + 1] = ds.pref[i] + ds.arr[i];
  }
}

static int countAbove(const DS& ds, int limit) {
  int idx = int(upper_bound(ds.arr.begin(), ds.arr.end(), limit) - ds.arr.begin());
  return int(ds.arr.size()) - idx;
}

static long long sumMinAbove(const DS& ds, int val, int limit) {
  if (val <= limit)
    return 0;

  int idx    = int(upper_bound(ds.arr.begin(), ds.arr.end(), limit) - ds.arr.begin());
  int idxVal = int(upper_bound(ds.arr.begin() + idx, ds.arr.end(), val) - ds.arr.begin());

  long long part1 = ds.pref[idxVal] - ds.pref[idx];

  int       cntAboveVal = (int)ds.arr.size() - idxVal;
  long long part2       = (long long)val * cntAboveVal;

  return part1 + part2;
}

void mergeDS(DS& A, DS& B, int wDepth) {
  int       validA     = countAbove(A, wDepth);
  int       validB     = countAbove(B, wDepth);
  long long validPairs = (long long)validA * validB;

  long long cross = 0;

  for (int y : B.arr) {
    if (y <= wDepth)
      continue;
    long long sm = sumMinAbove(A, y, wDepth);
    cross += 2LL * sm;
  }
  cross -= (2LL * wDepth + 1) * validPairs;

  ans += cross;

  {
    vector<int> tmp;
    tmp.reserve(A.arr.size() + B.arr.size());
    // merge standard
    int i = 0, j = 0;
    while (i < (int)A.arr.size() && j < (int)B.arr.size()) {
      if (A.arr[i] <= B.arr[j]) {
        tmp.push_back(A.arr[i++]);
      } else {
        tmp.push_back(B.arr[j++]);
      }
    }
    while (i < (int)A.arr.size())
      tmp.push_back(A.arr[i++]);
    while (j < (int)B.arr.size())
      tmp.push_back(B.arr[j++]);

    A.arr.swap(tmp);
    buildPrefix(A);
  }
}

// DFS
DS dfs(int u, int p) {
  DS cur;
  cur.arr.clear();
  cur.pref.clear();

  for (int v : adj[u]) {
    if (v == p)
      continue;
    depth[v]   = depth[u] + 1;
    DS childDS = dfs(v, u);

    if (childDS.arr.size() > cur.arr.size()) {
      swap(cur, childDS);
    }
    mergeDS(cur, childDS, depth[u]);
  }

  cur.arr.push_back(depth[u]);
  buildPrefix(cur);

  return cur;
}

//===---------------------------------------------------------------------===//
/* Main function */

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) {
      adj[i].clear();
    }
    for (int i = 1; i < n; i++) {
      int u, v;
      cin >> u >> v;
      adj[u].push_back(v);
      adj[v].push_back(u);
    }

    ans = 0LL;
    if (n == 1) {
      // Nessuna coppia
      cout << 0 << "\n";
      continue;
    }

    depth[1] = 0;
    dfs(1, -1);

    cout << ans << "\n";
  }

  return 0;
}

//===---------------------------------------------------------------------===//