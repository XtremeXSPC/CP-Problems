//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Round 1037 Div. 3
 * @author: Costantino Lombardi
 * @file: problem_F_v1.cc
 *
 * @status: NOT PASSED
 */
//===---------------------------------------------------------------------===//
/* Included library */

#include <bits/stdc++.h>
#include <iostream>
#include <map>

using namespace std;

#ifdef LOCAL
#include "algo/debug.h"
#else
#define debug(...) 42
#endif

//===---------------------------------------------------------------------===//
/* Function Definitions */

// Fast I/O
void setup_io() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
}

// Threshold based on the total constraint sum(N) + sum(Q) <= 2e5.
const int BLOCK_SIZE = 450;

// Function to solve a single test case
void solve() {
  int n;
  int q;
  cin >> n >> q;

  vector<int> colors(n + 1);
  for (int i = 1; i <= n; ++i) {
    cin >> colors[i];
  }

  vector<int> degree(n + 1, 0);

  vector<vector<pair<int, int>>> adj(n + 1);
  vector<tuple<int, int, int>>   edges;

  for (int i = 0; i < n - 1; ++i) {
    int u, v, c;
    cin >> u >> v >> c;
    adj[u].push_back({v, c});
    adj[v].push_back({u, c});
    degree[u]++;
    degree[v]++;
    edges.emplace_back(u, v, c);
  }

  // S[v][c] = sum of edge weights for edges from v to neighbors of color c
  vector<bool> is_heavy(n + 1, false);
  for (int i = 1; i <= n; ++i) {
    if (degree[i] > BLOCK_SIZE) {
      is_heavy[i] = true;
    }
  }

  vector<map<int, long long>> S(n + 1);

  long long total_cost = 0;

  // Initial calculation for total_cost and S maps for heavy nodes
  for (const auto& [u, v, c] : edges) {
    if (colors[u] != colors[v]) {
      total_cost += c;
    }
    if (is_heavy[u]) {
      S[u][colors[v]] += c;
    }
    if (is_heavy[v]) {
      S[v][colors[u]] += c;
    }
  }

  for (int k = 0; k < q; ++k) {
    int v_q, new_color;
    cin >> v_q >> new_color;

    int old_color = colors[v_q];
    if (old_color == new_color) {
      cout << total_cost << "\n";
      continue;
    }

    // Calculate cost change and update total_cost
    if (is_heavy[v_q]) {
      long long s_old = S[v_q].count(old_color) ? S[v_q][old_color] : 0;
      long long s_new = S[v_q].count(new_color) ? S[v_q][new_color] : 0;
      total_cost += s_old - s_new;
    } else { // v_q is light
      for (const auto& [neighbor, cost] : adj[v_q]) {
        if (colors[neighbor] == old_color) {
          total_cost += cost;
        }
        if (colors[neighbor] == new_color) {
          total_cost -= cost;
        }
      }
    }

    // Propagate the color change to the S maps of all neighbors
    for (const auto& [neighbor, cost] : adj[v_q]) {
      if (is_heavy[neighbor]) {
        S[neighbor][old_color] -= cost;
        S[neighbor][new_color] += cost;
      }
    }

    // Update color of the queried vertex
    colors[v_q] = new_color;

    cout << total_cost << "\n";
  }
}

//===---------------------------------------------------------------------===//
/* Main function */

int main() {
  setup_io();
  int t;
  cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}

//===---------------------------------------------------------------------===//
