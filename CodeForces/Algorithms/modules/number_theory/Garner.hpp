#ifndef CP_MODULES_NUMBER_THEORY_GARNER_HPP
#define CP_MODULES_NUMBER_THEORY_GARNER_HPP

#include "_Common.hpp"
#include "Euclid.hpp"

/**
 * @brief Garner algorithm (mixed radix CRT) for pairwise-coprime moduli.
 *
 * @details Reconstructs x from x ≡ residues[i] (mod moduli[i]) and returns
 * x modulo @p result_mod. Returns -1 when input is invalid or reconstruction
 * prerequisites are not satisfied.
 */
inline I64 garner(const Vec<I64>& residues, const Vec<I64>& moduli, I64 result_mod) {
  if (residues.size() != moduli.size()) return -1;
  if (result_mod <= 0) return -1;
  if (residues.empty()) return 0;

  I32 k = static_cast<I32>(residues.size());
  FOR(i, k) {
    if (moduli[i] <= 1) return -1;
  }
  FOR(i, k) {
    FOR(j, i + 1, k) {
      if (std::gcd(moduli[i], moduli[j]) != 1) return -1;
    }
  }

  auto norm_mod = [](I64 x, I64 mod) {
    x %= mod;
    if (x < 0) x += mod;
    return x;
  };

  Vec<I64> mods = moduli;
  mods.push_back(result_mod);

  Vec<I64> coeffs(static_cast<Size>(k + 1), 1);
  Vec<I64> constants(static_cast<Size>(k + 1), 0);

  FOR(i, k) {
    I64 mi = mods[i];
    I64 ai = norm_mod(residues[i], mi);
    I64 coeff_i = norm_mod(coeffs[i], mi);
    I64 inv = mod_inverse<I64>(coeff_i, mi);
    if (inv == -1) return -1;

    I64 delta = norm_mod(ai - constants[i], mi);
    I64 t = static_cast<I64>((static_cast<__int128>(delta) * inv) % mi);

    FOR(j, i + 1, k + 1) {
      I64 mj = mods[j];
      constants[j] = static_cast<I64>(
          (static_cast<__int128>(constants[j]) +
           static_cast<__int128>(coeffs[j]) * t) %
          mj);
      coeffs[j] =
          static_cast<I64>((static_cast<__int128>(coeffs[j]) * mi) % mj);
    }
  }

  return constants[k];
}

#endif
