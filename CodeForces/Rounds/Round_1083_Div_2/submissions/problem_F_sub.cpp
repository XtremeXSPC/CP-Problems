//===----------------------------------------------------------------------===//
/**
 * @file: problem_F_sub.cpp
 * @generated: 2026-02-27 00:52:07
 * @source: problem_F.cpp
 * @author: Costantino Lombardi
 *
 * @brief: Codeforces Round 1083 (Div. 2) - Problem F
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
/* Main Solver Function */

struct DSU {
  VecI32 parent;
  VecI32 size;
  VecI64 comp_weight;
  VecBool has_outer;

  explicit DSU(const I32 n) :
      parent(as<Size>(n)), size(as<Size>(n), 1), comp_weight(as<Size>(n), 0),
      has_outer(as<Size>(n), false) {
    FOR(i, n) parent[i] = i;
  }

  [[nodiscard]] auto find(const I32 x) -> I32 {
    if (parent[x] == x) return x;
    return parent[x] = find(parent[x]);
  }

  void unite(const I32 a, const I32 b) {
    I32 ra = find(a);
    I32 rb = find(b);
    if (ra == rb) return;

    if (size[ra] < size[rb]) std::swap(ra, rb);
    parent[rb] = ra;
    size[ra] += size[rb];
    comp_weight[ra] += comp_weight[rb];
    has_outer[ra] = has_outer[ra] || has_outer[rb];
  }
};

void solve() {
  INT(n, m);

  Vec2D<I64> w(as<Size>(n - 1), VecI64(as<Size>(m)));
  Vec2D<I64> v(as<Size>(n), VecI64(as<Size>(m - 1)));
  FOR(i, n - 1) FOR(j, m) IN(w[i][j]);
  FOR(i, n) FOR(j, m - 1) IN(v[i][j]);

  VecStr p(as<Size>(n - 1));
  VecStr q(as<Size>(n));
  FOR(i, n - 1) IN(p[i]);
  FOR(i, n) IN(q[i]);

  const I32 rows = n - 1;
  const I32 cols = m - 1;
  const I32 cells = rows * cols;

  const auto cell_id = [&](const I32 r, const I32 c) -> I32 {
    return r * cols + c + 1;
  };

  DSU dsu(cells + 1);
  dsu.has_outer[0] = true;

  FOR(r, rows) {
    FOR(c, cols) {
      const I32 id = cell_id(r, c);
      dsu.comp_weight[id] = w[r][c] - w[r][c + 1] + v[r][c] - v[r + 1][c];
    }
  }

  // Constraints from vertical streets between (i, j) and (i + 1, j): p[i][j].
  FOR(r, rows) {
    FOR(c, m) {
      if (p[r][c] != '0') continue;
      const I32 left_face = (c == 0 ? 0 : cell_id(r, c - 1));
      const I32 right_face = (c == m - 1 ? 0 : cell_id(r, c));
      dsu.unite(left_face, right_face);
    }
  }

  // Constraints from horizontal streets between (i, j) and (i, j + 1): q[i][j].
  FOR(r, n) {
    FOR(c, cols) {
      if (q[r][c] != '0') continue;
      const I32 up_face = (r == 0 ? 0 : cell_id(r - 1, c));
      const I32 down_face = (r == n - 1 ? 0 : cell_id(r, c));
      dsu.unite(up_face, down_face);
    }
  }

  I64 answer = 0;
  FOR(node, cells + 1) {
    if (dsu.find(node) != node) continue;
    if (dsu.has_outer[node]) continue;
    if (dsu.comp_weight[node] > 0) answer += dsu.comp_weight[node];
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

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
