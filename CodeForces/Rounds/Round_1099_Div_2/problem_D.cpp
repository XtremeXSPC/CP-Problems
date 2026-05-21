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

void solve() {
  INT(n);
  STR(s);

  VecI64 a(n + 1), c(n + 1), b(n + 1);
  FOR(i, 1, n + 1) IN(a[i]);
  FOR(i, 1, n + 1) IN(c[i]);

  bool OK = true;
  FOR(i, 2, n + 1) {
    if (c[i] < c[i - 1]) OK = false;
  }

  VecI64 pref(n + 1);
  for (I64 l = 0; OK && l <= n; ) {
    I64 r = l;
    while (r + 1 <= n && s[r] == '1') r++;

    pref[l] = 0;
    FOR(v, l + 1, r + 1) pref[v] = pref[v - 1] + a[v];

    bool has_eq = false;
    I64 eq = 0;
    I64 ub = infinity<I64>;

    FOR(v, max<I64>(1, l), r + 1) {
      I64 cur_ub = c[v] - pref[v];
      ub = min(ub, cur_ub);

      bool forced = (v == 1 || c[v] > c[v - 1]);
      if (forced) {
        if (has_eq && eq != cur_ub) OK = false;
        has_eq = true;
        eq = cur_ub;
      }
    }

    I64 off = 0;
    if (l != 0)   off = has_eq ? eq : ub;
    if (has_eq && off != eq) OK = false;
    if (off > ub) OK = false;

    FOR(v, l, r + 1) b[v] = off + pref[v];
    l = r + 1;
  }

  constexpr I64 LIM = 1'000'000'000'000'000'000LL;
  VecI64 ans(n + 1);
  FOR(i, 1, n + 1) {
    ans[i] = b[i] - b[i - 1];
    if (ans[i] < -LIM || ans[i] > LIM) OK = false;
    if (s[i - 1] == '1' && ans[i] != a[i]) OK = false;
  }

  I64 sum = 0;
  I64 mx = -infinity<I64>;
  FOR(i, 1, n + 1) {
    sum += ans[i];
    mx   = (i == 1 ? sum : max(mx, sum));
    if (mx != c[i]) OK = false;
  }

  if (!OK) {
    OUT("No");
    return;
  }

  VecI64 out(n);
  FOR(i, n) out[i] = ans[i + 1];
  OUT("Yes");
  OUT(out);
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
