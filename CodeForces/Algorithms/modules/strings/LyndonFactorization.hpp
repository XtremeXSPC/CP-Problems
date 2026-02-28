#ifndef CP_MODULES_STRINGS_LYNDON_FACTORIZATION_HPP
#define CP_MODULES_STRINGS_LYNDON_FACTORIZATION_HPP

#include "_Common.hpp"

/**
 * @brief Duval algorithm for Lyndon factorization and minimal cyclic shift.
 *
 * @details
 *  - Lyndon factorization in O(n)
 *  - minimal cyclic shift index in O(n)
 */

/// @brief Returns factor ranges [l, r) of Lyndon factorization.
inline VecPairI32 lyndon_factor_ranges(const String& s) {
  VecPairI32 ranges;
  const I32 n = as<I32>(s.size());
  I32 i = 0;
  while (i < n) {
    I32 j = i + 1;
    I32 k = i;
    while (j < n && s[as<Size>(k)] <= s[as<Size>(j)]) {
      if (s[as<Size>(k)] < s[as<Size>(j)]) {
        k = i;
      } else {
        ++k;
      }
      ++j;
    }
    const I32 len = j - k;
    while (i <= k) {
      ranges.push_back({i, i + len});
      i += len;
    }
  }
  return ranges;
}

/// @brief Returns Lyndon factors as substrings.
inline VecStr lyndon_factorization(const String& s) {
  auto ranges = lyndon_factor_ranges(s);
  VecStr factors;
  factors.reserve(ranges.size());
  for (const auto& [l, r] : ranges) {
    factors.push_back(s.substr(as<Size>(l), as<Size>(r - l)));
  }
  return factors;
}

/// @brief Returns index of lexicographically minimal cyclic shift.
inline I32 minimal_cyclic_shift_index(const String& s) {
  const I32 n = as<I32>(s.size());
  if (n == 0) return 0;

  String ss = s + s;
  I32 i = 0;
  I32 ans = 0;
  while (i < n) {
    ans = i;
    I32 j = i + 1;
    I32 k = i;
    while (j < 2 * n && ss[as<Size>(k)] <= ss[as<Size>(j)]) {
      if (ss[as<Size>(k)] < ss[as<Size>(j)]) {
        k = i;
      } else {
        ++k;
      }
      ++j;
    }
    while (i <= k) i += j - k;
  }
  return ans;
}

/// @brief Returns lexicographically minimal cyclic shift string.
inline String minimal_cyclic_shift(const String& s) {
  if (s.empty()) return {};
  const I32 idx = minimal_cyclic_shift_index(s);
  return s.substr(as<Size>(idx)) + s.substr(0, as<Size>(idx));
}

#endif
