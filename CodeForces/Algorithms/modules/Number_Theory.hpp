#pragma once
#include "../templates/Types.hpp"
#include "../templates/Macros.hpp"
#include "../templates/Math.hpp"
#include "../templates/Constants.hpp"
#include <algorithm>

/**
 * @file Number_Theory.hpp
 * @brief Number-theoretic algorithms and arithmetic utilities.
 */

//===----------------------------------------------------------------------===//
//=========================== Number Theory Module ===========================//

/**
 * @brief Prime sieve with smallest prime factor table.
 */
struct Sieve {
  I32 n;
  Vec<bool> is_prime;
  Vec<I32> primes;
  Vec<I32> smallest_factor;

  /**
   * @brief Builds sieve up to @p size.
   */
  Sieve(I32 size) : n(size), is_prime(size + 1, true), smallest_factor(size + 1) {
    is_prime[0] = is_prime[1] = false;

    FOR(i, 2, n + 1) {
      if (is_prime[i]) {
        primes.pb(i);
        smallest_factor[i] = i;

        for (I64 j = (I64)i * i; j <= n; j += i) {
          is_prime[j] = false;
          if (smallest_factor[j] == 0) smallest_factor[j] = i;
        }
      }
    }
  }

  /**
   * @brief Checks primality for x within sieve range.
   */
  bool check_prime(I32 x) const {
    return x >= 2 && x <= n && is_prime[x];
  }

  /**
   * @brief Prime factorization of x as (prime, exponent) pairs.
   *
   * Uses smallest_factor table for x <= n; trial division otherwise.
   */
  Vec<PII> factorize(I32 x) const {
    Vec<PII> factors;
    if (x <= 1) return factors;

    if (x > n) {
      for (I32 p = 2; (I64)p * p <= x; ++p) {
        if (x % p != 0) continue;
        I32 cnt = 0;
        while (x % p == 0) {
          x /= p;
          cnt++;
        }
        factors.pb({p, cnt});
      }
      if (x > 1) factors.pb({x, 1});
      return factors;
    }

    while (x > 1) {
      I32 p = smallest_factor[x];
      I32 cnt = 0;
      while (x % p == 0) {
        x /= p;
        cnt++;
      }
      factors.pb({p, cnt});
    }
    return factors;
  }

  /**
   * @brief Counts divisors of x from its prime factorization.
   */
  I32 count_divisors(I32 x) const {
    auto factors = factorize(x);
    I32 result = 1;
    for (auto [p, cnt] : factors) {
      result *= (cnt + 1);
    }
    return result;
  }
};

/**
 * @brief Extended Euclidean algorithm.
 * @tparam T Integer type.
 * @return Tuple (g, x, y) such that ax + by = g = gcd(a, b).
 */
template <typename T>
std::tuple<T, T, T> extended_gcd(T a, T b) {
  if (b == 0) return {a, 1, 0};
  auto [g, x1, y1] = extended_gcd(b, a % b);
  T x = y1;
  T y = x1 - (a / b) * y1;
  return {g, x, y};
}

/**
 * @brief Modular inverse using extended gcd.
 * @return a^{-1} mod m, or -1 if inverse does not exist.
 */
template <typename T>
T mod_inverse(T a, T m) {
  auto [g, x, _] = extended_gcd(a, m);
  if (g != 1) return -1;  // No inverse exists.
  return ((x % m) + m) % m;
}

/**
 * @brief General Chinese Remainder Theorem for possibly non-coprime moduli.
 * @return Pair (x, M) meaning x mod M is the combined congruence, or {-1,-1}.
 */
template <typename T>
std::pair<T, T> chinese_remainder(const Vec<T>& a, const Vec<T>& m) {
  T x = 0, M = 1;

  FOR(i, sz(a)) {
    auto [g, p, q] = extended_gcd(M, m[i]);
    if ((a[i] - x) % g != 0) return {-1, -1};  // No solution.

    x += M * ((a[i] - x) / g * p % (m[i] / g));
    M *= m[i] / g;
    x = ((x % M) + M) % M;
  }

  return {x, M};
}

/**
 * @brief Euler's totient φ(n) by trial factorization.
 */
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

/**
 * @brief Computes totients for all values in [0, n].
 */
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

/**
 * @brief Overflow-safe modular multiplication on 64-bit inputs.
 */
inline I64 mod_mul64(I64 a, I64 b, I64 mod) {
  return static_cast<I64>((static_cast<__int128>(a) * b) % mod);
}

/**
 * @brief Fast modular exponentiation on 64-bit inputs.
 */
inline I64 mod_pow64(I64 base, I64 exp, I64 mod) {
  I64 result = 1 % mod;
  base %= mod;
  while (exp > 0) {
    if (exp & 1) result = mod_mul64(result, base, mod);
    base = mod_mul64(base, base, mod);
    exp >>= 1;
  }
  return result;
}

/**
 * @brief Absolute difference helper avoiding signed overflow pitfalls.
 */
inline I64 abs_diff64(I64 a, I64 b) {
  return a > b ? a - b : b - a;
}

/**
 * @brief Deterministic Miller-Rabin primality test for 64-bit integers.
 */
inline bool miller_rabin(I64 n) {
  if (n < 2) return false;
  if (n == 2) return true;
  if (n % 2 == 0) return false;

  // Witnesses for deterministic test up to 2^64.
  const Vec<I64> witnesses = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

  I64 d = n - 1;
  I32 r = 0;
  while (d % 2 == 0) {
    d /= 2;
    r++;
  }

  auto check_witness = [&](I64 a) {
    I64 x = mod_pow64(a, d, n);
    if (x == 1 || x == n - 1) return false;

    FOR(_, r - 1) {
      x = mod_mul64(x, x, n);
      if (x == n - 1) return false;
    }
    return true;
  };

  for (I64 a : witnesses) {
    if (a >= n) break;
    if (check_witness(a)) return false;
  }

  return true;
}

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

