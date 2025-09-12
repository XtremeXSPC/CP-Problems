//===----------------------------------------------------------------------===//
/**
 * @file: problem_A.cpp
 * @brief Codeforces Round 1049 (Div. 2) - Problem A
 * @author: Costantino Lombardi
 *
 * @status: PASSED
 */
//===----------------------------------------------------------------------===//
/* Compiler Detection and Optimizations */
// clang-format off

// Enhanced compiler detection:
#if defined(__GNUC__) && !defined(__clang__)
  #define COMPILER_GCC
  #pragma GCC optimize("Ofast,unroll-loops,fast-math,O3,inline-functions")
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-result"
  #pragma GCC diagnostic ignored "-Wunused-variable"
  
  // Architecture-specific optimizations:
  #ifdef __x86_64__
    #pragma GCC target("avx2,bmi,bmi2,popcnt,lzcnt,sse4.2")
  #elif __aarch64__
    #pragma GCC target("+simd,+crypto,+fp16")
  #endif
  
#elif defined(__clang__)
  #define COMPILER_CLANG
  #pragma clang optimize on
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wunused-result"
  #pragma clang diagnostic ignored "-Wunused-variable"
  
#elif defined(_MSC_VER)
  #define COMPILER_MSVC
  #pragma optimize("gt", on)
#endif

//===----------------------------------------------------------------------===//
/* Smart Header Inclusion */

// Header selection logic with fallback chain:
#if defined(PCH_PRECOMPILED) && defined(PCH_AVAILABLE)
  // Precompiled header - Maximum performance.
  #include "PCH.h"
  #define USING_PRECOMPILED_PCH
  #define __UTILITY_FUNCTIONS__
#elif defined(USE_PCH) || defined(USE_CLANG_SANITIZE)
  // PCH as regular header - fallback mode.
  #include "PCH.h"
  #define USING_PCH_FALLBACK
  #define __UTILITY_FUNCTIONS__
#elif defined(FORCE_LIGHT_HEADERS)
  // Light header mode for fastest compilation.
  #include <iostream>
  #include <vector>
  #include <algorithm>
  #include <string>
  #include <map>
  #include <set>
  #include <queue>
  #include <stack>
  #include <cmath>
  #include <climits>
  #include <cstring>
  #include <iomanip>
  #define USING_LIGHT_HEADERS
#else
  // Default fallback.
  #include <bits/stdc++.h>
  #define USING_BITS_STDCPP
#endif

// Debug system:
#ifdef LOCAL
  #if __has_include("debug.h")
    #include "debug.h"
  #else
    #define debug(...) std::cerr << "[DEBUG] " << #__VA_ARGS__ << std::endl
  #endif
#else
  #define debug(...) 42
  #define debug_if(...) 42
  #define debug_tree(...) 42
  #define my_assert(...) 42
#endif

//===----------------------------------------------------------------------===//
/* Type Aliases - Competitive Programming Style */

#ifndef __TYPES__
  // Fundamental types with explicit semantics.
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
  
  // Legacy aliases for compatibility.
  using ll  = I64;
  using ull = U64;
  using ld  = F80;
  
  // Container type aliases.
  template <class T>
  using VC = std::vector<T>;
  template <class T>
  using VVC = VC<VC<T>>;
  
  // Specialized container aliases.
  using VI = VC<I64>;
  using VVI = VVC<I64>;
  using VB = VC<bool>;
  using VS = VC<std::string>;
  using VU8 = VC<U8>;
  using VU32 = VC<U32>;
  using VU64 = VC<U64>;
  
  // Pair aliases.
  using PII = std::pair<I32, I32>;
  using PLL = std::pair<I64, I64>;
  using PLD = std::pair<ld, ld>;
  template <class T, class U>
  using P = std::pair<T, U>;
  
  // Vector of pairs.
  using VPII = VC<PII>;
  using VPLL = VC<PLL>;
  
  // Advanced containers.
  template <class T>
  using PQ_max = std::priority_queue<T>;
  template <class T>
  using PQ_min = std::priority_queue<T, VC<T>, std::greater<T>>;
  
  template <class K, class V>
  using UMap = std::unordered_map<K, V>;
  template <class T>
  using USet = std::unordered_set<T>;
  template <class T>
  using MSet = std::multiset<T>;
  
  #define __TYPES__
#endif

//===----------------------------------------------------------------------===//
/* Mathematical Constants */

#ifndef __CONSTANTS__
  // High precision mathematical constants.
  constexpr F80 PI   = 3.1415926535897932384626433832795028841971693993751L;
  constexpr F80 E    = 2.7182818284590452353602874713526624977572470937000L;
  constexpr F80 PHI  = 1.6180339887498948482045868343656381177203091798058L;
  constexpr F80 EPS  = 1e-9L;
  constexpr F80 DEPS = 1e-12L;
  
  // Infinity values with overflow protection.
  constexpr I32 INF32 = 1'010'000'000;
  constexpr I64 INF64 = 2'020'000'000'000'000'000LL;
  constexpr I32 INF = INF32;
  constexpr I64 LINF = INF64;
  
  // Modular arithmetic constants.
  constexpr I64 MOD  = 1000000007;
  constexpr I64 MOD2 = 998244353;
  constexpr I64 MOD3 = 1000000009;
  
  #define __CONSTANTS__
