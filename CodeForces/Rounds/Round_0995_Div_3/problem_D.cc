//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Round 995 (Div. 3)
 * @author: Costantino Lombardi
 * @file: problem_D.cc
 *
 * @status: PASSED
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
/* Function Definitions */

long long countPairsLEQ(const vector<long long>& a, long long val) {
  long long count = 0;
  int       left = 0, right = (int)a.size() - 1;
  while (left < right) {
    if (a[left] + a[right] <= val) {
      count += (right - left);
      left++;
    } else {
      right--;
    }
  }
  return count;
}

//===---------------------------------------------------------------------===//
/* Main function */

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  while (t--) {
    long long n, x, y;
    cin >> n >> x >> y;
    vector<long long> a(n);
    long long         sumA = 0;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
      sumA += a[i];
    }

    long long L = sumA - y;
    long long R = sumA - x;

    sort(a.begin(), a.end());

    long long cntLEQ_R   = countPairsLEQ(a, R);
    long long cntLEQ_Lm1 = countPairsLEQ(a, L - 1);

    long long answer = cntLEQ_R - cntLEQ_Lm1;
    cout << answer << "\n";
  }

  return 0;
}

//===---------------------------------------------------------------------===//
