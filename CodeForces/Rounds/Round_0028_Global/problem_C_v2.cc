//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Global Round 28
 * @author: Costantino Lombardi
 * @file: problem_C_v2.cc
 *
 * @status: NOT PASSED
 */
//===---------------------------------------------------------------------===//
/* Included library */

#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "algo/debug.h"
#else
#define debug(...) 42
#endif

//===---------------------------------------------------------------------===//
/* Main function */

string xor_strings(const string& A, const string& B) {
  string res(A.size(), '0');
  for (int i = 0; i < (int)A.size(); i++) {
    res[i] = (A[i] == B[i]) ? '0' : '1';
  }
  return res;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);

  int t;
  cin >> t;

  while (t--) {
    string s;
    cin >> s;
    int n = (int)s.size();

    vector<vector<pair<string, int>>> substrings_by_length(n + 1);
    for (int m = 1; m <= n; m++) {
      substrings_by_length[m].reserve(n - m + 1);
      for (int i = 0; i <= n - m; i++) {
        substrings_by_length[m].push_back({s.substr(i, m), i + 1});
      }
      sort(substrings_by_length[m].begin(), substrings_by_length[m].end());
    }

    string bestXOR(n, '0');
    int    best_l1 = 1, best_r1 = 1, best_l2 = 1, best_r2 = 1;

    for (int m = 1; m < n; m++) {
      string prefix;
      prefix.reserve(n - m);
      prefix.push_back('1');
      for (int pos = 2; pos <= n - m; pos++) {
        prefix.push_back(s[pos - 1]);
      }
      string tail_A = s.substr(n - m, m);

      string complement;
      complement.reserve(m);
      for (char c : tail_A)
        complement.push_back(c == '1' ? '0' : '1');

      auto& arr = substrings_by_length[m];
      auto  it  = lower_bound(arr.begin(), arr.end(), pair<string, int>(complement, 0));

      vector<int> idxs_to_check;
      if (it != arr.end()) {
        idxs_to_check.push_back((int)(it - arr.begin()));
        auto nxt = next(it);
        if (nxt != arr.end()) {
          idxs_to_check.push_back((int)(nxt - arr.begin()));
        }
      }

      if (it != arr.begin()) {
        idxs_to_check.push_back((int)(it - arr.begin()) - 1);
      }

      sort(idxs_to_check.begin(), idxs_to_check.end());
      idxs_to_check.erase(unique(idxs_to_check.begin(), idxs_to_check.end()), idxs_to_check.end());

      for (auto idx : idxs_to_check) {
        if (idx < 0 || idx >= (int)arr.size())
          continue;
        const auto&   cand   = arr[idx];
        const string& tail_B = cand.first;
        int           start2 = cand.second;

        string tail_xor = xor_strings(tail_A, tail_B);

        string currentXOR = prefix + tail_xor;

        if (currentXOR > bestXOR) {
          bestXOR = currentXOR;
          best_l1 = 1;
          best_r1 = n;
          best_l2 = start2;
          best_r2 = start2 + m - 1;
        }
      }
    }

    cout << best_l1 << " " << best_r1 << " " << best_l2 << " " << best_r2 << "\n";
  }
  return 0;
}

//===---------------------------------------------------------------------===//
