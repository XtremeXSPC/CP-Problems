#pragma once
#include "Macros.hpp"
#ifdef CP_USE_ADVANCED
#include "templates/advanced/Concepts.hpp"
#endif

//===----------------------------------------------------------------------===//
/* Random Utilities */

inline U64 default_rng_seed() {
#ifdef CP_SEED
  return static_cast<U64>(CP_SEED);
#else
  return as<U64>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
#endif
}

inline std::mt19937_64 rng(default_rng_seed());

inline void reseed(U64 seed) { rng.seed(seed); }

#ifdef CP_USE_ADVANCED

template <cp::Integral T>
inline T rnd(T a, T b) { return std::uniform_int_distribution<T>(a, b)(rng); }

template <cp::Floating T>
inline T rnd(T a, T b) { return std::uniform_real_distribution<T>(a, b)(rng); }

#else

template <class T>
inline T rnd(T a, T b) {
  if constexpr (std::is_floating_point_v<T>) {
    return std::uniform_real_distribution<T>(a, b)(rng);
  } else {
    return std::uniform_int_distribution<T>(a, b)(rng);
  }
}

#endif
