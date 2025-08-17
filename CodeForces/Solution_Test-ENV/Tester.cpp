//===---------------------------------------------------------------------===//
/**
 * @file Tester.cpp
 * @brief Tester for coding data structures and algorithms.
 * @version 0.1
 * @date 2025-05-20
 *
 * @copyright Copyright MIT 2025
 *
 */
//===---------------------------------------------------------------------===//
/* Included library */

// Debug macro:
#ifdef LOCAL
#include "../Algorithms/debug.h"
#else
#define debug(...) 42
#endif

// Sanitaze macro:
#ifdef USE_CLANG_SANITIZE
#include "PCH.h"
#else
#include <bits/stdc++.h>
#endif

//===---------------------------------------------------------------------===//
/* Types and Function Definitons */
using namespace std;

struct Graph {
  int                 n;
  vector<vector<int>> adj;
  Graph(int n);
  void add_edge(int u, int v);
  void prepare();
  auto xor_up_to(int u, int x) -> int;
};

Graph::Graph(int n) : n(n), adj(n + 1, vector<int>()) {
}

void Graph::add_edge(int u, int v) {
  adj[u].push_back(v);
  adj[v].push_back(u);
}

void Graph::prepare() {
  for (int i = 1; i <= n; i++) {
    sort(adj[i].begin(), adj[i].end());
  }
}

auto Graph::xor_up_to(int u, int x) -> int {
  auto& vec = adj[u];
  int   idx = upper_bound(vec.begin(), vec.end(), x) - vec.begin();
  int   res = 0;
  for (int i = 0; i < idx; i++)
    res ^= vec[i];
  return res;
}

struct Query {
  int l, r, k, idx;
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
  // Start timer
  Timer timer;

  // Fast I/O
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

    int q;
    cin >> q;
    vector<Query> queries(q);
    for (int i = 0; i < q; i++) {
      cin >> queries[i].l >> queries[i].r >> queries[i].k;
      queries[i].idx = i;
    }

    vector<int> results(q);
    for (auto& qr : queries) {
      // calcolo i f(u) per l..r
      vector<int> vals;
      vals.reserve(qr.r - qr.l + 1);
      for (int u = qr.l; u <= qr.r; u++) {
        int val = g.xor_up_to(u, qr.r) ^ g.xor_up_to(u, qr.l - 1);
        vals.push_back(val);
      }
      sort(vals.begin(), vals.end());
      results[qr.idx] = vals[qr.k - 1];
    }

    for (int i = 0; i < q; i++) {
      cout << results[i] << "\n";
    }
  }
}

//===---------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  // Fast I/O
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  solve();
  return 0;
}

//===---------------------------------------------------------------------===//
