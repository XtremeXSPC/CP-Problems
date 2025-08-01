//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Round 1037 Div. 3
 * @author: Costantino Lombardi
 * @file: problem_D.cc
 *
 * @status: PASSED
 */
//===---------------------------------------------------------------------===//
/* Included library */

#include <bits/stdc++.h>
#include <iostream>
#include <queue>
#include <set>

using namespace std;

#ifdef LOCAL
#include "../../Algorithms/debug.h"
#else
#define debug(...) 42
#endif

//===---------------------------------------------------------------------===//
/* Structure and Classes */

// Structure to hold casino parameters
struct Casino {
  int       id;
  long long l, r, real;
};

//===---------------------------------------------------------------------===//
/* Function Definitions */

// Fast I/O
void setup_io() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
}

// Function to solve a single test case
void solve() {
  int       n;
  long long k;
  cin >> n >> k;

  vector<Casino> casinos(n);
  for (int i = 0; i < n; ++i) {
    casinos[i].id = i;
    cin >> casinos[i].l >> casinos[i].r >> casinos[i].real;
  }

  long long max_coins = k;

  // Queue for the BFS on coin amounts
  queue<long long> q;
  q.push(k);

  // Set to keep track of reachable coin amounts to avoid redundant processing
  set<long long> reachable_coins;
  reachable_coins.insert(k);

  while (!q.empty()) {
    long long current_coins = q.front();
    q.pop();

    max_coins = max(max_coins, current_coins);

    vector<Casino> remaining_casinos;
    bool           changed = false;

    for (const auto& casino : casinos) {
      // Check if the current casino is visitable
      if (current_coins >= casino.l && current_coins <= casino.r) {
        // If we can generate a new coin amount, add it to the queue
        if (reachable_coins.find(casino.real) == reachable_coins.end()) {
          q.push(casino.real);
          reachable_coins.insert(casino.real);
        }
      } else {
        // If not used, keep it for the next iteration
        remaining_casinos.push_back(casino);
      }
    }

    // The list of available casinos shrinks in each step of the BFS
    if (remaining_casinos.size() < casinos.size()) {
      casinos = remaining_casinos;
    }
  }

  cout << max_coins << "\n";
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
