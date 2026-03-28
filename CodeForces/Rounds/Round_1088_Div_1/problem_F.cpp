#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_MATH
#define NEED_TIMER
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Global Constants */

constexpr I64 MODF = 676'767'677LL;
constexpr I32 MAXV = 2'000'000;

VecI64 fact_f(MAXV + 1), inv_fact_f(MAXV + 1);

auto comb_f(I32 n, I32 k) -> I64 {
  if (n < 0 || k < 0 || k > n) {
    return 0;
  }
  return fact_f[n] * inv_fact_f[k] % MODF * inv_fact_f[n - k] % MODF;
}

struct CombInit {
  CombInit() {
    fact_f[0] = 1;
    FOR(i, 1, MAXV + 1) {
      fact_f[i] = fact_f[i - 1] * i % MODF;
    }

    inv_fact_f[MAXV] = mod_pow(fact_f[MAXV], MODF - 2, MODF);
    FOR_R(i, MAXV) {
      inv_fact_f[i] = inv_fact_f[i + 1] * (i + 1) % MODF;
    }
    inv_fact_f[0] = 1;
  }
} comb_init;

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n, m);

  const I64 total = comb_f(n + m - 1, n);
  I64 ans = 0;

  Vec<Tuple<I32, I32, I32>> st;
  st.eb(1, n, 1);

  while (!st.empty()) {
    auto [l, r, depth] = st.back();
    st.pop_back();
    if (l > r) {
      continue;
    }

    const I32 mid = (l + r) / 2;
    const I32 left_bound  = l - 1;
    const I32 right_bound = n - r;

    I64 ways = total;
    if (right_bound > 0) {
      ways = (ways - comb_f(mid - 1 + right_bound + m - 1, m - 1)) % MODF;
    }
    if (left_bound > 0) {
      ways = (ways - comb_f(left_bound + (n - mid) + m - 1, m - 1)) % MODF;
    }
    if (left_bound > 0 && right_bound > 0) {
      ways = (ways + comb_f(left_bound + right_bound + m - 2, m - 1)) % MODF;
    }

    ans = (ans + depth * ((ways % MODF + MODF) % MODF)) % MODF;

    st.eb(l, mid - 1, depth + 1);
    st.eb(mid + 1, r, depth + 1);
  }

  OUT(ans);
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
