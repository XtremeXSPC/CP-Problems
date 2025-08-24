//===----------------------------------------------------------------------===//
/**
 * @file: problem_C.cpp
 * @brief Codeforces Round 1044 (Div. 2) - Problem C - Interactive Problem
 * @author: Costantino Lombardi
 *
 * @status: In Progress
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

// Structure to represent the portal network
struct PortalNetwork {
  int          portal_count;
  vi           max_reachable_distance;
  map<int, vi> portals_at_distance;

  explicit PortalNetwork(int n) : portal_count(n) { max_reachable_distance.resize(n + 1); }

  // Send query and receive response
  int request_path_info(int origin, vi destinations) {
    cout << "? " << origin << " " << destinations.size();
    for (auto& dest : destinations) {
      cout << " " << dest;
    }
    cout << endl;

    int response;
    cin >> response;
    if (response == -1)
      exit(0);
    return response;
  }

  // Initialize network by querying all portals
  void initialize_distances() {
    vi universe;
    for (int i = 1; i <= portal_count; ++i) {
      universe.push_back(i);
    }

    for (int portal = 1; portal <= portal_count; ++portal) {
      max_reachable_distance[portal] = request_path_info(portal, universe);
    }

    // Group portals by their maximum reachable distance
    int highest_distance = *max_element(max_reachable_distance.begin() + 1, max_reachable_distance.end());

    for (int portal = 1; portal <= portal_count; ++portal) {
      int dist = max_reachable_distance[portal];
      portals_at_distance[dist].push_back(portal);
    }
  }

  // Find the optimal path using the network structure
  vi find_optimal_path() {
    // Identify the best starting portal
    auto it            = max_element(max_reachable_distance.begin() + 1, max_reachable_distance.end());
    int  origin_portal = distance(max_reachable_distance.begin(), it);
    int  path_length   = *it;

    if (path_length == 1) {
      return {origin_portal};
    }

    // Build path using BFS-like approach
    vi       optimal_route;
    set<int> visited_portals;

    optimal_route.push_back(origin_portal);
    visited_portals.insert(origin_portal);

    int active_portal  = origin_portal;
    int remaining_hops = path_length - 1;

    while (remaining_hops > 0) {
      bool found_next = false;

      // Search for next portal at the correct distance level
      if (portals_at_distance.count(remaining_hops)) {
        for (int candidate : portals_at_distance[remaining_hops]) {
          if (visited_portals.count(candidate))
            continue;

          // Test connectivity
          vi  pair_test = {active_portal, candidate};
          int result    = request_path_info(active_portal, pair_test);

          if (result == 2) {
            optimal_route.push_back(candidate);
            visited_portals.insert(candidate);
            active_portal = candidate;
            found_next    = true;
            break;
          }
        }
      }

      if (!found_next)
        break;
      remaining_hops--;
    }

    return optimal_route;
  }

  // Submit final answer
  void submit_solution(const vi& solution_path) {
    cout << "! " << solution_path.size();
    for (int portal : solution_path) {
      cout << " " << portal;
    }
    cout << endl;
  }
};

// Function to solve a single test case
void solve() {
  int n;
  cin >> n;

  PortalNetwork network(n);
  network.initialize_distances();
  vi optimal_path = network.find_optimal_path();
  network.submit_solution(optimal_path);
}

//===----------------------------------------------------------------------===//
/* Main function */

int main() {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  while (t--) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//