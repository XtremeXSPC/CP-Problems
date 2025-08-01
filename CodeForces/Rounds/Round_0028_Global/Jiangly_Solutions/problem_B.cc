//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Global Round 28
 * @author: Codeforces User "Jiangly"
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

using i64  = long long;
using u64  = unsigned long long;
using u32  = unsigned;
using u128 = unsigned __int128;

void solve() {
  int n, k;
  std::cin >> n >> k;

  std::vector<int> p(n, -1);
  int              cur = 0;
  for (int i = k - 1; i < n; i += k) {
    p[i] = ++cur;
  }
  for (int i = 0; i < n; i++) {
    if (p[i] == -1) {
      p[i] = ++cur;
    }
    std::cout << p[i] << " \n"[i == n - 1];
  }
}

//===---------------------------------------------------------------------===//
/* Main function */

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }

  return 0;
}

//===---------------------------------------------------------------------===//