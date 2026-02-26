#ifndef CP_MODULES_NUMBER_THEORY_FWHT_HPP
#define CP_MODULES_NUMBER_THEORY_FWHT_HPP

#include "_Common.hpp"

/// @brief Fast Walsh-Hadamard Transform for XOR convolution.
struct FWHT {
  /// @brief In-place XOR transform. When inverse=true, divides each entry by n.
  template <typename T>
  static void xor_transform(Vec<T>& a, const bool inverse) {
    const I32 n = sz(a);
    my_assert(n > 0 && (n & (n - 1)) == 0);

    for (I32 len = 1; len < n; len <<= 1) {
      for (I32 i = 0; i < n; i += (len << 1)) {
        for (I32 j = 0; j < len; ++j) {
          const T u = a[i + j];
          const T v = a[i + j + len];
          a[i + j] = u + v;
          a[i + j + len] = u - v;
        }
      }
    }

    if (inverse) {
      for (I32 i = 0; i < n; ++i) a[i] /= n;
    }
  }

  /// @brief XOR convolution of equal-sized vectors with power-of-two size.
  template <typename T>
  static auto xor_convolution(Vec<T> lhs, Vec<T> rhs) -> Vec<T> {
    my_assert(!lhs.empty() && sz(lhs) == sz(rhs));
    my_assert((sz(lhs) & (sz(lhs) - 1)) == 0);

    xor_transform(lhs, false);
    xor_transform(rhs, false);
    FOR(i, sz(lhs)) lhs[i] *= rhs[i];
    xor_transform(lhs, true);
    return lhs;
  }
};

#endif
