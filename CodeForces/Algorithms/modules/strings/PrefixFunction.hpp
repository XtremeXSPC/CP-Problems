#ifndef CP_MODULES_STRINGS_PREFIX_FUNCTION_HPP
#define CP_MODULES_STRINGS_PREFIX_FUNCTION_HPP

#include "_Common.hpp"

/**
 * @brief Prefix-function (pi-array) of a string.
 *
 * @details Computes pi[i] = length of the longest proper prefix of s
 * which is also a suffix of s[0..i]. Complexity: O(n).
 */
inline VI prefix_function(const std::string& s) {
  I32 n = as<I32>(s.size());
  VI pi(as<Size>(n), 0);
  FOR(i, 1, n) {
    I32 j = pi[as<Size>(i - 1)];
    while (j > 0 && s[as<Size>(i)] != s[as<Size>(j)]) {
      j = pi[as<Size>(j - 1)];
    }
    if (s[as<Size>(i)] == s[as<Size>(j)]) ++j;
    pi[as<Size>(i)] = j;
  }
  return pi;
}

#endif
