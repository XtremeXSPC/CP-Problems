#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"


//===----------------------------------------------------------------------===//
/* Main Solver Function */

using namespace std;

namespace {

auto add_mod(I32& x, I64 y) -> void {
  x = as<I32>((x + y) % MOD);
}

auto build_step_dp(I64 base_size, const Vec<I64>& inner_sizes, const Vec<I64>& inner_gains,
                   const Vec<I64>& outer_sizes, const Vec<I64>& outer_gains)
    -> Pair<Vec<Vec<I32>>, Vec<Vec<I32>>> {
  const I32 m = as<I32>(inner_sizes.size());
  const I32 l = as<I32>(outer_sizes.size());

  Vec<I64> pref_in(m + 1, 0), pref_out(l + 1, 0);
  FOR(i, m) pref_in[i + 1] = pref_in[i] + inner_gains[i];
  FOR(i, l) pref_out[i + 1] = pref_out[i] + outer_gains[i];

  const I32 max_moves = m + l;
  Vec<Vec<I32>> dp(max_moves + 1), dp_in(max_moves + 1);
  dp[0] = {1};
  dp_in[0] = {0};

  FOR(p, max_moves) {
    const I32 nxt_len = min<I32>(as<I32>(p) + 1, m) + 1;
    Vec<I32> nxt(nxt_len, 0), nxt_in(nxt_len, 0);
    const auto& cur = dp[p];

    FOR(i_inner, sz(cur)) {
      const I32 prob = cur[i_inner];
      if (prob == 0) continue;

      const I32 eaten_outer = p - i_inner;
      if (eaten_outer < 0 || eaten_outer > l) continue;

      const I64 size_now = base_size + pref_in[i_inner] + pref_out[eaten_outer];
      const bool can_inner = (i_inner < m) && (inner_sizes[i_inner] <= size_now);
      const bool can_outer = (eaten_outer < l) && (outer_sizes[eaten_outer] <= size_now);
      if (!can_inner && !can_outer) continue;

      if (can_inner && can_outer) {
        const I64 add = as<I64>(prob) * INV2 % MOD;
        add_mod(nxt[i_inner + 1], add);
        add_mod(nxt_in[i_inner + 1], add);
        add_mod(nxt[i_inner], add);
      } else if (can_inner) {
        add_mod(nxt[i_inner + 1], prob);
        add_mod(nxt_in[i_inner + 1], prob);
      } else {
        add_mod(nxt[i_inner], prob);
      }
    }

    dp[p + 1] = std::move(nxt);
    dp_in[p + 1] = std::move(nxt_in);
  }

  return {std::move(dp), std::move(dp_in)};
}

auto solve_case(I32 n, I32 x, I32 y, Vec<I64> a, Vec<I64> b) -> I32 {
  if (x > y) {
    reverse(all(a));
    reverse(all(b));
    x = n + 1 - x;
    y = n + 1 - y;
  }

  const I32 ax = x - 1;
  const I32 ay = y - 1;
  const I32 m = y - x - 1;
  const I32 al = x - 1;
  const I32 br = n - y;

  Vec<I64> inner_a_sizes, inner_a_gains, outer_a_sizes, outer_a_gains;
  Vec<I64> inner_b_sizes, inner_b_gains, outer_b_sizes, outer_b_gains;

  inner_a_sizes.reserve(m);
  inner_a_gains.reserve(m);
  FOR(i, ax + 1, ay) {
    inner_a_sizes.eb(a[i]);
    inner_a_gains.eb(b[i]);
  }

  outer_a_sizes.reserve(al);
  outer_a_gains.reserve(al);
  FOR(i, al) {
    outer_a_sizes.eb(a[ax - 1 - i]);
    outer_a_gains.eb(b[ax - 1 - i]);
  }

  inner_b_sizes.reserve(m);
  inner_b_gains.reserve(m);
  FOR(i, m) {
    inner_b_sizes.eb(a[ay - 1 - i]);
    inner_b_gains.eb(b[ay - 1 - i]);
  }

  outer_b_sizes.reserve(br);
  outer_b_gains.reserve(br);
  FOR(i, br) {
    outer_b_sizes.eb(a[ay + 1 + i]);
    outer_b_gains.eb(b[ay + 1 + i]);
  }

  Vec<I64> gain_a_in(m + 1, 0), gain_a_out(al + 1, 0);
  Vec<I64> gain_b_in(m + 1, 0), gain_b_out(br + 1, 0);
  FOR(i, m) gain_a_in[i + 1] = gain_a_in[i] + inner_a_gains[i];
  FOR(i, al) gain_a_out[i + 1] = gain_a_out[i] + outer_a_gains[i];
  FOR(i, m) gain_b_in[i + 1] = gain_b_in[i] + inner_b_gains[i];
  FOR(i, br) gain_b_out[i + 1] = gain_b_out[i] + outer_b_gains[i];

  const I32 w = br + 1;
  const I32 size = (al + 1) * w;
  Vec<I32> win_a(size, 0), win_b(size, 0);

  FOR_R(oa, 0, al + 1) {
    FOR_R(ob, 0, br + 1) {
      const I32 idx = oa * w + ob;

      {
        const I32 num = m - oa + ob;
        if ((num & 1) == 0) {
          const I32 i_a = num / 2;
          if (0 <= i_a && i_a <= m) {
            const I32 i_b = m - i_a;
            const I64 s_a = a[ax] + gain_a_out[oa] + gain_a_in[i_a];
            const I64 s_b = a[ay] + gain_b_out[ob] + gain_b_in[i_b];

            const bool can_opp = s_b <= s_a;
            const bool can_out = (oa < al) && (outer_a_sizes[oa] <= s_a);

            if (!can_opp && !can_out) {
              win_a[idx] = 0;
            } else if (can_opp && !can_out) {
              win_a[idx] = 1;
            } else if (!can_opp && can_out) {
              win_a[idx] = win_b[(oa + 1) * w + ob];
            } else {
              win_a[idx] = as<I32>((1 + win_b[(oa + 1) * w + ob]) * INV2 % MOD);
            }
          }
        }
      }

      {
        const I32 num = m + 1 - oa + ob;
        if ((num & 1) == 0) {
          const I32 i_a = num / 2;
          if (0 <= i_a && i_a <= m) {
            const I32 i_b = m - i_a;
            const I64 s_a = a[ax] + gain_a_out[oa] + gain_a_in[i_a];
            const I64 s_b = a[ay] + gain_b_out[ob] + gain_b_in[i_b];

            const bool can_opp = s_a <= s_b;
            const bool can_out = (ob < br) && (outer_b_sizes[ob] <= s_b);

            if (!can_opp && !can_out) {
              win_b[idx] = 1;
            } else if (can_opp && !can_out) {
              win_b[idx] = 0;
            } else if (!can_opp && can_out) {
              win_b[idx] = win_a[oa * w + (ob + 1)];
            } else {
              win_b[idx] = as<I32>(as<I64>(win_a[oa * w + (ob + 1)]) * INV2 % MOD);
            }
          }
        }
      }
    }
  }

  if (m == 0) return win_a[0];

  auto [dp_a, dp_a_in] = build_step_dp(a[ax], inner_a_sizes, inner_a_gains, outer_a_sizes, outer_a_gains);
  auto [dp_b, dp_b_in] = build_step_dp(a[ay], inner_b_sizes, inner_b_gains, outer_b_sizes, outer_b_gains);

  const I32 max_a = as<I32>(dp_a.size()) - 1;
  const I32 max_b = as<I32>(dp_b.size()) - 1;
  I64 ans = 0;

  FOR(q, max_b + 1) {
    const I32 p = q + 1;
    if (p > max_a) break;

    const auto& arr_a = dp_a[p];
    const auto& arr_b = dp_b[q];
    if (arr_a.empty() || arr_b.empty()) continue;

    Vec<I32> pref(sz(arr_a) + 1, 0);
    FOR(i, sz(arr_a)) pref[i + 1] = as<I32>((pref[i] + arr_a[i]) % MOD);

    FOR(ib, sz(arr_b)) {
      const I32 prob_b = arr_b[ib];
      if (prob_b == 0) continue;

      const I32 bound = m - 1 - ib;
      if (bound < 0) continue;

      const I32 ob = q - ib;
      const I64 s_b = a[ay] + gain_b_in[ib] + gain_b_out[ob];
      const bool can_in = inner_b_sizes[ib] <= s_b;
      const bool can_out = (ob < br) && (outer_b_sizes[ob] <= s_b);
      if (can_in || can_out) continue;

      const I32 up = min<I32>(bound + 1, as<I32>(arr_a.size()));
      const I32 sum_a = pref[up];
      ans = (ans + as<I64>(prob_b) * sum_a) % MOD;
    }
  }

  FOR(p, 1, max_a + 1) {
    const I32 q = p - 1;
    if (q < 0 || q > max_b) continue;

    const auto& arr_a_in = dp_a_in[p];
    const auto& arr_b = dp_b[q];

    FOR(ia, sz(arr_a_in)) {
      const I32 prob_a_in = arr_a_in[ia];
      if (prob_a_in == 0) continue;

      const I32 ib = m - ia;
      if (ib < 0 || ib >= as<I32>(arr_b.size())) continue;
      const I32 prob_b = arr_b[ib];
      if (prob_b == 0) continue;

      const I32 oa = p - ia;
      const I32 ob = q - ib;
      if (0 <= oa && oa <= al && 0 <= ob && ob <= br) {
        const I64 ways = as<I64>(prob_a_in) * prob_b % MOD;
        ans = (ans + ways * win_b[oa * w + ob]) % MOD;
      }
    }
  }

  FOR(q, 1, max_b + 1) {
    const I32 p = q;
    if (p > max_a) continue;

    const auto& arr_b_in = dp_b_in[q];
    const auto& arr_a = dp_a[p];

    FOR(ib, sz(arr_b_in)) {
      const I32 prob_b_in = arr_b_in[ib];
      if (prob_b_in == 0) continue;

      const I32 ia = m - ib;
      if (ia < 0 || ia >= as<I32>(arr_a.size())) continue;
      const I32 prob_a = arr_a[ia];
      if (prob_a == 0) continue;

      const I32 oa = p - ia;
      const I32 ob = q - ib;
      if (0 <= oa && oa <= al && 0 <= ob && ob <= br) {
        const I64 ways = as<I64>(prob_b_in) * prob_a % MOD;
        ans = (ans + ways * win_a[oa * w + ob]) % MOD;
      }
    }
  }

  return as<I32>(ans);
}

} // namespace

void solve() {
  INT(n);
  INT(x, y);
  Vec<I64> a(n), b(n);
  IN(a);
  IN(b);

  OUT(solve_case(n, x, y, std::move(a), std::move(b)));
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
