#pragma once
#include "Preamble.hpp"

//===----------------------------------------------------------------------===//
/* Advanced Type System and Aliases */
// clang-format off

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

// Legacy short aliases (deprecated -- use I64, U64, F80 instead):
using ll  [[deprecated("use I64 instead")]] = I64;
using ull [[deprecated("use U64 instead")]] = U64;
using ld  [[deprecated("use F80 instead")]] = F80;

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

// Legacy container aliases (deprecated -- use Vec, Vec2, Vec3 instead):
template <class T>
using VC [[deprecated("use Vec<T> instead")]] = Vec<T>;
template <class T>
using VVC [[deprecated("use Vec2<T> instead")]] = Vec2<T>;
template <class T>
using VVVC [[deprecated("use Vec3<T> instead")]] = Vec3<T>;

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
