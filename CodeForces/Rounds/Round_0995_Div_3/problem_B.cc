//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Round 995 (Div. 3)
 * @author: Costantino Lombardi
 * @file: problem_B.cc
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
/* Main function */

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  while (t--) {
    long long n, a, b, c;
    cin >> n >> a >> b >> c;

    long long S = a + b + c;
    long long K = n / S;
    long long R = n % S;

    if (R == 0) {
      cout << 3 * K << "\n";
    } else {
      long long days = 3 * K;
      if (R <= a) {
        days += 1;
      } else if (R <= a + b) {
        days += 2;
      } else {
        days += 3;
      }
      cout << days << "\n";
    }
  }

  return 0;
}

//===---------------------------------------------------------------------===//
