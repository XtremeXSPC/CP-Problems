/******************************************************************************
 * Codeforces Round 995 Div. 3                                                *
 * Author: Costantino Lombardi                                                *
 * File: problem_G.cc                                                         *
 *****************************************************************************/

/* ========================================================================= */
/* Included library */

#include <bits/stdc++.h>

#ifdef LOCAL
#include "algo/debug.h"
#else
#define debug(...) 42
#endif

using namespace std;

static const long long INF = 1e15;

int                       n;
long long                 bestScore;
vector<long long>         Gmax;
vector<vector<long long>> D;

vector<long long> posSnake;
vector<bool>      used;

void dfs(int k, long long currentMaxPos) {
  if (k == n) {
    bestScore = min(bestScore, currentMaxPos);
    return;
  }

  for (int i = 0; i < n; i++) {
    if (!used[i]) {
      long long candPos = 0;
      if (k > 0) {
        long long needed = 0;
        for (int x = 0; x < n; x++) {
          if (used[x]) {
            needed = max(needed, posSnake[x] + D[x][i]);
          }
        }
        candPos = needed;
      } else {
        candPos = 0;
      }

      long long newMaxPos = max(currentMaxPos, candPos + Gmax[i]);

      if (newMaxPos >= bestScore) {
        continue;
      }

      posSnake[i] = candPos;
      used[i]     = true;
      dfs(k + 1, newMaxPos);
      used[i] = false;
    }
  }
}

/* ========================================================================= */
/* Main function */

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  long long q;
  cin >> q;

  vector<vector<long long>> Gcount(n, vector<long long>(q + 1, 0));
  vector<vector<long long>> Scount(n, vector<long long>(q + 1, 0));

  for (int t = 1; t <= q; t++) {
    int  s;
    char op;
    cin >> s >> op;
    s--;
    for (int i = 0; i < n; i++) {
      Gcount[i][t] = Gcount[i][t - 1];
      Scount[i][t] = Scount[i][t - 1];
    }
    if (op == '+') {
      Gcount[s][t]++;
    } else {
      Scount[s][t]++;
    }
  }

  Gmax.resize(n, 0);
  D.assign(n, vector<long long>(n, 0));

  for (int i = 0; i < n; i++) {
    long long gmaxVal = 0;
    for (int t = 0; t <= q; t++) {
      gmaxVal = max(gmaxVal, Gcount[i][t]);
    }
    Gmax[i] = gmaxVal;
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == j) {
        D[i][j] = 0;
      } else {
        long long mval = LLONG_MIN;
        for (int t = 0; t <= q; t++) {
          mval = max(mval, Gcount[i][t] - Scount[j][t]);
        }
        D[i][j] = mval + 1;
      }
    }
  }

  posSnake.resize(n, 0);
  used.assign(n, false);
  bestScore = INF;

  dfs(0, 0LL);

  cout << bestScore + 1 << "\n";
  return 0;
}

/* ========================================================================= */