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
  #error "This PCH wrapper is designed for GCC. Use PCH.h for Clang instead."
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

  // Legacy short aliases are enabled by default for compatibility.
  // Define CP_ENABLE_LEGACY_SHORT_ALIASES=0 to hard-disable them.
  #ifndef CP_ENABLE_LEGACY_SHORT_ALIASES
    #define CP_ENABLE_LEGACY_SHORT_ALIASES 1
  #endif

  #if CP_ENABLE_LEGACY_SHORT_ALIASES
    using ll  [[deprecated("use I64 instead")]] = I64;
    using ull [[deprecated("use U64 instead")]] = U64;
    using ld  [[deprecated("use F80 instead")]] = F80;
  #endif

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

  // Canonical multidimensional aliases:
  template <class T>
  using Vec2 = Vec<Vec<T>>;
  template <class T>
  using Vec3 = Vec<Vec2<T>>;

  // Legacy container aliases are enabled by default for compatibility.
  // Define CP_ENABLE_LEGACY_CONTAINER_ALIASES=0 to hard-disable them.
  #ifndef CP_ENABLE_LEGACY_CONTAINER_ALIASES
    #define CP_ENABLE_LEGACY_CONTAINER_ALIASES 1
  #endif

  #if CP_ENABLE_LEGACY_CONTAINER_ALIASES
    template <class T>
    using VC [[deprecated("use Vec<T> instead")]] = Vec<T>;
    template <class T>
    using VVC [[deprecated("use Vec2<T> instead")]] = Vec2<T>;
    template <class T>
    using VVVC [[deprecated("use Vec3<T> instead")]] = Vec3<T>;
  #endif

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
  using VVI  = Vec2<I32>;
  using VVVI = Vec3<I32>;
  using VL   = Vec<I64>;
  using VVL  = Vec2<I64>;
  using VVVL = Vec3<I64>;
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
  #if defined(PBDS_AVAILABLE) && PBDS_AVAILABLE
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

#endif // PCH_WRAPPER_HPP
//===----------------------------------------------------------------------===//
