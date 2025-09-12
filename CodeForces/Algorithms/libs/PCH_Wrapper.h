//===----------------------------------------------------------------------===//
/**
 * @file PCH_Wrapper.hpp
 * @brief Stable Precompiled Header for Competitive Programming with GCC
 *
 * Contains only stable includes and definitions for maximum compilation speed.
 * Rules: No debug headers, only standard library, stable constants only.
 */
//===----------------------------------------------------------------------===//
// clang-format off
#ifndef PCH_WRAPPER_HPP
#define PCH_WRAPPER_HPP

// GCC compiler check
#if !defined(__GNUC__) || defined(__clang__)
  // #error "This PCH wrapper is designed for GCC. Use PCH.h for Clang instead."
#endif

// C++20 version check.
#if __cplusplus < 202002L
  #warning "C++20 or later recommended for full feature support"
#endif

// Core Standard Library - bits/stdc++.h includes everything.
#include <bits/stdc++.h>

// GCC-Specific Extensions - Policy-Based Data Structures.
#if __has_include(<ext/pb_ds/assoc_container.hpp>)
  #include <ext/pb_ds/assoc_container.hpp>
  #include <ext/pb_ds/tree_policy.hpp>
  #include <ext/pb_ds/hash_policy.hpp>
  #include <ext/pb_ds/trie_policy.hpp>
  #include <ext/pb_ds/priority_queue.hpp>
  #define PBDS_AVAILABLE 1
#else
  #define PBDS_AVAILABLE 0
  #pragma message("PBDS not available - Some advanced data structures won't be accessible.")
#endif

// Additional GCC built-ins.
#if __has_include(<ext/numeric>)
  #include <ext/numeric>
#endif

// Namespace imports.
using namespace std;
#if PBDS_AVAILABLE
  using namespace __gnu_pbds;
#endif

// Type aliases.
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

  // Extended precision types (when available):
  #ifdef __SIZEOF_INT128__
    using I128 = __int128;
    using U128 = unsigned __int128;
    #define HAS_INT128 1
  #else
    // Fallback for compilers that don't support 128-bit integers.
    using I128 = std::int64_t;
    using U128 = std::uint64_t;
    #define HAS_INT128 0
  #endif

  // Floating point types:
  using F32 = float;
  using F64 = double;
  using F80 = long double;

  #ifdef __FLOAT128__
    using F128 = __float128;
    #define HAS_FLOAT128 1
  #else
    // Fallback for compilers that don't support 128-bit floats.
    using F128 = long double;
    #define HAS_FLOAT128 0
  #endif

  // Legacy aliases for backward compatibility:
  using ll  = I64;
  using ull = U64;
  using ld  = F80;

  // Container type aliases:
  template <class T>
  using Vec = std::vector<T>;
  template <class T>
  using Deque = std::deque<T>;
  template <class T>
  using List = std::list<T>;
  template <class T>
  using Set = std::set<T>;
  template <class T>
  using MultiSet = std::multiset<T>;
  template <class T>
  using UnorderedSet = std::unordered_set<T>;
  template <class K, class V>
  using Map = std::map<K, V>;
  template <class K, class V>
  using MultiMap = std::multimap<K, V>;
  template <class K, class V>
  using UnorderedMap = std::unordered_map<K, V>;
  template <class T>
  using Stack = std::stack<T, std::deque<T>>;
  template <class T>
  using Queue = std::queue<T, std::deque<T>>;
  template <class T>
  using PriorityQueue = std::priority_queue<T, std::vector<T>>;
  template <class T>
  using MinPriorityQueue = std::priority_queue<T, std::vector<T>, std::greater<T>>;

  // Short aliases for competitive programming:
  template <class T>
  using VC = Vec<T>;
  template <class T>
  using VVC = VC<VC<T>>;
  template <class T>
  using VVVC = VC<VVC<T>>;

  // Pair and tuple aliases:
  template <class T, class U>
  using Pair = std::pair<T, U>;
  template <class T, class U>
  using P = Pair<T, U>;

  using PII = Pair<I32, I32>;
  using PLL = Pair<I64, I64>;
  using PLD = Pair<F80, F80>;

  // Specialized container aliases:
  using VI   = Vec<I32>;
  using VLL  = Vec<I64>;
  using VVI  = Vec<VI>;
  using VVLL = Vec<VLL>;
  using VB   = Vec<bool>;
  using VS   = Vec<std::string>;
  using VU8  = Vec<U8>;
  using VU32 = Vec<U32>;
  using VU64 = Vec<U64>;
  using VF   = Vec<F64>;

  // Vector of pairs:
  using VPII = Vec<PII>;
  using VPLL = Vec<PLL>;
  template <class T, class U>
  using VP = Vec<P<T, U>>;

  // Policy-based data structures:
  #ifdef PBDS_AVAILABLE
    using namespace __gnu_pbds;
    template <typename T>
    using ordered_set = tree<T, null_type, std::less<T>, rb_tree_tag, tree_order_statistics_node_update>;
    template <typename T>
    using ordered_multiset = tree<T, null_type, std::less_equal<T>, rb_tree_tag, tree_order_statistics_node_update>;
    template <typename K, typename V>
    using ordered_map = tree<K, V, std::less<K>, rb_tree_tag, tree_order_statistics_node_update>;
    template <typename K, typename V>
    using gp_hash_table = __gnu_pbds::gp_hash_table<K, V, std::hash<K>, std::equal_to<K>, direct_mask_range_hashing<>, linear_probe_fn<>, hash_standard_resize_policy<hash_exponential_size_policy<>, hash_load_check_resize_trigger<>, true>>;
  #endif
#endif

// Utility Macros.
#define SZ(x) static_cast<int>((x).size())
#define MINMAX(a, b) (((a) < (b)) ? make_pair(a, b) : make_pair(b, a))
#define IN_BOUNDS(i, j, n, m) ((i) >= 0 && (i) < (n) && (j) >= 0 && (j) < (m))
#define ALL(v) (v).begin(), (v).end()
#define RALL(v) (v).rbegin(), (v).rend()

// Fast I/O and precision macros.
#define FAST_IO() \
  ios_base::sync_with_stdio(false); \
  cin.tie(nullptr); \
  cout.tie(nullptr)

#define SET_PRECISION(n) \
  cout << fixed << setprecision(n)

// GCC optimizations (non-debug mode only).
#ifndef LOCAL
  #pragma GCC optimize("O3,unroll-loops")
  #pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#endif

// Stable Utility Functions.
template<typename T>
inline T power(T base, T exp, T mod) {
  T result = 1;
  base %= mod;
  while (exp > 0) {
    if (exp & 1) result = (result * base) % mod;
    base = (base * base) % mod;
    exp >>= 1;
  }
  return result;
}

template<typename T>
inline T gcd(T a, T b) {
  #if __cplusplus >= 201703L
    return __gcd(a, b);
  #else
    while (b) { T temp = b; b = a % b; a = temp; }
    return a;
  #endif
}

template<typename T>
inline T lcm(T a, T b) {
  return a / gcd(a, b) * b;
}

#endif // PCH_WRAPPER_HPP
//===----------------------------------------------------------------------===//