/******************************************************************************
 * Codeforces Global Round 28                                                 *
 * Author: Costantino Lombardi                                                *
 * File: problem_06_v2.cc                                                     *
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

int steps_needed(unsigned long long A, unsigned long long B) {
    int cnt = 0;
    while (A > 1) {
        A = (A + B - 1ULL) / B;
        cnt++;
    }
    return cnt;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; cin >> t;
    while(t--) {
        int n; cin >> n;
        vector<unsigned long long> a(n), b(n);
        for (int i = 0; i < n; i++) cin >> a[i];
        for (int i = 0; i < n; i++) cin >> b[i];

        struct Block {
            unsigned long long min_b;
            unsigned long long max_a;
            int steps;
        };

        vector<Block> stk;

        for (int i = 0; i < n; i++) {
            unsigned long long cur_min_b = b[i];
            unsigned long long cur_max_a = a[i];
            int cur_steps = steps_needed(cur_max_a, cur_min_b);

            while (!stk.empty()) {
                unsigned long long merged_min_b = min(stk.back().min_b, cur_min_b);
                unsigned long long merged_max_a = max(stk.back().max_a, cur_max_a);
                int merged_steps = steps_needed(merged_max_a, merged_min_b);

                if (merged_steps <= max(stk.back().steps, cur_steps)) {
                    cur_min_b = merged_min_b;
                    cur_max_a = merged_max_a;
                    cur_steps = merged_steps;
                    stk.pop_back();
                } else {
                    break;
                }
            }

            stk.push_back({cur_min_b, cur_max_a, cur_steps});
        }

        int ans = 0;
        for (auto &bl : stk) {
            ans = max(ans, bl.steps);
        }

        cout << ans << "\n";
    }

    return 0;
}

/* ========================================================================= */