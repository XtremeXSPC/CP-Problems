#ifndef CP_MODULES_STRINGS_PREFIX_AUTOMATON_HPP
#define CP_MODULES_STRINGS_PREFIX_AUTOMATON_HPP

#include "_Common.hpp"
#include "PrefixFunction.hpp"

/**
 * @brief Prefix-function automaton (KMP automaton).
 *
 * @details Builds transitions for pattern prefixes and allows linear scan over
 * text-like streams. Useful when transition table is reused many times.
 * Complexity:
 *  - build: O(|pattern| * ALPHABET)
 *  - transition/query: O(1)
 */
template <I32 ALPHABET = 26, char BASE = 'a'>
struct PrefixAutomaton {
  static_assert(ALPHABET > 0, "ALPHABET must be positive");

  using Row = std::array<I32, as<Size>(ALPHABET)>;

  std::string pattern;
  VI pi;
  Vec<Row> next_state_table;

  PrefixAutomaton() = default;
  explicit PrefixAutomaton(const std::string& p) { build(p); }

  void build(const std::string& p) {
    pattern = p;
    pi = prefix_function(pattern);

    const I32 n = as<I32>(pattern.size());
    next_state_table.assign(as<Size>(n + 1), Row{});
    for (auto& row : next_state_table) row.fill(0);

    FOR(i, n + 1) {
      FOR(c, ALPHABET) {
        const char ch = as<char>(BASE + c);
        if (i < n && pattern[as<Size>(i)] == ch) {
          next_state_table[as<Size>(i)][as<Size>(c)] = i + 1;
        } else if (i == 0) {
          next_state_table[as<Size>(i)][as<Size>(c)] = 0;
        } else {
          next_state_table[as<Size>(i)][as<Size>(c)] = next_state_table[as<Size>(pi[as<Size>(i - 1)])][as<Size>(c)];
        }
      }
    }
  }

  /// @brief Returns next automaton state after reading ch from state.
  I32 next_state(I32 state, char ch) const {
    if (state < 0 || state >= as<I32>(next_state_table.size())) return 0;
    const I32 c = as<I32>(ch - BASE);
    if (c < 0 || c >= ALPHABET) return 0;
    return next_state_table[as<Size>(state)][as<Size>(c)];
  }

  /// @brief Returns starting positions of all pattern occurrences in text.
  VI find_occurrences(const std::string& text) const {
    VI occ;
    const I32 m = as<I32>(pattern.size());
    const I32 n = as<I32>(text.size());
    if (m == 0) {
      occ.resize(as<Size>(n + 1));
      std::iota(all(occ), 0);
      return occ;
    }

    I32 state = 0;
    FOR(i, n) {
      state = next_state(state, text[as<Size>(i)]);
      if (state == m) {
        occ.push_back(i - m + 1);
      }
    }
    return occ;
  }
};

#endif
