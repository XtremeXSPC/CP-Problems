#ifndef CP_MODULES_NUMBER_THEORY_SIEVE_HPP
#define CP_MODULES_NUMBER_THEORY_SIEVE_HPP

#include "_Common.hpp"

/// @brief Prime sieve with smallest prime factor table.
struct Sieve {
  I32 n;
  Vec<bool> is_prime;
  Vec<I32> primes;
  Vec<I32> smallest_factor;

  /// @brief Builds sieve up to @p size.
  Sieve(I32 size)
      : n(std::max<I32>(size, 0)),
        is_prime(as<Size>(n + 1), true),
        smallest_factor(as<Size>(n + 1), 0) {
    is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;

    FOR(i, 2, n + 1) {
      if (is_prime[i]) {
        primes.push_back(i);
        smallest_factor[i] = i;

        for (I64 j = (I64)i * i; j <= n; j += i) {
          is_prime[j] = false;
          if (smallest_factor[j] == 0) smallest_factor[j] = i;
        }
      }
    }
  }

  /// @brief Checks primality for x within sieve range.
  bool check_prime(I32 x) const {
    return x >= 2 && x <= n && is_prime[x];
  }

  /// @brief Prime factorization of x as (prime, exponent) pairs.
  Vec<PairI32> factorize(I32 x) const {
    Vec<PairI32> factors;
    if (x <= 1) return factors;

    if (x > n) {
      for (I32 p = 2; (I64)p * p <= x; ++p) {
        if (x % p != 0) continue;
        I32 cnt = 0;
        while (x % p == 0) {
          x /= p;
          cnt++;
        }
        factors.push_back({p, cnt});
      }
      if (x > 1) factors.push_back({x, 1});
      return factors;
    }

    while (x > 1) {
      I32 p = smallest_factor[x];
      I32 cnt = 0;
      while (x % p == 0) {
        x /= p;
        cnt++;
      }
      factors.push_back({p, cnt});
    }
    return factors;
  }

  /// @brief Counts divisors of x from its prime factorization.
  I32 count_divisors(I32 x) const {
    auto factors = factorize(x);
    I32 result = 1;
    for (auto [p, cnt] : factors) {
      result *= (cnt + 1);
    }
    return result;
  }
};

#endif
