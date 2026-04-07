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

constexpr I32 MAX = 1'000'000;

VecI64 cat(MAX + 1), p2(MAX + 1), inv(MAX + 2);

auto sub_mod(I64 a, I64 b) -> I64 {
  a -= b;
  if (a < 0) a += MOD;
  return a;
}

void init() {
  inv[1] = 1;
  FOR(i, 2, MAX + 2) inv[i] = MOD - (MOD / i) * inv[MOD % i] % MOD;

  p2[0] = 1;
  FOR(i, 1, MAX + 1) p2[i] = p2[i - 1] * 2 % MOD;

  cat[0] = 1;
  FOR(i, MAX) {
    cat[i + 1] = cat[i] * 2 % MOD * (2 * i + 1) % MOD * inv[i + 2] % MOD;
  }
}

void build(VecI64& s, I32 n, I32 k) {
  s.assign(n + 1, 0);
  if (k < 0) return;

  const I32 lim = std::min(n, k);
  FOR(i, lim + 1) s[i] = p2[i];
  if (k >= n) return;

  I64 c = 1;
  FOR(i, k, n) {
    s[i + 1] = sub_mod(2 * s[i] % MOD, c);
    c = c * (i + 1) % MOD * inv[i + 1 - k] % MOD;
  }
}

void solve() {
  INT(n, k);

  VecI64 a, b;
  build(a, n - 1, k);
  build(b, n - 1, k - 2);

  I64 ans = 0;
  FOR(i, n) {
    const I64 j = n - 1 - i;
    const I64 ways = sub_mod(a[i] * a[j] % MOD, b[i] * b[j] % MOD);
    ans = (ans + cat[i] * cat[j] % MOD * ways) % MOD;
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

  init();

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
