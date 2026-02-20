#ifndef CP_MODULES_NUMBER_THEORY_PHI_HPP
#define CP_MODULES_NUMBER_THEORY_PHI_HPP

#include "_Common.hpp"

/// @brief Euler's totient φ(n) by trial factorization.
inline I64 euler_phi(I64 n) {
  I64 result = n;
  for (I64 i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      while (n % i == 0) n /= i;
      result -= result / i;
    }
  }
  if (n > 1) result -= result / n;
  return result;
}

/// @brief Computes totients for all values in [0, n].
inline Vec<I32> euler_phi_sieve(I32 n) {
  Vec<I32> phi(n + 1);
  std::iota(all(phi), 0);

  FOR(i, 2, n + 1) {
    if (phi[i] == i) {  // 'i' is prime.
      for (I32 j = i; j <= n; j += i) {
        phi[j] -= phi[j] / i;
      }
    }
  }

  return phi;
}

#endif
