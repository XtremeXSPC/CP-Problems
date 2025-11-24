//===----------------------------------------------------------------------===//
/**
 * @file: Sol_H.cpp
 * @brief Codeforces Round - Problem H - Keygen 3
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
  #ifdef __aarch64__
    #pragma GCC target("+simd")
  #endif
#endif

#ifdef __clang__
  #pragma clang optimize on
#endif

// Sanitize/Debug macro:
#if defined(USE_CLANG_SANITIZE) || defined(LOCAL)
  #define SHOW_COMPILATION_INFO
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

//===----------------------------------------------------------------------===//
/* Type Aliases and Constants */

#ifndef __TYPES__
#define __TYPES__
  // Fundamental type aliases with explicit sizes:
  using I8  = std::int8_t;
  using I16 = std::int16_t;
  using I32 = std::int32_t;
  using I64 = std::int64_t;
  using U8  = std::uint8_t;
  using U16 = std::uint16_t;
  using U32 = std::uint32_t;
  using U64 = std::uint64_t;
  using F32 = float;
  using F64 = double;
  using F80 = long double;

  using LL  = I64;
  using ULL = U64;
  using LD  = F80;

  // Extended precision types:
  #ifdef __SIZEOF_INT128__
    using I128 = __int128;
    using U128 = unsigned __int128;
  #else
    using I128 = std::int64_t;
    using U128 = std::uint64_t;
  #endif

  // Legacy aliases for backward compatibility:
  #ifdef __FLOAT128__
    using F128 = __float128;
  #else
    using F128 = long double;
  #endif

  // Container type aliases:
  template <class T>
  using VC = std::vector<T>;
  template <class T>
  using VVC = VC<VC<T>>;
  template <class T>
  using VVVC = VC<VVC<T>>;
  template <class T>
  using VVVVC = VC<VVVC<T>>;

  // Specialized container aliases:
  using VI   = VC<I32>;
  using VVI  = VVC<I32>;
  using VVVI = VVVC<I32>;
  using VL   = VC<I64>;
  using VVL  = VVC<I64>;
  using VVVL = VVVC<I64>;
  using VB   = VC<bool>;
  using VS   = VC<std::string>;
  using VU8  = VC<U8>;
  using VU32 = VC<U32>;
  using VU64 = VC<U64>;
  using VF   = VC<F64>;

  // Queue and stack aliases:
  template <class T>
  using Deque = std::deque<T>;
  template <class T>
  using Stack = std::stack<T, std::deque<T>>;
  template <class T>
  using Queue = std::queue<T, std::deque<T>>;

  // Pair and tuple aliases:
  template <class T, class U>
  using P = std::pair<T, U>;
  using PII = std::pair<I32, I32>;
  using PLL = std::pair<I64, I64>;
  using PLD = std::pair<ld, ld>;

  template <class T, class U>
  using VP = VC<P<T, U>>;
  using VPII = VC<PII>;
  using VPLL = VC<PLL>;

  // Priority queue aliases
  template <class T>
  using PQ_max = std::priority_queue<T>;
  template <class T>
  using PQ_min = std::priority_queue<T, VC<T>, std::greater<T>>;

  // Map and set aliases:
  template <class K, class V>
  using UMap = std::unordered_map<K, V>;
  template <class T>
  using USet = std::unordered_set<T>;
  template <class T>
  using MSet = std::multiset<T>;
#endif  // __TYPES__

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
// clang-format on

//===----------------------------------------------------------------------===//
/* Data Types and Function Definitions */

static const int LIMIT = 2000;

// DSU to track connected components (cycles) while building permutation.
struct DSU {
  int n;
  VI  parent;

  explicit DSU(int n_) : n(n_), parent(n_) { iota(parent.begin(), parent.end(), 0); }

  int find(int x) { return (parent[x] == x ? x : parent[x] = find(parent[x])); }

  // Returns true if merge happened (components decreased), false otherwise.
  bool unite(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y) return false;
    parent[x] = y;
    return true;
  }
};

