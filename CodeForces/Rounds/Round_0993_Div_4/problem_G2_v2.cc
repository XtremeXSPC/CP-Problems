//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Round 993 (Div. 4)
 * @author: Costantino Lombardi
 * @file: problem_G2_v2.cc
 *
 * @status: NOT PASSED
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
/* Main function */

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;

  while (t--) {
    int n;
    cin >> n;
    vector<int>  r(n + 1);
    vector<int>  peluches(n + 1, 1); // Inizialmente ogni ragno ha un peluche
    vector<bool> visited(n + 1, false);

    for (int i = 1; i <= n; i++) {
      cin >> r[i];
    }

    // Simulazione fino alla stabilizzazione
    int  year   = 1;
    bool stable = false;

    while (!stable) {
      stable                   = true;
      vector<int> new_peluches = peluches;

      // Distribuiamo i peluche per quest'anno
      for (int i = 1; i <= n; i++) {
        if (peluches[i] > 0) {
          new_peluches[r[i]]++; // Ogni ragno "i" regala un peluche al suo amico r[i]
          new_peluches[i]--;    // Simultaneamente "i" perde un peluche
        }
      }

      // Controlliamo se la configurazione è stabile
      for (int i = 1; i <= n; i++) {
        if (peluches[i] != new_peluches[i]) {
          stable = false;
          break;
        }
      }

      peluches = new_peluches;
      year++;
    }

    cout << year << "\n";
  }

  return 0;
}

//===---------------------------------------------------------------------===//
