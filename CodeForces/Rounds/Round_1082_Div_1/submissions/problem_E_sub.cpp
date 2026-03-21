//===----------------------------------------------------------------------===//
/**
 * @file: problem_E_sub.cpp
 * @generated: 2026-03-21 22:37:05
 * @source: problem_E.cpp
 * @author: Costantino Lombardi
 *
 * @brief: Codeforces Round 1082 (Div. 1) - Problem E
 */
//===----------------------------------------------------------------------===//
/* Included library and Compiler Optimizations */
#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define CP_IO_PROFILE_SIMPLE

// Compiler optimizations and target-specific features:
#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC optimize("Ofast,unroll-loops,fast-math,O3,inline-functions")
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-result"
  // Keep architecture-specific target tuning opt-in for judge portability.
  #ifdef CP_ENABLE_ARCH_TARGET_PRAGMAS
    #if defined(__x86_64__) && !defined(__MINGW32__) && !defined(__MINGW64__)
      #pragma GCC target("avx2,bmi,bmi2,popcnt,lzcnt,sse4.2,fma")
    #endif
    #if defined(__aarch64__) && !defined(__MINGW32__) && !defined(__MINGW64__)
      #pragma GCC target("+simd,+crypto,+fp16")
    #endif
  #endif
#endif

#ifdef __clang__
  #pragma clang optimize on
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wunused-result"
#endif

// Conditional header inclusion based on availability.
#if defined(__has_include) && __has_include(<bits/stdc++.h>)
  #include <bits/stdc++.h>
#else
  // Portable fallback for environments without "bits/stdc++.h".
  #include <algorithm>
  #include <array>
  #include <bit>
  #include <bitset>
  #include <cassert>
  #include <chrono>
  #include <cmath>
  #include <complex>
  #include <concepts>
  #include <cstddef>
  #include <cstdint>
  #include <cstring>
  #include <deque>
  #include <exception>
  #include <functional>
  #include <iomanip>
  #include <iostream>
  #include <iterator>
  #include <limits>
  #include <list>
  #include <map>
  #include <numeric>
  #include <optional>
  #include <queue>
  #include <random>
  #include <ranges>
  #include <set>
  #include <span>
  #include <sstream>
  #include <stack>
  #include <string>
  #include <string_view>
  #include <tuple>
  #include <type_traits>
  #include <unordered_map>
  #include <unordered_set>
  #include <utility>
  #include <variant>
  #include <vector>
#endif

// Policy-Based Data Structures (optional).
#if defined(__has_include) && __has_include(<ext/pb_ds/assoc_container.hpp>) && __has_include(<ext/pb_ds/tree_policy.hpp>)
  #include <ext/pb_ds/assoc_container.hpp>
  #include <ext/pb_ds/tree_policy.hpp>
  #ifndef PBDS_AVAILABLE
    #define PBDS_AVAILABLE 1
  #endif
#elif !defined(PBDS_AVAILABLE)
  #define PBDS_AVAILABLE 0
#endif

// Debug utilities:
#ifdef LOCAL
  // Keep debug core always on; expensive debug subsystems are opt-in.
  #ifndef CP_DEBUG_ENABLE_PERF
    #define CP_DEBUG_ENABLE_PERF 0
  #endif
  #ifndef CP_DEBUG_ENABLE_MEMORY
    #define CP_DEBUG_ENABLE_MEMORY 0
  #endif
  #ifndef CP_DEBUG_ENABLE_WATCH
    #define CP_DEBUG_ENABLE_WATCH 0
  #endif
  #ifndef CP_DEBUG_ENABLE_TREE
    #define CP_DEBUG_ENABLE_TREE 0
  #endif
  #include "debug.h"
#else
  #define debug(...)
  #define debug_if(...)
  #define debug_tree(...)
  #define debug_tree_verbose(...)
  #define debug_line()
  #define my_assert(...)
  #define COUNT_CALLS(...)
#endif

#ifdef __clang__
  #pragma clang diagnostic pop
#endif

#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC diagnostic pop
#endif
#ifndef __TYPES__
#define __TYPES__

//===----------------------------------------------------------------------===//
/* Core Type System and Aliases */

// Fundamental signed/unsigned integer aliases with explicit bit widths.
using I8  = std::int8_t;
using I16 = std::int16_t;
using I32 = std::int32_t;
using I64 = std::int64_t;
using U8  = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;

