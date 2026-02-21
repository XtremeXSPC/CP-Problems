//===----------------------------------------------------------------------===//
/**
 * @file: problem_F1_sub.cpp
 * @generated: 2026-02-21 01:48:02
 * @source: problem_F1.cpp
 * @author: Costantino Lombardi
 *
 * @brief: Codeforces Round 1077 (Div. 1) - Problem F1
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
/* High-Performance Buffered I/O */

#ifndef CP_FAST_IO_NAMESPACE_DEFINED
  #define CP_FAST_IO_NAMESPACE_DEFINED 1
#endif

namespace fast_io {

static constexpr U32 BUFFER_SIZE = 1U << 17; // 128 KB
alignas(64) inline char input_buffer[BUFFER_SIZE];
alignas(64) inline char output_buffer[BUFFER_SIZE];
alignas(64) inline char number_buffer[128];

inline U32 input_pos = 0;
inline U32 input_end = 0;
inline U32 output_pos = 0;

/* ------------------------------- INPUT API -------------------------------- */

inline void load_input() {
  const U32 remaining = input_end - input_pos;
  std::memmove(input_buffer, input_buffer + input_pos, remaining);
  input_end = remaining + static_cast<U32>(
      std::fread(input_buffer + remaining, 1, BUFFER_SIZE - remaining, stdin));
  input_pos = 0;
  if (input_end < BUFFER_SIZE) input_buffer[input_end++] = '\n';
}

inline void flush_output() {
  if (output_pos == 0) return;
  std::fwrite(output_buffer, 1, output_pos, stdout);
  output_pos = 0;
}

inline void read_char(char& c) {
  do {
    if (input_pos >= input_end) load_input();
    c = input_buffer[input_pos++];
  } while (std::isspace(static_cast<unsigned char>(c)));
}

template <typename T>
inline void read_integer(T& x) {
  if (input_pos + 64 >= input_end) load_input();

  char c;
  do {
    c = input_buffer[input_pos++];
  } while (c < '-');

  bool negative = false;
  if constexpr (std::is_signed_v<T>) {
    if (c == '-') {
      negative = true;
      c = input_buffer[input_pos++];
    }
  }

  x = 0;
  while (c >= '0') {
    x = x * 10 + (c - '0');
    c = input_buffer[input_pos++];
  }

  if constexpr (std::is_signed_v<T>) {
    if (negative) x = -x;
  }
}

inline void read_string(std::string& s) {
  s.clear();
  char c;
  do {
    if (input_pos >= input_end) load_input();
    c = input_buffer[input_pos++];
  } while (std::isspace(static_cast<unsigned char>(c)));

  do {
    s.push_back(c);
    if (input_pos >= input_end) load_input();
    c = input_buffer[input_pos++];
  } while (!std::isspace(static_cast<unsigned char>(c)));
}

inline void read(I32& x) { read_integer(x); }
inline void read(I64& x) { read_integer(x); }
inline void read(U32& x) { read_integer(x); }
inline void read(U64& x) { read_integer(x); }
inline void read(char& x) { read_char(x); }
inline void read(std::string& x) { read_string(x); }

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

/* ------------------------------- OUTPUT API ------------------------------- */

template <typename T>
inline void write_integer(T x) {
  if (output_pos + 64 >= BUFFER_SIZE) flush_output();

  using UnsignedT = std::make_unsigned_t<T>;
  UnsignedT ux;
  if constexpr (std::is_signed_v<T>) {
    if (x < 0) {
      output_buffer[output_pos++] = '-';
      ux = static_cast<UnsignedT>(-(x + 1));
      ux += 1;
    } else {
      ux = static_cast<UnsignedT>(x);
    }
  } else {
    ux = static_cast<UnsignedT>(x);
  }

  I32 digits = 0;
  do {
    number_buffer[digits++] = static_cast<char>('0' + (ux % 10));
    ux /= 10;
  } while (ux > 0);

  for (I32 i = digits - 1; i >= 0; --i) {
    output_buffer[output_pos++] = number_buffer[i];
  }
}

inline void write_char(char c) {
  if (output_pos >= BUFFER_SIZE) flush_output();
  output_buffer[output_pos++] = c;
}

inline void write_string(std::string_view s) {
  for (char c : s) write_char(c);
}

inline void write(I32 x) { write_integer(x); }
inline void write(I64 x) { write_integer(x); }
inline void write(U32 x) { write_integer(x); }
inline void write(U64 x) { write_integer(x); }
inline void write(char x) { write_char(x); }
inline void write(const std::string& x) { write_string(x); }
inline void write(const char* x) { write_string(x); }

template <class T, class U>
void write(const std::pair<T, U>& p) {
  write(p.first);
  write(' ');
  write(p.second);
}

template <class T>
void write(const Vec<T>& v) {
  for (I64 i = 0; i < static_cast<I64>(v.size()); ++i) {
    if (i) write(' ');
    write(v[i]);
  }
}

template <typename... Args>
void write(const std::tuple<Args...>& t) {
  I32 i = 0;
  std::apply([&i](const auto&... args) {
    ((i++ > 0 ? (write(' '), 0) : 0, write(args)), ...);
  }, t);
}

template <class Head, class... Tail>
void write(const Head& head, const Tail&... tail) {
  write(head);
  if constexpr (sizeof...(tail) > 0) {
    write(' ');
    write(tail...);
  }
}

inline void writeln() { write_char('\n'); }

template <class... Args>
void writeln(const Args&... args) {
  if constexpr (sizeof...(args) > 0) write(args...);
  write_char('\n');
}

struct IOFlusher {
  ~IOFlusher() { flush_output(); }
};
inline IOFlusher io_flusher;

} // namespace fast_io

