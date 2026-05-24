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

  explicit SuffixAutomaton(const String& s) {
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
    I32 cur = isz(st);
    st.push_back(State{});
    st[cur].len = st[last].len + 1;
    st[cur].occ = 1;

    I32 p = last;
    while (p != -1 && !st[p].next.contains(c)) {
      st[p].next[c] = cur;
      p = st[p].link;
    }

    if (p == -1) {
      st[cur].link = 0;
    } else {
      I32 q = st[p].next[c];
      if (st[p].len + 1 == st[q].len) {
        st[cur].link = q;
      } else {
        I32 clone = isz(st);
        st.push_back(st[q]);
        st[clone].len = st[p].len + 1;
        st[clone].occ = 0;

        while (p != -1 && st[p].next[c] == q) {
          st[p].next[c] = clone;
          p = st[p].link;
        }

        st[q].link = clone;
        st[cur].link = clone;
      }
    }

    last = cur;
    occ_ready = false;
  }

  /// @brief Builds SAM from whole string.
  void build(const String& s) {
    for (char c : s) extend(c);
  }

  /// @brief Checks if pattern is a substring of the built text.
  bool contains(const String& pattern) const {
    I32 v = 0;
    for (char c : pattern) {
      auto it = st[v].next.find(c);
      if (it == st[v].next.end()) return false;
      v = it->second;
    }
    return true;
  }

  /// @brief Number of distinct substrings of the built text.
  I64 distinct_substrings() const {
    I64 ans = 0;
    FOR(v, 1, isz(st)) {
      I32 link = st[v].link;
      ans += st[v].len - st[link].len;
    }
    return ans;
  }

  /// @brief Prepares occurrence counts for all states in O(|S|).
  void build_occurrences() {
    I32 max_len = 0;
    for (const auto& s : st) max_len = std::max(max_len, s.len);

    VecI32 cnt((max_len + 1), 0);
    for (const auto& s : st) ++cnt[s.len];
    FOR(i, 1, isz(cnt)) {
      cnt[i] += cnt[i - 1];
    }

    VecI32 order(st.size(), 0);
    FOR(v, isz(st)) {
      I32 len = st[v].len;
      order[--cnt[len]] = v;
    }

    FOR_R(i, isz(order)) {
      I32 v = order[i];
      I32 p = st[v].link;
      if (p != -1) st[p].occ += st[v].occ;
    }

    occ_ready = true;
  }

  /// @brief Returns number of occurrences of pattern in the text.
  I64 occurrences(const String& pattern) {
    if (!occ_ready) build_occurrences();
    I32 v = 0;
    for (char c : pattern) {
      auto it = st[v].next.find(c);
      if (it == st[v].next.end()) return 0;
      v = it->second;
    }
    return st[v].occ;
  }
};

#endif
