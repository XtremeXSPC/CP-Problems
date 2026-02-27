//===----------------------------------------------------------------------===//
/**
 * @file: problem_G_sub.cpp
 * @generated: 2026-02-27 01:01:53
 * @source: problem_G.cpp
 * @author: Costantino Lombardi
 *
 * @brief: Codeforces Round 1083 (Div. 2) - Problem G
 */
//===----------------------------------------------------------------------===//
/* Included library and Compiler Optimizations */
#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

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
/* Advanced Macro System */

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
[[gnu::always_inline]] constexpr To as(auto&& x) noexcept {
  return static_cast<To>(std::forward<decltype(x)>(x));
}

template <typename To>
[[gnu::always_inline]] constexpr To narrow_as(auto x) {
  To converted = static_cast<To>(x);
  return converted;
}

//===----------------------------------------------------------------------===//
/* High-Performance Buffered I/O */

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

  [[gnu::always_inline]] F64 elapsed() const { return std::chrono::duration<F64>(Clock::now() - start).count(); }

  void reset() { start = Clock::now(); }

  [[gnu::always_inline]] bool within(F64 limit) const { return elapsed() < limit; }
};

//===----------------------------------------------------------------------===//
/* Mathematical Utilities */

namespace cp::detail {

template <std::unsigned_integral T>
[[gnu::always_inline]] constexpr T mul_mod_unsigned(T a, T b, T mod) {
#if HAS_INT128
  if constexpr (sizeof(T) <= sizeof(U64)) {
    return static_cast<T>((static_cast<U128>(a) * static_cast<U128>(b)) % static_cast<U128>(mod));
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
  U umod = static_cast<U>(mod);
  U uexp = static_cast<U>(exp);
  U ubase = static_cast<U>(mod_floor(base, mod));
  U result = static_cast<U>(1) % umod;

  while (uexp > 0) {
    if ((uexp & 1U) != 0U) {
      result = cp::detail::mul_mod_unsigned(result, ubase, umod);
    }
    ubase = cp::detail::mul_mod_unsigned(ubase, ubase, umod);
    uexp >>= 1U;
  }
  return static_cast<T>(result);
}

template <cp::NonBoolIntegral T>
[[gnu::always_inline]] inline T floor_sqrt(T x) {
  if constexpr (std::is_signed_v<T>) {
    my_assert(x >= 0);
    if (x < 0) return 0;
  }

  using U = std::make_unsigned_t<T>;
  const U ux = static_cast<U>(x);
  if (ux <= 1) return static_cast<T>(ux);

  U r = static_cast<U>(std::sqrt(static_cast<F80>(ux)));
  while ((r + 1) <= ux / (r + 1)) ++r;
  while (r > ux / r) --r;
  return static_cast<T>(r);
}

template <cp::NonBoolIntegral T>
[[gnu::always_inline]] inline T ceil_sqrt(T x) {
  using U = std::make_unsigned_t<T>;
  const T root = floor_sqrt(x);
  const U uf = static_cast<U>(root);
  if (uf == 0) return 0;

  const U ux = static_cast<U>(x);
  if (ux / uf == uf && ux % uf == 0) return root;
  return static_cast<T>(uf + 1);
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
inline std::mt19937_64 rng(static_cast<U64>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

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

constexpr I32 INF32 = infinity<I32>;
constexpr I64 INF64 = infinity<I64>;
constexpr I64 LINF  = INF64; // Legacy alias
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
/* Advanced Bitwise Operations */

template <cp::Integral T>
[[gnu::always_inline]] constexpr I32 popcount(T x) {
  using Raw = std::remove_cv_t<T>;
  using U = std::conditional_t<std::is_same_v<Raw, bool>, U8, std::make_unsigned_t<Raw>>;
  if constexpr (sizeof(Raw) <= 4) return __builtin_popcount(static_cast<U32>(static_cast<U>(x)));
  else return __builtin_popcountll(static_cast<U64>(static_cast<U>(x)));
}

template <cp::Integral T>
[[gnu::always_inline]] constexpr I32 leading_zeros(T x) {
  using Raw = std::remove_cv_t<T>;
  using U = std::conditional_t<std::is_same_v<Raw, bool>, U8, std::make_unsigned_t<Raw>>;
  U ux = static_cast<U>(x);
  if (ux == 0) return sizeof(Raw) * 8;
  if constexpr (sizeof(Raw) <= 4) {
    return __builtin_clz(static_cast<U32>(ux)) - (32 - static_cast<I32>(sizeof(Raw) * 8));
  } else {
    return __builtin_clzll(static_cast<U64>(ux)) - (64 - static_cast<I32>(sizeof(Raw) * 8));
  }
}

template <cp::Integral T>
[[gnu::always_inline]] constexpr I32 trailing_zeros(T x) {
  using Raw = std::remove_cv_t<T>;
  using U = std::conditional_t<std::is_same_v<Raw, bool>, U8, std::make_unsigned_t<Raw>>;
  U ux = static_cast<U>(x);
  if (ux == 0) return sizeof(Raw) * 8;
  if constexpr (sizeof(Raw) <= 4) return __builtin_ctz(static_cast<U32>(ux));
  else return __builtin_ctzll(static_cast<U64>(ux));
}

template <cp::Integral T>
[[gnu::always_inline]] constexpr I32 bit_width(T x) {
  return sizeof(T) * 8 - leading_zeros(x);
}

template <cp::Integral T>
[[gnu::always_inline]] constexpr T bit_floor(T x) {
  if (x == 0) return 0;
  return T(1) << (bit_width(x) - 1);
}

template <cp::Integral T>
[[gnu::always_inline]] constexpr T bit_ceil(T x) {
  if (x <= 1) return 1;
  return T(1) << bit_width(x - 1);
}

template <cp::Integral T>
[[gnu::always_inline]] constexpr I32 parity_sign(T x) {
  return (popcount(x) & 1) ? -1 : 1;
}

template <cp::Integral T>
constexpr T kth_bit(I32 k) { return T(1) << k; }

template <cp::Integral T>
constexpr bool has_kth_bit(T x, I32 k) { return (x >> k) & 1; }

template <cp::Integral T>
struct bit_range {
  T mask;
  struct iterator {
    T current;
    iterator(T mask) : current(mask) {}
    I32 operator*() const { return trailing_zeros(current); }
    iterator& operator++() { current &= current - 1; return *this; }
    bool operator!=(const iterator&) const { return current != 0; }
  };
  bit_range(T mask) : mask(mask) {}
  iterator begin() const { return iterator(mask); }
  iterator end() const { return iterator(0); }
};

template <cp::Integral T>
struct subset_range {
  T mask;
  struct iterator {
    T subset, original;
    bool finished;
    iterator(T mask) : subset(mask), original(mask), finished(false) {}
    T operator*() const { return original ^ subset; }
    iterator& operator++() {
      if (subset == 0) finished = true;
      else subset = (subset - 1) & original;
      return *this;
    }
    bool operator!=(const iterator&) const { return !finished; }
  };
  subset_range(T mask) : mask(mask) {}
  iterator begin() const { return iterator(mask); }
  iterator end() const { return iterator(0); }
};

//===----------------------------------------------------------------------===//
/* Data Structures & Algorithms for the Problem */

struct FWHT {

  template <typename T>
  static void xor_transform(Vec<T>& a, const bool inverse) {
    const I32 n = sz(a);
    my_assert(n > 0 && (n & (n - 1)) == 0);

    for (I32 len = 1; len < n; len <<= 1) {
      for (I32 i = 0; i < n; i += (len << 1)) {
        for (I32 j = 0; j < len; ++j) {
          const T u = a[i + j];
          const T v = a[i + j + len];
          a[i + j] = u + v;
          a[i + j + len] = u - v;
        }
      }
    }

    if (inverse) {
      for (I32 i = 0; i < n; ++i) a[i] /= n;
    }
  }

  template <typename T>
  static auto xor_convolution(Vec<T> lhs, Vec<T> rhs) -> Vec<T> {
    my_assert(!lhs.empty() && sz(lhs) == sz(rhs));
    my_assert((sz(lhs) & (sz(lhs) - 1)) == 0);

    xor_transform(lhs, false);
    xor_transform(rhs, false);
    FOR(i, sz(lhs)) lhs[i] *= rhs[i];
    xor_transform(lhs, true);
    return lhs;
  }
};

struct Mobius {
  VecI32 mu;
  VecI32 primes;
  I32 n;

  explicit Mobius(const I32 size) : mu(size + 1, 0), n(size) {
    my_assert(size >= 1);
    mu[1] = 1;
    VecI32 min_prime(n + 1, 0);

    FOR(i, 2, n + 1) {
      if (min_prime[i] == 0) {
        min_prime[i] = i;
        primes.push_back(i);
        mu[i] = -1;
      }

      for (const I32 p : primes) {
        const I64 nxt = as<I64>(i) * p;
        if (nxt > n) break;
        min_prime[nxt] = p;
        if (i % p == 0) {
          mu[nxt] = 0;
          break;
        }
        mu[nxt] = -mu[i];
      }
    }
  }

  [[nodiscard]] auto invert(const VecI64& g) const -> VecI64 {
    VecI64 f(sz(g));
    FOR(i, 1, sz(g)) {
      for (I32 j = i; j < sz(g); j += i) {
        f[j] += mu[i] * g[j / i];
      }
    }
    return f;
  }

  [[nodiscard]] auto divisors_up_to_limit(const I64 value, const I32 limit) const -> VecI32 {
    VecI32 divisors;
    if (value <= 0 || limit <= 0) return divisors;

    const Vec<Pair<I64, I32>> factors = factorize(value);
    divisors.reserve(2048);

    const auto dfs = [&](const auto& self, const I32 idx, const I64 cur) -> void {
      if (idx == sz(factors)) {
        if (cur <= limit) divisors.push_back(as<I32>(cur));
        return;
      }

      const auto [prime, exponent] = factors[idx];
      I64 val = cur;
      for (I32 e = 0; e <= exponent; ++e) {
        if (val > limit) break;
        self(self, idx + 1, val);
        if (e == exponent) break;
        val *= prime;
      }
    };

    dfs(dfs, 0, 1);
    std::sort(divisors.begin(), divisors.end());
    return divisors;
  }

  [[nodiscard]] auto build_divisibility_weights(const I64 value, const I32 limit) const -> VecI32 {
    my_assert(limit <= n);
    VecI32 weights(as<Size>(limit + 1), 0);
    if (value <= 0 || limit <= 0) return weights;

    const VecI32 divisors = divisors_up_to_limit(value, limit);
    for (const I32 g : divisors) {
      for (I32 t = 1; t * g <= limit; ++t) {
        weights[t * g] += mu[t];
      }
    }
    return weights;
  }

private:
  [[nodiscard]] auto factorize(const I64 value) const -> Vec<Pair<I64, I32>> {
    Vec<Pair<I64, I32>> factors;
    I64 x = value;

    for (const I32 p32 : primes) {
      const I64 p = as<I64>(p32);
      if (p * p > x) break;
      if (x % p != 0) continue;

      I32 cnt = 0;
      while (x % p == 0) {
        x /= p;
        ++cnt;
      }
      factors.push_back({p, cnt});
    }

    if (x > 1) factors.push_back({x, 1});
    return factors;
  }
};

struct XorBoundIntervals {
  I32 m = 0;
  I32 limit = 0;
  I32 top_bit = 0;
  VecI32 overlap;
  VecI32 offset;
  Vec<PairI32> intervals;

  XorBoundIntervals() = default;

  XorBoundIntervals(const I32 bound, const I32 xor_limit) { build(bound, xor_limit); }

  void build(const I32 bound, const I32 xor_limit) {
    my_assert(bound >= 0);
    my_assert(xor_limit > 0 && (xor_limit & (xor_limit - 1)) == 0);

    m = bound;
    limit = xor_limit;
    top_bit = ::bit_width(as<U32>(std::max(1, m))) - 1;

    overlap.assign(as<Size>(limit), 0);
    offset.assign(as<Size>(limit + 1), 0);
    intervals.clear();
    intervals.reserve(as<Size>(limit) * 10);

    Vec<PairI32> tmp;
    tmp.reserve(32);

    FOR(x, limit) {
      offset[x] = as<I32>(intervals.size());
      collect_for_x(x, tmp);

      I32 covered = 0;
      for (const auto [l, r] : tmp) {
        covered += (r - l + 1);
        intervals.push_back({l, r});
      }
      overlap[x] = covered;
    }
    offset[limit] = as<I32>(intervals.size());
  }

  [[nodiscard]] inline auto overlap_count(const I32 x) const -> I32 {
    return overlap[x];
  }

  [[nodiscard]] inline auto interval_begin(const I32 x) const -> I32 {
    return offset[x];
  }

  [[nodiscard]] inline auto interval_end(const I32 x) const -> I32 {
    return offset[x + 1];
  }

  [[nodiscard]] inline auto interval_at(const I32 idx) const -> PairI32 {
    return intervals[idx];
  }

private:
  void collect_for_x(const I32 x, Vec<PairI32>& out) const {
    out.clear();
    if ((x >> (top_bit + 1)) != 0) return;

    const auto dfs = [&](
        const auto& self,
        const I32 bit, const bool tight_j,
        const bool tight_y, const I32 prefix
      ) -> void {
      if (bit < 0) {
        out.push_back({prefix, prefix});
        return;
      }

      if (!tight_j && !tight_y) {
        const I32 len = (1 << (bit + 1));
        const I32 l = prefix << (bit + 1);
        out.push_back({l, l + len - 1});
        return;
      }

      const I32 mb = (m >> bit) & 1;
      const I32 xb = (x >> bit) & 1;
      const I32 max_jb = tight_j ? mb : 1;

      for (I32 jb = 0; jb <= max_jb; ++jb) {
        const I32 yb = jb ^ xb;
        if (tight_y && yb > mb) continue;

        const bool next_tight_j = tight_j && (jb == mb);
        const bool next_tight_y = tight_y && (yb == mb);
        self(self, bit - 1, next_tight_j, next_tight_y, (prefix << 1) | jb);
      }
    };

    dfs(dfs, top_bit, true, true, 0);
    if (out.empty()) return;

    Vec<PairI32> merged;
    merged.reserve(out.size());
    for (const auto [l, r] : out) {
      if (!merged.empty() && l <= merged.back().second + 1) {
        merged.back().second = std::max(merged.back().second, r);
      } else {
        merged.push_back({l, r});
      }
    }
    out.swap(merged);
  }
};

//===----------------------------------------------------------------------===//
/* Main Solver Function */

constexpr I32 MAX_L = 1 << 19;
constexpr I32 HEAVY_THRESHOLD = 420;

[[nodiscard]] auto mobius_ctx() -> const Mobius& {
  static const Mobius mobius(MAX_L - 1);
  return mobius;
}

void solve() {
  I64 n;
  I32 m;
  IN(n, m);

  I32 limit_l = 1;
  while (limit_l <= m) limit_l <<= 1;
  const I32 max_xor = limit_l - 1;

  VecI64 ind(as<Size>(limit_l), 0);
  for (I32 x = 0; x <= m; ++x) ind[x] = 1;

  VecI64 ind_fwht = ind;
  FWHT::xor_transform(ind_fwht, false);

  const Mobius& mobius = mobius_ctx();
  VecI32 weight = mobius.build_divisibility_weights(n, max_xor);

  I64 sum_weights = 0;
  for (I32 d = 1; d <= max_xor; ++d) sum_weights += weight[d];

  I64 total = 0;
  VecI64 mult(as<Size>(limit_l));

  // Precompute interval decomposition:
  // J_x = { j in [0, m] : (j xor x) <= m } for all x in [0, limit_l - 1].
  const XorBoundIntervals xor_intervals(m, limit_l);

  for (I32 d = 1; d <= max_xor; ++d) {
    const I32 w = weight[d];
    if (w == 0) continue;

    I64 sum_sq = 0;

    if (d <= HEAVY_THRESHOLD) {
      std::fill(mult.begin(), mult.end(), 0);
      for (I32 x = 0; x < limit_l; x += d) mult[x] = 1;

      FWHT::xor_transform(mult, false);
      for (I32 i = 0; i < limit_l; ++i) mult[i] *= ind_fwht[i];
      FWHT::xor_transform(mult, true);

      for (I32 j = 0; j <= m; ++j) {
        const I64 v = mult[j];
        sum_sq += v * v;
      }
    } else if (d > m) {
      // Here multiples of d inside [0, limit_l - 1] are exactly {0, d}.
      const I64 r = xor_intervals.overlap_count(d);
      sum_sq = as<I64>(m + 1) + 3LL * r;
    } else {
      Vec<PairI32> events;
      events.reserve((limit_l / d + 1) * 20);
      for (I32 x = 0; x < limit_l; x += d) {
        const I32 from = xor_intervals.interval_begin(x);
        const I32 to = xor_intervals.interval_end(x);
        for (I32 idx = from; idx < to; ++idx) {
          const auto [l, r] = xor_intervals.interval_at(idx);
          events.push_back({l, +1});
          if (r < m) events.push_back({r + 1, -1});
        }
      }

      std::sort(events.begin(), events.end());

      I64 cur_cover = 0;
      I32 prev_pos = 0;
      I32 i = 0;
      while (i < sz(events)) {
        const I32 pos = events[i].first;
        if (pos > prev_pos) {
          const I64 len = as<I64>(pos - prev_pos);
          sum_sq += len * cur_cover * cur_cover;
          prev_pos = pos;
        }

        I64 delta = 0;
        while (i < sz(events) && events[i].first == pos) {
          delta += events[i].second;
          ++i;
        }
        cur_cover += delta;
      }

      if (prev_pos <= m) {
        const I64 len = as<I64>(m - prev_pos + 1);
        sum_sq += len * cur_cover * cur_cover;
      }
    }

    total += as<I64>(w) * sum_sq;
  }

  // Correction for (A, B) = (0, 0): i = j = k.
  // For n >= 1 this case is invalid (gcd(0, 0) = 0 does not divide n),
  // while the finite divisor expansion contributes (m + 1) * sum_d weight[d].
  total -= as<I64>(m + 1) * sum_weights;

  OUT(total);
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
