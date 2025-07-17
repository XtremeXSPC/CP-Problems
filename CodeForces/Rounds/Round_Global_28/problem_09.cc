/******************************************************************************
 * Codeforces Global Round 28                                                 *
 * Author: Costantino Lombardi                                                *
 * File: problem_09.cc                                                        *
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
    while(t--){
        int n; cin >> n;
        string s; cin >> s;

        vector<int> a(n, 1);

        if (s[0] == 'L') {
            a[0] = 0;
        }
        if (s[n-1] == 'R') {
            a[n-1] = 0;
        }

        auto c = [&](int l, int r) {
            if (l > r) return 0;

            bool zero = false, one = false;
            for(int i=l; i<=r; i++){
                if (a[i] == 0) zero = true;
                if (a[i] == 1) one = true;
                if (zero && one) return 2;
            }
            if (!zero && !one) return 0;
            return 1;
        };

        // Verifica
        bool ok = true;
        for (int i = 0; i < n && ok; i++) {
            if (s[i] == 'L') {
                int val = c(0, i-1);
                if (a[i] != val) ok = false;
            } else { // s[i] == 'R'
                int val = c(i+1, n-1);
                if (a[i] != val) ok = false;
            }
        }

        if (!ok) {

            cout << -1 << "\n";
        } else {
            for (int i = 0; i < n; i++){
                cout << a[i] << (i+1==n ? '\n':' ');
            }
        }
    }

    return 0;
}

/* ========================================================================= */