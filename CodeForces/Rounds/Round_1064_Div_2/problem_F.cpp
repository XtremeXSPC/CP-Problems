#include <bits/stdc++.h>

using namespace std;

void fast_io() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
}

struct MatchingSolver {
    int n;
    int max_val;
    vector<vector<int>> pos;

    MatchingSolver(int n, const vector<int>& a) : n(n) {
        max_val = 0;
        for (int x : a) max_val = max(max_val, x);
        // Resize to handle max_val + 2 safely
        pos.resize(max_val + 2);
        for (int i = 0; i < n; ++i) {
            pos[a[i]].push_back(i + 1); // 1-based indexing
        }
    }

    int solve(int start_val) {
        int matches = 0;
        vector<int> unmatched_prev_sinks;

        // Initialization Fix:
        if (start_val - 1 >= 1 && start_val - 1 < pos.size()) {
            unmatched_prev_sinks = pos[start_val - 1];
        }

        for (int v = start_val; v <= max_val; v += 2) {
            // Sources at current value v
            set<int> sources(pos[v].begin(), pos[v].end());

            // Phase 1: Match Sources (v) with "Expired" Sinks (v-1).
            if (!unmatched_prev_sinks.empty()) {
                sort(unmatched_prev_sinks.rbegin(), unmatched_prev_sinks.rend());

                for (int sink_idx : unmatched_prev_sinks) {
                    if (sources.empty()) break;

                    // We need source_idx < sink_idx.
                    auto it = sources.lower_bound(sink_idx);
                    if (it != sources.begin()) {
                        --it;
                        sources.erase(it);
                        matches++;
                    }
                }
            }

            // Phase 2: Match remaining Sources (v) with Next Sinks (v+1).
            vector<int>& next_sinks_candidates = pos[v + 1];
            vector<bool> is_matched(next_sinks_candidates.size(), false);

            // next_sinks_candidates is already sorted ascending by construction
            for (size_t i = 0; i < next_sinks_candidates.size(); ++i) {
                int sink_idx = next_sinks_candidates[i];
                if (sources.empty()) break;

                auto it = sources.lower_bound(sink_idx);
                if (it != sources.begin()) {
                    --it;
                    sources.erase(it);
                    matches++;
                    is_matched[i] = true;
                }
            }

            // Carry over unmatched sinks to the next step (v+2)
            unmatched_prev_sinks.clear();
            for (size_t i = 0; i < next_sinks_candidates.size(); ++i) {
                if (!is_matched[i]) {
                    unmatched_prev_sinks.push_back(next_sinks_candidates[i]);
                }
            }
        }
        return matches;
    }
};

void solve() {
    int n;
    if (!(cin >> n)) return;

    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    if (n == 0) {
        cout << 0 << "\n";
        return;
    }

    MatchingSolver solver(n, a);


    int m1 = solver.solve(1);
    int m2 = solver.solve(2);

    cout << n - (m1 + m2) << "\n";
}

int main() {
    fast_io();
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
