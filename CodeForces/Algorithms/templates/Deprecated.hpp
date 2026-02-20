#pragma once
#include "Types.hpp"

//===----------------------------------------------------------------------===//
/* Legacy Compatibility Layer */

#ifndef CP_ENABLE_DEPRECATED_COMPAT
  #define CP_ENABLE_DEPRECATED_COMPAT 1
#endif

#ifndef CP_ENABLE_LEGACY_SHORT_ALIASES
  #define CP_ENABLE_LEGACY_SHORT_ALIASES CP_ENABLE_DEPRECATED_COMPAT
#endif

#ifndef CP_ENABLE_LEGACY_CONTAINER_ALIASES
  #define CP_ENABLE_LEGACY_CONTAINER_ALIASES CP_ENABLE_DEPRECATED_COMPAT
#endif

#ifndef CP_ENABLE_LEGACY_SHORT_MACROS
  #define CP_ENABLE_LEGACY_SHORT_MACROS CP_ENABLE_DEPRECATED_COMPAT
#endif

#ifndef CP_ENABLE_LEGACY_POP_MACROS
  #define CP_ENABLE_LEGACY_POP_MACROS CP_ENABLE_DEPRECATED_COMPAT
#endif

#ifndef CP_ENABLE_LEGACY_MATH_FUNCTIONS
  #define CP_ENABLE_LEGACY_MATH_FUNCTIONS CP_ENABLE_DEPRECATED_COMPAT
#endif

#if CP_ENABLE_LEGACY_SHORT_ALIASES
  using ll  [[deprecated("use I64 instead")]] = I64;
  using ull [[deprecated("use U64 instead")]] = U64;
  using ld  [[deprecated("use F80 instead")]] = F80;
  using LL  [[deprecated("use I64 instead")]] = I64;
  using ULL [[deprecated("use U64 instead")]] = U64;
  using LD  [[deprecated("use F80 instead")]] = F80;

  template <class T, class U>
  using P [[deprecated("use Pair<T, U> instead")]] = Pair<T, U>;
  template <class T, class U>
  using TP [[deprecated("use Pair<T, U> instead")]] = Pair<T, U>;
  template <class T, class U>
  using VP [[deprecated("use VecPair<T, U> instead")]] = VecPair<T, U>;
#endif

#if CP_ENABLE_LEGACY_CONTAINER_ALIASES
  template <class T>
  using VC [[deprecated("use Vec<T> instead")]] = Vec<T>;
  template <class T>
  using VVC [[deprecated("use Vec2<T> instead")]] = Vec2<T>;
  template <class T>
  using VVVC [[deprecated("use Vec3<T> instead")]] = Vec3<T>;
#endif

#if CP_ENABLE_LEGACY_SHORT_MACROS
  #ifndef pb
    #define pb push_back
  #endif
  #ifndef fi
    #define fi first
  #endif
  #ifndef se
    #define se second
  #endif
  #ifndef mp
    #define mp make_pair
  #endif
  #ifndef mt
    #define mt make_tuple
  #endif

  // Range-based iteration:
  #define REP(i, n) _Pragma("GCC warning \"REP is deprecated, use FOR(i, n)\"") for (I64 i = 0, _rep_n = static_cast<I64>(n); i < _rep_n; ++i)
  #define RREP(i, n) _Pragma("GCC warning \"RREP is deprecated, use FOR_R(i, n)\"") for (I64 i = static_cast<I64>(n) - 1; i >= 0; --i)
#endif


#if CP_ENABLE_LEGACY_POP_MACROS
  // Legacy POP wrappers:
  template <typename T>
  [[deprecated("use pop_val() instead")]]
  T POP(std::deque<T>& container) {
    return pop_val(container);
  }

  template <typename T>
  [[deprecated("use pop_val() instead")]]
  T POP(MinPriorityQueue<T>& container) {
    return pop_val(container);
  }

  template <typename T>
  [[deprecated("use pop_val() instead")]]
  T POP(PriorityQueue<T>& container) {
    return pop_val(container);
  }

  template <typename T>
  [[deprecated("use pop_val() instead")]]
  T POP(Vec<T>& container) {
    return pop_val(container);
  }

  // Legacy aliases:
  template <typename T>
  [[deprecated("use popcount() instead")]]
  constexpr I32 popcnt(T x) { return popcount(x); }

  template <typename T>
  [[deprecated("use bit_width() - 1 instead")]]
  constexpr I32 topbit(T x) { return bit_width(x) - 1; }

  template <typename T>
  [[deprecated("use trailing_zeros() instead")]]
  constexpr I32 lowbit(T x) { return trailing_zeros(x); }
#endif

#ifdef CP_ENABLE_LEGACY_MATH_FUNCTIONS
  // Mathematical functions:
  template <typename T>
  [[deprecated("use std::gcd() instead")]]
  [[gnu::always_inline]] constexpr T _gcd(T a, T b) {
    static_assert(std::is_integral_v<T>, "_gcd requires an integral type.");
    return b ? _gcd(b, a % b) : a;
  }

  template <typename T>
  [[deprecated("use std::lcm() instead")]]
  [[gnu::always_inline]] constexpr T _lcm(T a, T b) {
    return a / _gcd(a, b) * b;
  }

  // Legacy alias:
  template <typename T>
  [[deprecated("use power(base, exp) or mod_pow(base, exp, mod) instead")]]
  [[gnu::always_inline]] constexpr T _power(T base, T exp, T mod = 0) {
    return mod ? mod_pow(base, exp, mod) : power(base, exp);
  }
#endif
