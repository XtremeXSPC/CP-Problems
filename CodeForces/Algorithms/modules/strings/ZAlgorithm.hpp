#ifndef CP_MODULES_STRINGS_Z_ALGORITHM_HPP
#define CP_MODULES_STRINGS_Z_ALGORITHM_HPP

#include "_Common.hpp"

/**
 * @brief Computes Z-array where z[i] is LCP(s, s[i..]).
 * @param s Input string.
 * @return Z-function values.
 *
 * Complexity: O(n).
 */
inline Vec<I32> z_algorithm(const std::string& s) {
  I32 n = sz(s);
  Vec<I32> z(n);
  if (n == 0) return z;
  z[0] = n;

  I32 l = 0, r = 0;
  FOR(i, 1, n) {
    if (i <= r) z[i] = _min(static_cast<I32>(r - i + 1), z[i - l]);
    while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
    if (i + z[i] - 1 > r) {
      l = i;
      r = i + z[i] - 1;
    }
  }

  return z;
}

#endif
