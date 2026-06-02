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
/* Helpers for the problem */

/*
 * A perfectly cooked chain uses pan times T_0, T_1, ... tied by
 *   b*T_{i-1} + a*T_i = k    =>    T_i = (k - b*T_{i-1}) / a.
 * Unrolling the recurrence gives a closed form for every step:
 *   a^i * T_i = (-b)^i * T_0 + off_i,   off_i = sum_{j<i} (-b)^{i-1-j} k a^j.
 * Hence each step constrains the starting time T_0 in two ways:
 *   - non-negativity:  (-b)^i * T_0 + off_i >= 0      (a window [lo, hi]),
 *   - integrality:     (-b)^i * T_0 + off_i ≡ 0 (mod a^i)  (a congruence).
 */

// Steps of the "golden" chain that starts already cooked (T_0 = k/a), so no
// pancake is sacrificed. Returns -1 when it does not exist (k % a != 0);
// otherwise the count of extra steps (total cooked pancakes = result + 1).
I64 golden_chain_steps(I64 a, I64 b, I64 k) {
  if (k % a != 0)
    return -1;
  I64 steps = 0;
  for (I64 t = k / a;;) {
    I64 next = k - b * t;
    if (next < 0 || next % a != 0)
      break;
    t = next / a;
    ++steps;
  }
  return steps;
}

// Longest feasible chain length L in [1, 60] for the forest case (a != b),
// or 0 when none exists.
I64 longest_chain(I64 a, I64 b, I64 k) {
  // Reduce by gcd(a, b): the T_i sequence is unchanged, but a smaller a keeps a^i
  // (and k*a^i) inside __int128 long enough to reach the true maximum length.
  // If gcd(a, b) does not divide k no pair is perfect, so the length is 0 anyway.
  const I64 g_ab = std::gcd(a, b);
  if (k % g_ab == 0) {
    a /= g_ab;
    b /= g_ab;
    k /= g_ab;
  }

  // Roll out (-b)^i, a^i and off_i while they stay inside __int128 range.
  Vec<I128> negb_pow{1}; // (-b)^i, also the coefficient of T_0 at step i
  Vec<I128> a_pow{1};    // a^i, the modulus at step i
  Vec<I128> off{0};      // off_i

  I64 reach = 0;
  while (reach < 60) {
    I128 next_negb, next_a, scaled, k_term, next_off;
    if (__builtin_mul_overflow(negb_pow[reach], -(I128)b, &next_negb))
      break;
    if (__builtin_mul_overflow(a_pow[reach], (I128)a, &next_a))
      break;
    if (__builtin_mul_overflow(off[reach], -(I128)b, &scaled))
      break;
    if (__builtin_mul_overflow((I128)k, a_pow[reach], &k_term))
      break;
    if (__builtin_add_overflow(scaled, k_term, &next_off))
      break;
    negb_pow.push_back(next_negb);
    a_pow.push_back(next_a);
    off.push_back(next_off);
    ++reach;
  }

  for (I64 L = reach; L >= 1; --L) {
    // 1) Non-negativity of every T_i pins T_0 to an integer window [lo, hi].
    //    T_0 <= floor(k/b) keeps T_1 = (k - b*T_0)/a non-negative.
    I128 lo = 0, hi = k / b;
    FOR(i, 1, L + 1) {
      const I128 coeff = negb_pow[i];
      if (coeff > 0)
        lo = std::max(lo, div_ceil(-off[i], coeff));
      else if (coeff < 0)
        hi = std::min(hi, div_floor(off[i], -coeff));
    }
    if (lo > hi)
      continue;

    // 2) Integrality intersects the congruences  coeff*T_0 ≡ -off_i (mod a^i).
    I128 rem = 0, mod = 1;
    bool feasible = true;
    FOR(i, 1, L + 1) {
      const I128 step_mod = a_pow[i];
      const I128 coeff    = safe_mod(negb_pow[i], step_mod);
      const I128 target   = safe_mod(-off[i], step_mod);

      auto [g, coeff_inv] = inv_gcd(coeff, step_mod);
      if (target % g != 0) {
        feasible = false;
        break;
      }

      const I128 class_mod = step_mod / g;
      I128       residue   = safe_mod(target / g, class_mod);
      residue              = safe_mod(residue * safe_mod(coeff_inv, class_mod), class_mod);

      if (!merge_congruences(rem, mod, residue, class_mod)) {
        feasible = false;
        break;
      }
    }
    if (!feasible)
      continue;

    // 3) Smallest T_0 >= lo in the congruence class; the length is usable iff <= hi.
    I128 t0 = rem;
    if (t0 < lo)
      t0 += div_ceil(lo - rem, mod) * mod;
    if (t0 <= hi)
      return L;
  }
  return 0;
}

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  I64 n, a, b, k;
  IN(n, a, b, k);

  // a == b: the map is the involution x <-> k/a - x; cook everything or nothing.
  if (a == b) {
    OUT(k % a == 0 ? n : 0);
    return;
  }

  // Self-loop at x* = k/(a+b): cook n-1 pancakes after sacrificing the first.
  I64 best = (k % (a + b) == 0) ? n - 1 : 0;

  // A prefix of the golden chain may already cook all n pancakes.
  I64 golden = golden_chain_steps(a, b, k);
  if (n <= golden + 1) {
    OUT(n);
    return;
  }

  // Otherwise: one golden block (if any) plus repeated longest-chain blocks,
  // each block cooking L of its L+1 pancakes.
  I64 golden_pts = max<I64>(0, golden + 1);
  I64 budget     = n - golden_pts;

  I64 path      = longest_chain(a, b, k);
  I64 block_pts = 0;
  if (path > 0) {
    I64 blocks   = budget / (path + 1);
    I64 leftover = budget % (path + 1);
    block_pts    = blocks * path + max<I64>(0, leftover - 1);
  }

  OUT(max(best, golden_pts + block_pts));
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
