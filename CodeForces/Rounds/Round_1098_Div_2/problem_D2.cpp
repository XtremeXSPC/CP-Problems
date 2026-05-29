#ifndef CP_TEMPLATE_PROFILE_STRICT
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_MACROS
#define NEED_TIMER
#define CP_IO_PROFILE_FAST_MINIMAL

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

const int MAXN = 2'000'005;
const int INF  = 1'000'000'000;

I32 CMin[MAXN], CMax[MAXN];
I32 RMin[MAXN], RMax[MAXN];
I32 PrsY[MAXN];

void solve() {
  INT(n);

  FOR(i, 1, n + 2) {
    CMin[i] = INF;
    CMax[i] = -1;
    PrsY[i] = 0;
  }

  FOR(i, 0, n) {
    INT(x, y);
    if (y < CMin[x]) CMin[x] = y;
    if (y > CMax[x]) CMax[x] = y;
    PrsY[y] = 1;
  }

  FOR(i, 1, n + 1) {
    PrsY[i] += PrsY[i - 1];
  }

  RMin[n + 1] = INF;
  RMax[n + 1] = -1;

  for (I32 i = n; i >= 1; --i) {
    RMin[i] = min(RMin[i + 1], CMin[i]);
    RMax[i] = max(RMax[i + 1], CMax[i]);
  }

  I64 ans  = 0;
  I32 LMin = INF, LMax = -1;

  FOR(x, 1, n + 1) {
    if (CMax[x] != -1) {
      if (CMin[x] < LMin) LMin = CMin[x];
      if (CMax[x] > LMax) LMax = CMax[x];
    } else {
      continue;
    }

    if (RMin[x + 1] == INF) continue;

    I32 low  = max(LMin, RMin[x + 1]);
    I32 high = min(LMax, RMax[x + 1]);

    if (low < high) {
      ans += PrsY[high - 1] - PrsY[low - 1];
    }
  }

  OUT(ans);
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Stopwatch timer;
#endif

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
