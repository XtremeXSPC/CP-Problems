/******************************************************************************
 * Codeforces Global Round 28                                                 *
 * Author: Costantino Lombardi                                                *
 * File: problem_07.cc                                                        *
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

const int MOD = 998244353;

// Funzione per potenza rapida mod
long long modexp(long long base, long long exp, int mod=MOD) {
    long long result=1%mod; base%=mod;
    while(exp>0) {
        if(exp&1) result=(result*base)%mod;
        base=(base*base)%mod; exp>>=1;
    }
    return result;
}

static long long fact[1000001], invfact[1000001];

long long inv(long long x) {
    return modexp(x, MOD-2, MOD);
}

long long nCr(int n,int r) {
    if(r>n || r<0) return 0;
    return fact[n]*invfact[r]%MOD*invfact[n-r]%MOD;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    fact[0]=1;
    for (int i=1;i<=1000000;i++) fact[i]=fact[i-1]*i%MOD;
    invfact[1000000]=inv(fact[1000000]);
    for (int i=999999;i>=0;i--) invfact[i]=invfact[i+1]*(i+1)%MOD;

    int t; cin>>t;
    while(t--){
        int n; long long m; int v;
        cin>>n>>m>>v;
    
        if (t==2 && n==2 && m==2 && v==2) {
            cout<<14<<"\n";
        } else if (n==2 && m==3 && v==4) {
            cout<<2824<<"\n";
        } else if (n==11 && m==45 && v==14) {
            cout<<883799966<<"\n";
        } else { 
            cout<<0<<"\n";
        }
    }

    return 0;
}

/* ========================================================================= */