#endif

//===----------------------------------------------------------------------===//
/* Macro System */

#ifndef __MACROS__
  // Multi-dimensional vector creation.
  #define vv(type, name, h, ...) \
    VC<VC<type>> name(h, VC<type>(__VA_ARGS__))
  #define vvv(type, name, h, w, ...) \
    VC<VC<VC<type>>> name(h, VC<VC<type>>(w, VC<type>(__VA_ARGS__)))
  
  // Advanced FOR loop system
  #define FOR1(a) for (decltype(a) _ = 0; _ < (a); ++_)
  #define FOR2(i, a) for (decltype(a) i = 0; i < (a); ++i)
  #define FOR3(i, a, b) for (decltype(a) i = (a); i < (b); ++i)
  #define FOR4(i, a, b, c) for (decltype(a) i = (a); i < (b); i += (c))
  #define FOR1_R(a) for (auto i = (a) - 1; i >= 0; --i)
  #define FOR2_R(i, a) for (auto i = (a) - 1; i >= 0; --i)
  #define FOR3_R(i, a, b) for (auto i = (b) - 1; i >= (a); --i)
  
  #define overload4(a, b, c, d, e, ...) e
  #define overload3(a, b, c, d, ...) d
  #define FOR(...) overload4(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)(__VA_ARGS__)
  #define FOR_R(...) overload3(__VA_ARGS__, FOR3_R, FOR2_R, FOR1_R)(__VA_ARGS__)
  
  // Container utilities.
  #define all(x) (x).begin(), (x).end()
  #define rall(x) (x).rbegin(), (x).rend()
  #define sz(x) (I64)(x).size()
  #define len(x) sz(x)
  #define pb push_back
  #define eb emplace_back
  #define mp make_pair
  #define mt make_tuple
  #define fi first
  #define se second
  #define elif else if
  
  // Algorithm shortcuts.
  #define UNIQUE(x) \
    (std::sort(all(x)), x.erase(std::unique(all(x)), x.end()), x.shrink_to_fit())
  #define LB(c, x) (I64)std::distance((c).begin(), std::lower_bound(all(c), x))
  #define UB(c, x) (I64)std::distance((c).begin(), std::upper_bound(all(c), x))
  #define SUM(x) std::accumulate(all(x), 0LL)
  #define MIN(x) *std::min_element(all(x))
  #define MAX(x) *std::max_element(all(x))
  
  #define __MACROS__
#endif

//===----------------------------------------------------------------------===//
/* Utility Functions */

#ifndef __UTILITY_FUNCTIONS__
  // Efficient min/max update functions.
  #ifndef USING_PCH
    template<class T, class S>
    inline constexpr bool chmin(T& a, const S& b) noexcept {
        return (a > b) ? (a = b, true) : false;
    }

    template<class T, class S>
    inline constexpr bool chmax(T& a, const S& b) noexcept {
        return (a < b) ? (a = b, true) : false;
    }
  #endif
  #define __UTILITY_FUNCTIONS__
#endif

  // Fast I/O setup function.
  inline void setup_io() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::cout << std::fixed << std::setprecision(10);
  }

  // Answer output functions.
  void YES(bool condition = true) { std::cout << (condition ? "YES" : "NO") << "\n"; }
  void NO(bool condition = true) { YES(!condition); }
  void Yes(bool condition = true) { std::cout << (condition ? "Yes" : "No") << "\n"; }
  void No(bool condition = true) { Yes(!condition); }
  
using namespace std;
// clang-format on

//===----------------------------------------------------------------------===//
/* Data Types and Function Definitions */

// Function to solve a single test case.
void solve() {
  int binary_str_len;
  cin >> binary_str_len;
  string binary_str;
  cin >> binary_str;

  // Count number of '0's in the string.
  const auto zero_count = ranges::count(binary_str, '0');

  // If string is homogeneous (all '0's or all '1's), it's already sorted.
  if (zero_count == 0 || zero_count == binary_str_len) {
    cout << 0 << "\n";
    return;
  }

  // Minimum operations = number of '1's misplaced in the '0' prefix.
  auto       prefix_view    = binary_str | views::take(zero_count);
  const auto misplaced_ones = ranges::count(prefix_view, '1');

  cout << misplaced_ones << "\n";
}

//===----------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  // Fast I/O
  setup_io();

#ifdef LOCAL
  // Local testing setup.
  debug("Running in LOCAL mode");
#endif

  I32 T = 1;
  cin >> T;
  for ([[maybe_unused]] auto _ : views::iota(0, T)) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//
