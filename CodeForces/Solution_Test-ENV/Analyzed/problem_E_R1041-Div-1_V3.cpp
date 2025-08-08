//===---------------------------------------------------------------------===//
/**
 * @file problem_E_R1041-Div-1_V3.cpp
 * @brief Problem E solution for Round 1041, Division 1 (Version 3).
 * @version 0.1
 * @date 2025-08-08
 *
 * @copyright Copyright MIT 2025
 *
 */
//===---------------------------------------------------------------------===//
/* Included library */
#include <bits/stdc++.h>
#include <iostream>

//===---------------------------------------------------------------------===//
/* Types and Function Definitions */
using namespace std;

struct Graph {
  int                 n;
  vector<vector<int>> adj;
  Graph(int n) : n(n), adj(n + 1) {}
  void add_edge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  void prepare() {
    for (int i = 1; i <= n; i++) {
      sort(adj[i].begin(), adj[i].end());
    }
  }
};

struct MergeSortTree {
  int                 n;
  vector<vector<int>> tree;
  MergeSortTree(int n) : n(n) { tree.resize(4 * n); }
  void build(const vector<int>& arr, int node, int l, int r) {
    if (l == r) {
      tree[node] = {arr[l]};
      return;
    }
    int mid = (l + r) / 2;
    build(arr, node * 2, l, mid);
    build(arr, node * 2 + 1, mid + 1, r);
    tree[node].resize(tree[node * 2].size() + tree[node * 2 + 1].size());
    merge(tree[node * 2].begin(), tree[node * 2].end(), tree[node * 2 + 1].begin(), tree[node * 2 + 1].end(), tree[node].begin());
  }
  int count_leq(int node, int l, int r, int ql, int qr, int val) {
    if (qr < l || ql > r)
      return 0;
    if (ql <= l && r <= qr) {
      return upper_bound(tree[node].begin(), tree[node].end(), val) - tree[node].begin();
    }
    int mid = (l + r) / 2;
    return count_leq(node * 2, l, mid, ql, qr, val) + count_leq(node * 2 + 1, mid + 1, r, ql, qr, val);
  }
};

void solve() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  while (t--) {
    int n, m;
    cin >> n >> m;
    Graph g(n);
    for (int i = 0; i < m; i++) {
      int u, v;
      cin >> u >> v;
      g.add_edge(u, v);
    }
    g.prepare();

    // Precompute f(u, G[1..n]) directly
    vector<int> fval(n + 1, 0);
    vector<int> arr(n + 1);
    for (int u = 1; u <= n; u++) {
      int val = 0;
      for (int v : g.adj[u])
        val ^= v;
      arr[u] = val;
    }

    // Build Merge Sort Tree
    MergeSortTree mst(n);
    mst.build(arr, 1, 1, n);

    int q;
    cin >> q;
    while (q--) {
      int l, r, k;
      cin >> l >> r >> k;

      // Binary search to find k-th smallest
      int low = 0, high = n, ans = -1;
      while (low <= high) {
        int mid = (low + high) / 2;
        int cnt = mst.count_leq(1, 1, n, l, r, mid);
        if (cnt >= k) {
          ans  = mid;
          high = mid - 1;
        } else {
          low = mid + 1;
        }
      }
      cout << ans << "\n";
    }
  }
}

//===---------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  solve();
  return 0;
}

//===---------------------------------------------------------------------===//
