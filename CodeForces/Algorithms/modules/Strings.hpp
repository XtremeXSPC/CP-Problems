#pragma once
#include "../templates/Types.hpp"
#include "../templates/Math.hpp"
#include "../templates/Macros.hpp"
#include "../templates/Constants.hpp"
#include <algorithm>

//===----------------------------------------------------------------------===//
//========================= String Algorithms Module =========================//

// Z-algorithm for pattern matching.
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

// KMP (Knuth-Morris-Pratt) pattern searching.
struct KMP {
  std::string pattern;
  Vec<I32> failure;
  
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

// Manacher's algorithm for palindrome detection.
struct Manacher {
  std::string s;
  Vec<I32> p;  // p[i] = radius of palindrome centered at 'i'.
  
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
  
  // Check if substring [l, r) is palindrome.
  bool is_palindrome(I32 l, I32 r) {
    I32 center = l + r + 1;  // Center in transformed string.
    I32 radius = r - l;
    return p[center] >= radius;
  }
  
  // Get longest palindrome length centered at position 'i' (0-indexed).
  I32 odd_palindrome_at(I32 i) {
    return p[2 * i + 2] / 2;
  }
  
  I32 even_palindrome_at(I32 i) {
    return (p[2 * i + 3] - 1) / 2;
  }
};

// Suffix Array construction using O(n log n) algorithm.
struct SuffixArray {
  std::string s;
  Vec<I32> sa, rank, lcp;
  
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
  
public:
  // Find all occurrences of pattern in O(m log n).
  Vec<I32> find_pattern(const std::string& pattern) {
    I32 n = sz(s), m = sz(pattern);
    Vec<I32> result;
    
    // Binary search for first occurrence.
    I32 left = 0, right = n;
    while (left < right) {
      I32 mid = (left + right) / 2;
      if (s.substr(sa[mid], std::min(m, n - sa[mid])) < pattern) {
        left = mid + 1;
      } else {
        right = mid;
      }
    }
    
    // Collect all occurrences.
    while (left < n && s.substr(sa[left], std::min(m, n - sa[left])) == pattern) {
      result.pb(sa[left]);
      left++;
    }
    
    return result;
  }
};

// Rolling Hash for string matching.
struct RollingHash {
  static constexpr I64 MOD1 = 1e9 + 7;
  static constexpr I64 MOD2 = 1e9 + 9;
  static constexpr I64 BASE1 = 31;
  static constexpr I64 BASE2 = 37;
  
  std::string s;
  Vec<I64> hash1, hash2, pow1, pow2;
  
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
  
  // Get hash of substring [l, r).
  PLL get_hash(I32 l, I32 r) {
    I64 h1 = (hash1[r] - hash1[l] * pow1[r - l] % MOD1 + MOD1) % MOD1;
    I64 h2 = (hash2[r] - hash2[l] * pow2[r - l] % MOD2 + MOD2) % MOD2;
    return {h1, h2};
  }
  
  // Check if two substrings are equal in O(1).
  bool equal(I32 l1, I32 r1, I32 l2, I32 r2) {
    return get_hash(l1, r1) == get_hash(l2, r2);
  }
  
  // Find longest common prefix of two suffixes.
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

// Trie data structure for efficient string operations.
struct Trie {
  struct Node {
    std::unordered_map<char, I32> children;
    bool is_end = false;
    I32 count = 0;  // Number of words ending at this node.
    I32 prefix_count = 0;  // Number of words with this prefix.
  };
  
  Vec<Node> nodes;
  
  Trie() {
    nodes.eb();  // Root node.
  }
  
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
  
  bool search(const std::string& word) {
    I32 current = 0;
    for (char c : word) {
      if (!nodes[current].children.count(c)) return false;
      current = nodes[current].children[c];
    }
    return nodes[current].is_end;
  }
  
  I32 count_prefix(const std::string& prefix) {
    I32 current = 0;
    for (char c : prefix) {
      if (!nodes[current].children.count(c)) return 0;
      current = nodes[current].children[c];
    }
    return nodes[current].prefix_count;
  }
  
  // Find all words with given prefix.
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

//===----------------------------------------------------------------------===//
