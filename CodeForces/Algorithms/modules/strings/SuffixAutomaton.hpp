#ifndef CP_MODULES_STRINGS_SUFFIX_AUTOMATON_HPP
#define CP_MODULES_STRINGS_SUFFIX_AUTOMATON_HPP

#include "_Common.hpp"

/**
 * @brief Suffix Automaton (SAM) for a string.
 *
 * @details Supports:
 *  - substring existence query in O(|t|)
 *  - number of distinct substrings in O(|S|)
 *  - occurrence count for a pattern after one O(|S|) preparation
 */
struct SuffixAutomaton {
  struct State {
    I32 link = -1;
    I32 len = 0;
    UnorderedMap<char, I32> next;
    I64 occ = 0;
  };

  Vec<State> st;
  I32 last = 0;
  bool occ_ready = false;

  SuffixAutomaton() { reset(); }

  explicit SuffixAutomaton(const std::string& s) {
    reset();
    build(s);
  }

  void reset() {
    st.clear();
    st.push_back(State{});
    last = 0;
    occ_ready = false;
  }

  /// @brief Appends one character to the automaton.
  void extend(char c) {
    I32 cur = static_cast<I32>(st.size());
    st.push_back(State{});
    st[static_cast<Size>(cur)].len = st[static_cast<Size>(last)].len + 1;
    st[static_cast<Size>(cur)].occ = 1;

    I32 p = last;
    while (p != -1 && !st[static_cast<Size>(p)].next.contains(c)) {
      st[static_cast<Size>(p)].next[c] = cur;
      p = st[static_cast<Size>(p)].link;
    }

    if (p == -1) {
      st[static_cast<Size>(cur)].link = 0;
    } else {
      I32 q = st[static_cast<Size>(p)].next[c];
      if (st[static_cast<Size>(p)].len + 1 == st[static_cast<Size>(q)].len) {
        st[static_cast<Size>(cur)].link = q;
      } else {
        I32 clone = static_cast<I32>(st.size());
        st.push_back(st[static_cast<Size>(q)]);
        st[static_cast<Size>(clone)].len = st[static_cast<Size>(p)].len + 1;
        st[static_cast<Size>(clone)].occ = 0;

        while (p != -1 && st[static_cast<Size>(p)].next[c] == q) {
          st[static_cast<Size>(p)].next[c] = clone;
          p = st[static_cast<Size>(p)].link;
        }

        st[static_cast<Size>(q)].link = clone;
        st[static_cast<Size>(cur)].link = clone;
      }
    }

    last = cur;
    occ_ready = false;
  }

  /// @brief Builds SAM from whole string.
  void build(const std::string& s) {
    for (char c : s) extend(c);
  }

  /// @brief Checks if pattern is a substring of the built text.
  bool contains(const std::string& pattern) const {
    I32 v = 0;
    for (char c : pattern) {
      auto it = st[static_cast<Size>(v)].next.find(c);
      if (it == st[static_cast<Size>(v)].next.end()) return false;
      v = it->second;
    }
    return true;
  }

  /// @brief Number of distinct substrings of the built text.
  I64 distinct_substrings() const {
    I64 ans = 0;
    FOR(v, 1, static_cast<I32>(st.size())) {
      I32 link = st[static_cast<Size>(v)].link;
      ans += st[static_cast<Size>(v)].len - st[static_cast<Size>(link)].len;
    }
    return ans;
  }

  /// @brief Prepares occurrence counts for all states in O(|S|).
  void build_occurrences() {
    I32 max_len = 0;
    for (const auto& s : st) max_len = std::max(max_len, s.len);

    VI cnt(static_cast<Size>(max_len + 1), 0);
    for (const auto& s : st) ++cnt[static_cast<Size>(s.len)];
    FOR(i, 1, static_cast<I32>(cnt.size())) {
      cnt[static_cast<Size>(i)] += cnt[static_cast<Size>(i - 1)];
    }

    VI order(static_cast<Size>(st.size()), 0);
    FOR(v, static_cast<I32>(st.size())) {
      I32 len = st[static_cast<Size>(v)].len;
      order[static_cast<Size>(--cnt[static_cast<Size>(len)])] = v;
    }

    FOR_R(i, static_cast<I32>(order.size())) {
      I32 v = order[static_cast<Size>(i)];
      I32 p = st[static_cast<Size>(v)].link;
      if (p != -1) st[static_cast<Size>(p)].occ += st[static_cast<Size>(v)].occ;
    }

    occ_ready = true;
  }

  /// @brief Returns number of occurrences of pattern in the text.
  I64 occurrences(const std::string& pattern) {
    if (!occ_ready) build_occurrences();
    I32 v = 0;
    for (char c : pattern) {
      auto it = st[static_cast<Size>(v)].next.find(c);
      if (it == st[static_cast<Size>(v)].next.end()) return 0;
      v = it->second;
    }
    return st[static_cast<Size>(v)].occ;
  }
};

#endif
