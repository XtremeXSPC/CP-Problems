//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Round 993 (Div. 4)
 * @author: Costantino Lombardi
 * @file: problem_E.cc
 *
 * @status: PASSED
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
    long long k, l1, r1, l2, r2;
    cin >> k >> l1 >> r1 >> l2 >> r2;

    // Itereremo sui possibili valori di n fino a quando k^n > r2 o overflow.
    // Iniziamo con n=0: k^n=1
    long long countPairs = 0;
    if (l2 > r2) {
      // Nessuna coppia valida se l2 > r2 ma i vincoli del problema
      // non permettono comunque questo caso.
      cout << 0 << "\n";
      continue;
    }

    // Casi limite:
    // Se k è grande e r2 è piccolo, potremmo avere pochissimi n da controllare.
    // Calcoleremo le potenze di k incrementando e ci fermeremo quando non ci saranno più n validi.

    __int128 kn = 1; // per controllare in sicurezza l'overflow quando si moltiplica per k
    while (true) {
      // kn = k^n
      // Controlla se kn > r2: non c'è bisogno di continuare perché
      // y = x * kn e min x=1 => y>=kn, se kn > r2 nessuna soluzione.
      if (kn > r2)
        break;

      long long KN = (long long)kn; // sicuro da castare perché ci fermiamo prima che si verifichi l'overflow

      // Calcola l'intervallo per x:
      // A = ceil(l2/KN) = (l2+KN-1)//KN
      long long A = (l2 + KN - 1) / KN;
      // B = floor(r2/KN)
      long long B = r2 / KN;

      // Intersezione con [l1,r1]:
      long long L = max(l1, A);
      long long R = min(r1, B);
      if (R >= L) {
        countPairs += (R - L + 1);
      }

      // Prova il prossimo n:
      // Controlla l'overflow quando si moltiplica per k:
      __int128 next = kn * k;
      if (next > LLONG_MAX) { // prevenire l'overflow sul cast
        break;
      }
      kn = next;
    }

    cout << countPairs << "\n";
  }
  return 0;
}

//===---------------------------------------------------------------------===//
