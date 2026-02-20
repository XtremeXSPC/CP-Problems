#ifndef CP_MODULES_NUMBER_THEORY_MOBIUS_HPP
#define CP_MODULES_NUMBER_THEORY_MOBIUS_HPP

#include "_Common.hpp"

/// @brief Mobius function precomputation and inversion helper.
struct Mobius {
  Vec<I32> mu;
  I32 n;

  /// @brief Computes mu[1..size] in O(size).
  Mobius(I32 size) : mu(size + 1), n(size) {
    mu[1] = 1;
    Vec<bool> is_prime(n + 1, true);
    Vec<I32> primes;

    FOR(i, 2, n + 1) {
      if (is_prime[i]) {
        primes.pb(i);
        mu[i] = -1;
      }

      for (I32 p : primes) {
        if (i * p > n) break;
        is_prime[i * p] = false;
        if (i % p == 0) {
          mu[i * p] = 0;
          break;
        }
        mu[i * p] = -mu[i];
      }
    }
  }

  /// @brief Mobius inversion: f(n) = sum_{d|n} mu(d) * g(n/d), given g(n) = sum_{d|n} f(d).
   Vec<I64> invert(const Vec<I64>& g) {
    Vec<I64> f(sz(g));
    FOR(i, 1, sz(g)) {
      for (I32 j = i; j < sz(g); j += i) {
        f[j] += mu[i] * g[j / i];
      }
    }
    return f;
  }
};

#endif