#ifdef CP_IO_IMPL_READ
  #undef CP_IO_IMPL_READ
#endif
#ifdef CP_IO_IMPL_WRITELN
  #undef CP_IO_IMPL_WRITELN
#endif
#ifdef CP_IO_IMPL_FLUSH
  #undef CP_IO_IMPL_FLUSH
#endif

#define CP_IO_IMPL_READ(...) fast_io::read(__VA_ARGS__)
#define CP_IO_IMPL_WRITELN(...) fast_io::writeln(__VA_ARGS__)
#define CP_IO_IMPL_FLUSH() fast_io::flush_output()

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
constexpr I64 MOD  = 1000000007;
constexpr I64 MOD2 = 998244353;
constexpr I64 MOD3 = 1000000009;

//===----------------------------------------------------------------------===//
/* Data Structures & Algorithms for the Problem */

template <typename T, typename F = std::plus<T>>
struct SegmentTree {
  I32 n;
  Vec<T> tree;
  F op;
  T identity;

  SegmentTree(I32 size, T id = T{}, F func = F{})
    : n(1), op(func), identity(id) {
    while (n < size) n *= 2;
    tree.assign(2 * n, identity);
  }

  SegmentTree(const Vec<T>& v, T id = T{}, F func = F{})
    : SegmentTree(sz(v), id, func) {
    FOR(i, sz(v)) tree[n + i] = v[i];
    FOR_R(i, 1, n) tree[i] = op(tree[2*i], tree[2*i + 1]);
  }

  void update(I32 pos, T val) {
    pos += n;
    tree[pos] = val;
    while (pos > 1) {
      pos >>= 1;
      tree[pos] = op(tree[pos << 1], tree[pos << 1 | 1]);
    }
  }

  T query(I32 l, I32 r) {  // [l, r)
    T res_left = identity, res_right = identity;
    l += n; r += n;
    while (l < r) {
      if (l & 1) res_left = op(res_left, tree[l++]);
      if (r & 1) res_right = op(tree[--r], res_right);
      l >>= 1; r >>= 1;
    }
    return op(res_left, res_right);
  }

  T get(I32 pos) { return tree[n + pos]; }
};

template <typename T = I64>
struct SegmentTreeMinAddSearch {
  I32 n = 0;
  Vec<T> min_val;
  Vec<T> max_val;
  Vec<T> lazy;

  SegmentTreeMinAddSearch() = default;

  explicit SegmentTreeMinAddSearch(I32 size, T initial_value = T{}) {
    init(size, initial_value);
  }

  explicit SegmentTreeMinAddSearch(const Vec<T>& values) { init(values); }

  void init(I32 size, T initial_value = T{}) {
    size = std::max<I32>(size, 0);
    Vec<T> values(as<Size>(size), initial_value);
    init(values);
  }

  void init(const Vec<T>& values) {
    n = as<I32>(values.size());
    const I32 tree_size = std::max<I32>(1, 4 * std::max<I32>(n, 1) + 5);
    min_val.assign(as<Size>(tree_size), T{});
    max_val.assign(as<Size>(tree_size), T{});
    lazy.assign(as<Size>(tree_size), T{});
    if (n > 0) build(1, 0, n - 1, values);
  }

