#ifndef CP_MODULES_NUMBER_THEORY_PRIMITIVE_ROOT_HPP
#define CP_MODULES_NUMBER_THEORY_PRIMITIVE_ROOT_HPP

#include "_Common.hpp"
#include "PollardRho.hpp"

/// @brief Finds a primitive root modulo prime p.
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

#endif