/**
 * @brief Generic modular exponentiation helper.
 */
template <typename T>
T mod_pow(T base, T exp, T mod) {
  T result = 1;
  base %= mod;
  while (exp > 0) {
    if (exp & 1) result = (__int128)result * base % mod;
    base = (__int128)base * base % mod;
    exp >>= 1;
  }
  return result;
}

/**
 * @brief Discrete logarithm via baby-step giant-step.
 * @return Smallest non-negative x such that a^x = b (mod m), or -1 if none.
 */
inline I64 discrete_log(I64 a, I64 b, I64 m) {
  I64 n = sqrt(m) + 1;

  std::unordered_map<I64, I64> values;
  I64 cur = b;
  FOR(p, n) {
    values[cur] = p;
    cur = (__int128)cur * a % m;
  }

  I64 factor = mod_pow(a, n, m);
  cur = factor;

  FOR(q, 1, n + 1) {
    if (values.count(cur)) {
      I64 ans = q * n - values[cur];
      if (mod_pow(a, ans, m) == b) return ans;
    }
    cur = (__int128)cur * factor % m;
  }

  return -1;  // No solution.
}

/**
 * @brief Finds a primitive root modulo prime p.
 * @return Primitive root or -1 if not found.
 */
inline I64 primitive_root(I64 p) {
  if (p == 2) return 1;

  I64 phi = p - 1;
  auto factors = factorize(phi);
  factors.erase(unique(all(factors)), factors.end());

  FOR(g, 2, p) {
    bool is_primitive = true;
    for (I64 factor : factors) {
      if (mod_pow(g, phi / factor, p) == 1) {
        is_primitive = false;
        break;
      }
    }
    if (is_primitive) return g;
  }

  return -1;
}

/**
 * @brief Lucas theorem for binomial coefficients modulo prime.
 */
struct Lucas {
  I64 p;
  Vec<I64> fact, inv_fact;

  /**
   * @brief Precomputes factorials modulo prime p for [0, p-1].
   */
  Lucas(I64 prime) : p(prime), fact(p), inv_fact(p) {
    fact[0] = 1;
    FOR(i, 1, p) fact[i] = fact[i - 1] * i % p;
    inv_fact[p - 1] = mod_pow(fact[p - 1], p - 2, p);
    FOR_R(i, p - 1) inv_fact[i] = inv_fact[i + 1] * (i + 1) % p;
  }

  /**
   * @brief Computes nCr for 0 <= n,r < p.
   */
  I64 nCr_small(I64 n, I64 r) {
    if (r > n || r < 0) return 0;
    return fact[n] * inv_fact[r] % p * inv_fact[n - r] % p;
  }

  /**
   * @brief Computes nCr modulo p for arbitrary n and r.
   */
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

/**
 * @brief Number Theoretic Transform over MOD=998244353.
 */
struct NTT {
  static constexpr I64 MOD = 998244353;  // 2^23 * 119 + 1
  static constexpr I64 ROOT = 3;  // Primitive root of MOD.

  /**
   * @brief In-place forward/inverse NTT.
   * @param a Coefficient vector (size must be power of two).
   * @param inverse true for inverse transform.
   */
  static void ntt(Vec<I64>& a, bool inverse) {
    I32 n = sz(a);
    if (n == 1) return;

    // Bit reversal.
    FOR(i, n) {
      I32 j = 0;
      I32 x = i, y = n - 1;
      while (y > 0) {
        j <<= 1;
        j |= x & 1;
        x >>= 1;
        y >>= 1;
      }
      if (i < j) std::swap(a[i], a[j]);
    }

    // NTT computation
    for (I32 len = 2; len <= n; len <<= 1) {
      I64 w = inverse ? mod_pow(ROOT, MOD - 1 - (MOD - 1) / len, MOD)
                      : mod_pow(ROOT, (MOD - 1) / len, MOD);

      for (I32 i = 0; i < n; i += len) {
        I64 wn = 1;
        FOR(j, len / 2) {
          I64 u = a[i + j];
          I64 v = a[i + j + len / 2] * wn % MOD;
          a[i + j] = (u + v) % MOD;
          a[i + j + len / 2] = (u - v + MOD) % MOD;
          wn = wn * w % MOD;
        }
      }
    }

    if (inverse) {
      I64 n_inv = mod_pow<I64>(n, MOD - 2, MOD);
      FOR(i, n) a[i] = a[i] * n_inv % MOD;
    }
  }

  /**
   * @brief Polynomial multiplication via NTT.
   */
  static Vec<I64> multiply(Vec<I64> a, Vec<I64> b) {
    I32 result_size = sz(a) + sz(b) - 1;
    I32 n = 1;
    while (n < result_size) n <<= 1;

    a.resize(n);
    b.resize(n);

    ntt(a, false);
    ntt(b, false);

    FOR(i, n) a[i] = a[i] * b[i] % MOD;

    ntt(a, true);
    a.resize(result_size);

    return a;
  }
};

/**
 * @brief Mobius function precomputation and inversion helper.
 */
struct Mobius {
  Vec<I32> mu;
  I32 n;

  /**
   * @brief Computes mu[1..size] in O(size).
   */
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

  /**
   * @brief Mobius inversion:
   * f(n) = sum_{d|n} mu(d) * g(n/d), given g(n) = sum_{d|n} f(d).
   */
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

//===----------------------------------------------------------------------===//
