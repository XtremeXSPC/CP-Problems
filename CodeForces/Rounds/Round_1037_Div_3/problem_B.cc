//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Round 1037 Div. 3
 * @uthor: Costantino Lombardi
 * @file: problem_B.cc
 */
//===---------------------------------------------------------------------===//
/* Included library */

#include <bits/stdc++.h>
#include <iostream>

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

// Function to solve a single test case
void solve() {
  int n;
  int k;
  cin >> n >> k;
  vector<int> a(n);
  for (int i = 0; i < n; ++i) {
    cin >> a[i];
  }

  int hikes_count = 0;
  int i           = 0;
  while (i <= n - k) {
    // Check for k consecutive good weather days.
    bool is_window_good = true;
    for (int j = 0; j < k; ++j) {
      if (a[i + j] == 1) {
        // Found a rainy day, hike is not possible here.
        is_window_good = false;
        // We can jump our search to the day after the rainy day.
        i = i + j + 1;
        break;
      }
    }

    if (is_window_good) {
      hikes_count++;
      // Hike found, next possible start is after these k days.
      i += k + 1;
    }
  }

  cout << hikes_count << "\n";
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
