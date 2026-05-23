#ifndef CP_TEMPLATE_PROFILE_STRICT
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_MACROS
#define NEED_TIMER
#define NEED_CORE
#define NEED_IO
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n, k);
  VecI64 A(n);
  FOR(i, n) IN(A[i]);

  const I64 total = accumulate(A.begin(), A.end(), 0LL);
  const I64 x = *max_element(A.begin(), A.end());

  if (k == 1) {
    OUT(total);
    return;
  }
  if (k == n) {
    OUT(x);
    return;
  }

  bool removed = false;
  VecI64 rest;
  rest.reserve(n - 1);
  for (const I64 v : A) {
    if (!removed && v == x) {
      removed = true;
      continue;
    }
    rest.push_back(v);
  }

  const I32 m    = n - 1;
  const I32 full = 1 << m;
  VecI64 sum(full, 0);
  FOR(mask, 1, full) {
    const I32 bit = __builtin_ctz(as<U32>(mask));
    sum[mask] = sum[mask ^ (1 << bit)] + rest[bit];
  }

  VecI32 cnt(full);
  VecI64 used(full);

  auto OK = [&](I64 need) -> bool {
    constexpr I64 INF_USED = 4'000'000'000'000'000'000LL;

    fill(cnt.begin(), cnt.end(), -1);
    fill(used.begin(), used.end(), INF_USED);
    cnt[0]  = 0;
    used[0] = 0;

    FOR(mask, full) {
      if (cnt[mask] < 0) continue;
      const I32 rem = (full - 1) ^ mask;

      for (I32 bits = rem; bits != 0; bits &= bits - 1) {
        const I32 bit = bits & -bits;
        const I32 nxt = mask | bit;
        I32 nc = cnt[mask];
        I64 nu = used[mask];

        if (sum[nxt] - used[mask] >= need) {
          ++nc;
          nu = sum[nxt];
        }

        if (nc > cnt[nxt] || (nc == cnt[nxt] && nu < used[nxt])) {
          cnt[nxt]  = nc;
          used[nxt] = nu;
        }
      }
    }

    return cnt[full - 1] >= k;
  };

  I64 lo   = 0;
  I64 hi   = sum[full - 1] / k;
  I64 best = 0;

  while (lo <= hi) {
    const I64 mid = lo + ((hi - lo) >> 1);
    if (OK(mid)) {
      best = mid;
      lo = mid + 1;
    } else {
      hi = mid - 1;
    }
  }

  OUT(best + x);
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
