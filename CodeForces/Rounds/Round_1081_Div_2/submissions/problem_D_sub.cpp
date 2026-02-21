//===----------------------------------------------------------------------===//
/**
 * @file: problem_D_sub.cpp
 * @generated: 2026-02-21 16:11:33
 * @source: problem_D.cpp
 * @author: Costantino Lombardi
 *
 * @brief: Codeforces Round 1081 (Div. 2) - Problem D
 */
//===----------------------------------------------------------------------===//
/* Included library and Compiler Optimizations */

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
using Vec2 = Vec<Vec<T>>;
template <class T>
using Vec3 = Vec<Vec2<T>>;
template <class T>
using Vec4 = Vec<Vec3<T>>;

// Specialized frequently-used aliases.
using VI   = Vec<I32>;
using VLL  = Vec<I64>;
using VVI  = Vec<VI>;
using VVLL = Vec<VLL>;
using VB   = Vec<bool>;
using VS   = Vec<String>;
using VU8  = Vec<U8>;
using VU16 = Vec<U16>;
using VU32 = Vec<U32>;
using VU64 = Vec<U64>;
using VF   = Vec<F64>;
using VLD  = Vec<F80>;

// Common pair aliases.
template <class T, class U>
using VecPair = Vec<Pair<T, U>>;

using PII = Pair<I32, I32>;
using PLL = Pair<I64, I64>;
using PLD = Pair<F80, F80>;
using VPII = Vec<PII>;
using VPLL = Vec<PLL>;

//===----------------------------------------------------------------------===//
/* Advanced Macro System */

#ifdef NEED_NDVEC
template <class T>
auto make_nd_vec(std::size_t size) {
  return Vec<T>(size);
}

template <class T>
auto make_nd_vec(std::size_t size, const T& value) {
  return Vec<T>(size, value);
}

template <class T>
auto make_vec2(std::size_t n1, std::size_t n2) {
  return Vec(n1, Vec<T>(n2));
}

template <class T>
auto make_vec2(std::size_t n1, std::size_t n2, const T& value) {
  return Vec(n1, Vec<T>(n2, value));
}

template <class T>
auto make_vec3(std::size_t n1, std::size_t n2, std::size_t n3) {
  return Vec(n1, Vec(n2, Vec<T>(n3)));
}

template <class T>
auto make_vec3(std::size_t n1, std::size_t n2, std::size_t n3, const T& value) {
  return Vec(n1, Vec(n2, Vec<T>(n3, value)));
}

template <class T>
auto make_vec4(std::size_t n1, std::size_t n2, std::size_t n3, std::size_t n4) {
  return Vec(n1, Vec(n2, Vec(n3, Vec<T>(n4))));
}

template <class T>
auto make_vec4(std::size_t n1, std::size_t n2, std::size_t n3, std::size_t n4, const T& value) {
  return Vec(n1, Vec(n2, Vec(n3, Vec<T>(n4, value))));
}

// Multi-dimensional vector creation macros:
#define make_vec(type, name, ...) Vec<type> name(__VA_ARGS__)
#define vv(type, name, h, ...) \
  auto name = make_vec2<type>(h, __VA_ARGS__)
#define vvv(type, name, h, w, ...) \
  auto name = make_vec3<type>(h, w, __VA_ARGS__)
#define vvvv(type, name, a, b, c, ...) \
  auto name = make_vec4<type>(a, b, c, __VA_ARGS__)
#endif

// Advanced FOR loop system:
#define FOR1(a) for (I64 _ = 0; _ < (a); ++_)
#define FOR2(i, a) for (I64 i = 0; i < (a); ++i)
#define FOR3(i, a, b) for (I64 i = (a); i < (b); ++i)
#define FOR4(i, a, b, c) for (I64 i = (a); i < (b); i += (c))
#define FOR1_R(a) for (I64 i = (a) - 1; i >= 0; --i)
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
#define SUM(x) std::accumulate(all(x), 0LL)
#define MIN(x) *std::ranges::min_element(x)
#define MAX(x) *std::ranges::max_element(x)

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
[[gnu::always_inline]] constexpr To as(auto x) noexcept {
  return static_cast<To>(x);
}

