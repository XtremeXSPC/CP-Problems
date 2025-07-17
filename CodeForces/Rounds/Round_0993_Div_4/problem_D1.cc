/******************************************************************************
 * Codeforces Round 993 (Div. 4)                                              *
 * Author: Costantino Lombardi                                                *
 * File: problem_D.cc                                                         *
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
    cin.tie(NULL);

    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; i++) cin >> a[i];

        vector<int> freq(n+1,0);
        vector<int> b(n);
        int maxFreq = 0;
        int nextUnused = 1;

        for (int i = 0; i < n; i++) {
            int x = a[i];
            if (freq[x] < maxFreq) {
                // Dobbiamo portare freq[x] a maxFreq inserendo x
                b[i] = x;
                freq[x]++;
                // Aggiorno maxFreq se necessario
                if (freq[x] > maxFreq) {
                    maxFreq = freq[x];
                }
            } else if (freq[x] == maxFreq) {
                // x è già al livello di maxFreq, cerchiamo un filler con freq=0
                int filler = 0;
                while (nextUnused <= n && freq[nextUnused] != 0) {
                    nextUnused++;
                }

                if (nextUnused <= n && freq[nextUnused] == 0) {
                    // Usiamo il filler
                    filler = nextUnused;
                    b[i] = filler;
                    freq[filler] = 1;
                    // Se maxFreq era 0 significa che adesso abbiamo freq[filler]=1, 
                    // quindi maxFreq deve diventare almeno 1
                    if (maxFreq == 0) {
                        maxFreq = 1;
                    }
                } else {
                    // Niente filler disponibile, incrementiamo la freq di x
                    b[i] = x;
                    freq[x]++;
                    if (freq[x] > maxFreq) {
                        maxFreq = freq[x];
                    }
                }
            } else {
                // freq[x] > maxFreq non dovrebbe mai accadere con questa logica.
                // In caso, lo gestiamo portando maxFreq in pari.
                b[i] = x;
                freq[x]++;
                if (freq[x] > maxFreq) {
                    maxFreq = freq[x];
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