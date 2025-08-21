//===----------------------------------------------------------------------===//
/**
 * @file: problem_G.cpp
 * @brief Codeforces Round 1043 (Div. 3) - Problem G
 * @author: Costantino Lombardi
 *
 * @status: PASSED
 */
//===----------------------------------------------------------------------===//
/* Included library */

// clang-format off
// Sanitaze macro:
#ifdef USE_CLANG_SANITIZE
  #include "PCH.h"
#else
  #include <bits/stdc++.h>
#endif

using namespace std;

//===----------------------------------------------------------------------===//
/* Macros and Type Aliases */

// Debug macro:
#ifdef LOCAL
  #include "debug.h"
#else
  #define debug(...) 42
#endif
// clang-format on

// Type aliases
using ll   = long long;
using ull  = unsigned long long;
using ld   = long double;
using pii  = std::pair<int, int>;
using pll  = std::pair<long long, long long>;
using vi   = std::vector<int>;
using vll  = std::vector<long long>;
using vvi  = std::vector<std::vector<int>>;
using vvll = std::vector<std::vector<long long>>;
using vs   = std::vector<std::string>;
using vpii = std::vector<std::pair<int, int>>;
using vpll = std::vector<std::pair<long long, long long>>;

// Mathematical constants
constexpr long double PI   = 3.141592653589793238462643383279502884L;
constexpr long double E    = 2.718281828459045235360287471352662498L;
constexpr long double EPS  = 1e-9L;
constexpr int         INF  = 0x3f3f3f3f;
constexpr long long   LINF = 0x3f3f3f3f3f3f3f3fLL;
constexpr int         MOD  = 1000000007;
constexpr int         MOD2 = 998244353;

//===----------------------------------------------------------------------===//
/* Data Types and Function Definitions */

// Polynomial rolling hash structure for efficient string comparison
struct PolyHash {
  static constexpr ull BASE = 137; // Prime base for hashing
  vector<ull>          hash_vals;
  vector<ull>          powers;

  PolyHash() {}

  // Build hash for string s, padded to length max_len
  PolyHash(const string& s, int max_len) {
    int n = s.size();
    hash_vals.resize(max_len + 1);
    powers.resize(max_len + 1);

    powers[0]    = 1;
    hash_vals[0] = 0;

    // Hash the actual string
    for (int i = 0; i < n; i++) {
      powers[i + 1]    = powers[i] * BASE;
      hash_vals[i + 1] = hash_vals[i] * BASE + (ull)(s[i] - 'a' + 1);
    }

    // Pad with zeros for the remaining positions
    for (int i = n; i < max_len; i++) {
      powers[i + 1]    = powers[i] * BASE;
      hash_vals[i + 1] = hash_vals[i] * BASE;
    }
  }

  // Get hash of substring [l, r] inclusive
  ull get_hash(int l, int r) const {
    if (l > r)
      return 0;
    return hash_vals[r + 1] - hash_vals[l] * powers[r - l + 1];
  }
};

// clang-format off
// Check if placing the grid with bottom-right corner at (row_target, col_target) works
bool is_valid_placement(
    int row_target, int col_target, int n, int m, 
    const vector<PolyHash>& forward_hash, 
    const vector<PolyHash>& reverse_hash
  ) {
  // Calculate the overlapping region between original and rotated grid
  int row_start = max(0, row_target - (n - 1));
  int row_end   = min(n - 1, row_target);
  if (row_start > row_end)
    return true; // No overlap in rows

  int col_start = max(0, col_target - (m - 1));
  int col_end   = min(m - 1, col_target);
  if (col_start > col_end)
    return true; // No overlap in columns

  // Shift for reverse hash indexing
  int reverse_shift = (m - 1) - col_target;

  // Check if all overlapping rows match after rotation
  for (int i = row_start; i <= row_end; i++) {
    int j = row_target - i; // Corresponding row in rotated grid

    // Get hash of the overlapping segment
    ull hash_forward = forward_hash[i].get_hash(col_start, col_end);
    ull hash_reverse = reverse_hash[j].get_hash(col_start + reverse_shift, col_end + reverse_shift);

    if (hash_forward != hash_reverse) {
      return false;
    }
  }

  return true;
}
// clang-format on

// Function to solve a single test case
void solve() {
  int n, m;
  cin >> n >> m;

  vs grid(n);
  for (int i = 0; i < n; i++) {
    cin >> grid[i];
  }

  // Precompute forward and reverse hashes for each row
  vector<PolyHash> forward_hash(n), reverse_hash(n);
  for (int i = 0; i < n; i++) {
    string reversed_row = grid[i];
    reverse(reversed_row.begin(), reversed_row.end());

    forward_hash[i] = PolyHash(grid[i], m);
    reverse_hash[i] = PolyHash(reversed_row, m);
  }

  ll min_cells_to_add = LINF;

  // Try different offsets for the extended grid
  // dx: additional rows, dy: additional columns
  for (int dx = 0; dx <= n - 1; dx++) {
    ll total_rows = (ll)n + dx;

    // Early termination: if minimum area already exceeds best
    if (total_rows * (ll)m - (ll)n * (ll)m >= min_cells_to_add) {
      break;
    }

    // Possible target positions for bottom-right corner with this dx
    vector<int> row_targets = {n - 1 + dx};
    if (dx > 0) {
      row_targets.push_back(n - 1 - dx);
    }

    for (int row_target : row_targets) {
      for (int dy = 0; dy <= m - 1; dy++) {
        ll total_cols   = (ll)m + dy;
        ll total_area   = total_rows * total_cols;
        ll cells_to_add = total_area - (ll)n * (ll)m;

        // Early termination
        if (cells_to_add >= min_cells_to_add) {
          break;
        }

        // Possible target positions for bottom-right corner with this dy
        vector<int> col_targets = {m - 1 + dy};
        if (dy > 0) {
          col_targets.push_back(m - 1 - dy);
        }

        for (int col_target : col_targets) {
          if (is_valid_placement(row_target, col_target, n, m, forward_hash, reverse_hash)) {
            min_cells_to_add = cells_to_add;
          }
        }
      }
    }
  }

  cout << min_cells_to_add << "\n";
}

//===----------------------------------------------------------------------===//
/* Main function */

int main() {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int t = 1;
  cin >> t;
  while (t--) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//
