//===----------------------------------------------------------------------===//
/**
 * @file: problem_E2.cpp
 * @brief Codeforces Round 1048 (Div. 2) - Problem E2
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
using VVV_i    = std::vector<std::vector<std::vector<int>>>;
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

using namespace std;

//===----------------------------------------------------------------------===//
/* Data Types and Function Definitions */

// Custom dynamic bitset class to handle large bitsets efficiently.
class DynamicBitset {
 private:
  vector<ull>          words;
  size_t               size_in_bits;
  static constexpr int BITS_PER_WORD = 64;

 public:
  DynamicBitset(size_t num_bits = 0) { resize(num_bits); }

  void resize(size_t num_bits) {
    size_in_bits = num_bits;
    words.assign((num_bits + BITS_PER_WORD - 1) / BITS_PER_WORD, 0);
  }

  void set(size_t pos) {
    if (pos < size_in_bits) {
      words[pos / BITS_PER_WORD] |= (1ULL << (pos % BITS_PER_WORD));
    }
  }

  // Performs the core DP transition: this |= (this << shift_amount).
  void shift_or(int shift_amount) {
    if (shift_amount <= 0) return;

    int word_shift = shift_amount / BITS_PER_WORD;
    int bit_shift  = shift_amount % BITS_PER_WORD;

    for (int i = words.size() - 1; i >= word_shift; --i) {
      ull shifted_word = 0;
      // Shift within the same word.
      if (bit_shift == 0) {
        shifted_word = words[i - word_shift];
      } else {
        // Shift from the current word.
        shifted_word = words[i - word_shift] << bit_shift;
        // Carry over from the previous word.
        if (i - word_shift - 1 >= 0) {
          shifted_word |= (words[i - word_shift - 1] >> (BITS_PER_WORD - bit_shift));
        }
      }
      words[i] |= shifted_word;
    }
  }

  // Checks if any bit is set in the range [l, r].
  bool has_set_bit_in_range(ll l, ll r) const {
    if (l > r || l >= (ll)size_in_bits) return false;
    r = min(r, (ll)size_in_bits - 1);

    size_t start_word = l / BITS_PER_WORD;
    size_t end_word   = r / BITS_PER_WORD;

    if (start_word == end_word) {
      ull mask = (~(0ULL)) >> (BITS_PER_WORD - (r % BITS_PER_WORD - l % BITS_PER_WORD + 1));
      mask <<= (l % BITS_PER_WORD);
      return (words[start_word] & mask) != 0;
    }

    // Check first partial word.
    ull first_mask = (~(0ULL)) << (l % BITS_PER_WORD);
    if ((words[start_word] & first_mask) != 0) return true;

    // Check full words in between.
    for (size_t i = start_word + 1; i < end_word; ++i) {
      if (words[i] != 0) return true;
    }

    // Check last partial word.
    ull last_mask = (~(0ULL)) >> (BITS_PER_WORD - (r % BITS_PER_WORD + 1));
    if ((words[end_word] & last_mask) != 0) return true;

    return false;
  }
};

// Class to encapsulate the tree and solve the problem.
class TreeBeautySolver {
 private:
  int  n, k;
  VV_i adj;

 public:
  TreeBeautySolver(int nodes, int zeros) : n(nodes), k(zeros), adj(nodes + 1) {}

  void read_tree_structure() {
    for (int i = 2; i <= n; ++i) {
      int parent;
      cin >> parent;
      adj[parent].push_back(i);
    }
  }

  // Function to find the maximum beauty of the tree.
  int find_maximum_beauty() {
    // Step 1: Compute node depths and find the shallowest leaf.
    V_i depths(n + 1, 0);
    int min_leaf_depth = n;

    queue<int> q;
    q.push(1);
    depths[1]         = 1;
    vector<int> bfs_q = {1};
    bfs_q.reserve(n);

    for (size_t i = 0; i < bfs_q.size(); ++i) {
      int u = bfs_q[i];
      if (adj[u].empty()) min_leaf_depth = min(min_leaf_depth, depths[u]);
      for (int v : adj[u]) {
        depths[v] = depths[u] + 1;
        bfs_q.push_back(v);
      }
    }

    // Step 2: Collect costs of valid levels using a range pipeline.
    map<int, int> nodes_per_level;
    for (int i = 1; i <= n; ++i) {
      if (depths[i] <= min_leaf_depth) {
        nodes_per_level[depths[i]]++;
      }
    }

    // Extract level counts, convert to vector, and sort.
    auto level_costs = nodes_per_level | views::values | ranges::to<V_i>();
    ranges::sort(level_costs);

    // Step 3: Solve Subset Sum DP using our DynamicBitset.
    DynamicBitset possible_sums(n + 1);
    possible_sums.set(0);

    int max_beauty         = 0;
    ll  total_nodes_in_use = 0;

    for (int beauty = 1; beauty <= ssize(level_costs); ++beauty) {
      total_nodes_in_use += level_costs[beauty - 1];
      possible_sums.shift_or(level_costs[beauty - 1]);

      // Step 4: Check for a valid label partition.
      ll min_zeros_needed   = max(0LL, total_nodes_in_use - (n - k));
      ll max_zeros_possible = min((ll)k, total_nodes_in_use);

      if (possible_sums.has_set_bit_in_range(min_zeros_needed, max_zeros_possible)) {
        max_beauty = beauty;
      }
    }
    return max_beauty;
  }
};

// Function to solve a single test case.
void solve() {
  int n, k;
  cin >> n >> k;
  TreeBeautySolver solver(n, k);
  solver.read_tree_structure();
  cout << solver.find_maximum_beauty() << "\n";
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
