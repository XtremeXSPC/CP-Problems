//===----------------------------------------------------------------------===//
/**
 * @file: problem_D.cpp
 * @brief Codeforces Round 1046 (Div. 2) - Problem D
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

// Structure to represent a beacon point.
struct Beacon {
  ll x_coord, y_coord;

  [[nodiscard]] ll diagonal_sum() const noexcept { return x_coord + y_coord; }

  [[nodiscard]] ll anti_diagonal_diff() const noexcept { return x_coord - y_coord; }
};

// Function to perform a movement query and receive Manhattan distance.
ll perform_movement(char direction, ll distance) {
  cout << "? " << direction << ' ' << distance << '\n';
  cout.flush();

  ll manhattan_dist;
  if (!(cin >> manhattan_dist) || manhattan_dist == -1) {
    exit(0);
  }
  return manhattan_dist;
}

// Function to announce the discovered coordinates.
void announce_position(ll x_pos, ll y_pos) {
  cout << "! " << x_pos << ' ' << y_pos << '\n';
  cout.flush();
}

// Function to solve a single test case.
void solve() {
  int beacon_count;
  cin >> beacon_count;

  vector<Beacon> beacons(beacon_count);
  for (auto& beacon : beacons) {
    cin >> beacon.x_coord >> beacon.y_coord;
  }

  // Calculate critical values from beacon positions.
  auto diagonal_values = beacons | views::transform(&Beacon::diagonal_sum);
  ll   max_diagonal    = ranges::max(diagonal_values);

  auto anti_diagonal_values = beacons | views::transform(&Beacon::anti_diagonal_diff);
  ll   max_anti_diagonal    = ranges::max(anti_diagonal_values);

  constexpr ll LARGE_STEP = 1'000'000'000LL;

  // Phase 1: Navigate to extreme northeast position (X + 2L, Y + 2L).
  perform_movement('R', LARGE_STEP);
  perform_movement('R', LARGE_STEP);
  perform_movement('U', LARGE_STEP);
  ll northeast_distance = perform_movement('U', LARGE_STEP);

  // Phase 2: Navigate to extreme southeast position (X + 2L, Y - 2L).
  perform_movement('D', LARGE_STEP);
  perform_movement('D', LARGE_STEP);
  perform_movement('D', LARGE_STEP);
  ll southeast_distance = perform_movement('D', LARGE_STEP);

  // Apply the linearization formula for extreme positions:
  // At northeast: distance ≈ (X + 2L) + (Y + 2L) - max_diagonal
  // At southeast: distance ≈ (X + 2L) - (Y - 2L) - max_anti_diagonal

  ll coordinate_sum  = northeast_distance - 4LL * LARGE_STEP + max_diagonal;
  ll coordinate_diff = southeast_distance - 4LL * LARGE_STEP + max_anti_diagonal;

  // Solve the system: X + Y = coordinate_sum, X - Y = coordinate_diff.
  ll origin_x = (coordinate_sum + coordinate_diff) >> 1;
  ll origin_y = (coordinate_sum - coordinate_diff) >> 1;

  announce_position(origin_x, origin_y);
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
