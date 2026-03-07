#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

struct ParentState {
  I8 prev_p = -1;
  I8 prev_q = -1;
  I8 p_bit  = 0;
  I8 q_bit  = 0;
};

auto solve() -> void {
  LL(x, y);

  constexpr I32 MAX_BIT = 30;
  constexpr I32 EQ      = 0;
  constexpr I32 LESS    = 1;
  constexpr I32 GREATER = 2;
  constexpr I64 INF     = INF64;

  Array<Array<Array<I64, 3>, 3>, MAX_BIT + 2> dp;
  Array<Array<Array<ParentState, 3>, 3>, MAX_BIT + 1> parent{};

  for (auto& layer : dp) {
    for (auto& row : layer) row.fill(INF);
  }
  dp[MAX_BIT + 1][EQ][EQ] = 0;

  constexpr Array<PairI32, 3> choices = {
      PairI32{0, 0},
      PairI32{0, 1},
      PairI32{1, 0},
  };

  auto next_cmp = [&](const I32 cmp, const I32 chosen, const I32 target) -> I32 {
    if (cmp != EQ) return cmp;
    if (chosen < target) return LESS;
    if (chosen > target) return GREATER;
    return EQ;
  };

  auto bit_cost = [&](const I32 cmp, const I32 chosen, const I32 target, const I64 bit_value) -> I64 {
    if (cmp == LESS) return 1LL * (target - chosen) * bit_value;
    if (cmp == GREATER) return 1LL * (chosen - target) * bit_value;
    return 0;
  };

  FOR_R(bit, MAX_BIT + 1) {
    const I32 x_bit = as<I32>((x >> bit) & 1LL);
    const I32 y_bit = as<I32>((y >> bit) & 1LL);
    const I64 bit_value = 1LL << bit;

    FOR(cmp_p, 3) {
      FOR(cmp_q, 3) {
        const I64 cur = dp[bit + 1][cmp_p][cmp_q];
        if (cur == INF) continue;

        for (const auto [p_bit, q_bit] : choices) {
          const I32 next_p = next_cmp(as<I32>(cmp_p), p_bit, x_bit);
          const I32 next_q = next_cmp(as<I32>(cmp_q), q_bit, y_bit);
          const I64 cand = cur + bit_cost(next_p, p_bit, x_bit, bit_value)
                               + bit_cost(next_q, q_bit, y_bit, bit_value);

          if (cand >= dp[bit][next_p][next_q]) continue;

          dp[bit][next_p][next_q] = cand;
          parent[bit][next_p][next_q] = {
              .prev_p = as<I8>(cmp_p),
              .prev_q = as<I8>(cmp_q),
              .p_bit  = as<I8>(p_bit),
              .q_bit  = as<I8>(q_bit),
          };
        }
      }
    }
  }

  I64 best  = INF;
  I32 end_p = EQ;
  I32 end_q = EQ;
  FOR(cmp_p, 3) {
    FOR(cmp_q, 3) {
      if (dp[0][cmp_p][cmp_q] >= best) continue;
      best  = dp[0][cmp_p][cmp_q];
      end_p = as<I32>(cmp_p);
      end_q = as<I32>(cmp_q);
    }
  }

  I64 p = 0;
  I64 q = 0;
  FOR(bit, MAX_BIT + 1) {
    const ParentState choice = parent[bit][end_p][end_q];
    if (choice.p_bit) p |= 1LL << bit;
    if (choice.q_bit) q |= 1LL << bit;
    end_p = choice.prev_p;
    end_q = choice.prev_q;
  }

  OUT(p, q);
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