// Extended precision integer aliases (when available).
#ifdef __SIZEOF_INT128__
  using I128 = __int128;
  using U128 = unsigned __int128;
  #define HAS_INT128 1
#else
  // Fallback keeps APIs available on toolchains without native int128.
  using I128 = std::int64_t;
  using U128 = std::uint64_t;
  #define HAS_INT128 0
#endif

// Floating-point aliases.
using F32 = float;
using F64 = double;
using F80 = long double;

#ifdef __FLOAT128__
  using F128 = __float128;
  #define HAS_FLOAT128 1
#else
  // Fallback keeps APIs available on toolchains without float128.
  using F128 = long double;
  #define HAS_FLOAT128 0
#endif

template <typename T>
using Limits = std::numeric_limits<T>;

// Common standard scalar aliases.
using Size = std::size_t;
using Diff = std::ptrdiff_t;
using Byte = std::byte;

// String aliases.
using String = std::string;
using StringView = std::string_view;

// Sequence containers.
template <class T>
using Vec = std::vector<T>;
template <class T>
using Deque = std::deque<T>;
template <class T>
using List = std::list<T>;
template <class T, Size N>
using Array = std::array<T, N>;
template <Size N>
using BitSet = std::bitset<N>;

// Associative containers.
template <class T>
using Set = std::set<T>;
template <class T>
using MultiSet = std::multiset<T>;
template <class K, class V>
using Map = std::map<K, V>;
template <class K, class V>
using MultiMap = std::multimap<K, V>;
template <class T>
using UnorderedSet = std::unordered_set<T>;
template <class K, class V>
using UnorderedMap = std::unordered_map<K, V>;

// Extended associative aliases with explicit comparator/hash control.
template <class T, class Compare>
using OrderedSetBy = std::set<T, Compare>;
template <class T, class Compare>
using OrderedMultiSetBy = std::multiset<T, Compare>;
template <class K, class V, class Compare>
using OrderedMapBy = std::map<K, V, Compare>;
template <class K, class V, class Compare>
using OrderedMultiMapBy = std::multimap<K, V, Compare>;
template <class T, class Hash, class Eq = std::equal_to<T>>
using HashedSetBy = std::unordered_set<T, Hash, Eq>;
template <class K, class V, class Hash, class Eq = std::equal_to<K>>
using HashedMapBy = std::unordered_map<K, V, Hash, Eq>;

// Container adaptors.
template <class T>
using Stack = std::stack<T, std::deque<T>>;
template <class T>
using Queue = std::queue<T, std::deque<T>>;
template <class T>
using PriorityQueue = std::priority_queue<T, std::vector<T>>;
template <class T>
using MinPriorityQueue = std::priority_queue<T, std::vector<T>, std::greater<T>>;

// Extended adaptor aliases with explicit container/comparator control.
template <class T, class Container>
using StackIn = std::stack<T, Container>;
template <class T, class Container>
using QueueIn = std::queue<T, Container>;
template <class T, class Container, class Compare>
using PriorityQueueBy = std::priority_queue<T, Container, Compare>;
template <class T, class Container = std::vector<T>>
using MinPriorityQueueIn = std::priority_queue<T, Container, std::greater<T>>;

// Utility wrappers.
template <class T, class U>
using Pair = std::pair<T, U>;
template <class... Args>
using Tuple = std::tuple<Args...>;
template <class T>
using Optional = std::optional<T>;
template <class... Ts>
using Variant = std::variant<Ts...>;
template <class Signature>
using Function = std::function<Signature>;
template <class T>
using Span = std::span<T>;

// Canonical multidimensional aliases.
template <class T>
using Vec2D = Vec<Vec<T>>;
template <class T>
using Vec3D = Vec<Vec2D<T>>;
template <class T>
using Vec4D = Vec<Vec3D<T>>;

// Short aliases for convenience (used by IO macros and legacy code).
template <class T>
using Vec2 = Vec2D<T>;
template <class T>
using Vec3 = Vec3D<T>;
template <class T>
using Vec4 = Vec4D<T>;

// Specialized frequently-used aliases.
using VecI32  = Vec<I32>;
using VecI64  = Vec<I64>;
using VecBool = Vec<bool>;
using VecStr  = Vec<String>;
using VecU8   = Vec<U8>;
using VecU16  = Vec<U16>;
using VecU32  = Vec<U32>;
using VecU64  = Vec<U64>;
using VecF64  = Vec<F64>;
using VecF80  = Vec<F80>;

