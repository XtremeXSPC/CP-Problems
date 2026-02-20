#ifndef CP_MODULES_NUMBER_THEORY_TONELLI_SHANKS_HPP
#define CP_MODULES_NUMBER_THEORY_TONELLI_SHANKS_HPP

#include "_Common.hpp"
#include "Primality64.hpp"

/// @brief Tonelli-Shanks algorithm for modular square roots.
inline I64 tonelli_shanks(I64 n, I64 p) {
  n %= p;
  if (n < 0) n += p;
  if (n == 0) return 0;
  if (p == 2) return n & 1;

  // Check if n is a quadratic residue via Euler's criterion.
  if (mod_pow64(n, (p - 1) / 2, p) != 1) return -1;

  // p = 3 (mod 4) shortcut.
  if (p % 4 == 3) {
    I64 x = mod_pow64(n, (p + 1) / 4, p);
    return std::min(x, p - x);
  }

  // Factor out powers of 2: p - 1 = Q * 2^S.
  I64 S = 0, Q = p - 1;
  while (Q % 2 == 0) { Q /= 2; ++S; }

  // Find a quadratic non-residue z.
  I64 z = 2;
  while (mod_pow64(z, (p - 1) / 2, p) != p - 1) ++z;

  I64 M = S;
  I64 c = mod_pow64(z, Q, p);
  I64 t = mod_pow64(n, Q, p);
  I64 R = mod_pow64(n, (Q + 1) / 2, p);

  while (true) {
    if (t == 1) return std::min(R, p - R);

    // Find least i such that t^(2^i) = 1 (mod p).
    I64 i = 0;
    I64 tmp = t;
    while (tmp != 1) { tmp = mod_mul64(tmp, tmp, p); ++i; }

    // Update.
    I64 b = c;
    FOR(j, M - i - 1) b = mod_mul64(b, b, p);
    M = i;
    c = mod_mul64(b, b, p);
    t = mod_mul64(t, c, p);
    R = mod_mul64(R, b, p);
  }
}

#endif
