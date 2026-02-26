//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Global Round 28
 * @author: Costantino Lombardi
 * @file: problem_B_v2.cc
 *
 * @status: PASSED
 */
//===---------------------------------------------------------------------===//
/* Included library */

#include <bits/stdc++.h>

using namespace std;

//===---------------------------------------------------------------------===//
/* Main function */

#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  while (t--) {
    int n, k;
    cin >> n >> k;
    int m          = n - k + 1;
    int fullChunks = m / k;
    int r          = m % k;

    vector<int> intersectionPositions;
    for (int i = 0; i < fullChunks; i++) {
      intersectionPositions.push_back((i + 1) * k);
    }
    if (r > 0) {
      intersectionPositions.push_back((fullChunks + 1) * k);
    }

    int requiredMinCount = (int)intersectionPositions.size();

    vector<int> p(n, 0);

    for (int i = 0; i < requiredMinCount; i++) {
      p[intersectionPositions[i] - 1] = i + 1;
    }

    int cur = requiredMinCount + 1;
    for (int i = 0; i < n; i++) {
      if (p[i] == 0) {
        p[i] = cur++;
      }
    }

    for (int i = 0; i < n; i++) {
      cout << p[i] << (i + 1 == n ? '\n' : ' ');
    }
  }

  return 0;
}
//===---------------------------------------------------------------------===//