// Common pair aliases.
template <class T, class U>
using VecPair = Vec<Pair<T, U>>;

using PairI32    = Pair<I32, I32>;
using PairI64    = Pair<I64, I64>;
using PairF80    = Pair<F80, F80>;
using VecPairI32 = Vec<PairI32>;
using VecPairI64 = Vec<PairI64>;

#endif // __TYPES__

//===----------------------------------------------------------------------===//
/* Mathematical Constants and Infinity Values */

// High-precision mathematical constants:
constexpr F80 PI   = 3.1415926535897932384626433832795028841971693993751L;
constexpr F80 E    = 2.7182818284590452353602874713526624977572470937000L;
constexpr F80 PHI  = 1.6180339887498948482045868343656381177203091798058L;
constexpr F80 LN2  = 0.6931471805599453094172321214581765680755001343602L;
constexpr F80 EPS  = 1e-9L;
constexpr F80 DEPS = 1e-12L;

// Robust infinity system:
template <class T>
constexpr T infinity = std::numeric_limits<T>::max() / 4;

template <class T>
constexpr T neg_infinity = std::numeric_limits<T>::lowest() / 4;

template <>
inline constexpr I32 infinity<I32> = 1'010'000'000;
template <>
inline constexpr I64 infinity<I64> = 2'020'000'000'000'000'000LL;
template <>
inline constexpr U32 infinity<U32> = 2'020'000'000U;
template <>
inline constexpr U64 infinity<U64> = 4'040'000'000'000'000'000ULL;
template <>
inline constexpr F64 infinity<F64> = 1e18;
template <>
inline constexpr F80 infinity<F80> = 1e18L;

template <>
inline constexpr I32 neg_infinity<I32> = -infinity<I32>;
template <>
inline constexpr I64 neg_infinity<I64> = -infinity<I64>;
template <>
inline constexpr U32 neg_infinity<U32> = 0U;
template <>
inline constexpr U64 neg_infinity<U64> = 0ULL;
template <>
inline constexpr F64 neg_infinity<F64> = -infinity<F64>;
template <>
inline constexpr F80 neg_infinity<F80> = -infinity<F80>;

#if HAS_INT128
  static_assert(sizeof(I128) > sizeof(I64), "I128 must be true 128-bit when HAS_INT128 is enabled.");
  template <>
  inline constexpr I128 infinity<I128> = I128(infinity<I64>) * 2'000'000'000'000'000'000LL;
  template <>
  inline constexpr I128 neg_infinity<I128> = -infinity<I128>;
#endif

constexpr I32 INF32  = infinity<I32>;
constexpr I64 INF64  = infinity<I64>;
constexpr I64 LINF   = INF64; // Legacy alias
constexpr I32 NINF32 = neg_infinity<I32>;
constexpr I64 NINF64 = neg_infinity<I64>;
constexpr I64 NLINF  = NINF64; // Legacy-style alias

// Powers of ten lookup table (10^k for k = 0..18):
constexpr I64 POW10[] = {
    1LL, 10LL, 100LL, 1000LL, 10000LL, 100000LL,
    1000000LL, 10000000LL, 100000000LL, 1000000000LL,
    10000000000LL, 100000000000LL, 1000000000000LL,
    10000000000000LL, 100000000000000LL, 1000000000000000LL,
    10000000000000000LL, 100000000000000000LL, 1000000000000000000LL,
};

// Modular arithmetic constants:
constexpr I64 MOD  = 1'000'000'007LL;
constexpr I64 MOD2 = 998'244'353LL;
constexpr I64 MOD3 = 1'000'000'009LL;
constexpr I64 INV2 = (MOD + 1) / 2;

//===----------------------------------------------------------------------===//
/* Advanced Macro System */

// Advanced FOR loop system:
#define FOR1(a) for (I64 _ = 0; _ < (a); ++_)
#define FOR2(i, a) for (I64 i = 0; i < (a); ++i)
#define FOR3(i, a, b) for (I64 i = (a); i < (b); ++i)
#define FOR4(i, a, b, c) for (I64 i = (a); i < (b); i += (c))
#define FOR1_R(a) for (I64 _ = (a) - 1; _ >= 0; --_)
#define FOR2_R(i, a) for (I64 i = (a) - 1; i >= 0; --i)
#define FOR3_R(i, a, b) for (I64 i = (b) - 1; i >= (a); --i)

