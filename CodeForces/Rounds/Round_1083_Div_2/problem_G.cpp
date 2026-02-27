#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_FAST_IO

#include "templates/Base.hpp"
#include "modules/number_theory/FWHT.hpp"
#include "modules/number_theory/Mobius.hpp"
#include "modules/bitwise/XorBoundIntervals.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

constexpr I32 MAX_L = 1 << 19;
constexpr I32 HEAVY_THRESHOLD = 420;

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

  VecI64 ind(as<Size>(limit_l), 0);
  for (I32 x = 0; x <= m; ++x) ind[x] = 1;

  VecI64 ind_fwht = ind;
  FWHT::xor_transform(ind_fwht, false);

  const Mobius& mobius = mobius_ctx();
  VecI32 weight = mobius.build_divisibility_weights(n, max_xor);

  I64 sum_weights = 0;
  for (I32 d = 1; d <= max_xor; ++d) sum_weights += weight[d];

  I64 total = 0;
  VecI64 mult(as<Size>(limit_l));

  // Precompute interval decomposition:
  // J_x = { j in [0, m] : (j xor x) <= m } for all x in [0, limit_l - 1].
  const XorBoundIntervals xor_intervals(m, limit_l);

  for (I32 d = 1; d <= max_xor; ++d) {
    const I32 w = weight[d];
    if (w == 0) continue;

    I64 sum_sq = 0;

    if (d <= HEAVY_THRESHOLD) {
      std::fill(mult.begin(), mult.end(), 0);
      for (I32 x = 0; x < limit_l; x += d) mult[x] = 1;

      FWHT::xor_transform(mult, false);
      for (I32 i = 0; i < limit_l; ++i) mult[i] *= ind_fwht[i];
      FWHT::xor_transform(mult, true);

      for (I32 j = 0; j <= m; ++j) {
        const I64 v = mult[j];
        sum_sq += v * v;
      }
    } else if (d > m) {
      // Here multiples of d inside [0, limit_l - 1] are exactly {0, d}.
      const I64 r = xor_intervals.overlap_count(d);
      sum_sq = as<I64>(m + 1) + 3LL * r;
    } else {
      Vec<PairI32> events;
      events.reserve((limit_l / d + 1) * 20);
      for (I32 x = 0; x < limit_l; x += d) {
        const I32 from = xor_intervals.interval_begin(x);
        const I32 to = xor_intervals.interval_end(x);
        for (I32 idx = from; idx < to; ++idx) {
          const auto [l, r] = xor_intervals.interval_at(idx);
          events.push_back({l, +1});
          if (r < m) events.push_back({r + 1, -1});
        }
      }

      std::sort(events.begin(), events.end());

      I64 cur_cover = 0;
      I32 prev_pos = 0;
      I32 i = 0;
      while (i < sz(events)) {
        const I32 pos = events[i].first;
        if (pos > prev_pos) {
          const I64 len = as<I64>(pos - prev_pos);
          sum_sq += len * cur_cover * cur_cover;
          prev_pos = pos;
        }

        I64 delta = 0;
        while (i < sz(events) && events[i].first == pos) {
          delta += events[i].second;
          ++i;
        }
        cur_cover += delta;
      }

      if (prev_pos <= m) {
        const I64 len = as<I64>(m - prev_pos + 1);
        sum_sq += len * cur_cover * cur_cover;
      }
    }

    total += as<I64>(w) * sum_sq;
  }

  // Correction for (A, B) = (0, 0): i = j = k.
  // For n >= 1 this case is invalid (gcd(0, 0) = 0 does not divide n),
  // while the finite divisor expansion contributes (m + 1) * sum_d weight[d].
  total -= as<I64>(m + 1) * sum_weights;

  OUT(total);
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
