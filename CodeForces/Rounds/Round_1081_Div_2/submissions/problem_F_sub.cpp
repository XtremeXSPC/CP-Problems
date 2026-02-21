//===----------------------------------------------------------------------===//
/**
 * @file: problem_F_sub.cpp
 * @generated: 2026-02-21 17:08:57
 * @source: problem_F.cpp
 * @author: Costantino Lombardi
 *
 * @brief: Codeforces Round 1081 (Div. 2) - Problem F
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
// constexpr I64 MOD  = 1000000007;
// constexpr I64 MOD2 = 998244353;
// constexpr I64 MOD3 = 1000000009;

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
/* Main Solver Function */

using namespace std;

namespace {

constexpr I64 MOD = 1'000'000'007LL;
constexpr I64 INV2 = (MOD + 1) / 2;

auto add_mod(I32& x, I64 y) -> void {
  x = as<I32>((x + y) % MOD);
}

auto build_step_dp(I64 base_size, const Vec<I64>& inner_sizes, const Vec<I64>& inner_gains,
                   const Vec<I64>& outer_sizes, const Vec<I64>& outer_gains)
    -> Pair<Vec<Vec<I32>>, Vec<Vec<I32>>> {
  const I32 m = as<I32>(inner_sizes.size());
  const I32 l = as<I32>(outer_sizes.size());

  Vec<I64> pref_in(m + 1, 0), pref_out(l + 1, 0);
  FOR(i, m) pref_in[i + 1] = pref_in[i] + inner_gains[i];
  FOR(i, l) pref_out[i + 1] = pref_out[i] + outer_gains[i];

  const I32 max_moves = m + l;
  Vec<Vec<I32>> dp(max_moves + 1), dp_in(max_moves + 1);
  dp[0] = {1};
  dp_in[0] = {0};

  FOR(p, max_moves) {
    const I32 nxt_len = min<I32>(as<I32>(p) + 1, m) + 1;
    Vec<I32> nxt(nxt_len, 0), nxt_in(nxt_len, 0);
    const auto& cur = dp[p];

    FOR(i_inner, sz(cur)) {
      const I32 prob = cur[i_inner];
      if (prob == 0) continue;

      const I32 eaten_outer = p - i_inner;
      if (eaten_outer < 0 || eaten_outer > l) continue;

      const I64 size_now = base_size + pref_in[i_inner] + pref_out[eaten_outer];
      const bool can_inner = (i_inner < m) && (inner_sizes[i_inner] <= size_now);
      const bool can_outer = (eaten_outer < l) && (outer_sizes[eaten_outer] <= size_now);
      if (!can_inner && !can_outer) continue;

      if (can_inner && can_outer) {
        const I64 add = as<I64>(prob) * INV2 % MOD;
        add_mod(nxt[i_inner + 1], add);
        add_mod(nxt_in[i_inner + 1], add);
        add_mod(nxt[i_inner], add);
      } else if (can_inner) {
        add_mod(nxt[i_inner + 1], prob);
        add_mod(nxt_in[i_inner + 1], prob);
      } else {
        add_mod(nxt[i_inner], prob);
      }
    }

    dp[p + 1] = std::move(nxt);
    dp_in[p + 1] = std::move(nxt_in);
  }

  return {std::move(dp), std::move(dp_in)};
}

auto solve_case(I32 n, I32 x, I32 y, Vec<I64> a, Vec<I64> b) -> I32 {
  if (x > y) {
    reverse(all(a));
    reverse(all(b));
    x = n + 1 - x;
    y = n + 1 - y;
  }

  const I32 ax = x - 1;
  const I32 ay = y - 1;
  const I32 m = y - x - 1;
  const I32 al = x - 1;
  const I32 br = n - y;

  Vec<I64> inner_a_sizes, inner_a_gains, outer_a_sizes, outer_a_gains;
  Vec<I64> inner_b_sizes, inner_b_gains, outer_b_sizes, outer_b_gains;

  inner_a_sizes.reserve(m);
  inner_a_gains.reserve(m);
  FOR(i, ax + 1, ay) {
    inner_a_sizes.eb(a[i]);
    inner_a_gains.eb(b[i]);
  }

  outer_a_sizes.reserve(al);
  outer_a_gains.reserve(al);
  FOR(i, al) {
    outer_a_sizes.eb(a[ax - 1 - i]);
    outer_a_gains.eb(b[ax - 1 - i]);
  }

  inner_b_sizes.reserve(m);
  inner_b_gains.reserve(m);
  FOR(i, m) {
    inner_b_sizes.eb(a[ay - 1 - i]);
    inner_b_gains.eb(b[ay - 1 - i]);
  }

  outer_b_sizes.reserve(br);
  outer_b_gains.reserve(br);
  FOR(i, br) {
    outer_b_sizes.eb(a[ay + 1 + i]);
    outer_b_gains.eb(b[ay + 1 + i]);
  }

  Vec<I64> gain_a_in(m + 1, 0), gain_a_out(al + 1, 0);
  Vec<I64> gain_b_in(m + 1, 0), gain_b_out(br + 1, 0);
  FOR(i, m) gain_a_in[i + 1] = gain_a_in[i] + inner_a_gains[i];
  FOR(i, al) gain_a_out[i + 1] = gain_a_out[i] + outer_a_gains[i];
  FOR(i, m) gain_b_in[i + 1] = gain_b_in[i] + inner_b_gains[i];
  FOR(i, br) gain_b_out[i + 1] = gain_b_out[i] + outer_b_gains[i];

  const I32 w = br + 1;
  const I32 size = (al + 1) * w;
  Vec<I32> win_a(size, 0), win_b(size, 0);

  FOR_R(oa, 0, al + 1) {
    FOR_R(ob, 0, br + 1) {
      const I32 idx = oa * w + ob;

      {
        const I32 num = m - oa + ob;
        if ((num & 1) == 0) {
          const I32 i_a = num / 2;
          if (0 <= i_a && i_a <= m) {
            const I32 i_b = m - i_a;
            const I64 s_a = a[ax] + gain_a_out[oa] + gain_a_in[i_a];
            const I64 s_b = a[ay] + gain_b_out[ob] + gain_b_in[i_b];

            const bool can_opp = s_b <= s_a;
            const bool can_out = (oa < al) && (outer_a_sizes[oa] <= s_a);

            if (!can_opp && !can_out) {
              win_a[idx] = 0;
            } else if (can_opp && !can_out) {
              win_a[idx] = 1;
            } else if (!can_opp && can_out) {
              win_a[idx] = win_b[(oa + 1) * w + ob];
            } else {
              win_a[idx] = as<I32>((1 + win_b[(oa + 1) * w + ob]) * INV2 % MOD);
            }
          }
        }
      }

      {
        const I32 num = m + 1 - oa + ob;
        if ((num & 1) == 0) {
          const I32 i_a = num / 2;
          if (0 <= i_a && i_a <= m) {
            const I32 i_b = m - i_a;
            const I64 s_a = a[ax] + gain_a_out[oa] + gain_a_in[i_a];
            const I64 s_b = a[ay] + gain_b_out[ob] + gain_b_in[i_b];

            const bool can_opp = s_a <= s_b;
            const bool can_out = (ob < br) && (outer_b_sizes[ob] <= s_b);

            if (!can_opp && !can_out) {
              win_b[idx] = 1;
            } else if (can_opp && !can_out) {
              win_b[idx] = 0;
            } else if (!can_opp && can_out) {
              win_b[idx] = win_a[oa * w + (ob + 1)];
            } else {
              win_b[idx] = as<I32>(as<I64>(win_a[oa * w + (ob + 1)]) * INV2 % MOD);
            }
          }
        }
      }
    }
  }

  if (m == 0) return win_a[0];

  auto [dp_a, dp_a_in] = build_step_dp(a[ax], inner_a_sizes, inner_a_gains, outer_a_sizes, outer_a_gains);
  auto [dp_b, dp_b_in] = build_step_dp(a[ay], inner_b_sizes, inner_b_gains, outer_b_sizes, outer_b_gains);

  const I32 max_a = as<I32>(dp_a.size()) - 1;
  const I32 max_b = as<I32>(dp_b.size()) - 1;
  I64 ans = 0;

  FOR(q, max_b + 1) {
    const I32 p = q + 1;
    if (p > max_a) break;

    const auto& arr_a = dp_a[p];
    const auto& arr_b = dp_b[q];
    if (arr_a.empty() || arr_b.empty()) continue;

    Vec<I32> pref(sz(arr_a) + 1, 0);
    FOR(i, sz(arr_a)) pref[i + 1] = as<I32>((pref[i] + arr_a[i]) % MOD);

    FOR(ib, sz(arr_b)) {
      const I32 prob_b = arr_b[ib];
      if (prob_b == 0) continue;

      const I32 bound = m - 1 - ib;
      if (bound < 0) continue;

      const I32 ob = q - ib;
      const I64 s_b = a[ay] + gain_b_in[ib] + gain_b_out[ob];
      const bool can_in = inner_b_sizes[ib] <= s_b;
      const bool can_out = (ob < br) && (outer_b_sizes[ob] <= s_b);
      if (can_in || can_out) continue;

      const I32 up = min<I32>(bound + 1, as<I32>(arr_a.size()));
      const I32 sum_a = pref[up];
      ans = (ans + as<I64>(prob_b) * sum_a) % MOD;
    }
  }

  FOR(p, 1, max_a + 1) {
    const I32 q = p - 1;
    if (q < 0 || q > max_b) continue;

    const auto& arr_a_in = dp_a_in[p];
    const auto& arr_b = dp_b[q];

    FOR(ia, sz(arr_a_in)) {
      const I32 prob_a_in = arr_a_in[ia];
      if (prob_a_in == 0) continue;

      const I32 ib = m - ia;
      if (ib < 0 || ib >= as<I32>(arr_b.size())) continue;
      const I32 prob_b = arr_b[ib];
      if (prob_b == 0) continue;

      const I32 oa = p - ia;
      const I32 ob = q - ib;
      if (0 <= oa && oa <= al && 0 <= ob && ob <= br) {
        const I64 ways = as<I64>(prob_a_in) * prob_b % MOD;
        ans = (ans + ways * win_b[oa * w + ob]) % MOD;
      }
    }
  }

  FOR(q, 1, max_b + 1) {
    const I32 p = q;
    if (p > max_a) continue;

    const auto& arr_b_in = dp_b_in[q];
    const auto& arr_a = dp_a[p];

    FOR(ib, sz(arr_b_in)) {
      const I32 prob_b_in = arr_b_in[ib];
      if (prob_b_in == 0) continue;

      const I32 ia = m - ib;
      if (ia < 0 || ia >= as<I32>(arr_a.size())) continue;
      const I32 prob_a = arr_a[ia];
      if (prob_a == 0) continue;

      const I32 oa = p - ia;
      const I32 ob = q - ib;
      if (0 <= oa && oa <= al && 0 <= ob && ob <= br) {
        const I64 ways = as<I64>(prob_b_in) * prob_a % MOD;
        ans = (ans + ways * win_a[oa * w + ob]) % MOD;
      }
    }
  }

  return as<I32>(ans);
}

} // namespace

void solve() {
  INT(n);
  INT(x, y);
  Vec<I64> a(n), b(n);
  IN(a);
  IN(b);

  OUT(solve_case(n, x, y, std::move(a), std::move(b)));
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