//===----------------------------------------------------------------------===//
/* Mathematical Utilities */

// Integer division and modulus with floor/ceil semantics:
template <typename T>
[[gnu::always_inline]] constexpr T div_floor(T a, T b) {
  return a / b - (a % b != 0 && (a ^ b) < 0);
}

template <typename T>
[[gnu::always_inline]] constexpr T div_ceil(T a, T b) {
  if constexpr (std::is_signed_v<T>) {
    return a / b + ((a % b != 0) && ((a ^ b) >= 0));
  } else {
    return a / b + (a % b != 0);
  }
}

template <typename T>
[[gnu::always_inline]] constexpr T mod_floor(T a, T b) {
  return a - b * div_floor(a, b);
}

template <typename T>
[[gnu::always_inline]] constexpr std::pair<T, T> divmod(T a, T b) {
  T q = div_floor(a, b);
  return {q, a - q * b};
}

// Exponentiation without modulus:
template <typename T>
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
template <typename T>
[[gnu::always_inline]] constexpr T mod_pow(T base, T exp, T mod) {
  T result = 1 % mod;
  base %= mod;
  while (exp > 0) {
    if (exp & 1) result = (__int128)result * base % mod;
    base = (__int128)base * base % mod;
    exp >>= 1;
  }
  return result;
}

#ifndef __UTILITY_FUNCTIONS__
template <class T, class S, class Compare = std::less<>>
[[gnu::always_inline]] inline bool chmax(T& a, const S& b, const Compare& cmp = {}) {
  return cmp(a, b) ? (a = b, true) : false;
}

template <class T, class S, class Compare = std::less<>>
[[gnu::always_inline]] inline bool chmin(T& a, const S& b, const Compare& cmp = {}) {
  return cmp(b, a) ? (a = b, true) : false;
}
#endif

// Seeded random number generator:
inline std::mt19937_64 rng(
    static_cast<U64>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

template <typename T>
  requires std::integral<T>
inline T rnd(T a, T b) {
  return std::uniform_int_distribution<T>(a, b)(rng);
}

template <typename T>
  requires std::floating_point<T>
inline T rnd(T a, T b) {
  return std::uniform_real_distribution<T>(a, b)(rng);
}

// High-resolution timer for time-limited heuristics:
struct Stopwatch {
  using Clock = std::chrono::high_resolution_clock;
  Clock::time_point start;

  Stopwatch() : start(Clock::now()) {}

  [[gnu::always_inline]] F64 elapsed() const {
    return std::chrono::duration<F64>(Clock::now() - start).count();
  }

  void reset() { start = Clock::now(); }

  [[gnu::always_inline]] bool within(F64 limit) const {
    return elapsed() < limit;
  }
};

// Variadic min/max:
template <typename T>
constexpr const T& _min(const T& a, const T& b) { return (b < a) ? b : a; }

template <typename T>
constexpr const T& _max(const T& a, const T& b) { return (a < b) ? b : a; }

template <typename T, typename... Args>
constexpr const T& _min(const T& a, const T& b, const Args&... args) {
  return _min(a, _min(b, args...));
}

template <typename T, typename... Args>
constexpr const T& _max(const T& a, const T& b, const Args&... args) {
  return _max(a, _max(b, args...));
}

//===----------------------------------------------------------------------===//
/* Lightweight I/O Utilities */

#if !defined(CP_FAST_IO_NAMESPACE_DEFINED)
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
void read(T& x) {
  std::cin >> x;
}

template <class T, class U>
void read(std::pair<T, U>& p) {
  read(p.first);
  read(p.second);
}

template <class T>
void read(Vec<T>& v) {
  for (auto& x : v) read(x);
}

template <typename... Args>
void read(std::tuple<Args...>& t) {
  std::apply([](auto&... args) { (read(args), ...); }, t);
}

template <class Head, class... Tail>
void read(Head& head, Tail&... tail) {
  read(head);
  if constexpr (sizeof...(tail) > 0) read(tail...);
}

template <class T>
void write_one(const T& x) {
  std::cout << x;
}

template <class T, class U>
void write_one(const std::pair<T, U>& p) {
  write_one(p.first);
  std::cout << ' ';
  write_one(p.second);
}

template <class T>
void write_one(const Vec<T>& v) {
  for (std::size_t i = 0; i < v.size(); ++i) {
    if (i) std::cout << ' ';
    write_one(v[i]);
  }
}

template <typename... Args>
void write_one(const std::tuple<Args...>& t) {
  I32 i = 0;
  std::apply([&i](const auto&... args) {
    ((i++ > 0 ? (std::cout << ' ', 0) : 0, write_one(args)), ...);
  }, t);
}

template <class Head, class... Tail>
void write(const Head& head, const Tail&... tail) {
  write_one(head);
  ((std::cout << ' ', write_one(tail)), ...);
}

inline void writeln() {
  std::cout << '\n';
}

template <class... Args>
void writeln(const Args&... args) {
  if constexpr (sizeof...(args) > 0) write(args...);
  std::cout << '\n';
}

} // namespace cp_io

