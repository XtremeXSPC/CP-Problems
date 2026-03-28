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

constexpr I32 MAXN = 100'000;
constexpr I32 MAXB = 29;

VecI64 fact(MAXN + 1), inv_fact(MAXN + 1);

auto comb(I32 n, I32 k) -> I64 {
  if (k < 0 || k > n) {
    return 0;
  }
  return fact[n] * inv_fact[k] % MOD * inv_fact[n - k] % MOD;
}

struct CombInit {
  CombInit() {
    fact[0] = 1;
    FOR(i, 1, MAXN + 1) {
      fact[i] = fact[i - 1] * i % MOD;
    }

    inv_fact[MAXN] = mod_pow(fact[MAXN], MOD - 2, MOD);
    FOR_R(i, MAXN) {
      inv_fact[i] = inv_fact[i + 1] * (i + 1) % MOD;
    }
    inv_fact[0] = 1;
  }
} comb_init;

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n);
  VecI64 b(n + 1);
  FOR(i, 1, n + 1) {
    IN(b[i]);
  }

  VecI32 cnt(MAXB, -1);
  FOR_R(k, 1, n + 1) {
    I64 residual = b[k];
    FOR(bit, MAXB) {
      if (cnt[bit] == -1) {
        continue;
      }
      residual = (residual - ((1LL << bit) % MOD) * comb(cnt[bit], k)) % MOD;
    }
    residual = (residual % MOD + MOD) % MOD;

    FOR(bit, MAXB) {
      if (cnt[bit] == -1 && ((residual >> bit) & 1LL)) {
        cnt[bit] = k;
      }
    }
  }

  FOR(bit, MAXB) {
    if (cnt[bit] == -1) {
      cnt[bit] = 0;
    }
  }

  VecI64 a(n, 0);
  FOR(bit, MAXB) {
    FOR(i, cnt[bit]) {
      a[i] |= (1LL << bit);
    }
  }

  OUT(a);
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
