#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_MATH
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

constexpr I64 MODV = MOD2;

struct Query {
  I32 n;
  I64 m;
  I32 v;
};

Vec<I64> fac;
Vec<I64> ifac;

[[nodiscard]] inline auto norm(I64 x) -> I64 {
  x %= MODV;
  if (x < 0) x += MODV;
  return x;
}

[[nodiscard]] inline auto add_mod(I64 a, I64 b) -> I64 {
  a += b;
  if (a >= MODV) a -= MODV;
  return a;
}

[[nodiscard]] inline auto sub_mod(I64 a, I64 b) -> I64 {
  a -= b;
  if (a < 0) a += MODV;
  return a;
}

[[nodiscard]] inline auto mul_mod(I64 a, I64 b) -> I64 {
  return (a * b) % MODV;
}

[[nodiscard]] inline auto mod_pow64(I64 base, I64 exp) -> I64 {
  return mod_pow<I64>(base, exp, MODV);
}

void init_factorials(const I32 max_n) {
  fac.assign(as<Size>(max_n + 1), 1);
  ifac.assign(as<Size>(max_n + 1), 1);

  FOR(i, 1, max_n + 1) fac[i] = mul_mod(fac[i - 1], i);
  ifac[max_n] = mod_pow64(fac[max_n], MODV - 2);
  FOR_R(i, max_n) ifac[i] = mul_mod(ifac[i + 1], i + 1);
}

[[nodiscard]] inline auto nCk(const I32 n, const I32 k) -> I64 {
  if (k < 0 || k > n) return 0;
  return mul_mod(fac[n], mul_mod(ifac[k], ifac[n - k]));
}

[[nodiscard]] auto count_easy(const Query& q, const I64 v_pow_nm, const Vec<I64>& lhs_pow_n,
                              const Vec<I64>& rhs_pow_m, const I32 p, const I32 qv) -> I64 {
  if (p > q.v || qv <= 0) return 0;

  const I32 p_minus_1 = p - 1;
  const I32 v_minus_q = q.v - qv;
  const I64 lhs = lhs_pow_n[p_minus_1];
  const I64 rhs = rhs_pow_m[v_minus_q];
  return norm(lhs - v_pow_nm + rhs);
}

[[nodiscard]] auto count_hard(const Query& q, const I64 v_pow_n, const I64 inv_v, const Vec<I64>& comb,
                              const Vec<I64>& pow_n, const Vec<I64>& inv, const I32 x) -> I64 {
  if (x <= 1 || x >= q.v) return 0;

  const I32 tail = q.v - x + 1;
  const I64 inv_tail = inv[tail];

  I64 x_pow = 1;
  I64 v_pow = v_pow_n;
  I64 tail_pow = pow_n[tail];
  I64 sum = 0;

  FOR(k, q.n + 1) {
    const I64 left = mul_mod(x_pow, v_pow);
    const I64 gk = sub_mod(left, tail_pow);
    const I64 ways = mod_pow64(gk, q.m);
    const I64 term = mul_mod(comb[k], ways);

    if (k & 1) {
      sum = sub_mod(sum, term);
    } else {
      sum = add_mod(sum, term);
    }

    x_pow = mul_mod(x_pow, x);
    v_pow = mul_mod(v_pow, inv_v);
    tail_pow = mul_mod(tail_pow, inv_tail);
  }

  return sum;
}

[[nodiscard]] auto solve_query(const Query& q) -> I64 {
  const I64 nm = as<I64>(q.n) * q.m;

  if (q.n == 1 || q.m == 1 || q.v == 1) {
    return mod_pow64(q.v, nm);
  }

  const I64 v_pow_m = mod_pow64(q.v, q.m);
  const I64 v_pow_n = mod_pow64(q.v, q.n);
  const I64 v_pow_nm = mod_pow64(q.v, nm);
  const I64 inv_v = mod_pow64(q.v, MODV - 2);

  Vec<I64> pow_m(as<Size>(q.v + 1), 0);
  Vec<I64> pow_n(as<Size>(q.v + 1), 0);
  Vec<I64> lhs_pow_n(as<Size>(q.v + 1), 0);
  Vec<I64> rhs_pow_m(as<Size>(q.v + 1), 0);
  Vec<I64> inv(as<Size>(q.v + 1), 0);

  FOR(i, q.v + 1) {
    pow_m[i] = mod_pow64(i, q.m);
    pow_n[i] = mod_pow64(i, q.n);
  }

  FOR(i, q.v + 1) {
    lhs_pow_n[i] = mod_pow64(sub_mod(v_pow_m, pow_m[i]), q.n);
    rhs_pow_m[i] = mod_pow64(sub_mod(v_pow_n, pow_n[i]), q.m);
  }

  inv[1] = 1;
  FOR(i, 2, q.v + 1) {
    inv[i] = sub_mod(0, mul_mod(MODV / i, inv[MODV % i]));
  }

  Vec<I64> comb(as<Size>(q.n + 1), 0);
  FOR(k, q.n + 1) comb[k] = nCk(q.n, k);

  I64 answer = 0;

  FOR(x, 1, q.v + 1) {
    const I64 a = count_easy(q, v_pow_nm, lhs_pow_n, rhs_pow_m, x, x);
    const I64 b = count_easy(q, v_pow_nm, lhs_pow_n, rhs_pow_m, x + 1, x);
    const I64 c = count_easy(q, v_pow_nm, lhs_pow_n, rhs_pow_m, x, x - 1);
    const I64 d = count_hard(q, v_pow_n, inv_v, comb, pow_n, inv, x);
    const I64 fx = norm(a - b - c + d);
    answer = add_mod(answer, fx);
  }

  return answer;
}

auto solve() -> void {
  INT(T);

  Vec<Query> queries(as<Size>(T));
  I32 max_n = 0;
  FOR(i, T) {
    IN(queries[i].n, queries[i].m, queries[i].v);
    max_n = std::max(max_n, queries[i].n);
  }

  init_factorials(max_n);
  for (const auto& q : queries) OUT(solve_query(q));

}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  solve();
  return 0;
}

//===----------------------------------------------------------------------===//