namespace fast_io {
template <class T>
inline void read_integer(T& x) { cp_io::read(x); }
inline void read_char(char& x) { cp_io::read(x); }
inline void read_string(std::string& x) { cp_io::read(x); }

template <class T>
inline void write_integer(T x) { cp_io::write_one(x); }
inline void write_char(char c) { std::cout.put(c); }
inline void write_string(const std::string& s) { cp_io::write_one(s); }

inline void flush_output() { std::cout.flush(); }

using cp_io::read;
using cp_io::write;
using cp_io::writeln;
} // namespace fast_io
#endif

#ifdef CP_IO_IMPL_READ
  #undef CP_IO_IMPL_READ
#endif
#ifdef CP_IO_IMPL_WRITELN
  #undef CP_IO_IMPL_WRITELN
#endif
#ifdef CP_IO_IMPL_FLUSH
  #undef CP_IO_IMPL_FLUSH
#endif

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

#ifndef CP_IO_ANSWER_HELPERS_DEFINED
  #define CP_IO_ANSWER_HELPERS_DEFINED 1
  inline void YES(bool condition = true) { OUT(condition ? "YES" : "NO"); }
  inline void NO(bool condition = true) { YES(!condition); }
  inline void Yes(bool condition = true) { OUT(condition ? "Yes" : "No"); }
  inline void No(bool condition = true) { Yes(!condition); }
#endif

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
constexpr T neg_infinity = std::numeric_limits<T>::min() / 4;

template <>
inline constexpr I32 infinity<I32> = 1'010'000'000;
template <>
inline constexpr I64 infinity<I64> = 2'020'000'000'000'000'000LL;
template <>
inline constexpr U32 infinity<U32> = 2'020'000'000U;
template <>
inline constexpr U64 infinity<U64> = 4'040'000'000'000'000'000ULL;
template <>
inline constexpr I64 neg_infinity<I64> = -infinity<I64>;
template <>
inline constexpr F64 infinity<F64> = 1e18;
template <>
inline constexpr F80 infinity<F80> = 1e18L;

#if HAS_INT128
  static_assert(sizeof(I128) > sizeof(I64), "I128 must be true 128-bit when HAS_INT128 is enabled.");
  template <>
  inline constexpr I128 infinity<I128> = I128(infinity<I64>) * 2'000'000'000'000'000'000LL;
#endif

constexpr I32 INF32 = infinity<I32>;
constexpr I64 INF64 = infinity<I64>;
constexpr I64 LINF  = INF64; // Legacy alias

// Powers of ten lookup table (10^k for k = 0..18):
constexpr I64 POW10[] = {
    1LL, 10LL, 100LL, 1000LL, 10000LL, 100000LL,
    1000000LL, 10000000LL, 100000000LL, 1000000000LL,
    10000000000LL, 100000000000LL, 1000000000000LL,
    10000000000000LL, 100000000000000LL, 1000000000000000LL,
    10000000000000000LL, 100000000000000000LL, 1000000000000000000LL,
};

