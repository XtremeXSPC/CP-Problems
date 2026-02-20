#ifndef CP_MODULES_STRINGS_TRIE_HPP
#define CP_MODULES_STRINGS_TRIE_HPP

#include "_Common.hpp"

/**
 * @brief Trie with multiplicity and prefix-frequency support.
 */
struct Trie {
  /**
   * @brief Trie node keyed by character transitions.
   */
  struct Node {
    std::unordered_map<char, I32> children;
    bool is_end = false;
    I32 count = 0;  // Number of words ending at this node.
    I32 prefix_count = 0;  // Number of words with this prefix.
  };

  Vec<Node> nodes;

  /**
   * @brief Initializes trie with one root node.
   */
  Trie() {
    nodes.eb();  // Root node.
  }

  /**
   * @brief Inserts one word occurrence.
   */
  void insert(const std::string& word) {
    I32 current = 0;
    for (char c : word) {
      nodes[current].prefix_count++;
      if (!nodes[current].children.count(c)) {
        nodes[current].children[c] = sz(nodes);
        nodes.eb();
      }
      current = nodes[current].children[c];
    }
    nodes[current].prefix_count++;
    nodes[current].is_end = true;
    nodes[current].count++;
  }

  /**
   * @brief Checks exact word existence.
   */
  bool search(const std::string& word) {
    I32 current = 0;
    for (char c : word) {
      if (!nodes[current].children.count(c)) return false;
      current = nodes[current].children[c];
    }
    return nodes[current].is_end;
  }

  /**
   * @brief Counts inserted words sharing given prefix.
   */
  I32 count_prefix(const std::string& prefix) {
    I32 current = 0;
    for (char c : prefix) {
      if (!nodes[current].children.count(c)) return 0;
      current = nodes[current].children[c];
    }
    return nodes[current].prefix_count;
  }

  /**
   * @brief Enumerates all stored words with given prefix.
   * @return Words repeated according to multiplicity.
   */
  Vec<std::string> find_with_prefix(const std::string& prefix) {
    Vec<std::string> result;
    I32 current = 0;

    for (char c : prefix) {
      if (!nodes[current].children.count(c)) return result;
      current = nodes[current].children[c];
    }

    std::function<void(I32, std::string)> dfs = [&](I32 node, std::string word) {
      if (nodes[node].is_end) {
        FOR(nodes[node].count) result.pb(word);
      }
      for (auto [c, child] : nodes[node].children) {
        dfs(child, word + c);
      }
    };

    dfs(current, prefix);
    return result;
  }
};

#endif
