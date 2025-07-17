#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n, l, r;
    cin >> n >> l >> r;
    l--; r--;
    vector<int> a(n);
    for (auto &x : a) cin >> x;
    
    vector<int> A, left_B, right_B;
    for (int i = l; i <= r; i++) A.push_back(a[i]);
    for (int i = 0; i < l; i++) left_B.push_back(a[i]);
    for (int i = r + 1; i < n; i++) right_B.push_back(a[i]);
    
    sort(A.rbegin(), A.rend());
    sort(left_B.begin(), left_B.end());
    sort(right_B.begin(), right_B.end());
    
    vector<ll> sum_left = {0};
    ll current_sum = 0;
    for (int k = 1; k <= min(left_B.size(), A.size()); k++) {
        if (left_B[k - 1] >= A[k - 1]) break;
        current_sum += (left_B[k - 1] - A[k - 1]);
        sum_left.push_back(current_sum);
    }
    
    vector<ll> prefix_right_B(1, 0);
    for (int x : right_B) prefix_right_B.push_back(prefix_right_B.back() + x);
    vector<ll> prefix_A(1, 0);
    for (int x : A) prefix_A.push_back(prefix_A.back() + x);
    
    vector<ll> sum_right(A.size() + 1, 0);
    for (int s = 0; s <= A.size(); s++) {
        int max_p = 0;
        int low = 0;
        int high = min((int)right_B.size(), (int)A.size() - s);
        while (low <= high) {
            int mid = (low + high) / 2;
            if (mid == 0) {
                max_p = mid;
                low = mid + 1;
                continue;
            }
            if (s + mid - 1 >= A.size()) {
                high = mid - 1;
                continue;
            }
            if (right_B[mid - 1] < A[s + mid - 1]) {
                max_p = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        sum_right[s] = prefix_right_B[max_p] - (prefix_A[s + max_p] - prefix_A[s]);
    }
    
    ll max_gain = 0;
    for (int k = 0; k < sum_left.size(); k++) {
        if (k > A.size()) continue;
        ll current = sum_left[k] + sum_right[k];
        if (current < max_gain) max_gain = current;
    }
    
    ll original_sum = 0;
    for (int i = l; i <= r; i++) original_sum += a[i];
    cout << original_sum + max_gain << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    while (t--) solve();
    return 0;
}
