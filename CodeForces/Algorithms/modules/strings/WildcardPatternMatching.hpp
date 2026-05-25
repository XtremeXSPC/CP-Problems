#ifndef CP_MODULES_STRINGS_WILDCARD_PATTERN_MATCHING_HPP
#define CP_MODULES_STRINGS_WILDCARD_PATTERN_MATCHING_HPP

#include "_Common.hpp"
#include "modules/number_theory/NTT.hpp"

/**
 * @brief Wildcard-aware pattern matching for '?' single-character wildcards.
 *
 * @details Returns all alignments where text and pattern are compatible, treating
 * the wildcard as matching any one character on either side. The implementation
 * uses two independent randomized convolution passes, giving negligible
 * collision probability in practice.
 */
struct WildcardPatternMatching {
  /// @brief Boolean table of valid pattern alignments in @p text.
  static Vec<char> match_table(const String& text, const String& pattern, char wildcard = '?') {
    const I32 n = sz(text);
    const I32 m = sz(pattern);
    if (m == 0) return Vec<char>(n + 1, true);
    if (m > n) return {};
    Vec<char> ok = match_table_with_shift(text, pattern, wildcard, rnd<I64>(1, MOD2 - 1));
    const Vec<char> confirm = match_table_with_shift(text, pattern, wildcard, rnd<I64>(1, MOD2 - 1));
    FOR(i, sz(ok)) ok[i] = ok[i] && confirm[i];
    return ok;
  }

  /// @brief Start positions of all valid pattern occurrences in @p text.
  static VecI32 search(const String& text, const String& pattern, char wildcard = '?') {
    const Vec<char> ok = match_table(text, pattern, wildcard);
    VecI32 pos;
    FOR(i, sz(ok)) {
      if (ok[i]) pos.eb(i);
    }
    return pos;
  }

 private:
  /// @brief One randomized convolution pass for wildcard-compatible alignments.
  static Vec<char> match_table_with_shift(
    const String& text,
    const String& pattern,
    char wildcard,
    I64 shift
  ) {
    const I32 n = sz(text);
    const I32 m = sz(pattern);
    Vec<I64> a1(n), a2(n), a3(n);
    Vec<I64> b1(m), b2(m), b3(m);

    FOR(i, n) {
      const I64 x = lift(text[i], wildcard, shift);
      a1[i] = x;
      a2[i] = x * x % MOD2;
      a3[i] = a2[i] * x % MOD2;
    }

    FOR(i, m) {
      const I64 y = lift(pattern[m - 1 - i], wildcard, shift);
      b1[i] = y;
      b2[i] = y * y % MOD2;
      b3[i] = b2[i] * y % MOD2;
    }

    const Vec<I64> c13 = NTT::multiply(a1, b3);
    const Vec<I64> c22 = NTT::multiply(a2, b2);
    const Vec<I64> c31 = NTT::multiply(a3, b1);

    Vec<char> ok(n - m + 1, false);
    FOR(i, sz(ok)) {
      const I32 k = i + m - 1;
      I64 val = c13[k] - 2 * c22[k] + c31[k];
      val %= MOD2;
      if (val < 0) val += MOD2;
      ok[i] = (val == 0);
    }
    return ok;
  }

  static I64 lift(char c, char wildcard, I64 shift) {
    if (c == wildcard) return 0;
    return (I64(U8(c)) + 1 + shift) % MOD2;
  }
};

#endif
