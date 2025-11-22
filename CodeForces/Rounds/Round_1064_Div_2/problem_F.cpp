//===----------------------------------------------------------------------===//
/**
 * @file: problem_F.cpp
 * @brief Codeforces Round 1064 Div. 2 - Problem F
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

using namespace std;

//===----------------------------------------------------------------------===//
/* Data Types and Function Definitions */

constexpr int MAXN = 1'000'000;

int a[MAXN + 5];
bool used_pos[MAXN + 5];
vector<int> pos[2 * MAXN + 5];

// Function to solve a single test case.
int greedy_match(int n, const int arr[]) {
    // Reset state.
    fill(used_pos + 1, used_pos + n + 1, false);
    for (int v = 1; v <= 2 * n; ++v) pos[v].clear();

    // Group positions by value.
    for (int i = 1; i <= n; ++i) {
        pos[arr[i]].push_back(i);
    }

    int matched = 0;
    vector<int> tmp;
    tmp.reserve(n);

    // Greedy matching over pairs of consecutive values v, v+1.
    for (int v = 1; v < 2 * n; ++v) {
        auto &cur = pos[v];
        auto &nxt = pos[v + 1];

        if (v & 1) {
            // v treated as "left": match each index in cur to the earliest >= idx in nxt.
            int p = 0;
            int m = (int)nxt.size();
            for (int idx : cur) {
                while (p < m && nxt[p] < idx) ++p;
                if (p < m) {
                    used_pos[nxt[p]] = true;
                    ++p;
                }
            }
        } else {
            // v treated as "right": match from right to left.
            int p = (int)nxt.size();
            for (int it = (int)cur.size() - 1; it >= 0; --it) {
                int idx = cur[it];
                while (p > 0 && nxt[p - 1] > idx) --p;
                if (p > 0) {
                    used_pos[nxt[p - 1]] = true;
                    --p;
                }
            }
        }

        // Keep only unmatched positions of value v+1 for later steps.
        tmp.clear();
        for (int idx : nxt) {
            if (used_pos[idx]) {
                ++matched;
            } else {
                tmp.push_back(idx);
            }
        }
        nxt.swap(tmp);
    }

    return matched;
}



//===----------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;
        for (int i = 1; i <= n; ++i) {
            cin >> a[i];
        }

        int m1 = greedy_match(n, a);

        reverse(a + 1, a + n + 1);
        int m2 = greedy_match(n, a);

        int ans = n - (m1 + m2);
        cout << ans << '\n';
  }

  return 0;
}

//===----------------------------------------------------------------------===//
