/******************************************************************************
 * Codeforces Round 993 (Div. 4)                                              *
 * Author: Costantino Lombardi                                                *
 * File: problem_F.cc                                                         *
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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    cin >> n >> m >> q;

    vector<long long> a(n), b(m);
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int j = 0; j < m; j++) cin >> b[j];

    long long A = 0, B = 0;
    for (auto x : a) A += x;
    for (auto y : b) B += y;

    // Creiamo gli insiemi U e V contenenti (a_i - A) e (b_j - B)
    unordered_set<long long> U, V;
    U.reserve(n*2); U.max_load_factor(0.7);
    V.reserve(m*2); V.max_load_factor(0.7);

    for (auto x : a) U.insert(x - A);
    for (auto y : b) V.insert(y - B);

    // Funzione per trovare i divisori positivi di un numero positivo
    auto divisors = [&](long long val) {
        vector<long long> d;
        for (long long i = 1; i * i <= val; i++) {
            if (val % i == 0) {
                d.push_back(i);
                if (i * i != val) d.push_back(val / i);
            }
        }
        return d;
    };

    while (q--) {
        long long X; cin >> X;
        // Caso X = 0
        if (X == 0) {
            // Cerchiamo u * v = 0 => u=0 o v=0
            if (U.count(0) || V.count(0)) cout << "YES\n"; 
            else cout << "NO\n";
            continue;
        }

        bool possible = false;
        long long absX = llabs(X);
        vector<long long> d = divisors(absX);

        // Se X > 0:
        //   controlliamo (d, X/d) e (-d, -X/d)
        // Se X < 0:
        //   X = -Y, controlliamo (d, -Y/d) e (-d, Y/d)
        if (X > 0) {
            for (auto dd : d) {
                long long dd2 = X / dd; 
                // coppia (dd, dd2)
                if (U.count(dd) && V.count(dd2)) { possible = true; break; }
                // coppia (-dd, -dd2)
                if (U.count(-dd) && V.count(-dd2)) { possible = true; break; }
            }
        } else {
            // X < 0, X = -Y con Y = absX
            // coppie (d, -Y/d) e (-d, Y/d)
            for (auto dd : d) {
                long long dd2 = absX / dd;
                // (d, -dd2) => d * (-dd2) = - (d*dd2) se d*dd2=Y allora prod = -Y = X
                if (U.count(dd) && V.count(-dd2)) { possible = true; break; }
                // (-d, dd2)
                if (U.count(-dd) && V.count(dd2)) { possible = true; break; }
            }
        }

        cout << (possible ? "YES\n" : "NO\n");
    }

    return 0;
}

/* ========================================================================= */