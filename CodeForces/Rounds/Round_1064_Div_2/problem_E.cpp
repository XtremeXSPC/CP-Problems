//===----------------------------------------------------------------------===//
/**
 * @file: problem_E.cpp
 * @brief Codeforces Round 1064 (Div. 2) - Problem E
 * @author: Costantino Lombardi
 *
 * @status: PASSED
 */
//===----------------------------------------------------------------------===//
/* Included library */

// clang-format off
// Compiler optimizations:
#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC optimize("Ofast,unroll-loops,fast-math,O3")
  // Apple Silicon optimizations:
  #ifdef __aarch64__
    #pragma GCC target("+simd")
  #endif
#endif

#ifdef __clang__
  #pragma clang optimize on
#endif

// Sanitaze macro:
#ifdef USE_CLANG_SANITIZE
  #include "PCH.h"
#else
  #include <bits/stdc++.h>
#endif

// Debug macro:
#ifdef LOCAL
  #include "debug.h"
#else
  #define debug(...) 42
#endif
// clang-format on

//===----------------------------------------------------------------------===//
/* Type Aliases and Constants */

// Type aliases:
using ll       = long long;
using ull      = unsigned long long;
using ld       = long double;
using P_ii     = std::pair<int, int>;
using P_ll     = std::pair<long long, long long>;
using V_b      = std::vector<bool>;
using V_i      = std::vector<int>;
using V_ui     = std::vector<unsigned int>;
using V_ll     = std::vector<long long>;
using VV_i     = std::vector<std::vector<int>>;
using VV_ll    = std::vector<std::vector<long long>>;
using V_s      = std::vector<std::string>;
using VV_s     = std::vector<std::vector<std::string>>;
using VP_ii    = std::vector<std::pair<int, int>>;
using VVP_ii   = std::vector<std::vector<std::pair<int, int>>>;
using VP_ll    = std::vector<std::pair<long long, long long>>;
using VT_iii   = std::vector<std::tuple<int, int, int>>;
using Map_ll   = std::map<long long, long long>;
using VUMap_il = std::vector<std::unordered_map<int, ll>>;

// Mathematical constants:
constexpr long double PI   = 3.141592653589793238462643383279502884L;
constexpr long double E    = 2.718281828459045235360287471352662498L;
constexpr long double EPS  = 1e-9L;
constexpr int         INF  = 0x3f3f3f3f;
constexpr long long   LINF = 0x3f3f3f3f3f3f3f3fLL;
constexpr int         LIM  = 1000000 + 5;
constexpr int         MOD  = 1000000007;
constexpr int         MOD2 = 998244353;

// Problem Specific Constants:
const int MAX_BITS = 30;

using namespace std;

//===----------------------------------------------------------------------===//
/* Data Types and Function Definitions */

struct TrieNode {
  int children[2];
  int count;

  TrieNode() {
    children[0] = children[1] = -1;
    count                     = 0;
  }
};

// Wrapper to manage the node vector.
struct Trie {
  vector<TrieNode> nodes;

  Trie() {
    // Pre-allocation: ~35 numbers * 30 bits = ~1050 nodes.
    nodes.reserve(2000);
    create_node();
  }

  int create_node() {
    nodes.emplace_back();
    return nodes.size() - 1;
  }

  // Classic insertion into the Trie.
  void insert(long long val) {
    int curr = 0;
    nodes[curr].count++;
    for (int i = MAX_BITS - 1; i >= 0; --i) {
      int bit = (val >> i) & 1;
      if (nodes[curr].children[bit] == -1) {
        nodes[curr].children[bit] = create_node();
      }
      curr = nodes[curr].children[bit];
      nodes[curr].count++;
    }
  }

  // Greedy: extracts and removes the largest number present in the Trie.
  long long extract_largest() {
    int curr = 0;
    if (nodes[curr].count == 0) return 0;

    long long val = 0;
    nodes[curr].count--;

    for (int i = MAX_BITS - 1; i >= 0; --i) {
      // Try to go right (bit 1) to maximize the value.
      int right_child = nodes[curr].children[1];

      if (right_child != -1 && nodes[right_child].count > 0) {
        val |= (1LL << i);
        curr = right_child;
      } else {
        // Otherwise go left (bit 0)
        int left_child = nodes[curr].children[0];
        // If the node doesn't exist (partial trie), assume 0 for remaining bits.
        if (left_child == -1) return val;
        curr = left_child;
      }
      nodes[curr].count--;
    }
    return val;
  }
};

// Function to solve a single test case.
void solve() {
  int num_elements, num_queries;
  if (!(cin >> num_elements >> num_queries)) return;

  vector<long long> elements(num_elements);
  for (int i = 0; i < num_elements; ++i) {
    cin >> elements[i];
  }

  // To cover bits up to 2^29, the largest numbers are sufficient.
  sort(elements.rbegin(), elements.rend());
  if (elements.size() > 35) {
    elements.resize(35);
  }

  // Build the base Trie with the selected numbers.
  Trie base_trie;
  // Add zeros to ensure the Trie always has enough "numbers" to extract.
  while (elements.size() < 35) elements.push_back(0);

  for (long long val : elements) {
    base_trie.insert(val);
  }

  for (int q = 0; q < num_queries; ++q) {
    long long target_xor;
    cin >> target_xor;

    // Copy the Trie state for this query.
    Trie      current_trie = base_trie;
    long long total_cost   = 0;

    // Process bits from highest.
    for (int bit = MAX_BITS - 1; bit >= 0; --bit) {
      long long bit_value = (1LL << bit);

      // If the target requires this bit.
      if (target_xor & bit_value) {
        // Take the best available number.
        long long max_val = current_trie.extract_largest();

        if (max_val < bit_value) {
          // The number is too small, pay to upgrade it.
          total_cost += (bit_value - max_val);
          // The number is fully consumed.
        } else {
          // The number covers the bit.
          if (max_val >= target_xor) {
            // If it's large enough to cover the rest of the target, cost 0.
            break;
          }
          // Use capacity for this bit and reinsert the remainder into the Trie.
          max_val -= bit_value;
          current_trie.insert(max_val);
        }

        // Remove the bit from the required target.
        target_xor -= bit_value;
      }
    }
    cout << total_cost << "\n";
  }
}

//===----------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int T = 1;
  cin >> T;
  for ([[maybe_unused]] auto _ : views::iota(0, T)) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//