// Overload resolution for FOR macros:
#define overload4(a, b, c, d, e, ...) e
#define overload3(a, b, c, d, ...) d
#define FOR(...) overload4(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)(__VA_ARGS__)
#define FOR_R(...) overload3(__VA_ARGS__, FOR3_R, FOR2_R, FOR1_R)(__VA_ARGS__)

// Range-based iteration macros:
#define ALL(x) std::ranges::begin(x), std::ranges::end(x)
#define RALL(x) std::ranges::rbegin(x), std::ranges::rend(x)

// Container utility macros:
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x) (I64)(x).size()
#define len(x) sz(x)
#define eb emplace_back
#define elif else if

// Advanced container operations:
#define UNIQUE(x) (std::ranges::sort(x), x.erase(std::ranges::unique(x).begin(), x.end()), x.shrink_to_fit())
#define LB(c, x) (I64)std::distance((c).begin(), std::ranges::lower_bound(c, x))
#define UB(c, x) (I64)std::distance((c).begin(), std::ranges::upper_bound(c, x))
#define SUM(x) std::accumulate(all(x), std::iter_value_t<decltype((x).begin())>{})
#define MIN(x) ([&]() -> decltype(auto) { \
  auto&& _cp_min_range = (x); \
  if (std::ranges::empty(_cp_min_range)) std::abort(); \
  return *std::ranges::min_element(_cp_min_range); \
}())
#define MAX(x) ([&]() -> decltype(auto) { \
  auto&& _cp_max_range = (x); \
  if (std::ranges::empty(_cp_max_range)) std::abort(); \
  return *std::ranges::max_element(_cp_max_range); \
}())

// Y-combinator for recursive lambdas:
template <class F>
struct YCombinator {
  F fn;
  template <class... Args>
  decltype(auto) operator()(Args&&... args) const {
    return fn(*this, std::forward<Args>(args)...);
  }
};

template <class F>
YCombinator(F) -> YCombinator<F>;

template <class F>
[[gnu::always_inline]] constexpr auto fix(F&& fn) {
  return YCombinator<std::decay_t<F>>{std::forward<F>(fn)};
}

// Type-safe cast alias:
template <typename To>
[[gnu::always_inline]] constexpr To as(auto&& x) noexcept {
  return static_cast<To>(std::forward<decltype(x)>(x));
}

template <typename To>
[[gnu::always_inline]] constexpr To narrow_as(auto x) {
  To converted = static_cast<To>(x);
  return converted;
}

//===----------------------------------------------------------------------===//
/* Lightweight I/O Utilities */

namespace cp_io {

inline void setup() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::cout << std::fixed << std::setprecision(10);
}

struct IOSetup {
  IOSetup() { setup(); }
};
inline IOSetup io_setup;

template <class T>
void read(T& x) { std::cin >> x; }

template <class T>
void write(const T& x) { std::cout << x; }

template <class T, class U>
inline void read(Pair<T, U>& p) { read(p.first); read(p.second); }

template <class T>
inline void read(Vec<T>& v) { for (auto& x : v) read(x); }

template <typename... Args>
inline void read(std::tuple<Args...>& t) { std::apply([](auto&... args) { (read(args), ...); }, t); }

template <class Head, class... Tail>
  requires (sizeof...(Tail) > 0)
inline void read(Head& head, Tail&... tail) { read(head); read(tail...); }

template <class T, class U>
inline void write(const Pair<T, U>& p) { write(p.first); write(' '); write(p.second); }

template <class T>
inline void write(const Vec<T>& v) {
  for (I64 i = 0; i < as<I64>(v.size()); ++i) {
    if (i) write(' ');
    write(v[i]);
  }
}

template <typename... Args>
inline void write(const std::tuple<Args...>& t) {
  I32 i = 0;
  std::apply([&i](const auto&... args) {
    ((i++ > 0 ? (write(' '), 0) : 0, write(args)), ...);
  }, t);
}

template <class Head, class... Tail>
  requires (sizeof...(Tail) > 0)
inline void write(const Head& head, const Tail&... tail) { write(head); write(' '); write(tail...); }

inline void writeln() { write('\n'); }