// Modular arithmetic constants:
constexpr I64 MOD  = 1000000007;
constexpr I64 MOD2 = 998244353;
constexpr I64 MOD3 = 1000000009;

//===----------------------------------------------------------------------===//
/* Data Structures & Algorithms for the Problem */

struct SegmentTreeHull {
  static constexpr I64 NEG_INF = -4'000'000'000'000'000'000LL;
  static constexpr I64 NO_TAG = NEG_INF;

  I32 n = 0;
  Vec<I64> lazy;
  Vec<I64> max_val;
  Vec<I32> hull_start;
  Vec<I32> hull_len;
  Vec<I64> hull_m;
  Vec<I64> hull_c;

  SegmentTreeHull() = default;

  void init(I32 n_lines, const Vec<Pair<I64, I64>>& lines) {
    n = std::max<I32>(0, n_lines);
    if (n == 0) {
      lazy.clear();
      max_val.clear();
      hull_start.clear();
      hull_len.clear();
      hull_m.clear();
      hull_c.clear();
      return;
    }

    my_assert(as<I32>(lines.size()) >= n + 1);

    const I32 st_size = 4 * n + 8;
    lazy.assign(as<Size>(st_size), NO_TAG);
    max_val.assign(as<Size>(st_size), NEG_INF);
    hull_start.assign(as<Size>(st_size), 0);
    hull_len.assign(as<Size>(st_size), 0);

    hull_m.clear();
    hull_c.clear();
    hull_m.reserve(as<Size>(n) * 20);
    hull_c.reserve(as<Size>(n) * 20);

    build(1, 1, n, lines);
  }

  void range_assign(I32 l, I32 r, I64 x) {
    if (n == 0) return;
    l = std::max<I32>(l, 1);
    r = std::min<I32>(r, n);
    if (l > r) return;
    update(1, 1, n, l, r, x);
  }

  [[nodiscard]] I64 range_max(I32 l, I32 r) {
    if (n == 0) return NEG_INF;
    l = std::max<I32>(l, 1);
    r = std::min<I32>(r, n);
    if (l > r) return NEG_INF;
    return query(1, 1, n, l, r);
  }

  [[nodiscard]] I64 all_max() const {
    if (n == 0) return NEG_INF;
    return max_val[1];
  }

private:
  void build(I32 u, I32 l, I32 r, const Vec<Pair<I64, I64>>& lines) {
    lazy[as<Size>(u)] = NO_TAG;
    max_val[as<Size>(u)] = NEG_INF;

    if (l == r) {
      hull_start[as<Size>(u)] = as<I32>(hull_m.size());
      hull_len[as<Size>(u)] = 1;
      hull_m.eb(lines[as<Size>(l)].first);
      hull_c.eb(lines[as<Size>(l)].second);
      return;
    }

    const I32 mid = (l + r) / 2;
    build(2 * u, l, mid, lines);
    build(2 * u + 1, mid + 1, r, lines);

    const I32 start = as<I32>(hull_m.size());
    I32 count = 0;
    FOR(i, l, r + 1) {
      const auto [m, c] = lines[as<Size>(i)];
      while (count >= 2) {
        const I64 m0 = hull_m[as<Size>(start + count - 2)];
        const I64 c0 = hull_c[as<Size>(start + count - 2)];
        const I64 m1 = hull_m[as<Size>(start + count - 1)];
        const I64 c1 = hull_c[as<Size>(start + count - 1)];

        const I128 lhs = as<I128>(c - c1) * (m0 - m1);
        const I128 rhs = as<I128>(c1 - c0) * (m1 - m);
        if (lhs >= rhs) {
          --count;
          hull_m.pop_back();
          hull_c.pop_back();
        } else {
          break;
        }
      }
      hull_m.eb(m);
      hull_c.eb(c);
      ++count;
    }

    hull_start[as<Size>(u)] = start;
    hull_len[as<Size>(u)] = count;
  }

