/******************************************************************************
 * Codeforces Round 995 Div. 3                                                *
 * Author: Costantino Lombardi                                                *
 * File: problem_F.cc                                                         *
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

struct Interval {
  long long l, r;
  Interval(long long l_, long long r_) : l(l_), r(r_) {}
};

long long length(const Interval& iv) {
  return (iv.r >= iv.l) ? (iv.r - iv.l + 1) : 0LL;
}

vector<Interval> mergeIntervals(vector<Interval>& ivs) {
  sort(ivs.begin(), ivs.end(), [](auto& a, auto& b) { return a.l < b.l; });

  vector<Interval> res;
  for (auto& iv : ivs) {
    if (iv.l > iv.r) {
      continue;
    }
    if (res.empty() || res.back().r < iv.l - 1) {
      res.push_back(iv);
    } else {
      res.back().r = max(res.back().r, iv.r);
    }
  }
  return res;
}

vector<Interval> transformTop(const Interval& iv, long long a, long long n) {
  long long L = iv.l, R = iv.r;
  if (L > R)
    return {}; // vuoto

  if (R < a) {
    long long newL = max(1LL, L + 1);
    long long newR = min(n, R + 1);
    if (newL <= newR)
      return {Interval(newL, newR)};
    else
      return {};
  }

  if (L > a) {
    long long newL = max(1LL, L);
    long long newR = min(n, R);
    if (newL <= newR)
      return {Interval(newL, newR)};
    else
      return {};
  }

  vector<Interval> res;

  if (L <= a - 1) {
    long long xL = L, xR = a - 1;
    long long tL = max(1LL, xL + 1);
    long long tR = min(n, xR + 1);
    if (tL <= tR)
      res.push_back(Interval(tL, tR));
  }

  if (a >= L && a <= R) {
    res.push_back(Interval(1, 1));
  }

  if (a + 1 <= R) {
    long long xL = a + 1, xR = R;
    long long tL = max(1LL, xL);
    long long tR = min(n, xR);
    if (tL <= tR)
      res.push_back(Interval(tL, tR));
  }

  return res;
}

vector<Interval> transformBottom(const Interval& iv, long long a, long long n) {
  long long L = iv.l, R = iv.r;
  if (L > R)
    return {};

  if (R < a) {
    long long newL = max(1LL, L);
    long long newR = min(n, R);
    if (newL <= newR)
      return {Interval(newL, newR)};
    else
      return {};
  }

  if (L > a) {
    long long newL = max(1LL, L - 1);
    long long newR = min(n, R - 1);
    if (newL <= newR)
      return {Interval(newL, newR)};
    else
      return {};
  }

  vector<Interval> res;

  if (L <= a - 1) {
    long long xL = L, xR = a - 1;
    long long bL = max(1LL, xL);
    long long bR = min(n, xR);
    if (bL <= bR)
      res.push_back(Interval(bL, bR));
  }

  if (a >= L && a <= R) {
    res.push_back(Interval(n, n));
  }

  if (a + 1 <= R) {
    long long xL = a + 1, xR = R;
    long long bL = max(1LL, xL - 1);
    long long bR = min(n, xR - 1);
    if (bL <= bR)
      res.push_back(Interval(bL, bR));
  }

  return res;
}

/* ========================================================================= */
/* Main function */

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  while (t--) {
    long long n, m;
    int       q;
    cin >> n >> m >> q;
    vector<long long> A(q);
    for (int i = 0; i < q; i++) {
      cin >> A[i];
    }

    vector<Interval> U;
    U.push_back(Interval(m, m));

    for (int i = 0; i < q; i++) {
      long long a = A[i];

      vector<Interval> vT;
      for (auto& iv : U) {
        auto tmp = transformTop(iv, a, n);
        vT.insert(vT.end(), tmp.begin(), tmp.end());
      }
      vector<Interval> vB;
      for (auto& iv : U) {
        auto tmp = transformBottom(iv, a, n);
        vB.insert(vB.end(), tmp.begin(), tmp.end());
      }

      vector<Interval> vNew;
      vNew.insert(vNew.end(), vT.begin(), vT.end());
      vNew.insert(vNew.end(), vB.begin(), vB.end());

      vNew = mergeIntervals(vNew);

      long long countPos = 0;
      for (auto& iv : vNew) {
        countPos += length(iv);
      }
      cout << countPos << " ";

      U = std::move(vNew);
    }
    cout << "\n";
  }

  return 0;
}

/* ========================================================================= */
