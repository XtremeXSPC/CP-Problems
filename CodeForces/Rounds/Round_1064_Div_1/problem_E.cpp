//===----------------------------------------------------------------------===//
/**
 * @file: problem_E.cpp
 * @brief Codeforces Round 1064 Div. 1 - Problem E - Alternate Solution
 * @author: Costantino Lombardi - C.
 *
 * @status: In progress
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

// Sanitize macro:
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

#ifndef __TYPES__
#define __TYPES__

  // Fundamental type aliases with explicit sizes:
  using I8   = std::int8_t;
  using I16  = std::int16_t;
  using I32  = std::int32_t;
  using I64  = std::int64_t;
  using U8   = std::uint8_t;
  using U16  = std::uint16_t;
  using U32  = std::uint32_t;
  using U64  = std::uint64_t;
  using F32  = float;
  using F64  = double;
  using F80  = long double;

  // Extended precision types:
  #ifdef __SIZEOF_INT128__
  using I128 = __int128;
  using U128 = unsigned __int128;
  #else
  using I128 = std::int64_t;
  using U128 = std::uint64_t;
  #endif

  #ifdef __FLOAT128__
  using F128 = __float128;
  #else
  using F128 = long double;
  #endif

  // Legacy aliases for backward compatibility:
  using ll  = I64;
  using ull = U64;
  using ld  = F80;

  // Container type aliases:
  template <class T>
  using VC   = std::vector<T>;
  template <class T>
  using VVC  = VC<VC<T>>;
  template <class T>
  using VVVC = VC<VVC<T>>;
  template <class T>
  using VVVVC = VC<VVVC<T>>;

  // Specialized container aliases:
  using VI    = VC<I64>;
  using VVI   = VVC<I64>;
  using VVVI  = VVVC<I64>;
  using VB    = VC<bool>;
  using VS    = VC<std::string>;
  using VU8   = VC<U8>;
  using VU32  = VC<U32>;
  using VU64  = VC<U64>;

  // Queue and stack aliases:
  template <class T>
  using Deque = std::deque<T>;
  template <class T>
  using Stack = std::stack<T, std::deque<T>>;
  template <class T>
  using Queue = std::queue<T, std::deque<T>>;

  // Pair and tuple aliases:
  using PII = std::pair<I32, I32>;
  using PLL = std::pair<I64, I64>;
  using PLD = std::pair<ld, ld>;
  template <class T, class U>
  using P = std::pair<T, U>;

  using VPII = VC<PII>;
  using VPLL = VC<PLL>;
  template <class T, class U>
  using VP = VC<P<T, U>>;

  // Priority queue aliases:
  template <class T>
  using PQ_max = std::priority_queue<T>;
  template <class T>
  using PQ_min = std::priority_queue<T, VC<T>, std::greater<T>>;

  // Hash-based containers:
  template <class K, class V>
  using UMap = std::unordered_map<K, V>;
  template <class T>
  using USet = std::unordered_set<T>;
  template <class T>
  using MSet = std::multiset<T>;

#endif // __TYPES__

// Mathematical constants:
constexpr long double PI   = 3.141592653589793238462643383279502884L;
constexpr long double E    = 2.718281828459045235360287471352662498L;
constexpr long double EPS  = 1e-9L;
constexpr int         INF  = 0x3f3f3f3f;
constexpr long long   LINF = 0x3f3f3f3f3f3f3f3fLL;
constexpr int         LIM  = 1000000 + 5;
constexpr int         MOD  = 1000000007;
constexpr int         MOD2 = 998244353;

// Advanced FOR loop system:
#define FOR1(a) for (I64 _ = 0; _ < (a); ++_)
#define FOR2(i, a) for (I64 i = 0; i < (a); ++i)
#define FOR3(i, a, b) for (I64 i = (a); i < (b); ++i)
#define FOR4(i, a, b, c) for (I64 i = (a); i < (b); i += (c))
#define FOR1_R(a) for (I64 i = (a) - 1; i >= 0; --i)
#define FOR2_R(i, a) for (I64 i = (a) - 1; i >= 0; --i)
#define FOR3_R(i, a, b) for (I64 i = (b) - 1; i >= (a); --i)

#define overload4(a, b, c, d, e, ...) e
#define overload3(a, b, c, d, ...) d
#define FOR(...) overload4(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)(__VA_ARGS__)
#define FOR_R(...) overload3(__VA_ARGS__, FOR3_R, FOR2_R, FOR1_R)(__VA_ARGS__)

using namespace std;

//===----------------------------------------------------------------------===//
/* Data Types and Function Definitions */

// Solver for each test case.
struct Solver {
    int n;
    VVI adj;

    // best_d_for_k[k] = minimal inconvenience d achievable with at least k colors.
    VI best_d_for_k;

    // K_of_d[d] = max number of colors k achievable with inconvenience <= d.
    VI K_of_d;

