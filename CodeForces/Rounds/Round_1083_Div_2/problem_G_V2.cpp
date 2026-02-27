#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_FAST_IO

#include "templates/Base.hpp"
#include "modules/bitwise/XorBoundIntervals.hpp"
#include "modules/data_structures/SparseDeltaSweep.hpp"
#include "modules/number_theory/Mobius.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

constexpr I32 MAX_L = 1 << 19;

[[nodiscard]] auto mobius_ctx() -> const Mobius& {
  static const Mobius mobius(MAX_L - 1);
  return mobius;
}

void solve() {
  I64 n;
  I32 m;
  IN(n, m);

  I32 limit_l = 1;
  while (limit_l <= m) limit_l <<= 1;
  const I32 max_xor = limit_l - 1;

  const Mobius& mobius = mobius_ctx();
  const VecI32 weight = mobius.build_divisibility_weights(n, max_xor);
  const VecI32 divisors = mobius.divisors_up_to_limit(n, max_xor);

  const XorBoundIntervals xor_intervals(m, limit_l);

  I128 total = 0;
  for (const I32 d : divisors) {
    total += as<I128>(2) * xor_intervals.overlap_count(d);
  }

  SparseDeltaSweep sweep(m + 1);

  for (I32 x = 1; x <= max_xor; ++x) {
    const I32 hx = weight[x];
    if (hx == 0) continue;

    sweep.begin_round();

    for (I32 v = x; v <= max_xor; v += x) {
      const I32 from = xor_intervals.interval_begin(v);
      const I32 to = xor_intervals.interval_end(v);
      for (I32 idx = from; idx < to; ++idx) {
        const auto [l, r] = xor_intervals.interval_at(idx);
        sweep.add(l, +1);
        sweep.add(r + 1, -1);
      }
    }

    total += as<I128>(hx) * sweep.squared_coverage_sum(m);
  }

  my_assert(total >= as<I128>(Limits<I64>::min()));
  my_assert(total <= as<I128>(Limits<I64>::max()));
  OUT(as<I64>(total));
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
