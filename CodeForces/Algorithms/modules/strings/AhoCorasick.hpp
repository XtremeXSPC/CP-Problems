#ifndef CP_MODULES_STRINGS_AHO_CORASICK_HPP
#define CP_MODULES_STRINGS_AHO_CORASICK_HPP

#include "_Common.hpp"

/**
 * @brief Aho-Corasick automaton for multi-pattern matching.
 *
 * @details Current alphabet is fixed to lowercase ['a'..'z'].
 */
struct AhoCorasick {
  static constexpr I32 ALPHABET = 26;
  static constexpr char BASE = 'a';

  /// @brief Automaton node with goto transitions, suffix link and output list.
  struct Node {
    std::array<I32, ALPHABET> next;
    I32 link;
    Vec<I32> out;

    Node() : link(0) {
      next.fill(-1);
    }
  };

  Vec<Node> nodes;

  /// @brief Initializes empty automaton with root node.
  AhoCorasick() {
    nodes.eb();
  }

  /// @brief Adds one pattern to trie backbone.
  void add_pattern(const std::string& pattern, I32 id) {
    I32 v = 0;
    for (char ch : pattern) {
      I32 c = ch - BASE;
      if (c < 0 || c >= ALPHABET) {
        my_assert(false && "AhoCorasick supports lowercase ['a'..'z'] patterns.");
        return;
      }
      if (nodes[v].next[c] == -1) {
        nodes[v].next[c] = sz(nodes);
        nodes.eb();
      }
      v = nodes[v].next[c];
    }
    nodes[v].out.push_back(id);
  }

  /// @brief Builds full automaton from a pattern set.
  void build(const Vec<std::string>& patterns) {
    nodes.assign(1, Node{});
    FOR(i, sz(patterns)) add_pattern(patterns[i], i);

    Queue<I32> q;
    FOR(c, ALPHABET) {
      I32 to = nodes[0].next[c];
      if (to != -1) {
        nodes[to].link = 0;
        q.push(to);
      } else {
        nodes[0].next[c] = 0;
      }
    }

    while (!q.empty()) {
      I32 v = q.front();
      q.pop();

      FOR(c, ALPHABET) {
        I32 to = nodes[v].next[c];
        if (to != -1) {
          nodes[to].link = nodes[nodes[v].link].next[c];
          const auto& fail_out = nodes[nodes[to].link].out;
          nodes[to].out.insert(nodes[to].out.end(), all(fail_out));
          q.push(to);
        } else {
          nodes[v].next[c] = nodes[nodes[v].link].next[c];
        }
      }
    }
  }

  /// @brief Matches text and returns output ids at each end position.
  Vec<Vec<I32>> match(const std::string& text) const {
    Vec<Vec<I32>> result(sz(text));
    I32 v = 0;

    FOR(i, sz(text)) {
      char ch = text[i];
      I32 c = ch - BASE;
      if (c < 0 || c >= ALPHABET) {
        v = 0;
        continue;
      }
      v = nodes[v].next[c];
      for (I32 id : nodes[v].out) {
        result[i].push_back(id);
      }
    }

    return result;
  }
};

#endif
