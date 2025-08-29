//===----------------------------------------------------------------------===//
/**
 * @brief Codeforces Round 1037 Div. 3
 * @author: Costantino Lombardi
 * @file: problem_C.cc
 *
 * @status: PASSED
 */
//===----------------------------------------------------------------------===//
/* Included library */

// clang-format off
// Compiler optimizations:
#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC optimize("Ofast,unroll-loops,fast-math,O3")
  // Apple Silicon optimizations:
  #ifdef __aarch64__
    #pragma GCC target("+simd")
  #endif
#endif

#ifdef __clang__
  #pragma clang optimize on
#endif

// Sanitaze macro:
#ifdef USE_CLANG_SANITIZE
  #include "PCH.h"
#else
  #include <bits/stdc++.h>
#endif

// Debug macro:
#ifdef LOCAL
  #include "debug.h"
#else
  #define debug(...) 42
#endif
// clang-format on

using namespace std;

using ll     = long long;
const ll INF = 1e18;

//===----------------------------------------------------------------------===//
/* Function Definitions */

// Fast I/O
void setup_io() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
}

// Function to solve a single test case
void solve() {
  int n;
  int k;
  cin >> n >> k;
  k--; // 0-indexed

  vector<ll> h(n);
  ll         h_max = 0;
  for (int i = 0; i < n; ++i) {
    cin >> h[i];
    if (h[i] > h_max) {
      h_max = h[i];
    }
  }

  if (h[k] == h_max) {
    cout << "YES\n";
    return;
  }

  priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;

  vector<ll>         dist(n, INF);
  set<pair<ll, int>> unvisited;

  for (int i = 0; i < n; ++i) {
    unvisited.insert({h[i], i});
  }

  dist[k] = 0;
  pq.push({0, k});

  while (!pq.empty()) {
    auto top_pair = pq.top();
    ll   time     = top_pair.first;
    int  u        = top_pair.second;
    pq.pop();

    if (time > dist[u]) {
      continue;
    }

    if (h[u] == h_max) {
      cout << "YES\n";
      return;
    }

    unvisited.erase({h[u], u});

    auto relax_edge = [&](int v) {
      if (u == v)
        return;
      ll time_cost    = abs(h[u] - h[v]);
      ll arrival_time = time + time_cost;

      bool can_survive_on_u = (arrival_time <= h[u]);
      bool can_survive_on_v = (arrival_time + 1 <= h[v]);

      if (can_survive_on_u && can_survive_on_v) {
        if (arrival_time < dist[v]) {
          dist[v] = arrival_time;
          pq.push({dist[v], v});
        }
      }
    };

    // Find and check the two best neighbors in the unvisited set
    auto it = unvisited.lower_bound({h[u], -1});

    if (it != unvisited.end()) {
      relax_edge(it->second);
    }
    if (it != unvisited.begin()) {
      relax_edge(prev(it)->second);
    }
  }

  cout << "NO\n";
}

//===----------------------------------------------------------------------===//
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

//===----------------------------------------------------------------------===//
