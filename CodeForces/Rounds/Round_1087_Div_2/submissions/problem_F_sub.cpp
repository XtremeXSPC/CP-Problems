//===----------------------------------------------------------------------===//
/**
 * @file: problem_F_sub.cpp
 * @generated: 2026-03-25 00:02:58
 * @source: problem_F.cpp
 * @author: Costantino Lombardi
 *
 * @brief: Codeforces Round 1087 (Div. 2) - Problem F
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
/* Data Structures & Algorithms for the Problem */

template <typename T>
struct FenwickTree {
  I32 n;
  Vec<T> tree;

  FenwickTree(I32 size) : n(size), tree(size + 1, T{}) {}

  void add(I32 idx, T val) {
    idx++;  // 1-indexed internally.
    while (idx <= n) {
      tree[idx] += val;
      idx += idx & -idx;
    }
  }

  T sum(I32 idx) {
    idx++;
    T res{};
    while (idx > 0) {
      res += tree[idx];
      idx -= idx & -idx;
    }
    return res;
  }

  T range_sum(I32 l, I32 r) { return sum(r) - (l > 0 ? sum(l - 1) : T{}); }

  I32 lower_bound(T val) {
    if (n == 0) return 0;
    if (val <= T{}) return 0;
    if (val > sum(n - 1)) return n;
    I32 pos = 0;
    I32 pw = 1;
    while (pw <= n) pw *= 2;
    pw /= 2;

    while (pw > 0) {
      I32 next = pos + pw;
      if (next <= n && tree[next] < val) {
        val -= tree[next];
        pos = next;
      }
      pw /= 2;
    }
    return pos;
  }
};

//===----------------------------------------------------------------------===//
/* Main Solver Function */

struct State {
  I32 depth = 0;
  I32 leaf  = 0;
  I32 heavy = -1;
  I64 sum   = 0;
};

struct Candidate {
  bool valid = false;
  I32 nei    = -1;
  I32 depth  = -1;
  I32 leaf   = Limits<I32>::max();
  I64 sum    = 0;
};

auto better(const Candidate& a, const Candidate& b) -> bool {
  if (!a.valid) return false;
  if (!b.valid) return true;
  if (a.depth != b.depth) return a.depth > b.depth;
  return a.leaf < b.leaf;
}

auto leaf_state(I32 u, const VecI64& values) -> State {
  State st;
  st.depth = 0;
  st.leaf  = u;
  st.heavy = -1;
  st.sum   = values[u];
  return st;
}

