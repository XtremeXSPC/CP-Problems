//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Round 1037 Div. 3
 * @author: Costantino Lombardi
 * @file: problem_E.cc
 *
 * @status: PASSED
 */
//===---------------------------------------------------------------------===//
/* Included library */

#include <bits/stdc++.h>
#include <iostream>
#include <set>

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

// Function to compute Least Common Multiple - LCM
long long lcm(long long a, long long b) {
  if (a == 0 || b == 0)
    return 0;
  return abs(a * b) / gcd(a, b);
}

// Function to solve a single test case
void solve() {
  int n;
  cin >> n;
  vector<long long> p(n), s(n);
  for (int i = 0; i < n; ++i)
    cin >> p[i];
  for (int i = 0; i < n; ++i)
    cin >> s[i];

  bool possible = true;

  // "Condition 1: p_n must be equal to s_1 (0-indexed: p[n-1] == s[0])"
  if (p[n - 1] != s[0]) {
    possible = false;
  }

  // "Condition 2: p[i] must divide p[i-1] (0-indexed: p[i+1] must divide p[i])"
  if (possible) {
    for (int i = 0; i < n - 1; ++i) {
      if (p[i] % p[i + 1] != 0) {
        possible = false;
        break;
      }
    }
  }

  // "Condition 3: s[i] must divide s[i+1] (0-indexed: s[i] must divide s[i+1])"
  if (possible) {
    for (int i = 0; i < n - 1; ++i) {
      if (s[i + 1] % s[i] != 0) {
        possible = false;
        break;
      }
    }
  }

  // "Condition 4: gcd(p[i], s[i+1]) must be equal to p[n-1]"
  if (possible) {
    for (int i = 0; i < n - 1; ++i) {
      if (gcd(p[i], s[i + 1]) != p[n - 1]) {
        possible = false;
        break;
      }
    }
  }

  if (possible) {
    cout << "YES\n";
  } else {
    cout << "NO\n";
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
