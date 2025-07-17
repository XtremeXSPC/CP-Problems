/******************************************************************************
 * Codeforces Global Round 28                                                 *
 * Author: Costantino Lombardi                                                *
 * File: problem_08.cc                                                        *
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

#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

long long modexp(long long base, long long exp) {
    long long result = 1 % MOD;
    long long cur = base % MOD;
    while (exp > 0) {
        if (exp & 1) result = (result * cur) % MOD;
        cur = (cur * cur) % MOD;
        exp >>= 1;
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; cin >> t;
    while (t--) {
        string s; cin >> s;
        int n = (int)s.size();
        int c = 0; // conta '1'
        for (auto ch : s) if (ch == '1') c++;

        if (c == 0) {
            // tutti zero
            cout << n % MOD << "\n";
            continue;
        }

        // trova l
        int first_one = 0;
        while (first_one < n && s[first_one] != '1') first_one++;
        int l = first_one;

        // trova r
        int last_one = n-1;
        while (last_one >= 0 && s[last_one] != '1') last_one--;
        int r = (n-1 - last_one);

        // blocchi di zeri tra '1'
        vector<int> zero_blocks;
        int prev_one = first_one;
        for (int i = first_one + 1; i <= last_one; i++) {
            if (s[i] == '1') {
                int zeros_count = i - prev_one - 1;
                if (zeros_count > 0) zero_blocks.push_back(zeros_count);
                prev_one = i;
            }
        }

        long long ans = 1;
        ans = ans * (l+1) % MOD;
        ans = ans * (r+1) % MOD;
        for (auto z: zero_blocks) ans = ans * (z+1) % MOD;
        ans = ans * modexp(2, c-1) % MOD;

        cout << ans << "\n";
    }

    return 0;
}

/* ========================================================================= */