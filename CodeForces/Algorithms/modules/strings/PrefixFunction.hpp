#ifndef CP_MODULES_STRINGS_PREFIX_FUNCTION_HPP
#define CP_MODULES_STRINGS_PREFIX_FUNCTION_HPP

#include "_Common.hpp"

/**
 * @brief Prefix-function (pi-array) of a string.
 *
 * @details Computes pi[i] = length of the longest proper prefix of s
 * which is also a suffix of s[0..i]. Complexity: O(n).
 */
inline VecI32 prefix_function(const String& s) {
  I32 n = isz(s);
  VecI32 pi(n, 0);
  FOR(i, 1, n) {
    I32 j = pi[i - 1];
    while (j > 0 && s[i] != s[j]) {
      j = pi[j - 1];
    }
    if (s[i] == s[j]) ++j;
    pi[i] = j;
  }
  return pi;
}

#endif
