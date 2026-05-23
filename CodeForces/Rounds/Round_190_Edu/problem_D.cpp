#ifndef CP_TEMPLATE_PROFILE_STRICT
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_MACROS
#define NEED_TIMER
#define NEED_CONTAINERS
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

struct Event {
  I32 idx;
  I32 type;
};

void solve() {
  INT(n);
  VecI32 A(n);
  FOR(i, n) {
    IN(A[i]);
  }
  VecI32 B(n);
  FOR(i, n) {
    IN(B[i]);
  }

  Vec2D<Event> E(n + 2);

  for (I32 i = 0; i < n; ++i) {
    if (A[i] == B[i]) {
      E[A[i]].push_back({i + 1, 1});
    } else {
      E[A[i]].push_back({i + 1, 0});
      E[B[i]].push_back({i + 1, 0});
    }
  }

  VecI32 DP(n + 2, 0);

  FORD(c, n, 1) {
    I32 ptr = 0;
    for (auto ev : E[c]) {
      if (ev.type == 1) {
        I32 g = ev.idx;
        while (ptr < sz(E[c + 1]) && E[c + 1][ptr].idx <= g) {
          ptr++;
        }
        I32 death = n + 1;
        if (ptr < sz(E[c + 1])) {
          if (E[c + 1][ptr].type == 0) {
            death = E[c + 1][ptr].idx;
          } else {
            death = DP[E[c + 1][ptr].idx];
          }
        }
        DP[g] = death;
      }
    }
  }

  I64 total_valid = 0;
  I32 ptr = 0;
  FOR(L, 1, n + 1) {
    while (ptr < sz(E[1]) && E[1][ptr].idx < L) {
      ptr++;
    }
    I32 death = n + 1;
    if (ptr < sz(E[1])) {
      if (E[1][ptr].type == 0) {
        death = E[1][ptr].idx;
      } else {
        death = DP[E[1][ptr].idx];
      }
    }
    total_valid += (death - L);
  }

  OUT(total_valid);
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
