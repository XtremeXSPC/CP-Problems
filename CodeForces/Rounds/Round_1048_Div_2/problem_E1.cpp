//===----------------------------------------------------------------------===//
/**
 * @file: problem_E1.cpp
 * @brief Codeforces Round 1048 (Div. 2) - Problem E1
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

// Maximum N size for the problem is 1000.
const int MAX_NODES = 1001;

// Class to encapsulate the tree and solve the problem.
class TreeBeautySolver {
private:
    int n, k;
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
        depths[1] = 1;

        int head = 0;
        vector<int> bfs_q(n);
        bfs_q[head++] = 1;

        for (int i = 0; i < n; ++i) {
            int u = bfs_q[i];
            if (adj[u].empty()) { // Leaf node found.
                min_leaf_depth = min(min_leaf_depth, depths[u]);
            }
            for (int v : adj[u]) {
                depths[v] = depths[u] + 1;
                bfs_q[head++] = v;
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

        // Step 3: Solve Subset Sum DP using std::bitset.
        bitset<MAX_NODES> possible_sums;
        possible_sums[0] = 1; // Sum 0 is always achievable (by picking no levels).

        int max_beauty = 0;
        ll total_nodes_in_use = 0;

        for (int beauty = 1; beauty <= ssize(level_costs); ++beauty) {
            total_nodes_in_use += level_costs[beauty - 1];

            // Update achievable sums using the new level's cost (Subset Sum DP transition).
            possible_sums |= (possible_sums << level_costs[beauty - 1]);

            // Step 4: Check if a valid partition of labels exists.
            ll min_zeros_needed = max(0LL, total_nodes_in_use - (n - k));
            ll max_zeros_possible = min((ll)k, total_nodes_in_use);

            if (min_zeros_needed > max_zeros_possible) continue;
            
            // Check if any sum in the valid range is achievable.
            auto valid_range = views::iota(static_cast<size_t>(min_zeros_needed), static_cast<size_t>(max_zeros_possible) + 1);
            auto result = ranges::find_if(valid_range, [&](size_t i){
                return i < possible_sums.size() && possible_sums.test(i);
            });

            if (result != valid_range.end()) {
                max_beauty = beauty; // A valid partition was found.
            }
        }
        return max_beauty;
    }
};

// Function to solve each test case.
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
