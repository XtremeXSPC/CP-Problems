//===----------------------------------------------------------------------===//
/**
 * @file: problem_F_sub.cpp
 * @generated: 2026-04-14 00:51:24
 * @source: problem_F.cpp
 * @author: C.L.
 *
 * @brief: Codeforces Round 1093 (Div. 2) - Problem F
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

using namespace std;

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
#define ROF(...) FOR_R(__VA_ARGS__)
#define FORD3(i, a, b) for (I64 i = (a); i >= (b); --i)
#define FORD4(i, a, b, c) for (I64 i = (a); i >= (b); i -= (c))
#define FORD(...) overload4(__VA_ARGS__, FORD4, FORD3)(__VA_ARGS__)

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

//===----------------------------------------------------------------------===//
/* Main Solver Function */

struct MinOp {
  auto operator()(const I32 A, const I32 B) const -> I32 { return min(A, B); }
};

struct MexTree {
  I32 lim;
  SegmentTree<I32, MinOp> seg;

  explicit MexTree(I32 size)
    : lim(size),
      seg(VecI32(size, 0), Limits<I32>::max(), MinOp{}) {}

  void set(I32 X, I32 val) { seg.update(X, val); }

  auto first_zero_dfs(I32 node, I32 L, I32 R, I32 start) const -> I32 {
    if (R <= start || L >= lim || seg.tree[node] > 0) return -1;
    if (node >= seg.n) return L;

    const I32 mid  = (L + R) / 2;
    const I32 left = first_zero_dfs(node * 2, L, mid, start);
    if (left != -1) return left;
    return first_zero_dfs(node * 2 + 1, mid, R, start);
  }

  auto first_zero(I32 start) const -> I32 {
    const I32 Res = first_zero_dfs(1, 0, seg.n, start);
    my_assert(Res != -1);
    return Res;
  }
};

void solve() {
  INT(N);

  VecI32 P(N + 1);
  FOR(U, 1, N + 1) IN(P[U]);

  Vec2D<I32> adj(N + 1);
  FOR(_, N - 1) {
    INT(U, V);
    adj[U].eb(V);
    adj[V].eb(U);
  }

  VecI32 parent(N + 1, 0), tin(N + 1), tout(N + 1), F(N + 1), G(N + 1);
  VecI32 ord;
  ord.reserve(N);

  MexTree mex_tree(N + 2);
  Vec<Array<I32, 3>> stk = {{1, 0, 0}};
  I32 timer = 0;
  I64 base  = 0;

  while (!stk.empty()) {
    const auto [U, Par, typ] = stk.back();
    stk.pop_back();

    if (typ == 0) {
      parent[U] = Par;
      mex_tree.set(P[U], 1);

      F[U] = mex_tree.first_zero(0);
      G[U] = mex_tree.first_zero(F[U] + 1);
      tin[U] = timer++;
      ord.eb(U);
      base += F[U];

      stk.eb(Array<I32, 3>{U, Par, 1});
      for (I32 I = sz(adj[U]) - 1; I >= 0; --I) {
        const I32 V = adj[U][I];
        if (V == Par) continue;
        stk.eb(Array<I32, 3>{V, U, 0});
      }
    } else {
      tout[U] = timer - 1;
      mex_tree.set(P[U], 0);
    }
  }

  Vec<VecI32> grp(N + 1);
  FOR(U, 1, N + 1) grp[F[U]].eb(U);

  VecI64 delta(N + 1, 0);
  FenwickTree<I64> bit_cnt(N), bit_sum(N);

  FOR(K, N + 1) {
    auto& nodes = grp[K];
    if (nodes.empty()) continue;

    VecI32 pos;
    pos.reserve(sz(nodes));
    for (I32 U : nodes) pos.eb(tin[U]);
    ranges::sort(pos);

    VecI32 by_g = nodes;
    ranges::sort(by_g, [&](const I32 A, const I32 B) {
      return G[A] < G[B];
    });

    VecI32 by_p = nodes;
    ranges::sort(by_p, [&](const I32 A, const I32 B) {
      return P[A] < P[B];
    });

    I32 ptr = 0;
    for (I32 V : by_p) {
      while (ptr < sz(by_g) && G[by_g[ptr]] <= P[V]) {
        const I32 U = by_g[ptr++];
        bit_cnt.add(tin[U], 1);
        bit_sum.add(tin[U], G[U]);
      }

      const I32 total = as<I32>(
        ranges::upper_bound(pos, tout[V]) - ranges::lower_bound(pos, tin[V])
      );
      const I64 cnt_small = bit_cnt.range_sum(tin[V], tout[V]);
      const I64 sum_small = bit_sum.range_sum(tin[V], tout[V]);

      delta[V] += sum_small
        + (as<I64>(total) - cnt_small) * P[V]
        - as<I64>(total) * F[V];
    }

    FOR(I, ptr) {
      const I32 U = by_g[I];
      bit_cnt.add(tin[U], -1);
      bit_sum.add(tin[U], -G[U]);
    }
  }

  VecI32 nodes = ord;
  ranges::sort(nodes, [&](const I32 A, const I32 B) {
    return F[A] > F[B];
  });

  VecI32 queries = ord;
  ranges::sort(queries, [&](const I32 A, const I32 B) {
    return max(F[A], P[A]) > max(F[B], P[B]);
  });

  FenwickTree<I64> bit_cnt_hi(N), bit_sum_hi(N);
  I32 ptr = 0;
  for (I32 V : queries) {
    const I32 need = max(F[V], P[V]);
    while (ptr < N && F[nodes[ptr]] > need) {
      const I32 U = nodes[ptr++];
      bit_cnt_hi.add(tin[U], 1);
      bit_sum_hi.add(tin[U], F[U]);
    }

    const I64 cnt_hi = bit_cnt_hi.range_sum(tin[V], tout[V]);
    const I64 sum_hi = bit_sum_hi.range_sum(tin[V], tout[V]);
    delta[V] -= sum_hi - cnt_hi * P[V];
  }

  I64 ans = base;
  FOR(U, 1, N + 1) ans = max(ans, base + delta[U]);
  cout << ans << '\n';
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  I32 T;
  if (!(cin >> T)) return 0;
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