  void range_add(I32 l, I32 r, T delta) {
    normalize_range(l, r);
    if (l >= r || n == 0) return;
      range_add_impl(1, 0, n - 1, l, r - 1, delta);
  }

  [[nodiscard]] T range_min(I32 l, I32 r) {
    normalize_range(l, r);
    if (l >= r || n == 0) return infinity<T>;
      return range_min_impl(1, 0, n - 1, l, r - 1);
  }

  [[nodiscard]] T range_max(I32 l, I32 r) {
    normalize_range(l, r);
    if (l >= r || n == 0) return -infinity<T>;
      return range_max_impl(1, 0, n - 1, l, r - 1);
  }

  [[nodiscard]] T all_min() const {
    if (n == 0) return infinity<T>;
      return min_val[1];
  }

  [[nodiscard]] T all_max() const {
    if (n == 0) return -infinity<T>;
      return max_val[1];
  }

  [[nodiscard]] I32 find_rightmost_eq(I32 l, I32 r, T target) {
    normalize_range(l, r);
    if (l >= r || n == 0) return -1;
      return find_rightmost_eq_impl(1, 0, n - 1, l, r - 1, target);
  }

  [[nodiscard]] I32 find_rightmost_leq(I32 l, I32 r, T target) {
    normalize_range(l, r);
    if (l >= r || n == 0) return -1;
      return find_rightmost_leq_impl(1, 0, n - 1, l, r - 1, target);
  }

private:
  static void normalize_range(I32& l, I32& r) {
    if (l < 0) l = 0;
    if (r < 0) r = 0;
  }

  void build(I32 node, I32 tl, I32 tr, const Vec<T>& values) {
    lazy[as<Size>(node)] = T{};
    if (tl == tr) {
      const T v = values[as<Size>(tl)];
      min_val[as<Size>(node)] = v;
      max_val[as<Size>(node)] = v;
      return;
    }

    const I32 tm = (tl + tr) >> 1;
    build(node << 1, tl, tm, values);
    build(node << 1 | 1, tm + 1, tr, values);
    pull(node);
  }

  inline void apply(I32 node, T delta) {
    min_val[as<Size>(node)] += delta;
    max_val[as<Size>(node)] += delta;
    lazy[as<Size>(node)] += delta;
  }

  inline void pull(I32 node) {
    min_val[as<Size>(node)] = std::min(
        min_val[as<Size>(node << 1)],
        min_val[as<Size>(node << 1 | 1)]);
    max_val[as<Size>(node)] = std::max(
        max_val[as<Size>(node << 1)],
        max_val[as<Size>(node << 1 | 1)]);
  }

  inline void push(I32 node) {
    const T lz = lazy[as<Size>(node)];
    if (lz == T{}) return;
    apply(node << 1, lz);
    apply(node << 1 | 1, lz);
    lazy[as<Size>(node)] = T{};
  }

  void range_add_impl(I32 node, I32 tl, I32 tr, I32 ql, I32 qr, T delta) {
    if (qr < tl || tr < ql) return;
    if (ql <= tl && tr <= qr) {
      apply(node, delta);
      return;
    }

    push(node);
    const I32 tm = (tl + tr) >> 1;
    if (ql <= tm) range_add_impl(node << 1, tl, tm, ql, qr, delta);
    if (qr > tm) range_add_impl(node << 1 | 1, tm + 1, tr, ql, qr, delta);
    pull(node);
  }

  T range_min_impl(I32 node, I32 tl, I32 tr, I32 ql, I32 qr) {
    if (qr < tl || tr < ql) return infinity<T>;
    if (ql <= tl && tr <= qr) return min_val[as<Size>(node)];

    push(node);
    const I32 tm = (tl + tr) >> 1;
    return std::min(
        range_min_impl(node << 1, tl, tm, ql, qr),
        range_min_impl(node << 1 | 1, tm + 1, tr, ql, qr));
  }

  T range_max_impl(I32 node, I32 tl, I32 tr, I32 ql, I32 qr) {
    if (qr < tl || tr < ql) return -infinity<T>;
    if (ql <= tl && tr <= qr) return max_val[as<Size>(node)];

    push(node);
    const I32 tm = (tl + tr) >> 1;
    return std::max(
        range_max_impl(node << 1, tl, tm, ql, qr),
        range_max_impl(node << 1 | 1, tm + 1, tr, ql, qr));
  }