template <class... Args>
inline void writeln(const Args&... args) { if constexpr (sizeof...(args) > 0) write(args...); writeln(); }

} // namespace cp_io

namespace fast_io {
template <class T>
inline void read_integer(T& x) { cp_io::read(x); }
inline void read_char(char& x) { cp_io::read(x); }
inline void read_string(String& x) { cp_io::read(x); }

template <class T>
inline void write_integer(T x) { cp_io::write(x); }
inline void write_char(char c) { std::cout.put(c); }
inline void write_string(const String& s) { cp_io::write(s); }

inline void flush_output() { std::cout.flush(); }

using cp_io::read;
using cp_io::write;
using cp_io::writeln;
} // namespace fast_io

#if defined(CP_FAST_IO_NAMESPACE_DEFINED)
  #define CP_IO_IMPL_READ(...) fast_io::read(__VA_ARGS__)
  #define CP_IO_IMPL_WRITELN(...) fast_io::writeln(__VA_ARGS__)
  #define CP_IO_IMPL_FLUSH() fast_io::flush_output()
#else
  #define CP_IO_IMPL_READ(...) cp_io::read(__VA_ARGS__)
  #define CP_IO_IMPL_WRITELN(...) cp_io::writeln(__VA_ARGS__)
  #define CP_IO_IMPL_FLUSH() std::cout.flush()
#endif

//===----------------------------------------------------------------------===//
/* Shared I/O Macro and Answer Helper Definitions */

#define IN(...) CP_IO_IMPL_READ(__VA_ARGS__)
#define OUT(...) CP_IO_IMPL_WRITELN(__VA_ARGS__)
#define FLUSH() CP_IO_IMPL_FLUSH()

#ifndef CP_IO_DECL_MACROS_DEFINED
  #define CP_IO_DECL_MACROS_DEFINED 1
  #define INT(...) I32 __VA_ARGS__; IN(__VA_ARGS__)
  #define LL(...) I64 __VA_ARGS__; IN(__VA_ARGS__)
  #define ULL(...) U64 __VA_ARGS__; IN(__VA_ARGS__)
  #define STR(...) std::string __VA_ARGS__; IN(__VA_ARGS__)
  #define CHR(...) char __VA_ARGS__; IN(__VA_ARGS__)
  #define DBL(...) F64 __VA_ARGS__; IN(__VA_ARGS__)

  #ifndef CP_ENABLE_LEGACY_IO_VEC_MACROS
    #define CP_ENABLE_LEGACY_IO_VEC_MACROS 1
  #endif

  #if CP_ENABLE_LEGACY_IO_VEC_MACROS
    #define VEC(type, name, size) Vec<type> name(size); IN(name)
    #define VV(type, name, h, w) Vec2<type> name(h, Vec<type>(w)); IN(name)
  #endif
#endif

  #define CP_IO_ANSWER_HELPERS_DEFINED 1
  inline void YES(bool condition = true) { OUT(condition ? "YES" : "NO"); }
  inline void NO(bool condition = true) { YES(!condition); }
  inline void Yes(bool condition = true) { OUT(condition ? "Yes" : "No"); }
  inline void No(bool condition = true) { Yes(!condition); }

//===----------------------------------------------------------------------===//
/* Core Concepts for Type-Safe CP Templates */

namespace cp {

template <class T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <class T>
concept Integral = std::integral<remove_cvref_t<T>>;

template <class T>
concept SignedIntegral = Integral<T> && std::is_signed_v<remove_cvref_t<T>>;

template <class T>
concept UnsignedIntegral = Integral<T> && !SignedIntegral<T>;

template <class T>
concept NonBoolIntegral = Integral<T> && !std::same_as<remove_cvref_t<T>, bool>;

template <class T>
concept Floating = std::floating_point<remove_cvref_t<T>>;

template <class T>
concept Arithmetic = Integral<T> || Floating<T>;

template <class T>
concept IndexLike = NonBoolIntegral<T>;

template <class T>
concept Enum = std::is_enum_v<remove_cvref_t<T>>;

template <class F, class... Args>
concept Predicate = std::predicate<F, Args...>;

template <class R>
concept Range = std::ranges::range<remove_cvref_t<R>>;

template <class R>
concept SizedRange = Range<R> && requires(remove_cvref_t<R> r) { std::ranges::size(r); };

template <class T>
concept StreamReadable = requires(std::istream& is, T& value) {
  { is >> value } -> std::same_as<std::istream&>;
};

template <class T>
concept StreamWritable = requires(std::ostream& os, const T& value) {
  { os << value } -> std::same_as<std::ostream&>;
};

template <class T>
concept Hashable = requires(const remove_cvref_t<T>& value) {
  { std::hash<remove_cvref_t<T>>{}(value) } -> std::convertible_to<std::size_t>;
};

} // namespace cp

