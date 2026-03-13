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
/* Main Solver Function */

constexpr I32 MAX_N = 3000;

VecI64 FACT(MAX_N + 1);
VecI64 IFACT(MAX_N + 1);

auto mod_inv(const I64 x) -> I64 {
  return mod_pow(x, MOD - 2, MOD);
}

auto build_factorials() -> void {
  FACT[0] = 1;
  FOR(i, 1, MAX_N + 1) FACT[i] = (FACT[i - 1] * i) % MOD;

  IFACT[MAX_N] = mod_inv(FACT[MAX_N]);
  for (I32 i = MAX_N; i > 0; --i) IFACT[i - 1] = (IFACT[i] * i) % MOD;
}

auto inv_ncr(const I32 n, const I32 r) -> I64 {
  return ((FACT[r] * FACT[n - r]) % MOD * IFACT[n]) % MOD;
}

auto solve() -> void {
  LL(n, x);

  I64 add_sum = 0;
  VecI64 mult_ops;
  mult_ops.reserve(n);

  FOR(_, 0, n) {
    STR(op);
    const char type = op[0];
    const I64 y = std::stoll(op.substr(1)) % MOD;

    if (type == '+') {
      add_sum = (add_sum + y) % MOD;
    } else if (type == '-') {
      add_sum = (add_sum - y + MOD) % MOD;
    } else if (type == 'x') {
      mult_ops.push_back(y);
    } else { // '/'
      mult_ops.push_back(mod_inv(y));
    }
  }

  I64 base_value = x % MOD;
  for (const I64 m : mult_ops) base_value = (base_value * m) % MOD;

  const I32 m = as<I32>(mult_ops.size());
  VecI64 dp(m + 1, 0);
  dp[0] = 1;

  for (const I64 v : mult_ops) {
    for (I32 k = m; k >= 1; --k) {
      dp[k] = (dp[k] + (dp[k - 1] * v) % MOD) % MOD;
    }
  }

  I64 expected_coeff = 0;
  FOR(k, 0, m + 1) {
    const I64 term = (dp[k] * inv_ncr(m, as<I32>(k))) % MOD;
    expected_coeff = (expected_coeff + term) % MOD;
  }
  expected_coeff = (expected_coeff * mod_inv(m + 1)) % MOD;

  const I64 answer = (base_value + (add_sum * expected_coeff) % MOD) % MOD;
  OUT(answer);
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  build_factorials();

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
