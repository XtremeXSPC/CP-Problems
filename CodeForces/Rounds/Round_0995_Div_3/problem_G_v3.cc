/******************************************************************************
 * Codeforces Round 995 Div. 3                                                *
 * Author: Costantino Lombardi                                                *
 * File: problem_G_v3.cc                                                      *
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

// ----------------------------------------------------------------------------
// Constants and global variables

static const long long INF  = (1LL << 60);
static const int       MAXN = 20;

int       n;
long long q;

static long long Gcount[MAXN][200001], Scount[MAXN][200001];

long long GmaxArr[MAXN], SmaxArr[MAXN];

long long D[MAXN][MAXN];

long long posSnake[MAXN];

long long bestScore;

bool used[MAXN];

// ----------------------------------------------------------------------------

long long lowerBound(long long currentMaxPos) {
  long long m = 0;
  for (int i = 0; i < n; i++) {
    if (!used[i])
      m = max(m, GmaxArr[i]);
  }
  return currentMaxPos + m;
}

void dfs(int placed, long long curMax) {
  if (placed == n) {
    if (curMax < bestScore)
      bestScore = curMax;
    return;
  }
  if (curMax >= bestScore)
    return;

  long long lb = lowerBound(curMax);
  if (lb >= bestScore)
    return;

  vector<int> candidates;
  candidates.reserve(n);
  for (int i = 0; i < n; i++) {
    if (!used[i])
      candidates.push_back(i);
  }
  sort(candidates.begin(), candidates.end(), [&](int a, int b) { return GmaxArr[a] > GmaxArr[b]; });
  for (int s : candidates) {
    long long candPos = 0;
    for (int x = 0; x < n; x++) {
      if (used[x]) {
        long long need = posSnake[x] + D[x][s];
        if (need > candPos)
          candPos = need;
      }
    }

    if (candPos < -SmaxArr[s])
      continue;

    if (candPos + GmaxArr[s] > 999999999)
      continue;

    long long newMax = max(curMax, candPos + GmaxArr[s]);
    if (newMax >= bestScore)
      continue;

    used[s]     = true;
    posSnake[s] = candPos;
    dfs(placed + 1, newMax);
    used[s] = false;
  }
}

/* ========================================================================= */
/* Main function */

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> q;
  for (int i = 0; i < n; i++) {
    Gcount[i][0] = 0;
    Scount[i][0] = 0;
  }
  for (int t = 1; t <= q; t++) {
    int  s;
    char op;
    cin >> s >> op;
    s--;
    for (int i = 0; i < n; i++) {
      Gcount[i][t] = Gcount[i][t - 1];
      Scount[i][t] = Scount[i][t - 1];
    }
    if (op == '+')
      Gcount[s][t]++;
    else
      Scount[s][t]++;
  }
  for (int i = 0; i < n; i++) {
    long long g = 0, ss = 0;
    for (int t = 0; t <= q; t++) {
      if (Gcount[i][t] > g)
        g = Gcount[i][t];
      if (Scount[i][t] > ss)
        ss = Scount[i][t];
    }
    GmaxArr[i] = g;
    SmaxArr[i] = ss;
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == j) {
        D[i][j] = 0;
      } else {
        long long mx = -LLONG_MAX;
        for (int t = 0; t <= q; t++) {
          long long val = Gcount[i][t] - Scount[j][t];
          if (val > mx)
            mx = val;
        }
        D[i][j] = mx + 1;
      }
    }
  }
  memset(used, false, sizeof(used));
  bestScore = INF;
  dfs(0, 0LL);
  cout << bestScore + 1 << "\n";
  return 0;
}

/* ========================================================================= */
