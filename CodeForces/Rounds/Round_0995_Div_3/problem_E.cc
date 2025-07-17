/******************************************************************************
 * Codeforces Round 995 Div. 3                                                *
 * Author: Costantino Lombardi                                                *
 * File: problem_E.cc                                                         *
 *****************************************************************************/

/* ========================================================================= */
/* Included library */

#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "algo/debug.h"
#else
#define debug(...) 42
#endif

static void fastIO() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
}

inline long long countGreaterOrEqual(const vector<long long>& v, long long x) {
  auto it = std::lower_bound(v.begin(), v.end(), x);
  return (long long)v.size() - (it - v.begin());
}

/* ========================================================================= */
/* Main function */

int main() {
  fastIO();

  int t;
  cin >> t;
  while (t--) {
    long long n, k;
    cin >> n >> k;

    vector<long long> a(n), b(n);
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }
    for (int i = 0; i < n; i++) {
      cin >> b[i];
    }

    vector<long long> sortedA = a, sortedB = b;
    sort(sortedA.begin(), sortedA.end());
    sort(sortedB.begin(), sortedB.end());

    vector<long long> candidates;
    candidates.reserve(3 * n);
    for (int i = 0; i < n; i++) {
      candidates.push_back(a[i]);
      candidates.push_back(b[i]);
      if (a[i] + 1 <= b[i]) {
        candidates.push_back(a[i] + 1);
      }
    }

    sort(candidates.begin(), candidates.end());
    candidates.erase(unique(candidates.begin(), candidates.end()), candidates.end());

    long long answer = 0;

    for (auto& c : candidates) {
      long long buy = countGreaterOrEqual(sortedB, c);
      if (buy == 0) {
        continue;
      }
      long long pos = countGreaterOrEqual(sortedA, c);
      long long neg = buy - pos;

      if (neg <= k) {
        long long revenue = c * buy;
        if (revenue > answer) {
          answer = revenue;
        }
      }
    }

    cout << answer << "\n";
  }

  return 0;
}

/* ========================================================================= */