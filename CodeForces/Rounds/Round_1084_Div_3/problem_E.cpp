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

constexpr I32 MAX_A = 1000000;

auto build_spf() -> VecI32 {
  VecI32 spf(MAX_A + 1);
  FOR(i, 0, MAX_A + 1) spf[i] = i;
  for (I32 i = 2; i * i <= MAX_A; ++i) {
    if (spf[i] != i) continue;
    for (I32 j = i * i; j <= MAX_A; j += i) {
      if (spf[j] == j) spf[j] = i;
    }
  }
  return spf;
}

const VecI32 SPF = build_spf();

auto solve() -> void {
  INT(n);
  VecI32 a(n);
  IN(a);

  if (std::is_sorted(a.begin(), a.end())) {
    OUT("Bob");
    return;
  }

  bool has_mixed_prime_factors = false;
  bool bases_non_decreasing    = true;
  I32 prev_base                = 0;

  FOR(i, 0, n) {
    const I32 x = a[i];

    I32 base = 1;
    if (x > 1) {
      base = SPF[x];
      I32 cur = x;
      while (cur % base == 0) cur /= base;
      if (cur != 1) has_mixed_prime_factors = true;
    }

    if (i > 0 && prev_base > base) bases_non_decreasing = false;
    prev_base = base;
  }

  OUT(has_mixed_prime_factors || !bases_non_decreasing ? "Alice" : "Bob");
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
