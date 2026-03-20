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

constexpr I32 MAX_N = 300000;

void solve() {
  INT(n);
  STR(s);

  static VecI64 pow2;
  if (pow2.empty()) {
    pow2.resize(MAX_N + 1, 1);
    FOR(i, 1, MAX_N + 1) {
      pow2[i] = (pow2[i - 1] * 2) % MOD2;
    }
  }

  I64 ans  = 0;
  I64 d0   = 1;
  I64 d1   = 0;
  I32 pref = 0;

  FOR(i, n) {
    I64 nd0 = d0;
    I64 nd1 = d1;

    if (s[i] == '(') {
      ++pref;
      ans = (ans + pow2[i]) % MOD2;
      nd1 = (d1 * 2 + d0) % MOD2;
    } else {
      --pref;
      ans = (ans + d0 + d1) % MOD2;
      nd0 = (d0 * 2 + d1) % MOD2;
    }

    if (pref <= 1) {
      nd1 = 0;
    }

    d0 = nd0;
    d1 = nd1;
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