  I32 find_rightmost_eq_impl(I32 node, I32 tl, I32 tr, I32 ql, I32 qr, T target) {
    if (qr < tl || tr < ql) return -1;
    if (target < min_val[as<Size>(node)] || target > max_val[as<Size>(node)]) {
      return -1;
    }
    if (tl == tr) return tl;

    push(node);
    const I32 tm = (tl + tr) >> 1;

    I32 res = -1;
    if (qr > tm) res = find_rightmost_eq_impl(node << 1 | 1, tm + 1, tr, ql, qr, target);
    if (res == -1 && ql <= tm) {
      res = find_rightmost_eq_impl(node << 1, tl, tm, ql, qr, target);
    }
    return res;
  }

  I32 find_rightmost_leq_impl(I32 node, I32 tl, I32 tr, I32 ql, I32 qr, T target) {
    if (qr < tl || tr < ql || min_val[as<Size>(node)] > target) return -1;
    if (tl == tr) return tl;

    push(node);
    const I32 tm = (tl + tr) >> 1;

    I32 res = -1;
    if (qr > tm) res = find_rightmost_leq_impl(node << 1 | 1, tm + 1, tr, ql, qr, target);
    if (res == -1 && ql <= tm) {
      res = find_rightmost_leq_impl(node << 1, tl, tm, ql, qr, target);
    }
    return res;
  }
};

//===----------------------------------------------------------------------===//
/* Main Solver Function */

struct MaxOp {
  [[nodiscard]] I32 operator()(I32 a, I32 b) const { return std::max(a, b); }
};

void solve() {
  INT(n);

  VI a(as<Size>(n + 1), 0);
  VI b(as<Size>(n + 1), 0);

  FOR(i, 1, n + 1) IN(a[as<Size>(i)]);
  FOR(i, 1, n + 1) IN(b[as<Size>(i)]);

  VI pos_b(as<Size>(n + 1), 0);
  FOR(i, n + 1) pos_b[as<Size>(b[as<Size>(i)])] = i;

  VI a_prime(as<Size>(n + 1), 0);
  FOR(i, n + 1) a_prime[as<Size>(i)] = pos_b[as<Size>(a[as<Size>(i)])];

  VI next_smaller(as<Size>(n + 1), n + 1);
  VI stack_min;
  stack_min.reserve(as<Size>(n + 1));
  FOR(i, n + 1) {
    while (!stack_min.empty() && a_prime[as<Size>(stack_min.back())] > a_prime[as<Size>(i)]) {
      next_smaller[as<Size>(stack_min.back())] = i;
      stack_min.pop_back();
    }
    stack_min.push_back(i);
  }

  VLL base_min(as<Size>(n + 1), 0);
  FOR(i, n + 1) base_min[as<Size>(i)] = -as<I64>(i);
  SegmentTreeMinAddSearch<I64> min_tree(base_min);
  SegmentTree<I32, MaxOp> dp_tree(n + 1, 0, MaxOp{});

  VI st;
  st.reserve(as<Size>(n + 1));

  for (I32 i = n; i >= 0; --i) {
    const I32 new_max = a_prime[as<Size>(i)];

    while (!st.empty() && a_prime[as<Size>(st.back())] < new_max) {
      const I32 j = st.back();
      st.pop_back();
      const I32 next_idx = st.empty() ? n : st.back() - 1;
      const I32 old_max = a_prime[as<Size>(j)];
      min_tree.range_add(j, next_idx + 1, as<I64>(new_max - old_max));
    }

    st.push_back(i);
    min_tree.range_add(i, i + 1, as<I64>(new_max));

    const I64 target = as<I64>(new_max - i);
    const I32 limit = next_smaller[as<Size>(i)] - 1;

    I32 r_i = i;
    if (i <= limit) {
      const I32 candidate = min_tree.find_rightmost_eq(i, limit + 1, target);
      if (candidate != -1) r_i = candidate;
    }

    const I32 best_child = dp_tree.query(i + 1, r_i + 1);
    const I32 dp_i = 1 + best_child;
    dp_tree.update(i, dp_i);

    if (i == 0) {
      OUT(dp_i - 1);
      return;
    }
  }
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
