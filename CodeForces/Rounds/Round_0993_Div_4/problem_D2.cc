/******************************************************************************
 * Codeforces Round 993 (Div. 4)                                              *
 * Author: Costantino Lombardi                                                *
 * File: problem_D2.cc                                                        *
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

/* ========================================================================= */
/* Main function */

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        vector<int> a(n);
        for(int i = 0; i < n; i++) cin >> a[i];

        vector<int> freq(n+1, 0);
        int max_freq = 0;
        vector<int> b(n, 0);

        // next_filler punterà al prossimo numero candidato come filler (con freq = 0)
        // Inizialmente i filler potenziali sono tutti i numeri da 1 a n, li scorreremo per trovarne uno non usato.
        int next_filler = 1; 

        for (int i = 0; i < n; i++) {
            int needed = a[i];
            if (freq[needed] < max_freq) {
                // freq[a_i] < max_freq: dobbiamo incrementare freq[a_i]
                b[i] = needed;
                freq[needed]++;
                if (freq[needed] > max_freq) max_freq = freq[needed];
            } else {
                // freq[a_i] == max_freq
                if (max_freq == 0) {
                    // primo elemento: deve essere a_i
                    b[i] = needed;
                    freq[needed] = 1;
                    max_freq = 1;
                } else {
                    // cerchiamo un filler con freq=0 diverso da needed, se possibile
                    int chosen = 0;
                    while (next_filler <= n && (next_filler == needed || freq[next_filler] != 0)) {
                        next_filler++;
                    }
                    if (next_filler <= n) {
                        // trovato filler con freq=0
                        chosen = next_filler;
                        b[i] = chosen;
                        freq[chosen] = 1; // ora freq[chosen]=1
                        // freq[a_i] = max_freq, freq[chosen]=1 ≤ max_freq (se max_freq≥1)
                        // Se max_freq=1, allora 1=1 e a_i resta una moda.
                    } else {
                        // Nessun filler freq=0 disponibile,
                        // scegliamo a_i per incrementarne la frequenza
                        b[i] = needed;
                        freq[needed]++;
                        if (freq[needed] > max_freq) {
                            max_freq = freq[needed];
                        }
                    }
                }
            }
        }

        for (int i = 0; i < n; i++) {
            cout << b[i] << (i == n-1 ? '\n' : ' ');
        }
    }

    return 0;
}

/* ========================================================================= */