//===----------------------------------------------------------------------===//
/* Lightweight Stopwatch Utility */

struct Stopwatch {
  using Clock = std::chrono::high_resolution_clock;
  Clock::time_point start;

  Stopwatch() : start(Clock::now()) {}
  void reset() { start = Clock::now(); }
  [[gnu::always_inline]] F64 elapsed() const { return std::chrono::duration<F64>(Clock::now() - start).count(); }
  [[gnu::always_inline]] bool within(F64 limit) const { return elapsed() < limit; }
};

//===----------------------------------------------------------------------===//
/* Mathematical Utilities */

namespace cp::detail {

template <std::unsigned_integral T>
[[gnu::always_inline]] constexpr T mul_mod_unsigned(T a, T b, T mod) {
#if HAS_INT128
  if constexpr (sizeof(T) <= sizeof(U64)) {
    return as<T>((as<U128>(a) * as<U128>(b)) % as<U128>(mod));
  }
#endif

  // Portable fallback for toolchains without native 128-bit multiplication.
  T result = 0;
  while (b > 0) {
    if ((b & 1U) != 0U) {
      if (result >= mod - a) result -= (mod - a);
      else result += a;
    }
    b >>= 1U;
    if (b == 0) break;
    if (a >= mod - a) a -= (mod - a);
    else a += a;
  }
  return result;
}

} // namespace cp::detail

// Integer division and modulus with floor/ceil semantics:
template <cp::NonBoolIntegral T>
[[gnu::always_inline]] constexpr T div_floor(T a, T b) {
  my_assert(b != 0);
  if constexpr (std::is_signed_v<T>) {
    T q = a / b;
    T r = a % b;
    if (r != 0 && ((r > 0) != (b > 0))) --q;
    return q;
  } else {
    return a / b;
  }
}

template <cp::NonBoolIntegral T>
[[gnu::always_inline]] constexpr T div_ceil(T a, T b) {
  my_assert(b != 0);
  if constexpr (std::is_signed_v<T>) {
    T q = a / b;
    T r = a % b;
    if (r != 0 && ((r > 0) == (b > 0))) ++q;
    return q;
  } else {
    return a / b + (a % b != 0);
  }
}

template <cp::NonBoolIntegral T>
[[gnu::always_inline]] constexpr T mod_floor(T a, T b) {
  return a - b * div_floor(a, b);
}

template <cp::NonBoolIntegral T>
[[gnu::always_inline]] constexpr std::pair<T, T> divmod(T a, T b) {
  T q = div_floor(a, b);
  return {q, a - q * b};
}

// Exponentiation without modulus:
template <cp::NonBoolIntegral T>
[[gnu::always_inline]] constexpr T power(T base, T exp) {
  T result = 1;
  while (exp > 0) {
    if (exp & 1) result *= base;
    base *= base;
    exp >>= 1;
  }
  return result;
}

// Modular exponentiation:
template <cp::NonBoolIntegral T>
[[gnu::always_inline]] constexpr T mod_pow(T base, T exp, T mod) {
  my_assert(mod != 0);
  if constexpr (std::is_signed_v<T>) {
    my_assert(mod > 0);
    my_assert(exp >= 0);
    if (mod <= 0) return 0;
    if (exp < 0) return 0;
  }

  using U = std::make_unsigned_t<T>;
  U umod   = as<U>(mod);
  U uexp   = as<U>(exp);
  U ubase  = as<U>(mod_floor(base, mod));
  U result = as<U>(1) % umod;

  while (uexp > 0) {
    if ((uexp & 1U) != 0U) {
      result = cp::detail::mul_mod_unsigned(result, ubase, umod);
    }
    ubase = cp::detail::mul_mod_unsigned(ubase, ubase, umod);
    uexp >>= 1U;
  }
  return as<T>(result);
}

