/******************************************************************************
 * Codeforces Round 995 Div. 3                                                *
 * Author: Costantino Lombardi                                                *
 * File: problem_G_v2.cc                                                      *
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

static const long long INF = 1LL << 60;

static const int MAXN = 20;

int       n;
long long q;

static long long Gcount[MAXN][200001];
static long long Scount[MAXN][200001];

long long GmaxArr[MAXN];
long long D[MAXN][MAXN];

// Per il DFS
bool      used[MAXN];
long long posSnake[MAXN];

long long bestScore;

int sortedSnakes[MAXN];

// ----------------------------------------------------------------------------

long long computeLowerBound(long long currentMaxPos, int placedCount) {
  long long gBig = 0;
  for (int i = 0; i < n; i++) {
    if (!used[i]) {
      gBig = max(gBig, GmaxArr[i]);
    }
  }

  long long lb = max(currentMaxPos, currentMaxPos + gBig);
  return lb;
}

void dfs(int k, long long currentMaxPos) {
  if (k == n) {
    if (currentMaxPos < bestScore) {
      bestScore = currentMaxPos;
    }
    return;
  }

  if (currentMaxPos >= bestScore)
    return;

  long long lb = computeLowerBound(currentMaxPos, k);
  if (lb >= bestScore)
    return;

  for (int idx = 0; idx < n; idx++) {
    int s = sortedSnakes[idx];
    if (!used[s]) {
      long long candPos = 0;
      for (int x = 0; x < n; x++) {
        if (used[x]) {
          long long need = posSnake[x] + D[x][s];
          if (need > candPos)
            candPos = need;
        }
      }

      long long newMaxPos     = currentMaxPos;
      long long possibleRight = candPos + GmaxArr[s];
      if (possibleRight > newMaxPos) {
        newMaxPos = possibleRight;
      }

      if (newMaxPos >= bestScore)
        continue;

      used[s]     = true;
      posSnake[s] = candPos;

      dfs(k + 1, newMaxPos);

      used[s] = false;
    }
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
    // Aggiorno l'evento
    if (op == '+') {
      Gcount[s][t]++;
    } else {
      Scount[s][t]++;
    }
  }

  for (int i = 0; i < n; i++) {
    long long gmaxVal = 0;
    for (int t = 0; t <= q; t++) {
      if (Gcount[i][t] > gmaxVal) {
        gmaxVal = Gcount[i][t];
      }
    }
    GmaxArr[i] = gmaxVal;
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == j) {
        D[i][j] = 0; // non useremo
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

  vector<pair<long long, int>> tmp;
  tmp.reserve(n);
  for (int i = 0; i < n; i++) {
    tmp.push_back({GmaxArr[i], i});
  }
  sort(tmp.begin(), tmp.end(), [](auto& a, auto& b) { return a.first > b.first; });
  for (int i = 0; i < n; i++) {
    sortedSnakes[i] = tmp[i].second;
  }

  bestScore = INF;
  memset(used, false, sizeof(used));

  dfs(0, 0LL);

  cout << (bestScore + 1) << "\n";

  return 0;
}

/* ========================================================================= */