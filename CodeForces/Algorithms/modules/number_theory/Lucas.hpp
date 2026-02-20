#ifndef CP_MODULES_NUMBER_THEORY_LUCAS_HPP
#define CP_MODULES_NUMBER_THEORY_LUCAS_HPP

#include "_Common.hpp"

/// @brief Lucas theorem for binomial coefficients modulo prime.
struct Lucas {
  I64 p;
  Vec<I64> fact, inv_fact;

  /// @brief Precomputes factorials modulo prime p for [0, p-1].
  Lucas(I64 prime) : p(prime), fact(p), inv_fact(p) {
    fact[0] = 1;
    FOR(i, 1, p) fact[i] = fact[i - 1] * i % p;
    inv_fact[p - 1] = mod_pow(fact[p - 1], p - 2, p);
    FOR_R(i, p - 1) inv_fact[i] = inv_fact[i + 1] * (i + 1) % p;
  }

  /// @brief Computes nCr for 0 <= n,r < p.
  I64 nCr_small(I64 n, I64 r) {
    if (r > n || r < 0) return 0;
    return fact[n] * inv_fact[r] % p * inv_fact[n - r] % p;
  }

  /// @brief Computes nCr modulo p for arbitrary n and r.
  I64 nCr(I64 n, I64 r) {
    if (r > n || r < 0) return 0;
    I64 result = 1;
    while (n > 0 || r > 0) {
      result = result * nCr_small(n % p, r % p) % p;
      n /= p;
      r /= p;
    }
    return result;
  }
};

#endif
