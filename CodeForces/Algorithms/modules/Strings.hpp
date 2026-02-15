#pragma once
#include "../templates/Types.hpp"
#include "../templates/Math.hpp"
#include "../templates/Macros.hpp"
#include "../templates/Constants.hpp"
#include <algorithm>

/**
 * @file Strings.hpp
 * @brief String algorithms and data structures for CP.
 */

//===----------------------------------------------------------------------===//
//========================= String Algorithms Module =========================//

/**
 * @brief Computes Z-array where z[i] is LCP(s, s[i..]).
 * @param s Input string.
 * @return Z-function values.
 *
 * Complexity: O(n).
 */
inline Vec<I32> z_algorithm(const std::string& s) {
  I32 n = sz(s);
  Vec<I32> z(n);
  if (n == 0) return z;
  z[0] = n;
  
  I32 l = 0, r = 0;
  FOR(i, 1, n) {
    if (i <= r) z[i] = _min(static_cast<I32>(r - i + 1), z[i - l]);
    while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
    if (i + z[i] - 1 > r) {
      l = i;
      r = i + z[i] - 1;
    }
  }
  
  return z;
}

/**
 * @brief KMP matcher with linear-time preprocessing and matching.
 */
struct KMP {
  std::string pattern;
  Vec<I32> failure;
  
  /**
   * @brief Builds prefix-function table for a pattern.
   */
  KMP(const std::string& p) : pattern(p) {
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
  
  /**
   * @brief Finds all pattern occurrences in text.
   * @param text Text to search.
   * @return Start indices of all matches.
   *
   * Complexity: O(|text| + |pattern|).
   */
  Vec<I32> search(const std::string& text) {
    Vec<I32> matches;
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
        matches.pb(i - m + 1);
        j = failure[j - 1];
      }
    }
    
    return matches;
  }
};

/**
 * @brief Manacher algorithm for palindromic substrings in linear time.
 */
struct Manacher {
  std::string s;
  Vec<I32> p;  // p[i] = radius of palindrome centered at 'i'.
  
  /**
   * @brief Builds transformed string and palindrome radii.
   */
  Manacher(const std::string& str) {
    // Transform string: "abc" -> "^#a#b#c#$".
    s = "^";
    for (char c : str) {
      s += "#";
      s += c;
    }
    s += "#$";
    
    I32 n = sz(s);
    p.assign(n, 0);
    
    I32 center = 0, right = 0;
    FOR(i, 1, n - 1) {
      I32 mirror = 2 * center - i;
      
      if (i < right) {
        p[i] = _min(static_cast<I32>(right - i), p[mirror]);
      }
      
      // Expand around center 'i'.
      while (s[i + p[i] + 1] == s[i - p[i] - 1]) {
        p[i]++;
      }
      
      // Update center and right.
      if (i + p[i] > right) {
        center = i;
        right = i + p[i];
      }
    }
  }
  
  /**
   * @brief Checks if substring [l, r) of original string is palindrome.
   */
  bool is_palindrome(I32 l, I32 r) {
    I32 center = l + r + 1;  // Center in transformed string.
    I32 radius = r - l;
    return p[center] >= radius;
  }
  
  /**
   * @brief Radius of longest odd palindrome centered at i.
   */
  I32 odd_palindrome_at(I32 i) {
    return p[2 * i + 2] / 2;
  }
  
  /**
   * @brief Radius of longest even palindrome centered between i and i+1.
   */
  I32 even_palindrome_at(I32 i) {
    return (p[2 * i + 3] - 1) / 2;
  }
};

/**
 * @brief Suffix array + LCP construction and pattern lookup.
 *
 * Construction uses O(n log n) doubling; LCP uses Kasai-style pass.
 */
struct SuffixArray {
  std::string s;
  Vec<I32> sa, rank, lcp;
  
  /**
   * @brief Builds suffix array for input string.
   */
  SuffixArray(const std::string& str) : s(str) {
    I32 n = sz(s);
    sa.resize(n);
    rank.resize(n);
    lcp.resize(n > 0 ? n - 1 : 0);
    if (n == 0) return;

    build_sa();
    build_lcp();
  }
  
private:
  /**
   * @brief Builds suffix array using rank-doubling.
   */
  void build_sa() {
    I32 n = sz(s);
    Vec<I32> cnt(256), pos(n), tmp(n);
    
    // Initial ranking based on first character.
    FOR(i, n) {
      rank[i] = s[i];
      sa[i] = i;
    }
    
    // Doubling algorithm.
    for (I32 gap = 1; ; gap *= 2) {
      auto cmp = [&](I32 i, I32 j) {
        if (rank[i] != rank[j]) return rank[i] < rank[j];
        i += gap;
        j += gap;
        return (i < n && j < n) ? rank[i] < rank[j] : i > j;
      };
      
      std::sort(all(sa), cmp);
      
      tmp[sa[0]] = 0;
      FOR(i, 1, n) {
        tmp[sa[i]] = tmp[sa[i - 1]] + cmp(sa[i - 1], sa[i]);
      }
      
      rank = tmp;
      if (rank[sa[n - 1]] == n - 1) break;
    }
  }
  