// Generate up to LIMIT bitonic permutations of length N with exactly M cycles.
VVI solve_bitonic(int N, int M) {
  VVI res;

  // Build permutations with "core" length n, target m cycles,
  // then lift to size N by adding prefix fixed points (k) and optional internal fixed point.
  auto build_core = [&](auto&& self, int n, int m, int k, int extra) -> void {
    if ((int)res.size() == LIMIT) return;

    // x is length of middle segment; require 2*x <= n.
    FOR3(x, m, n / 2 + 1) {
      if ((int)res.size() == LIMIT) return;

      VI perm(n, -1);  // core permutation [0..n-1]
      VI u;            // helper for middle segment
      u.push_back(n - 1);

      // DFS: place values 1..(n-x-1) on two wings.
      auto dfs_wings = [&](auto&& self2, int v, int w, int left_pos, int right_pos) -> void {
        if ((int)res.size() == LIMIT) return;

        if (v == n - x) {
          // All wing values placed.
          assert(w == x);

          // Fill middle block [L,R) = [n-2x, n-x) with values [n-x..n-1].
          auto fill_middle = [&](auto&& self3, int L, int R, int at, int comps, DSU d) -> void {
            if ((int)res.size() == LIMIT) return;

            if (L == R) {
              // All middle positions assigned.
              if (comps == m) {
                // Build final permutation of size N.
                VI out = perm;
                // Shift values by k (prefix fixed points).
                for (int& val : out) val += k;
                // Add prefix 0..k-1.
                FOR(k) out.insert(out.begin(), k - 1 - _);
                // Optional internal fixed point.
                if (extra) {
                  int where = N - 1 - x;
                  for (int& val : out) {
                    if (val >= where) ++val;
                  }
                  out.insert(out.begin() + where, where);
                }
                res.push_back(out);
              }
              return;
            }

            // Try placing value "at" at position L.
            DSU backup    = d;
            int new_comps = comps - (d.unite(L, at) ? 1 : 0);
            if (comps >= m) {
              perm[L] = at;
              self3(self3, L + 1, R, at + 1, new_comps, d);
            }

            // Try placing "at" at position R-1.
            if (L + 1 < R) {
              d         = backup;
              new_comps = comps - (d.unite(R - 1, at) ? 1 : 0);
              if (comps >= m) {
                perm[R - 1] = at;
                self3(self3, L, R - 1, at + 1, new_comps, d);
              }
            }
          };

          // Initialize DSU on entire core [0..n-1].
          DSU d(n);
          // Connect middle segment positions with u structure.
          FOR3(pos, n - 2 * x, n - x) {
            int idx = pos - (n - 2 * x);
            d.unite(pos, u[idx]);
          }
          fill_middle(fill_middle, n - 2 * x, n - x, n - x, x, d);
          return;
        }

        // Place value v: w is count pushed into 'u'.

        // Option 1: put v on right wing at right_pos.
        if ((v + 1) - w <= n - 2 * x) {
          perm[right_pos] = v;
          rotate(u.begin(), u.begin() + 1, u.end());
          self2(self2, v + 1, w, left_pos, right_pos + 1);
          rotate(u.begin(), u.end() - 1, u.end());
        }

        // Option 2: put v on left wing at left_pos.
        if (w < x) {
          perm[left_pos] = v;
          u.push_back(n - 1 - w);
          self2(self2, v + 1, w + 1, left_pos - 1, right_pos);
          u.pop_back();
        }
      };

      // Smallest value 0 fixed at end (ensures bitonic shape).
      perm[n - 1] = 0;
      dfs_wings(dfs_wings, 1, 1, n - 2, 0);
    }
  };

  // Special case: M == N means identity permutation only.
  if (M == N) {
    VI id(N);
    iota(id.begin(), id.end(), 0);
    res.push_back(id);
    return res;
  }

  // General case: strip k fixed points at front and optional extra inside.
  FOR3_R(k, 0, M) {
    FOR(extra, 2) {
      if (k + extra >= M) continue;
      int core_n = N - k - extra;
      int core_m = M - k - extra;
      if (core_n <= 0 || core_m <= 0) continue;
      build_core(build_core, core_n, core_m, k, extra);
    }
  }

  return res;
}

//===----------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  if (!(cin >> n >> m)) return 0;

  auto perms = solve_bitonic(n, m);
  cout << perms.size() << '\n';

  // Print permutations.
  for (const auto& p : perms) {
    FOR(i, n) {
      cout << p[i] + 1 << (i + 1 == n ? '\n' : ' ');
    }
  }

  return 0;
}

//===----------------------------------------------------------------------===//
