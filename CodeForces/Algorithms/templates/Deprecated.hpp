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

#ifndef CP_ENABLE_DEPRECATED_POP_MACROS
  #define CP_ENABLE_DEPRECATED_POP_MACROS CP_ENABLE_DEPRECATED_COMPAT
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
#endif


#if CP_ENABLE_DEPRECATED_POP_MACROS
  // Legacy POP wrappers (deprecated -- use pop_val instead):
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
#endif
