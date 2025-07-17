/******************************************************************************
 * Codeforces Round 993 (Div. 4)                                              *
 * Author: Costantino Lombardi                                                *
 * File: problem_B.cc                                                         *
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

    int t; cin >> t;
    while (t--) {
        string a; cin >> a;
        reverse(a.begin(), a.end());
        for (auto &ch : a) {
            if (ch == 'p') ch = 'q';
            else if (ch == 'q') ch = 'p';
            // 'w' rimane invariato
        }
        cout << a << "\n";
    }

    return 0;
}
/* ========================================================================= */