  [[nodiscard]] I64 eval_max(I32 u, I64 x) const {
    const I32 start = hull_start[as<Size>(u)];
    const I32 len = hull_len[as<Size>(u)];
    if (len == 1) {
      return as<I64>(as<I128>(hull_m[as<Size>(start)]) * x + hull_c[as<Size>(start)]);
    }

    I32 lo = 0;
    I32 hi = len - 2;
    I32 ans = len - 1;
    while (lo <= hi) {
      const I32 mid = (lo + hi) / 2;
      const I32 idx = start + mid;

      const I64 m1 = hull_m[as<Size>(idx)];
      const I64 c1 = hull_c[as<Size>(idx)];
      const I64 m2 = hull_m[as<Size>(idx + 1)];
      const I64 c2 = hull_c[as<Size>(idx + 1)];

      const I128 lhs = as<I128>(x) * (m1 - m2);
      const I128 rhs = as<I128>(c2) - c1;
      if (lhs >= rhs) {
        ans = mid;
        hi = mid - 1;
      } else {
        lo = mid + 1;
      }
    }

    const I32 best = start + ans;
    return as<I64>(as<I128>(hull_m[as<Size>(best)]) * x + hull_c[as<Size>(best)]);
  }

  void apply(I32 u, I64 x) {
    lazy[as<Size>(u)] = x;
    max_val[as<Size>(u)] = eval_max(u, x);
  }

  void push_down(I32 u) {
    const I64 tag = lazy[as<Size>(u)];
    if (tag == NO_TAG) return;
    apply(2 * u, tag);
    apply(2 * u + 1, tag);
    lazy[as<Size>(u)] = NO_TAG;
  }

  void update(I32 u, I32 l, I32 r, I32 ql, I32 qr, I64 x) {
    if (ql <= l && r <= qr) {
      apply(u, x);
      return;
    }
    push_down(u);
    const I32 mid = (l + r) / 2;
    if (ql <= mid) update(2 * u, l, mid, ql, qr, x);
    if (qr > mid) update(2 * u + 1, mid + 1, r, ql, qr, x);
    max_val[as<Size>(u)] = std::max(max_val[as<Size>(2 * u)], max_val[as<Size>(2 * u + 1)]);
  }

  I64 query(I32 u, I32 l, I32 r, I32 ql, I32 qr) {
    if (ql <= l && r <= qr) return max_val[as<Size>(u)];
    push_down(u);
    const I32 mid = (l + r) / 2;
    I64 ans = NEG_INF;
    if (ql <= mid) ans = query(2 * u, l, mid, ql, qr);
    if (qr > mid) ans = std::max(ans, query(2 * u + 1, mid + 1, r, ql, qr));
    return ans;
  }
};

//===----------------------------------------------------------------------===//
/* Main Solver Function */

using namespace std;

