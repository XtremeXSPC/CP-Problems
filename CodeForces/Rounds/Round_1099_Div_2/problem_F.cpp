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

constexpr I32 MAXN = 200'000;

Vec<bool> is_sq(MAXN + 1);
Vec<bool> sum_sq(MAXN + 1);
Vec2D<I32> divs(MAXN + 1);
VecI32 sq;

void prepare() {
  for (I64 x = 1; x * x <= MAXN; x++) {
    is_sq[x * x] = true;
    sq.push_back(x * x);
  }

  FOR(i, sz(sq)) {
    FOR(j, i, sz(sq)) {
      I64 sum = sq[i] + sq[j];
      if (sum > MAXN) break;
      sum_sq[sum] = true;
    }
  }

  FOR(u, 1, MAXN + 1) {
    for (I64 d = u + u; d <= MAXN; d += u) {
      if (I64(u) * u < d) divs[d].push_back(u);
    }
  }
}

bool dist2(I32 n, I32 a, I32 b) {
  if (a > b) swap(a, b);
  I32 diff = b - a;

  if (sum_sq[diff]) return true;

  for (I32 u : divs[diff]) {
    I32 v = diff / u;
    if ((v - u) % 2 != 0) continue;

    I64 k = (v - u) / 2;
    if (k == 0) continue;

    I64 step = k * k;
    if (step <= a - 1 || step <= n - b) return true;
  }

  return false;
}

bool dist3_small(I32 n, I32 a, I32 b) {
  for (I32 step : sq) {
    if (step > n) break;

    if (a + step <= n && a + step != b && dist2(n, a + step, b)) {
      return true;
    }
    if (a - step >= 1 && a - step != b && dist2(n, a - step, b)) {
      return true;
    }
  }

  return false;
}

void solve() {
  INT(n, q);
  FOR(q) {
    INT(a, b);
    I32 diff = b - a;

    if (is_sq[diff]) {
      OUT(1);
    } else if (dist2(n, a, b)) {
      OUT(2);
    } else {
      OUT(dist3_small(n, a, b) ? 3 : 4);
    }
  }
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  prepare();

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
