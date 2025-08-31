#pragma once
#include "Preamble.hpp"

//===----------------------------------------------------------------------===//
/* Advanced Type System and Aliases */

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

// Extended precision types (when available):
#ifdef __SIZEOF_INT128__
using I128 = __int128;
using U128 = unsigned __int128;
#else
// Fallback for compilers that don't support 128-bit integers
using I128 = std::int64_t;
using U128 = std::uint64_t;
#endif

#ifdef __FLOAT128__
using F128 = __float128;
#else
// Fallback for compilers that don't support 128-bit floats
using F128 = long double;
#endif

// Legacy aliases for backward compatibility:
using ll  = I64;
using ull = U64;
using ld  = F80;

// Container type aliases with template parameters:
template <class T>
using VC = std::vector<T>;
template <class T>
using VVC = VC<VC<T>>;
template <class T>
using VVVC = VC<VVC<T>>;
template <class T>
using VVVVC = VC<VVVC<T>>;

// Specialized container aliases:
using VI   = VC<I64>;
using VVI  = VVC<I64>;
using VVVI = VVVC<I64>;
using VB   = VC<bool>;
using VS   = VC<std::string>;
using VU8  = VC<U8>;
using VU32 = VC<U32>;
using VU64 = VC<U64>;

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

// Advanced container aliases:
template <class K, class V>
using UMap = std::unordered_map<K, V>;
template <class T>
using USet = std::unordered_set<T>;
template <class T>
using MSet = std::multiset<T>;

// Policy-based data structures:
using namespace __gnu_pbds;
template <typename T>
using ordered_set = tree<T, null_type, std::less<T>, rb_tree_tag, tree_order_statistics_node_update>;
template <typename T>
using ordered_multiset = tree<T, null_type, std::less_equal<T>, rb_tree_tag, tree_order_statistics_node_update>;
