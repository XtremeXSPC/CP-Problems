/******************************************************************************
 * Codeforces Round 993 (Div. 4)                                              *
 * Author: Costantino Lombardi                                                *
 * File: problem_G1.cc                                                        *
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
        int n; cin >> n;
        vector<int> r(n+1);
        vector<int> in_degree(n+1,0);

        for (int i=1; i<=n; i++){
            cin >> r[i];
        }

        // Calcoliamo il grado entrante
        for (int i=1; i<=n; i++){
            in_degree[r[i]]++;
        }

        // Coda dei nodi che spariscono presto (grado entrante zero)
        queue<int>q;
        vector<int> vanish_time(n+1,0);
        // I nodi con grado entrante zero spariscono a partire dall'anno 2
        for (int i=1; i<=n; i++){
            if (in_degree[i]==0) {
                vanish_time[i]=2;
                q.push(i);
            }
        }

        // Propagazione dell'inattivazione
        while(!q.empty()){
            int u=q.front(); q.pop();
            int v=r[u];
            in_degree[v]--;
            if (in_degree[v]==0 && vanish_time[v]==0) {
                // v sparirà l'anno successivo a u
                vanish_time[v]=vanish_time[u]+1;
                q.push(v);
            }
        }

        int mx=0;
        for (int i=1; i<=n; i++){
            mx = max(mx, vanish_time[i]);
        }

        // Se mx=0, nessun nodo è sparito, tutti su cicli => anno di stabilità=2
        // Altrimenti anno di stabilità = mx+1
        int ans = (mx==0 ? 2 : mx+1);
        cout << ans << "\n";
    }

    return 0;
}

/* ========================================================================= */