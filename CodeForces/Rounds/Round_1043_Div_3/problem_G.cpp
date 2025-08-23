//===----------------------------------------------------------------------===//
/**
 * @file: problem_G.cpp
 * @brief: Codeforces Round 1043 (Div. 3) - Problem G
 * @author: Costantino Lombardi
 *
 * @status: PASSED
 */
//===----------------------------------------------------------------------===//
/* Included library and Macros */

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

// Type aliases
using ll     = long long;
using ull    = unsigned long long;
using ld     = long double;
using pii    = std::pair<int, int>;
using pll    = std::pair<long long, long long>;
using vi     = std::vector<int>;
using vui    = std::vector<unsigned int>;
using vll    = std::vector<long long>;
using vvi    = std::vector<std::vector<int>>;
using vvll   = std::vector<std::vector<long long>>;
using vs     = std::vector<std::string>;
using vpii   = std::vector<std::pair<int, int>>;
using vpll   = std::vector<std::pair<long long, long long>>;
using map_ll = std::map<long long, long long>;

// Mathematical constants
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

// Handles rolling hash calculations with static power precomputation.
class PolynomialHasher {
private:
  vll               prefix_hashes;
  inline static vll base_powers;
  inline static ull HASH_BASE = 0;

public:
  // Precomputes powers of the base up to max_size. Should be called once.
  static void precompute_powers(int max_size, ull base) {
    if (HASH_BASE == base && base_powers.size() >= static_cast<size_t>(max_size + 1))
      return;
    HASH_BASE = base;
    base_powers.assign(max_size + 1, 0);
    base_powers[0] = 1;
    for (int i = 1; i <= max_size; ++i) {
      base_powers[i] = base_powers[i - 1] * HASH_BASE;
    }
  }

  // Constructor builds prefix hashes for a given string.
  PolynomialHasher(const string& s) {
    prefix_hashes.resize(s.length() + 1, 0);
    for (int i = 0; i < (int)s.length(); ++i) {
      prefix_hashes[i + 1] = prefix_hashes[i] * HASH_BASE + (ull)(s[i] - 'a' + 1);
    }
  }

  // Default constructor for creating vectors of this class.
  PolynomialHasher() = default;

  // Get hash of substring [l, r].
  inline ull get_hash(int l, int r) const {
    if (l > r)
      return 0;
    int len = r - l + 1;
    return prefix_hashes[r + 1] - prefix_hashes[l] * base_powers[len];
  }
};

// Encapsulates all logic for solving the grid palindrome problem.
class GridAnalyzer {
private:
  int n, m;
  // Pair of hashers for each row: (forward, reversed)
  vector<pair<PolynomialHasher, PolynomialHasher>> row_hashers;

  // Checks if the superimposition of the grid and its 180-degree rotated
  // version is palindromic.
  bool is_superimposition_valid(int tx, int ty) const {
    int r_start = std::max(0, tx - (n - 1));
    int r_end   = std::min(n - 1, tx);
    int c_start = std::max(0, ty - (m - 1));
    int c_end   = std::min(m - 1, ty);

    if (r_start > r_end || c_start > c_end)
      return true;

    int rev_c_shift = (m - 1) - ty;

    for (int i = r_start; i <= r_end; ++i) {
      int         rotated_i  = tx - i;
      const auto& fwd_hasher = row_hashers[i].first;
      const auto& rev_hasher = row_hashers[rotated_i].second;

      ull fwd_hash = fwd_hasher.get_hash(c_start, c_end);
      ull rev_hash = rev_hasher.get_hash(c_start + rev_c_shift, c_end + rev_c_shift);

      if (fwd_hash != rev_hash)
        return false;
    }
    return true;
  }

public:
  // Constructor initializes the analyzer with the grid.
  GridAnalyzer(const vs& grid) {
    n = grid.size();
    m = grid[0].length();
    row_hashers.resize(n);

    for (int i = 0; i < n; ++i) {
      row_hashers[i].first = PolynomialHasher(grid[i]);
      string reversed_row  = grid[i];
      reverse(reversed_row.begin(), reversed_row.end());
      row_hashers[i].second = PolynomialHasher(reversed_row);
    }
  }

  // Finds the minimum number of cells to add.
  ll find_minimum_expansion() {
    ll best_cost = LINF;

    for (int row_expansion = 0; row_expansion < n; ++row_expansion) {
      ll new_rows = (ll)n + row_expansion;
      if (new_rows * m - (ll)n * m >= best_cost)
        break;

      array<int, 2> tx_options = {(n - 1) + row_expansion, (n - 1) - row_expansion};
      int           tx_count   = (row_expansion == 0) ? 1 : 2;

      for (int i = 0; i < tx_count; ++i) {
        int tx = tx_options[i];
        for (int col_expansion = 0; col_expansion < m; ++col_expansion) {
          ll new_cols     = (ll)m + col_expansion;
          ll current_cost = new_rows * new_cols - (ll)n * m;
          if (current_cost >= best_cost)
            break;

          array<int, 2> ty_options = {(m - 1) + col_expansion, (m - 1) - col_expansion};
          int           ty_count   = (col_expansion == 0) ? 1 : 2;

          for (int j = 0; j < ty_count; ++j) {
            int ty = ty_options[j];
            if (is_superimposition_valid(tx, ty)) {
              best_cost = std::min(best_cost, current_cost);
            }
          }
        }
      }
    }
    return best_cost;
  }
};

// Function to solve a single test case
void solve() {
  int n, m;
  cin >> n >> m;
  vs a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }

  // Initialize the solver and compute the result
  GridAnalyzer solver(a);
  cout << solver.find_minimum_expansion() << endl;
}

//===----------------------------------------------------------------------===//
/* Main function */

int main() {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  // Precompute hasher's base powers once for the largest possible m.
  PolynomialHasher::precompute_powers(1000000 + 5, 301999ULL);

  int t = 1;
  cin >> t;
  while (t--) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//
