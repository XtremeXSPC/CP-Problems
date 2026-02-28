#ifndef CP_MODULES_STRINGS_KMP_HPP
#define CP_MODULES_STRINGS_KMP_HPP

#include "_Common.hpp"

/// @brief KMP matcher with linear-time preprocessing and matching.
struct KMP {
  String pattern;
  VecI32 failure;

  /// @brief Builds prefix-function table for a pattern.
  KMP(const String& p) : pattern(p) {
    I32 m = sz(pattern);
    failure.assign(m, 0);

    FOR(i, 1, m) {
      I32 j = failure[i - 1];
      while (j > 0 && pattern[i] != pattern[j]) {
        j = failure[j - 1];
      }
      if (pattern[i] == pattern[j]) j++;
      failure[i] = j;
    }
  }

  /// @brief Finds all pattern occurrences in text.
  VecI32 search(const String& text) const {
    VecI32 matches;
    I32 n = sz(text), m = sz(pattern);
    if (m == 0) {
      matches.resize(n + 1);
      std::iota(all(matches), 0);
      return matches;
    }
    I32 j = 0;

    FOR(i, n) {
      while (j > 0 && text[i] != pattern[j]) {
        j = failure[j - 1];
      }
      if (text[i] == pattern[j]) j++;

      if (j == m) {
        matches.push_back(i - m + 1);
        j = failure[j - 1];
      }
    }

    return matches;
  }
};

#endif
