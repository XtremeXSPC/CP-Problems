//===---------------------------------------------------------------------===//
/**
 * @file problem_E_R1041-Div-1_V1.cpp
 * @brief Problem E solution for Round 1041, Division 1.
 * @version 0.1
 * @date 2025-08-08
 *
 * @copyright Copyright MIT 2025
 *
 */
//===---------------------------------------------------------------------===//
/* Included library */
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <iostream>

//===---------------------------------------------------------------------===//
/* Types and Function Definitions */
using namespace std;
using namespace __gnu_pbds;

template <typename T>
using ordered_multiset = tree<pair<T, int>, null_type, less<pair<T, int>>, rb_tree_tag, tree_order_statistics_node_update>;

struct Query {
  int l, r, k, idx;
};

const int             MAXN = 150005;
vector<int>           adj[MAXN];
int                   fval[MAXN];
bool                  present[MAXN];
int                   n, m, q;
ordered_multiset<int> os;

void add_node(int u) {
  present[u] = true;
  for (int v : adj[u]) {
    if (present[v]) {
      os.erase({fval[v], v});
      fval[v] ^= u;
      os.insert({fval[v], v});

      os.erase({fval[u], u});
      fval[u] ^= v;
      os.insert({fval[u], u});
    }
  }
  if (os.find({fval[u], u}) == os.end())
    os.insert({fval[u], u});
}

void remove_node(int u) {
  present[u] = false;
  os.erase({fval[u], u});
  for (int v : adj[u]) {
    if (present[v]) {
      os.erase({fval[v], v});
      fval[v] ^= u;
      os.insert({fval[v], v});
    }
  }
  fval[u] = 0;
}

void solve() {
  int t;
  cin >> t;
  while (t--) {
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
      adj[i].clear();
      fval[i]    = 0;
      present[i] = false;
    }
    for (int i = 0; i < m; i++) {
      int u, v;
      cin >> u >> v;
      adj[u].push_back(v);
      adj[v].push_back(u);
    }
    cin >> q;
    vector<Query> queries(q);
    for (int i = 0; i < q; i++) {
      cin >> queries[i].l >> queries[i].r >> queries[i].k;
      queries[i].idx = i;
    }

    int block = max(1, (int)(sqrt(n)));
    sort(queries.begin(), queries.end(), [&](auto& a, auto& b) {
      int ba = a.l / block, bb = b.l / block;
      if (ba != bb)
        return ba < bb;
      return (ba & 1) ? a.r > b.r : a.r < b.r;
    });

    vector<int> ans(q);
    int         L = 1, R = 0;
    os.clear();
    for (auto& qr : queries) {
      while (L > qr.l)
        add_node(--L);
      while (R < qr.r)
        add_node(++R);
      while (L < qr.l)
        remove_node(L++);
      while (R > qr.r)
        remove_node(R--);
      ans[qr.idx] = os.find_by_order(qr.k - 1)->first;
    }

    for (int i = 0; i < q; i++) {
      cout << ans[i] << "\n";
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
