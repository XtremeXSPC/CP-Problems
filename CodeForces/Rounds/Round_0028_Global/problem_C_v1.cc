//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Global Round 28
 * @author: Costantino Lombardi
 * @file: problem_C_v1.cc
 *
 * @status: NOT PASSED
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

// Confronta due stringhe binarie di uguale lunghezza n.
// Ritorna true se a > b in ordine lessicografico.
bool greaterBinary(const string& a, const string& b) {
  return a > b;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);

  int t;
  cin >> t;
  while (t--) {
    string s;
    cin >> s;
    int n = (int)s.size();

    // Convertiamo s in array di int per comodità (0/1)
    vector<int> bits(n);
    for (int i = 0; i < n; i++) {
      bits[i] = (s[i] == '1');
    }

    string bestXOR(n, '0');
    // Salviamo gli indici delle sottostringhe corrispondenti
    int best_l1 = 1, best_r1 = 1, best_l2 = 1, best_r2 = 1;

    for (int m = 1; m < n; m++) {

      vector<int> B(m);
      for (int k = 0; k < m; k++) {
        B[k] = bits[n - m + k];
      }

      string prefix;
      prefix.push_back('1'); // primo bit fisso
      for (int pos = 2; pos <= n - m; pos++) {
        prefix.push_back(bits[pos - 1] ? '1' : '0');
      }

      for (int i = 0; i <= n - m; i++) {
        // Costruiamo XOR come stringa

        bool   better = false;
        bool   worse  = false;
        string currentXOR;
        currentXOR.reserve(n);

        for (int idx = 0; idx < (int)prefix.size(); idx++) {
          char c = prefix[idx];
          if (!better && !worse) {
            if (c > bestXOR[idx]) {
              better = true;
            } else if (c < bestXOR[idx]) {
              worse = true;
            }
          }
          currentXOR.push_back(c);
        }

        for (int k = 0; k < m; k++) {
          int  x   = B[k] ^ bits[i + k];
          char c   = x ? '1' : '0';
          int  pos = (int)prefix.size() + k;

          if (!better && !worse) {
            if (c > bestXOR[pos]) {
              better = true;
            } else if (c < bestXOR[pos]) {
              worse = true;
            }
          }
          currentXOR.push_back(c);

          if (worse)
            break;
        }

        if (worse) {
          // Scartiamo questa soluzione
          continue;
        }

        if (better) {
          bestXOR = currentXOR;
          // Prima sottostringa: (1, n)
          best_l1 = 1;
          best_r1 = n;
          // Seconda sottostringa: (i+1, i+m)
          best_l2 = i + 1;
          best_r2 = i + m;
        } else if (!worse && bestXOR == currentXOR) {
        }
      }
    }

    cout << best_l1 << " " << best_r1 << " " << best_l2 << " " << best_r2 << "\n";
  }
  return 0;
}

//===---------------------------------------------------------------------===//
