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
    I32 cur = as<I32>(st.size());
    st.push_back(State{});
    st[as<Size>(cur)].len = st[as<Size>(last)].len + 1;
    st[as<Size>(cur)].occ = 1;

    I32 p = last;
    while (p != -1 && !st[as<Size>(p)].next.contains(c)) {
      st[as<Size>(p)].next[c] = cur;
      p = st[as<Size>(p)].link;
    }

    if (p == -1) {
      st[as<Size>(cur)].link = 0;
    } else {
      I32 q = st[as<Size>(p)].next[c];
      if (st[as<Size>(p)].len + 1 == st[as<Size>(q)].len) {
        st[as<Size>(cur)].link = q;
      } else {
        I32 clone = as<I32>(st.size());
        st.push_back(st[as<Size>(q)]);
        st[as<Size>(clone)].len = st[as<Size>(p)].len + 1;
        st[as<Size>(clone)].occ = 0;

        while (p != -1 && st[as<Size>(p)].next[c] == q) {
          st[as<Size>(p)].next[c] = clone;
          p = st[as<Size>(p)].link;
        }

        st[as<Size>(q)].link = clone;
        st[as<Size>(cur)].link = clone;
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
      auto it = st[as<Size>(v)].next.find(c);
      if (it == st[as<Size>(v)].next.end()) return false;
      v = it->second;
    }
    return true;
  }

  /// @brief Number of distinct substrings of the built text.
  I64 distinct_substrings() const {
    I64 ans = 0;
    FOR(v, 1, as<I32>(st.size())) {
      I32 link = st[as<Size>(v)].link;
      ans += st[as<Size>(v)].len - st[as<Size>(link)].len;
    }
    return ans;
  }

  /// @brief Prepares occurrence counts for all states in O(|S|).
  void build_occurrences() {
    I32 max_len = 0;
    for (const auto& s : st) max_len = std::max(max_len, s.len);

    VI cnt(as<Size>(max_len + 1), 0);
    for (const auto& s : st) ++cnt[as<Size>(s.len)];
    FOR(i, 1, as<I32>(cnt.size())) {
      cnt[as<Size>(i)] += cnt[as<Size>(i - 1)];
    }

    VI order(as<Size>(st.size()), 0);
    FOR(v, as<I32>(st.size())) {
      I32 len = st[as<Size>(v)].len;
      order[as<Size>(--cnt[as<Size>(len)])] = v;
    }

    FOR_R(i, as<I32>(order.size())) {
      I32 v = order[as<Size>(i)];
      I32 p = st[as<Size>(v)].link;
      if (p != -1) st[as<Size>(p)].occ += st[as<Size>(v)].occ;
    }

    occ_ready = true;
  }

  /// @brief Returns number of occurrences of pattern in the text.
  I64 occurrences(const std::string& pattern) {
    if (!occ_ready) build_occurrences();
    I32 v = 0;
    for (char c : pattern) {
      auto it = st[as<Size>(v)].next.find(c);
      if (it == st[as<Size>(v)].next.end()) return 0;
      v = it->second;
    }
    return st[as<Size>(v)].occ;
  }
};

#endif