template <cp::NonBoolIntegral T>
[[gnu::always_inline]] inline T floor_sqrt(T x) {
  if constexpr (std::is_signed_v<T>) {
    my_assert(x >= 0);
    if (x < 0) return 0;
  }

  using U = std::make_unsigned_t<T>;
  const U ux = as<U>(x);
  if (ux <= 1) return as<T>(ux);

  U r = as<U>(std::sqrt(as<F80>(ux)));
  while ((r + 1) <= ux / (r + 1)) ++r;
  while (r > ux / r) --r;
  return as<T>(r);
}

template <cp::NonBoolIntegral T>
[[gnu::always_inline]] inline T ceil_sqrt(T x) {
  using U = std::make_unsigned_t<T>;
  const T root = floor_sqrt(x);
  const U uf = as<U>(root);
  if (uf == 0) return 0;

  const U ux = as<U>(x);
  if (ux / uf == uf && ux % uf == 0) return root;
  return as<T>(uf + 1);
}

template <class T, class S, class Compare = std::less<>>
[[gnu::always_inline]] inline bool chmax(T& a, const S& b, const Compare& cmp = {}) {
  return cmp(a, b) ? (a = b, true) : false;
}

template <class T, class S, class Compare = std::less<>>
[[gnu::always_inline]] inline bool chmin(T& a, const S& b, const Compare& cmp = {}) {
  return cmp(b, a) ? (a = b, true) : false;
}

