//===---------------------------------------------------------------------===//
/* Codeforces Round 1000 Div. 2
 * Author: Costantino Lombardi
 * File: problem_E_v1.cc
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
  map<int, int> freq;
  int           size = 0;
};

long long countAbove(const DS& ds, int limit) {
  long long c  = 0;
  auto      it = ds.freq.upper_bound(limit);
  while (it != ds.freq.end()) {
    c += it->second;
    ++it;
  }
  return c;
}

long long sumMinAbove(const DS& ds, int val, int limit) {
  if (val <= limit)
    return 0;

  long long ret = 0;
  auto      it  = ds.freq.lower_bound(limit + 1);

  long long cAbove = 0;

  while (it != ds.freq.end()) {
    int x   = it->first;
    int cnt = it->second;
    if (x <= val) {
      ret += (long long)x * cnt;
    } else {
      cAbove += cnt;
    }
    ++it;
  }

  ret += (long long)val * cAbove;
  return ret;
}

void mergeDS(DS& A, DS& B, int wDepth) {
  long long validA     = countAbove(A, wDepth);
  long long validB     = countAbove(B, wDepth);
  long long validPairs = validA * validB;

  long long cross = 0;

  for (auto& pb : B.freq) {
    int dB = pb.first;
    int cB = pb.second;
    if (dB <= wDepth)
      continue;

    long long sm = sumMinAbove(A, dB, wDepth);
    cross += 2LL * sm * cB;
  }

  cross -= (2LL * wDepth + 1) * validPairs;

  ans += cross;

  for (auto& pb : B.freq) {
    A.freq[pb.first] += pb.second;
  }
  A.size += B.size;
}

DS dfs(int u, int p) {
  DS cur;
  cur.size = 0;

  for (int v : adj[u]) {
    if (v == p)
      continue;
    depth[v]   = depth[u] + 1;
    DS childDS = dfs(v, u);
    if (childDS.size > cur.size) {
      swap(cur, childDS);
    }
    mergeDS(cur, childDS, depth[u]);
  }

  cur.freq[depth[u]]++;
  cur.size++;

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
    cin >> std::ws;
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