    // Run the leaf-peeling process once and fill best_d_for_k.
    void preprocess_best_d() {
        best_d_for_k.assign(n + 2, INF);

        // Degrees and degree histogram.
        VI degree(n + 1, 0);
        VI degree_count(n + 1, 0);
        FOR(i, 1, n + 1) {
            degree[i] = static_cast<int>(adj[i].size());
            degree_count[degree[i]]++;
        }

        // Leaf layer for each vertex (first leaf layer = 1).
        VI layer_depth(n + 1, 1);

        Queue<int> q;
        FOR(i, 1, n + 1) {
            if (degree[i] == 1) {
                q.push(i);
            }
        }

        int removed_vertices = 0;

        // Current maximum degree in the remaining graph.
        int current_max_degree = n;
        while (current_max_degree > 0 && degree_count[current_max_degree] == 0) {
            --current_max_degree;
        }

        // Base case: all edges same color → inconvenience = 1.
        best_d_for_k[1] = 1;

        // Inconvenience 2: pick a vertex with maximum degree.
        if (current_max_degree > 0) {
            best_d_for_k[current_max_degree] =
                std::min(best_d_for_k[current_max_degree], 2);
        }

        auto decrease_degree = [&](int v) -> int {
            int old_deg = degree[v];
            if (old_deg == 0) return 0; // already removed
            degree_count[old_deg]--;
            int new_deg = --degree[v];
            degree_count[new_deg]++;
            return new_deg;
        };

        // Peel the tree from leaves inward.
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            // Remove v from degree histogram.
            int dv = degree[v];
            if (dv > 0) {
                degree_count[dv]--;
                degree[v] = 0;
                degree_count[0]++;
            }

            // Update neighbours.
            for (int to : adj[v]) {
                if (decrease_degree(to) == 1) {
                    // 'to' just became a new leaf.
                    layer_depth[to] = layer_depth[v] + 1;
                    q.push(to);
                }
            }

            ++removed_vertices;

            while (current_max_degree > 0 &&
                   degree_count[current_max_degree] == 0) {
                --current_max_degree;
            }

            // Case 1: odd inconvenience d = 2 * layer_depth[v] + 1
            // Colors >= removed_vertices + 1.
            int odd_d = 2 * layer_depth[v] + 1;
            int k_odd = removed_vertices + 1;
            if (k_odd <= n - 1) {
                best_d_for_k[k_odd] =
                    std::min(best_d_for_k[k_odd], odd_d);
            }

            // Case 2: even inconvenience d = 2 * layer_depth[v] + 2
            // Colors >= removed_vertices + current_max_degree.
            if (current_max_degree > 0) {
                int even_d = 2 * layer_depth[v] + 2;
                int k_even = removed_vertices + current_max_degree;
                if (k_even <= n - 1) {
                    best_d_for_k[k_even] =
                        std::min(best_d_for_k[k_even], even_d);
                }
            }
        }

        // Ensure best_d_for_k is non-decreasing in k.
       FOR_R(k, 1, n - 1) {
            best_d_for_k[k] =
                std::min(best_d_for_k[k], best_d_for_k[k + 1]);
        }
    }

    // Build K_of_d so that get_max_cuts(d) can be answered in O(1).
    void build_K_of_d() {
        int max_d = 0;
        FOR(k, 1, n) {
            if (best_d_for_k[k] < std::numeric_limits<int>::max()) {
                if (best_d_for_k[k] > max_d) max_d = best_d_for_k[k];
            }
        }
        if (max_d == 0) max_d = n; // safeguard

        K_of_d.assign(max_d + 2, 0);

        // best_d_for_k[k] = minimal d with K(d) >= k.
        for (int k = 1; k < n; ++k) {
            int d = best_d_for_k[k];
            if (d == std::numeric_limits<int>::max()) continue;
            if (d > max_d) d = max_d;
            K_of_d[d] = std::max(K_of_d[d], k);
        }

        // Prefix max over d → for each d, K_of_d[d] = max k with best_d_for_k[k] <= d.
        FOR (d, 1, max_d + 1) {
            K_of_d[d] = std::max(K_of_d[d], K_of_d[d - 1]);
        }
    }

    // Returns max number of cuts (= colors - 1) achievable with inconvenience <= d.
    int get_max_cuts(int d) {
        if (K_of_d.empty()) return 0;
        int max_d = static_cast<int>(K_of_d.size()) - 2;
        if (d > max_d) d = max_d;
        if (d <= 0) return 0;
        int colors = K_of_d[d];
        if (colors <= 0) return 0;
        return colors - 1;
    }

    void run() {
        cin >> n;
        adj.assign(n + 1, {});
        FOR(i, n - 1) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        // Precompute k -> minimal d, then invert in d -> max colors.
        preprocess_best_d();
        build_K_of_d();

        VI result(n);

        // Divide and conquer to fill result[k] = minimal d with at least k colors.
        auto solve_dc = [&](auto&& self, int k_min, int k_max, int ans_min, int ans_max) -> void {
            if (k_min > k_max) return;
            int k_mid = k_min + (k_max - k_min) / 2;

            int best_d = ans_max;
            int low = ans_min, high = ans_max;
            while (low <= high) {
                int mid_d = low + (high - low) / 2;
                if (get_max_cuts(mid_d) + 1 >= k_mid) {
                    best_d = mid_d;
                    high = mid_d - 1;
                } else {
                    low = mid_d + 1;
                }
            }

            result[k_mid] = best_d;

            if (k_min == k_max) return;
            self(self, k_min, k_mid - 1, ans_min, best_d);
            self(self, k_mid + 1, k_max, best_d, ans_max);
        };

        // Bound ragionevoli per d: 1..n (diametro in colori non supera n-1).
        solve_dc(solve_dc, 1, n - 1, 1, n);

        FOR(i, 1, n) {
            std::cout << result[i] << (i + 1 == n ? '\n' : ' ');
        }
        if (n == 1) {
            std::cout << '\n';
        }
    }
};

//===----------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int T = 1;
  cin >> T;
  for ([[maybe_unused]] auto _ : std::views::iota(0, T)) {
    Solver solver;
    solver.run();
  }

  return 0;
}

//===----------------------------------------------------------------------===//
