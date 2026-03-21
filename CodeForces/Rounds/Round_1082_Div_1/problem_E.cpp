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
#include "modules/number_theory/NTT.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

constexpr I32 MAX_N = 400000;

VecI64 pow2_mod;

auto get_divisors(I32 x) -> VecI32 {
  VecI32 divs;
  for (I32 d = 1; d * d <= x; ++d) {
    if (x % d != 0) {
      continue;
    }
    divs.eb(d);
    if (d * d != x) {
      divs.eb(x / d);
    }
  }
  std::sort(all(divs));
  return divs;
}

auto circular_corr(const VecI64& a, const VecI64& b) -> VecI64 {
  const I32 d = as<I32>(a.size());
  VecI64 a2(2 * d, 0);
  VecI64 br(d, 0);

  FOR(i, d) {
    a2[i] = a[i];
    a2[i + d] = a[i];
    br[i] = b[d - 1 - i];
  }

  const VecI64 conv = NTT::multiply(a2, br);
  VecI64 res(d, 0);
  FOR(r, d) res[r] = conv[r + d - 1];
  return res;
}

auto build_superset_counts(const String& t, I32 m, I32 d) -> VecI64 {
  VecI32 p_mask(d, 0);
  VecI32 q_mask(d, 0);

  bool bad = false;
  FOR(i, m) {
    const char ch = t[i];
    if (ch == '?') {
      continue;
    }
    const I32 bit = (ch == 'a' ? 1 : 2);
    I32& cur = p_mask[i % d];
    cur |= bit;
    if (cur == 3) {
      bad = true;
      break;
    }
  }

  if (bad) {
    return VecI64(d, 0);
  }

  FOR(i, m) {
    const char ch = t[m + i];
    if (ch == '?') {
      continue;
    }
    const I32 bit = (ch == 'a' ? 1 : 2);
    I32& cur = q_mask[i % d];
    cur |= bit;
    if (cur == 3) {
      bad = true;
      break;
    }
  }

  if (bad) {
    return VecI64(d, 0);
  }

  VecI64 a_a(d, 0), a_b(d, 0), a_q(d, 0);
  VecI64 b_a(d, 0), b_b(d, 0), b_q(d, 0);

  FOR(x, d) {
    if (p_mask[x] == 0) a_q[x] = 1;
    if (p_mask[x] == 1) a_a[x] = 1;
    if (p_mask[x] == 2) a_b[x] = 1;

    if (q_mask[x] == 0) b_q[x] = 1;
    if (q_mask[x] == 1) b_a[x] = 1;
    if (q_mask[x] == 2) b_b[x] = 1;
  }

  const VecI64 ab = circular_corr(a_a, b_b);
  const VecI64 ba = circular_corr(a_b, b_a);
  const VecI64 qq = circular_corr(a_q, b_q);

  VecI64 f(d, 0);
  FOR(r, d) {
    const I64 conflicts = ab[r] + ba[r];
    if (conflicts != 0) {
      continue;
    }
    const I32 free_cnt = as<I32>(qq[r]);
    f[r] = pow2_mod[free_cnt];
  }

  return f;
}

void solve() {
  INT(n);
  STR(t);

  const I32 m = n / 2;
  const VecI32 divs = get_divisors(m);
  const I32 k = as<I32>(divs.size());

  Vec<VecI64> exact(k);
  I64 answer = 0;

  FOR(i, k) {
    const I32 d = divs[i];
    VecI64 cur = build_superset_counts(t, m, d);

    FOR(j, i) {
      const I32 small_d = divs[j];
      if (d % small_d != 0) {
        continue;
      }
      const VecI64& small = exact[j];
      FOR(r, d) {
        cur[r] -= small[r % small_d];
        if (cur[r] < 0) {
          cur[r] += MOD2;
        }
      }
    }

    for (I64 x : cur) {
      answer += x;
      if (answer >= MOD2) {
        answer -= MOD2;
      }
    }

    exact[i] = std::move(cur);
  }

  OUT(answer);
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  pow2_mod.assign(MAX_N + 1, 1);
  FOR(i, 1, MAX_N + 1) {
    pow2_mod[i] = (pow2_mod[i - 1] * 2) % MOD2;
  }

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
