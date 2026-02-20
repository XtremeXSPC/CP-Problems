#ifndef CP_MODULES_NUMBER_THEORY_NTT_HPP
#define CP_MODULES_NUMBER_THEORY_NTT_HPP

#include "_Common.hpp"

/// @brief Number Theoretic Transform over MOD=998244353.
struct NTT {
  static constexpr I64 MOD = 998244353;  // 2^23 * 119 + 1
  static constexpr I64 ROOT = 3;  // Primitive root of MOD.

  /// @brief In-place NTT on vector a. If inverse=true, computes inverse transform.
  static void ntt(Vec<I64>& a, bool inverse) {
    I32 n = sz(a);
    if (n <= 1) return;

    // Bit reversal (iterative Gray-code style).
    for (I32 i = 1, j = 0; i < n; ++i) {
      I32 bit = n >> 1;
      while (j & bit) {
        j ^= bit;
        bit >>= 1;
      }
      j ^= bit;
      if (i < j) std::swap(a[i], a[j]);
    }

    // NTT computation.
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

  /// @brief Polynomial multiplication via NTT.
  static Vec<I64> multiply(Vec<I64> a, Vec<I64> b) {
    if (a.empty() || b.empty()) return {};
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

#endif
