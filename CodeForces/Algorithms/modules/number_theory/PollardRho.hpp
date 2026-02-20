#ifndef CP_MODULES_NUMBER_THEORY_POLLARD_RHO_HPP
#define CP_MODULES_NUMBER_THEORY_POLLARD_RHO_HPP

#include "_Common.hpp"
#include "Primality64.hpp"

/**
 * @brief Pollard's Rho randomized factor finder.
 * @return A non-trivial factor of @p n (or n itself if prime).
 */
inline I64 pollard_rho(I64 n) {
  if (n % 2 == 0) return 2;
  if (n % 3 == 0) return 3;
  if (miller_rabin(n)) return n;

  static thread_local std::mt19937_64 rng(
      static_cast<U64>(std::chrono::steady_clock::now().time_since_epoch().count()));

  std::uniform_int_distribution<I64> dist_c(1, n - 1);
  std::uniform_int_distribution<I64> dist_x(0, n - 1);

  while (true) {
    I64 c = dist_c(rng);
    I64 x = dist_x(rng);
    I64 y = x;
    I64 d = 1;

    auto f = [&](I64 v) { return (mod_mul64(v, v, n) + c) % n; };

    while (d == 1) {
      x = f(x);
      y = f(f(y));
      d = std::gcd(abs_diff64(x, y), n);
    }

    if (d != n) return d;
  }
}

/**
 * @brief Full prime factorization for 64-bit integer.
 * @return Sorted multiset of prime factors.
 */
inline Vec<I64> factorize(I64 n) {
  Vec<I64> factors;

  std::function<void(I64)> factor = [&](I64 x) {
    if (x == 1) return;
    if (miller_rabin(x)) {
      factors.pb(x);
      return;
    }

    I64 p = pollard_rho(x);
    while (p == x) p = pollard_rho(x);
    factor(p);
    factor(x / p);
  };

  factor(n);
  sort(all(factors));
  return factors;
}

#endif
