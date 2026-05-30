#ifndef CP_TEMPLATE_PROFILE_STRICT
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_TIMER
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"
#include "templates/modules/IntegerMath.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

const I128 MAX_I128 = ~((I128)1 << 127);

void solve() {
  I64 n, a, b, k;
  IN(n, a, b, k);

  if (a == b) {
    OUT(k % a == 0 ? n : 0);
    return;
  }

  I64 max_ans = (k % (a + b) == 0) ? n - 1 : 0;

  // Golden path evaluation.
  I64 l_gold = -1;
  if (k % a == 0) {
    I64 cur = k / a;
    while (cur >= 0) {
      l_gold++;
      I64 nxt = k - b * cur;
      if (nxt < 0 || nxt % a != 0)
        break;
      cur = nxt / a;
    }
  }

  if (n <= l_gold + 1) {
    OUT(n);
    return;
  }

  // Reduce by gab = gcd(a, b):
  I64 gab = std::gcd(a, b);
  if (k % gab == 0) {
    a /= gab;
    b /= gab;
    k /= gab;
  }

  // Precompute bounds to avoid overflow.
  Vec<I128> A_arr(1, 1);
  Vec<I128> B_arr(1, 0);
  Vec<I128> PA_arr(1, 1);

  I64 l_safe = 0;
  while (l_safe < 60) {
    I128 A = A_arr.back(), B = B_arr.back(), pa = PA_arr.back();

    I128 abs_A = A < 0 ? -A : A;
    if (abs_A > 0 && MAX_I128 / b < abs_A)
      break;
    if (pa > 0 && MAX_I128 / a < pa)
      break;

    I128 abs_B = B < 0 ? -B : B;
    if (abs_B > 0 && MAX_I128 / b < abs_B)
      break;
    if (MAX_I128 / k < pa)
      break;

    I128 t1 = -(I128)b * B;
    I128 t2 = (I128)k * pa;
    if (t1 > 0 && t2 > 0 && MAX_I128 - t1 < t2)
      break;
    if (t1 < 0 && t2 < 0 && -t1 > MAX_I128 - (-t2))
      break;

    A_arr.push_back(-(I128)b * A);
    B_arr.push_back(t1 + t2);
    PA_arr.push_back(pa * a);
    l_safe++;
  }

  // Max path length search.
  I64 l_max = 0;
  for (I64 L = l_safe; L >= 1; --L) {
    I128 LB = 0, UB = k / b; // T0 <= floor(k/b) so that T1 = (k - b*T0)/a >= 0
    bool VL = true;

    FOR(i, 1, L + 1) {
      I128 A = A_arr[i], B = B_arr[i];
      if (A > 0)
        LB = max(LB, div_ceil(-B, A));
      else if (A < 0)
        UB = min(UB, div_floor(B, -A));
    }

    if (LB > UB)
      continue;

    I128 R = 0, M = 1;
    FOR(i, 1, L + 1) {
      I128 MOD_i = PA_arr[i];
      I128 X = A_arr[i] % MOD_i;
      if (X < 0)
        X += MOD_i;
      I128 Y = (-B_arr[i]) % MOD_i;
      if (Y < 0)
        Y += MOD_i;

      auto [g, inv] = inv_gcd(X, MOD_i);
      if (Y % g != 0) {
        VL = false;
        break;
      }

      I128 m_i = MOD_i / g;
      I128 r_i = (Y / g) % m_i;
      if (r_i < 0)
        r_i += m_i;
      inv %= m_i;
      if (inv < 0)
        inv += m_i;
      r_i = (r_i * inv) % m_i;
      if (r_i < 0)
        r_i += m_i;

      if (!merge_congruences(R, M, r_i, m_i)) {
        VL = false;
        break;
      }
    }

    if (VL) {
      I128 T0 = R;
      if (T0 < LB) {
        I128 q = (LB - R + M - 1) / M;
        T0 = R + q * M;
      }
      if (T0 <= UB) {
        l_max = L;
        break;
      }
    }
  }

  // Score combination.
  I64 v_gold = max((I64)0, l_gold + 1);
  I64 n_rem  = n - v_gold;

  I64 p_blk = 0;
  if (l_max > 0) {
    I64 blk = n_rem / (l_max + 1);
    I64 rem = n_rem % (l_max + 1);
    p_blk = blk * l_max + max((I64)0, rem - 1);
  }

  I64 p_tot = v_gold + p_blk;

  OUT(max(max_ans, p_tot));
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Stopwatch timer;
#endif

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