// Seeded random number generator:
inline std::mt19937_64 rng(as<U64>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

template <cp::Integral T>
inline T rnd(T a, T b) { return std::uniform_int_distribution<T>(a, b)(rng); }

template <cp::Floating T>
inline T rnd(T a, T b) { return std::uniform_real_distribution<T>(a, b)(rng); }

// Variadic min/max:
template <typename T>
constexpr const T& _min(const T& a, const T& b) { return (b < a) ? b : a; }

template <typename T>
constexpr const T& _max(const T& a, const T& b) { return (a < b) ? b : a; }

template <typename T, typename... Args>
constexpr const T& _min(const T& a, const T& b, const Args&... args) { return _min(a, _min(b, args...)); }

template <typename T, typename... Args>
constexpr const T& _max(const T& a, const T& b, const Args&... args) { return _max(a, _max(b, args...)); }

//===----------------------------------------------------------------------===//
/* Data Structures & Algorithms for the Problem */

struct NTT {
  static constexpr I64 ROOT = 3;  // Primitive root of MOD2.
  static constexpr I32 MAX_LOG_N = 23;  // 998244353 = 119 * 2^23 + 1
  static constexpr I32 MAX_SIZE = 1 << MAX_LOG_N;

  static void ntt(VecI64& a, bool inverse) {
    I32 n = sz(a);
    if (n == 0) return;
    my_assert((n & (n - 1)) == 0);
    my_assert(n <= MAX_SIZE);
    if (n == 1) return;

    // Bit reversal (iterative Gray-code style).
    for (I32 i = 1, j = 0; i < n; ++i) {
      I32 bit = n >> 1;
      while (j & bit) {
        j ^= bit;
        bit >>= 1;
      }
      j ^= bit;
      if (i < j) std::swap(a[i], a[j]);
    }

    // NTT computation.
    for (I32 len = 2; len <= n; len <<= 1) {
      I64 w = inverse ? mod_pow(ROOT, MOD2 - 1 - (MOD2 - 1) / len, MOD2)
                      : mod_pow(ROOT, (MOD2 - 1) / len, MOD2);

      for (I32 i = 0; i < n; i += len) {
        I64 wn = 1;
        FOR(j, len / 2) {
          I64 u = a[i + j];
          I64 v = a[i + j + len / 2] * wn % MOD2;
          a[i + j] = (u + v) % MOD2;
          a[i + j + len / 2] = (u - v + MOD2) % MOD2;
          wn = wn * w % MOD2;
        }
      }
    }

    if (inverse) {
      I64 n_inv = mod_pow<I64>(n, MOD2 - 2, MOD2);
      FOR(i, n) a[i] = a[i] * n_inv % MOD2;
    }
  }

  static Vec<I64> multiply(Vec<I64> a, Vec<I64> b) {
    if (a.empty() || b.empty()) return {};
    I32 result_size = sz(a) + sz(b) - 1;
    I32 n = 1;
    while (n < result_size) n <<= 1;
    my_assert(n <= MAX_SIZE);

    a.resize(n);
    b.resize(n);

    ntt(a, false);
    ntt(b, false);

    FOR(i, n) a[i] = a[i] * b[i] % MOD2;

    ntt(a, true);
    a.resize(result_size);

    return a;
  }
};

//===----------------------------------------------------------------------===//
/* Main Solver Function */

constexpr I32 MAX_N = 400000;

VecI64 pow2_mod;

auto get_divisors(I32 x) -> VecI32 {
  VecI32 divs;
  for (I32 d = 1; d * d <= x; ++d) {
    if (x % d != 0) {
      continue;
    }
    divs.eb(d);
    if (d * d != x) {
      divs.eb(x / d);
    }
  }
  std::sort(all(divs));
  return divs;
}

auto circular_corr(const VecI64& a, const VecI64& b) -> VecI64 {
  const I32 d = as<I32>(a.size());
  VecI64 a2(2 * d, 0);
  VecI64 br(d, 0);

  FOR(i, d) {
    a2[i] = a[i];
    a2[i + d] = a[i];
    br[i] = b[d - 1 - i];
  }

  const VecI64 conv = NTT::multiply(a2, br);
  VecI64 res(d, 0);
  FOR(r, d) res[r] = conv[r + d - 1];
  return res;
}

auto build_superset_counts(const String& t, I32 m, I32 d) -> VecI64 {
  VecI32 p_mask(d, 0);
  VecI32 q_mask(d, 0);

  bool bad = false;
  FOR(i, m) {
    const char ch = t[i];
    if (ch == '?') {
      continue;
    }
    const I32 bit = (ch == 'a' ? 1 : 2);
    I32& cur = p_mask[i % d];
    cur |= bit;
    if (cur == 3) {
      bad = true;
      break;
    }
  }

  if (bad) {
    return VecI64(d, 0);
  }

  FOR(i, m) {
    const char ch = t[m + i];
    if (ch == '?') {
      continue;
    }
    const I32 bit = (ch == 'a' ? 1 : 2);
    I32& cur = q_mask[i % d];
    cur |= bit;
    if (cur == 3) {
      bad = true;
      break;
    }
  }

  if (bad) {
    return VecI64(d, 0);
  }

  VecI64 a_a(d, 0), a_b(d, 0), a_q(d, 0);
  VecI64 b_a(d, 0), b_b(d, 0), b_q(d, 0);

  FOR(x, d) {
    if (p_mask[x] == 0) a_q[x] = 1;
    if (p_mask[x] == 1) a_a[x] = 1;
    if (p_mask[x] == 2) a_b[x] = 1;

    if (q_mask[x] == 0) b_q[x] = 1;
    if (q_mask[x] == 1) b_a[x] = 1;
    if (q_mask[x] == 2) b_b[x] = 1;
  }

  const VecI64 ab = circular_corr(a_a, b_b);
  const VecI64 ba = circular_corr(a_b, b_a);
  const VecI64 qq = circular_corr(a_q, b_q);

  VecI64 f(d, 0);
  FOR(r, d) {
    const I64 conflicts = ab[r] + ba[r];
    if (conflicts != 0) {
      continue;
    }
    const I32 free_cnt = as<I32>(qq[r]);
    f[r] = pow2_mod[free_cnt];
  }

  return f;
}

void solve() {
  INT(n);
  STR(t);

  const I32 m = n / 2;
  const VecI32 divs = get_divisors(m);
  const I32 k = as<I32>(divs.size());

  Vec<VecI64> exact(k);
  I64 answer = 0;

  FOR(i, k) {
    const I32 d = divs[i];
    VecI64 cur = build_superset_counts(t, m, d);

    FOR(j, i) {
      const I32 small_d = divs[j];
      if (d % small_d != 0) {
        continue;
      }
      const VecI64& small = exact[j];
      FOR(r, d) {
        cur[r] -= small[r % small_d];
        if (cur[r] < 0) {
          cur[r] += MOD2;
        }
      }
    }

    for (I64 x : cur) {
      answer += x;
      if (answer >= MOD2) {
        answer -= MOD2;
      }
    }

    exact[i] = std::move(cur);
  }

  OUT(answer);
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  pow2_mod.assign(MAX_N + 1, 1);
  FOR(i, 1, MAX_N + 1) {
    pow2_mod[i] = (pow2_mod[i - 1] * 2) % MOD2;
  }

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