void solve() {
  constexpr I64 NEG_INF = SegmentTreeHull::NEG_INF;
  INT(n);
  Vec<I64> a(n + 1);
  FOR(i, 1, n + 1) IN(a[i]);

  Vec<Vec<I32>> adj(n + 1);
  FOR(_, n - 1) {
    INT(u, v);
    adj[u].eb(v);
    adj[v].eb(u);
  }

  Vec<I32> parent(n + 1, 0);
  Vec<I32> depth(n + 1, 0);
  Vec<I32> ptr(n + 1, 0);
  Vec<I32> order;
  order.reserve(n);

  Vec<I32> st;
  st.reserve(n);
  st.eb(1);

  while (!st.empty()) {
    const I32 u = st.back();
    if (ptr[u] == 0) order.eb(u);

    if (ptr[u] < as<I32>(adj[u].size())) {
      const I32 v = adj[u][ptr[u]++];
      if (v == parent[u]) continue;
      parent[v] = u;
      depth[v] = depth[u] + 1;
      st.eb(v);
    } else {
      st.pop_back();
    }
  }

  Vec<I64> s(n + 1, 0);
  Vec<I64> c(n + 1, 0);
  Vec<I32> d1(n + 1, 0);
  Vec<I32> d2(n + 1, 0);
  Vec<I32> heavy(n + 1, 0);

  FOR_R(i, 0, n) {
    const I32 u = order[i];
    s[u] = a[u];
    d1[u] = depth[u];

    I32 max_d = -1;
    I32 h_child = 0;
    for (I32 v : adj[u]) {
      if (parent[v] != u) continue;
      s[u] += s[v];
      if (d1[v] > max_d) {
        max_d = d1[v];
        h_child = v;
      }
    }

    if (h_child != 0) {
      d1[u] = max_d;
      heavy[u] = h_child;
    }

    I32 max_d2 = depth[u];
    for (I32 v : adj[u]) {
      if (parent[v] != u || v == h_child) continue;
      chmax(max_d2, d1[v]);
    }
    d2[u] = max_d2;
  }

  FOR_R(i, 0, n) {
    const I32 u = order[i];
    I64 cur = 0;
    for (I32 v : adj[u]) {
      if (parent[v] != u) continue;
      cur += c[v] + s[v];
    }
    c[u] = cur;
  }

  Vec<char> is_head(n + 1, true);
  is_head[0] = false;
  FOR(u, 1, n + 1) {
    if (heavy[u] != 0) is_head[heavy[u]] = false;
  }

  Vec<Vec<I32>> paths;
  paths.reserve(n);
  FOR(u, 1, n + 1) {
    if (!is_head[u]) continue;
    Vec<I32> path;
    I32 curr = u;
    while (curr != 0) {
      path.eb(curr);
      curr = heavy[curr];
    }
    paths.eb(std::move(path));
  }

  Vec<I64> final_ans(n + 1, 0);
  SegmentTreeHull seg;

  for (const auto& path : paths) {
    const I32 k = as<I32>(path.size()) - 1;
    const I32 d_max = depth[path.back()];

    Vec<I64> max_light(k + 1, NEG_INF);
    FOR(i, k + 1) {
      const I32 u = path[i];
      const I32 nxt = (i < k ? path[i + 1] : 0);
      for (I32 v : adj[u]) {
        if (parent[v] != u || v == nxt) continue;

        I64 best = NEG_INF;
        Vec<I32> light_st;
        light_st.eb(v);
        while (!light_st.empty()) {
          const I32 curr = light_st.back();
          light_st.pop_back();

          const I64 val = s[curr] * (as<I64>(d_max) + 1 - depth[curr]);
          chmax(best, val);

          for (I32 child : adj[curr]) {
            if (parent[child] == curr) light_st.eb(child);
          }
        }
        chmax(max_light[i], best);
      }
    }

    Vec<I64> suf_max(k + 1, NEG_INF);
    suf_max[k] = max_light[k];
    FOR_R(i, 0, k) suf_max[i] = max(max_light[i], suf_max[i + 1]);

    Vec<I64> ans_case2(k + 1, NEG_INF);
    if (k > 0) {
      Vec<Pair<I64, I64>> path_lines(k + 1, {0, 0});
      FOR(i, k + 1) {
        const I32 u = path[i];
        path_lines[i] = {s[u], s[u] * (1 - depth[u])};
      }

      seg.init(k, path_lines);

      Vec<tuple<I32, I32, I32>> mono_st;
      mono_st.reserve(k);

      FOR_R(i, 0, k) {
        const I32 v_val = d2[path[i]];
        I32 curr_l = i + 1;
        I32 curr_r = i + 1;

        while (!mono_st.empty() && get<0>(mono_st.back()) <= v_val) {
          curr_r = get<2>(mono_st.back());
          mono_st.pop_back();
        }
        mono_st.eb(v_val, curr_l, curr_r);

        seg.range_assign(curr_l, curr_r, v_val);
        ans_case2[i] = seg.range_max(i + 1, k);
      }
    }

    FOR(i, k + 1) {
      I64 max_inc = max<I64>(0, suf_max[i]);
      if (k > 0 && i < k) chmax(max_inc, ans_case2[i]);
      final_ans[path[i]] = c[path[i]] + max_inc;
    }
  }

  FOR(i, 1, n + 1) {
    if (i > 1) cout << ' ';
    cout << final_ans[i];
  }
  cout << '\n';
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