void solve() {
  INT(n, k);

  VecI64 values(n + 1);
  FOR(i, 1, n + 1) IN(values[i]);

  Vec2D<I32> graph(n + 1);
  FOR(_, n - 1) {
    INT(u, v);
    graph[u].eb(v);
    graph[v].eb(u);
  }

  VecI32 parent(n + 1, 0);
  Vec2D<I32> children(n + 1);
  VecI32 order;
  order.reserve(n);

  {
    VecI32 st = {1};
    parent[1] = 0;
    while (!st.empty()) {
      const I32 u = st.back();
      st.pop_back();
      order.eb(u);
      for (I32 v : graph[u]) {
        if (v == parent[u]) continue;
        parent[v] = u;
        children[u].eb(v);
        st.eb(v);
      }
    }
  }

  Vec<State> down(n + 1), up(n + 1), root_state(n + 1);

  for (I32 it = n - 1; it >= 0; --it) {
    const I32 u = order[it];
    if (children[u].empty()) {
      down[u] = leaf_state(u, values);
      continue;
    }

    Candidate best;
    for (I32 v : children[u]) {
      Candidate cand;
      cand.valid = true;
      cand.nei   = v;
      cand.depth = down[v].depth + 1;
      cand.leaf  = down[v].leaf;
      cand.sum   = values[u] + down[v].sum;
      if (better(cand, best)) best = cand;
    }

    State st;
    st.depth = best.depth;
    st.leaf  = best.leaf;
    st.heavy = best.nei;
    st.sum   = best.sum;
    down[u]  = st;
  }

  auto get_state = [&](I32 par, I32 node) -> const State& {
    if (par == 0) {
      return root_state[node];
    }
    if (parent[node] == par) {
      return down[node];
    }
    my_assert(parent[par] == node);
    return up[par];
  };

  for (I32 u : order) {
    Candidate best1, best2;

    for (I32 v : graph[u]) {
      const State& st = get_state(u, v);
      Candidate cand;
      cand.valid = true;
      cand.nei   = v;
      cand.depth = st.depth + 1;
      cand.leaf  = st.leaf;
      cand.sum   = values[u] + st.sum;

      if (better(cand, best1)) {
        best2 = best1;
        best1 = cand;
      } else if (better(cand, best2)) {
        best2 = cand;
      }
    }

    if (!best1.valid) {
      root_state[u] = leaf_state(u, values);
    } else {
      State st;
      st.depth = best1.depth;
      st.leaf  = best1.leaf;
      st.heavy = best1.nei;
      st.sum   = best1.sum;
      root_state[u] = st;
    }

    for (I32 v : children[u]) {
      const Candidate chosen = (best1.valid && best1.nei == v ? best2 : best1);
      if (!chosen.valid) {
        up[v] = leaf_state(u, values);
      } else {
        State st;
        st.depth = chosen.depth;
        st.leaf  = chosen.leaf;
        st.heavy = chosen.nei;
        st.sum   = chosen.sum;
        up[v] = st;
      }
    }
  }

  VecI64 coords;
  coords.reserve(3 * n + 5);
  FOR(u, 1, n + 1) {
    coords.eb(root_state[u].sum - values[u]);
    for (I32 v : graph[u]) {
      coords.eb(get_state(u, v).sum);
    }
  }
  std::sort(all(coords));
  coords.erase(std::unique(all(coords)), coords.end());

  FenwickTree<I64> bit_cnt(as<I32>(coords.size()));
  FenwickTree<I64> bit_sum(as<I32>(coords.size()));
  I64 active_count = 0;
  I64 active_sum   = 0;

  auto add_value = [&](I64 value, I32 delta) -> void {
    const I32 idx = as<I32>(std::lower_bound(all(coords), value) - coords.begin());
    bit_cnt.add(idx, delta);
    bit_sum.add(idx, value * delta);
    active_count += delta;
    active_sum   += value * delta;
  };

  auto query_top = [&](I64 take) -> I64 {
    if (take <= 0) return 0;
    if (take >= active_count) return active_sum;

    const I64 need_small = active_count - take;
    const I32 idx = bit_cnt.lower_bound(need_small);
    const I64 cnt_before = (idx > 0 ? bit_cnt.sum(idx - 1) : 0);
    const I64 sum_before = (idx > 0 ? bit_sum.sum(idx - 1) : 0);
    const I64 at_idx     = need_small - cnt_before;
    const I64 small_sum  = sum_before + at_idx * coords[idx];
    return active_sum - small_sum;
  };

  auto is_excluded = [&](I32 node, I32 par, I32 nei) -> bool {
    if (par != 0 && nei == par) {
      return true;
    }
    const I32 heavy = get_state(par, node).heavy;
    return nei == heavy;
  };

  auto update_node_par = [&](I32 node, I32 old_par, I32 new_par) -> void {
    VecI32 cand;
    cand.reserve(4);

    if (old_par != 0) {
      cand.eb(old_par);
    }
    if (new_par != 0) {
      cand.eb(new_par);
    }

    const I32 old_heavy = get_state(old_par, node).heavy;
    const I32 new_heavy = get_state(new_par, node).heavy;
    if (old_heavy != -1) {
      cand.eb(old_heavy);
    }
    if (new_heavy != -1) {
      cand.eb(new_heavy);
    }

    std::sort(all(cand));
    cand.erase(std::unique(all(cand)), cand.end());

    for (I32 nei : cand) {
      const bool old_ex = is_excluded(node, old_par, nei);
      const bool new_ex = is_excluded(node, new_par, nei);
      if (old_ex == new_ex) {
        continue;
      }

      const I64 path_sum = get_state(node, nei).sum;
      if (old_ex && !new_ex) {
        add_value(path_sum, +1);
      } else {
        add_value(path_sum, -1);
      }
    }
  };

  auto apply_contrib = [&](I32 node, I32 par, I32 delta) -> void {
    const State& st_parent = get_state(par, node);
    const I32 heavy = st_parent.heavy;

    for (I32 nei : graph[node]) {
      if (nei == par) continue;
      if (nei == heavy) continue;
      const I64 path_sum = get_state(node, nei).sum;
      add_value(path_sum, delta);
    }
  };

  auto move_root = [&](I32 old_root, I32 new_root) -> void {
    update_node_par(old_root, 0, new_root);
    update_node_par(new_root, old_root, 0);
  };

  FOR(u, 1, n + 1) {
    const I32 par = (u == 1 ? 0 : parent[u]);
    apply_contrib(u, par, +1);
  }

  I64 answer = 0;
  auto evaluate_root = [&](I32 root) -> void {
    I64 best = values[root];
    if (k > 1) {
      const I64 root_path = root_state[root].sum - values[root];
      add_value(root_path, +1);
      const I64 take = std::min<I64>(k - 1, active_count);
      best += query_top(take);
      add_value(root_path, -1);
    }
    answer = std::max(answer, best);
  };

  struct Frame {
    I32 u;
    I32 idx;
  };

  Vec<Frame> st;
  st.reserve(n);
  st.push_back({1, 0});

  while (!st.empty()) {
    Frame& cur = st.back();

    if (cur.idx == 0) {
      evaluate_root(cur.u);
    }

    if (cur.idx < as<I32>(children[cur.u].size())) {
      const I32 v = children[cur.u][cur.idx++];
      move_root(cur.u, v);
      st.push_back({v, 0});
    } else {
      const I32 u = cur.u;
      st.pop_back();
      if (!st.empty()) {
        const I32 p = st.back().u;
        move_root(u, p);
      }
    }
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
