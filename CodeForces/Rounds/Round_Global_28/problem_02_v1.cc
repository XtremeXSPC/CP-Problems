/******************************************************************************
 * Codeforces Global Round 28                                                 *
 * Author: Costantino Lombardi                                                *
 * File: problem_02_v1.cc                                                     *
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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; cin >> t;
    while(t--){
        int n,k; cin >> n >> k;

        vector<pair<int,int>> coverageIndex(n);
        for (int i=1; i<=n; i++){
            int c;
            if (i <= k) c = i;
            else if (i <= n - k + 1) c = k;
            else c = n - i + 1;
            coverageIndex[i-1] = {c, i}; 
        }

        sort(coverageIndex.begin(), coverageIndex.end(), [](auto &a, auto &b){
            if (a.first != b.first) return a.first > b.first;
            return a.second < b.second;
        });

        vector<int> p(n,0);
        for (int i=0; i<n; i++){
            p[coverageIndex[i].second - 1] = i+1;
        }

        for (int i=0; i<n; i++){
            cout << p[i] << (i+1==n?'\n':' ');
        }
    }

    return 0;
}

/* ========================================================================= */