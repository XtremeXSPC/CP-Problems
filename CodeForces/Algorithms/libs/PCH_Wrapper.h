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
#ifndef CP_TYPES_NO_PREAMBLE
  #define CP_TYPES_NO_PREAMBLE 1
  #define CP_TYPES_NO_PREAMBLE_LOCAL_SCOPE 1
#endif
#include "templates/Types.hpp"
#ifdef CP_TYPES_NO_PREAMBLE_LOCAL_SCOPE
  #undef CP_TYPES_NO_PREAMBLE
  #undef CP_TYPES_NO_PREAMBLE_LOCAL_SCOPE
#endif

// Legacy aliases for older round sources that include PCH directly.
#ifndef CP_PCH_ENABLE_SHORT_ALIASES
  #define CP_PCH_ENABLE_SHORT_ALIASES 1
#endif
#if CP_PCH_ENABLE_SHORT_ALIASES
  using ll  = I64;
  using ull = U64;
  using ld  = F80;
  using LL  = I64;
  using ULL = U64;
  using LD  = F80;
#endif

#ifndef CP_PCH_ENABLE_CONTAINER_ALIASES
  #define CP_PCH_ENABLE_CONTAINER_ALIASES 1
#endif
#if CP_PCH_ENABLE_CONTAINER_ALIASES
  template <class T>
  using VC = Vec<T>;
  template <class T>
  using VVC = Vec2<T>;
  template <class T>
  using VVVC = Vec3<T>;
  template <class T, class U>
  using P = Pair<T, U>;
  template <class T, class U>
  using VP = VecPair<T, U>;
  template <class T, class U>
  using TP = Pair<T, U>;
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
  #if defined(__x86_64__) || defined(__i386__)
    #pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
  #elif defined(__aarch64__) || defined(__arm__)
    #pragma GCC target("+simd")
  #endif
#endif

#endif // PCH_WRAPPER_HPP
//===----------------------------------------------------------------------===//
