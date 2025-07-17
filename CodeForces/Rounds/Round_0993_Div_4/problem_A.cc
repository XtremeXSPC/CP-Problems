/*****************************************************************************
 * Codeforces Round 993 (Div. 4)                                             *
 * Author: Costantino Lombardi                                               *
 * File: problem_A.cc                                                        *
 ****************************************************************************/

/* ========================================================================= */
/* Included library */

#include <bits/stdc++.h>

#ifdef LOCAL
#include "algo/debug.h"
#else
#define debug(...) 42
#endif

using namespace std;

/* ========================================================================= */
/* Main function */

int main() {
    int t;
    cin >> t;

    for (int i = 0; i < t; i++) {
        int n;
        cin >> n;
        // Il numero di coppie (a, b) è semplicemente n - 1
        cout << n - 1 << "\n";
    }

    return 0;
}

/* ========================================================================= */