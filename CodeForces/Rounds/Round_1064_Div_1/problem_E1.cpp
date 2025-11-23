//===----------------------------------------------------------------------===//
/**
 * @file: problem_E.cpp
 * @brief Codeforces Round 1064 Div. 1 - Problem E
 * @author: Costantino Lombardi - C.
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

constexpr int MAXN = 300000 + 5;

VI adj_list[MAXN];
int degree_arr[MAXN];
int degree_count[MAXN];
int layer_depth[MAXN];
int best_inconvenience_for_k[MAXN];


// Function to solve a single test case.
void solve() {
    int n;
    cin >> n;

    // Reset adjacency.
    FOR (i, n) {
        adj_list[i].clear();
    }

    // Read tree.
    FOR (i, n - 1) {
        int u, v;
        cin >> u >> v;
        adj_list[u].push_back(v);
        adj_list[v].push_back(u);
    }

    // Reset per-test arrays.
    FOR (i, n + 1) {
        best_inconvenience_for_k[i] = INF;
        degree_count[i] = 0;
        layer_depth[i] = 1;   // first "leaf layer" depth
    }

    // Initial degrees and degree histogram.
    FOR (i, 1, n + 1) {
        degree_arr[i] = static_cast<int>(adj_list[i].size());
        ++degree_count[degree_arr[i]];
    }

    Queue<int> q;
    FOR (i, 1, n + 1) {
        if (degree_arr[i] == 1) {
            q.push(i);
        }
    }

    int removed_vertices = 0;
    int current_max_degree = n;
    while (current_max_degree > 0 && degree_count[current_max_degree] == 0) {
        --current_max_degree;
    }

    // Base cases:
    // k = 1 -> all edges same color -> inconvenience = 1
    best_inconvenience_for_k[1] = 1;

    // Inconvenience = 2: we can use up to "max degree" distinct colors.
    if (current_max_degree > 0) {
        best_inconvenience_for_k[current_max_degree] =
            min(best_inconvenience_for_k[current_max_degree], 2);
    }

    auto decrease_degree = [&](int v) -> int {
        int old_deg = degree_arr[v];
        if (old_deg == 0) return 0;
        --degree_count[old_deg];
        int new_deg = --degree_arr[v];
        ++degree_count[new_deg];
        return new_deg;
    };

    // Peel the tree from leaves inward.
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        // Remove vertex v and update its neighbours' degrees.
        for (int to : adj_list[v]) {
            if (decrease_degree(to) == 1) {
                // "to" just became a leaf: its layer is one deeper.
                layer_depth[to] = layer_depth[v] + 1;
                q.push(to);
            }
        }

        ++removed_vertices;

        while (current_max_degree > 0 &&
                degree_count[current_max_degree] == 0) {
            --current_max_degree;
        }

        // Case 1: odd inconvenience d = 2 * layer_depth[v] + 1.
        // Maximum number of colors achievable for this d.
        int odd_d = 2 * layer_depth[v] + 1;
        int k_odd = removed_vertices + 1;
        if (k_odd <= n - 1) {
            best_inconvenience_for_k[k_odd] =
                min(best_inconvenience_for_k[k_odd], odd_d);
        }

        // Case 2: even inconvenience d = 2 * layer_depth[v] + 2.
        if (current_max_degree > 0) {
            int even_d = 2 * layer_depth[v] + 2;
            int k_even = removed_vertices + current_max_degree;
            if (k_even <= n - 1) {
                best_inconvenience_for_k[k_even] =
                    min(best_inconvenience_for_k[k_even], even_d);
            }
        }
    }

    // Make the function k -> answer(k) non-decreasing via suffix minima.
    FOR_R (k, n - 1) {
        best_inconvenience_for_k[k] =
            min(best_inconvenience_for_k[k],
                best_inconvenience_for_k[k + 1]);
    }

    // Output answers.
    FOR (k, 1, n) {
        if (k > 1) cout << ' ';
        cout << best_inconvenience_for_k[k];
    }
    cout << '\n';
}

//===----------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int T = 1;
  cin >> T;
  for ([[maybe_unused]] auto _ : std::views::iota(0, T)) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//