  /**
   * @brief Builds LCP array between adjacent suffixes in SA order.
   */
  void build_lcp() {
    I32 n = sz(s);
    I32 k = 0;
    
    FOR(i, n) {
      if (rank[i] == n - 1) {
        k = 0;
        continue;
      }
      
      I32 j = sa[rank[i] + 1];
      while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
      lcp[rank[i]] = k;
      if (k) k--;
    }
  }

  /**
   * @brief Lexicographically compares suffix with pattern.
   * @return -1 if suffix < pattern, 0 if pattern is prefix of suffix, +1 otherwise.
   */
  I32 compare_suffix(I32 suffix_pos, const std::string& pattern) const {
    I32 i = 0;
    const I32 n = sz(s);
    const I32 m = sz(pattern);

    while (suffix_pos + i < n && i < m) {
      char a = s[suffix_pos + i];
      char b = pattern[i];
      if (a != b) return a < b ? -1 : 1;
      ++i;
    }

    if (i == m) return 0;
    return -1;
  }
  
public:
  /**
   * @brief Finds all occurrences of pattern using binary search on SA.
   * @param pattern Pattern to match.
   * @return Sorted list of start positions.
   *
   * Complexity: O(m log n + occ * m_cmp_tail), with no substring allocations.
   */
  Vec<I32> find_pattern(const std::string& pattern) {
    I32 n = sz(s), m = sz(pattern);
    Vec<I32> result;
    
    // Binary search for first occurrence.
    I32 left = 0, right = n;
    while (left < right) {
      I32 mid = (left + right) / 2;
      if (compare_suffix(sa[mid], pattern) < 0) {
        left = mid + 1;
      } else {
        right = mid;
      }
    }
    
    // Collect all occurrences.
    while (left < n && compare_suffix(sa[left], pattern) == 0) {
      result.pb(sa[left]);
      left++;
    }
    
    return result;
  }
};

/**
 * @brief Double rolling hash utility for substring hashing/LCP.
 */
struct RollingHash {
  static constexpr I64 MOD1 = 1e9 + 7;
  static constexpr I64 MOD2 = 1e9 + 9;
  static constexpr I64 BASE1 = 31;
  static constexpr I64 BASE2 = 37;
  
  std::string s;
  Vec<I64> hash1, hash2, pow1, pow2;
  
  /**
   * @brief Precomputes prefix hashes and powers.
   */
  RollingHash(const std::string& str) : s(str) {
    I32 n = sz(s);
    hash1.resize(n + 1);
    hash2.resize(n + 1);
    pow1.resize(n + 1);
    pow2.resize(n + 1);
    
    pow1[0] = pow2[0] = 1;
    FOR(i, n) {
      hash1[i + 1] = (hash1[i] * BASE1 + s[i]) % MOD1;
      hash2[i + 1] = (hash2[i] * BASE2 + s[i]) % MOD2;
      pow1[i + 1] = (pow1[i] * BASE1) % MOD1;
      pow2[i + 1] = (pow2[i] * BASE2) % MOD2;
    }
  }
  
  /**
   * @brief Returns pair hash of substring [l, r).
   */
  PLL get_hash(I32 l, I32 r) {
    I64 h1 = (hash1[r] - hash1[l] * pow1[r - l] % MOD1 + MOD1) % MOD1;
    I64 h2 = (hash2[r] - hash2[l] * pow2[r - l] % MOD2 + MOD2) % MOD2;
    return {h1, h2};
  }
  
  /**
   * @brief Checks equality of two substrings in O(1).
   */
  bool equal(I32 l1, I32 r1, I32 l2, I32 r2) {
    return get_hash(l1, r1) == get_hash(l2, r2);
  }
  
  /**
   * @brief Longest common prefix of suffixes starting at i and j.
   */
  I32 lcp(I32 i, I32 j) {
    I32 left = 0, right = std::min(sz(s) - i, sz(s) - j) + 1;
    while (right - left > 1) {
      I32 mid = (left + right) / 2;
      if (equal(i, i + mid, j, j + mid)) {
        left = mid;
      } else {
        right = mid;
      }
    }
    return left;
  }
};

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

/**
 * @brief Aho-Corasick automaton for multi-pattern matching.
 *
 * Current alphabet is fixed to lowercase ['a'..'z'].
 */
struct AhoCorasick {
  static constexpr I32 ALPHABET = 26;
  static constexpr char BASE = 'a';

  /**
   * @brief Automaton node with goto transitions, suffix link and output list.
   */
  struct Node {
    std::array<I32, ALPHABET> next;
    I32 link;
    Vec<I32> out;

    Node() : link(0) {
      next.fill(-1);
    }
  };

  Vec<Node> nodes;

  /**
   * @brief Initializes empty automaton with root node.
   */
  AhoCorasick() {
    nodes.eb();
  }

  /**
   * @brief Adds one pattern to trie backbone.
   * @param pattern Lowercase pattern.
   * @param id External pattern identifier.
   */
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
    nodes[v].out.pb(id);
  }

  /**
   * @brief Builds full automaton from a pattern set.
   * @param patterns Lowercase patterns.
   */
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

  /**
   * @brief Matches text and returns output ids at each end position.
   * @param text Query text.
   * @return result[i] contains ids of patterns ending at text index i.
   */
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
        result[i].pb(id);
      }
    }

    return result;
  }
};

//===----------------------------------------------------------------------===//
