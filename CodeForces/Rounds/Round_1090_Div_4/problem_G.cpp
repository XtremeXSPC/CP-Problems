#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_TIMER
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

constexpr I32 MOD_V = 676767677;

void solve() {
  I32 N, M;
  IN(N, M);

  VecI32 B(N);
  VecI32 A(M, 0);

  FOR(i, N) {
    IN(B[i]);
    A[B[i]]++;
  }

  VecI32 CS(M, 0);
  CS[0] = A[0];
  FOR(t, 1, M) {
    CS[t] = CS[t - 1] + A[t];
  }

  I64 PA = 1;

  FOR(i, N) {
    I32 CT = B[i];
    if (CT == 0)
      continue;

    I32 MinNT = INT_MAX;
    if (i > 0)
      MinNT = min(MinNT, B[i - 1]);
    if (i < N - 1)
      MinNT = min(MinNT, B[i + 1]);

    I64 CC = 0;

    if (MinNT >= CT)
      CC = 0;
    else if (MinNT == CT - 1)
      CC = CS[CT - 1];
    else
      CC = A[CT - 1];

    PA = (PA * CC) % MOD_V;
  }

  OUT(PA);
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
