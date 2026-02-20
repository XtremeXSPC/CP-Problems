#ifndef CP_MODULES_STRINGS_AHO_CORASICK_ADVANCED_HPP
#define CP_MODULES_STRINGS_AHO_CORASICK_ADVANCED_HPP

#include "_Common.hpp"

/**
 * @brief Advanced Aho-Corasick for lowercase alphabet.
 *
 * @details Supports:
 *  - adding patterns incrementally
 *  - collecting all matches (position, pattern_id)
 *  - counting occurrences of each pattern in O(|text| + states + matches)
 */
struct AhoCorasickAdvanced {
  static constexpr I32 ALPHABET = 26;
  static constexpr char BASE = 'a';

  struct Node {
    std::array<I32, ALPHABET> next{};
    I32 link = 0;
    I32 parent = -1;
    char parent_char = 0;
    Vec<I32> out;

    Node() { next.fill(-1); }
  };

  Vec<Node> nodes;
  Vec<I32> pattern_len;
  bool built = false;
  VI bfs_order;

  AhoCorasickAdvanced() { reset(); }

  void reset() {
    nodes.assign(1, Node{});
    pattern_len.clear();
    built = false;
    bfs_order.clear();
  }

  /// @brief Adds one pattern and returns its id.
  I32 add_pattern(const std::string& pattern) {
    I32 id = static_cast<I32>(pattern_len.size());
    pattern_len.push_back(static_cast<I32>(pattern.size()));

    I32 v = 0;
    for (char ch : pattern) {
      I32 c = ch - BASE;
      if (c < 0 || c >= ALPHABET) {
        #ifndef NDEBUG
          assert(false && "AhoCorasickAdvanced supports only ['a'..'z']");
        #endif
        return id;
      }
      if (nodes[static_cast<Size>(v)].next[static_cast<Size>(c)] == -1) {
        nodes[static_cast<Size>(v)].next[static_cast<Size>(c)] = static_cast<I32>(nodes.size());
        nodes.push_back(Node{});
        nodes.back().parent = v;
        nodes.back().parent_char = ch;
      }
      v = nodes[static_cast<Size>(v)].next[static_cast<Size>(c)];
    }
    nodes[static_cast<Size>(v)].out.push_back(id);
    built = false;
    return id;
  }

  void build() {
    Queue<I32> q;
    bfs_order.clear();
    bfs_order.reserve(nodes.size());

    FOR(c, ALPHABET) {
      I32 to = nodes[0].next[static_cast<Size>(c)];
      if (to == -1) {
        nodes[0].next[static_cast<Size>(c)] = 0;
      } else {
        nodes[static_cast<Size>(to)].link = 0;
        q.push(to);
      }
    }

    while (!q.empty()) {
      I32 v = q.front();
      q.pop();
      bfs_order.push_back(v);

      FOR(c, ALPHABET) {
        I32 to = nodes[static_cast<Size>(v)].next[static_cast<Size>(c)];
        if (to == -1) {
          nodes[static_cast<Size>(v)].next[static_cast<Size>(c)] =
            nodes[static_cast<Size>(nodes[static_cast<Size>(v)].link)]
              .next[static_cast<Size>(c)];
          continue;
        }

        I32 link_to = nodes[static_cast<Size>(nodes[static_cast<Size>(v)].link)].next[static_cast<Size>(c)];
        nodes[static_cast<Size>(to)].link = link_to;
        const auto& fail_out = nodes[static_cast<Size>(link_to)].out;
        nodes[static_cast<Size>(to)].out.insert(nodes[static_cast<Size>(to)].out.end(), all(fail_out));
        q.push(to);
      }
    }

    built = true;
  }

  /// @brief Returns all matches as {end_position, pattern_id}.
  Vec<PII> all_matches(const std::string& text) {
    if (!built) build();
    Vec<PII> matches;
    I32 v = 0;
    FOR(i, static_cast<I32>(text.size())) {
      char ch = text[static_cast<Size>(i)];
      I32 c = ch - BASE;
      if (c < 0 || c >= ALPHABET) {
        v = 0;
        continue;
      }
      v = nodes[static_cast<Size>(v)].next[static_cast<Size>(c)];
      for (I32 id : nodes[static_cast<Size>(v)].out) {
        matches.push_back({i, id});
      }
    }
    return matches;
  }

  /// @brief Counts pattern occurrences in text.
  Vec<I64> count_occurrences(const std::string& text) {
    if (!built) build();
    Vec<I64> ans(pattern_len.size(), 0);
    I32 v = 0;
    for (char ch : text) {
      I32 c = ch - BASE;
      if (c < 0 || c >= ALPHABET) {
        v = 0;
        continue;
      }
      v = nodes[static_cast<Size>(v)].next[static_cast<Size>(c)];
      for (I32 id : nodes[static_cast<Size>(v)].out) {
        ++ans[static_cast<Size>(id)];
      }
    }
    return ans;
  }
};

#endif
