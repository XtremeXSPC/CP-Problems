//===----------------------------------------------------------------------===//
/**
 * @file: problem_E2_sub.cpp
 * @generated: 2026-02-20 17:25:58
 * @source: problem_E2.cpp
 * @author: Costantino Lombardi
 *
 * @brief: Codeforces Round 1079 (Div. 1) - Problem E2
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

// Associative containers (kept compatible with PCH aliases).
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

// Container adaptors (kept compatible with PCH aliases).
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
/* Data Structures & Algorithms for the Problem */

/// @brief Default min operation functor for SparseTable.
template <typename T>
struct SparseTableMinOp {
  constexpr T operator()(const T& a, const T& b) const { return a < b ? a : b; }
};

/// @brief Sparse table for static idempotent range queries.
template <typename T, typename Op = SparseTableMinOp<T>>
struct SparseTable {
  Vec<Vec<T>> table;
  Vec<I32> lg;
  Op op;

  SparseTable() = default;
  explicit SparseTable(const Vec<T>& v, Op merge_op = Op{}) { build(v, merge_op); }

  /// @brief Precomputes sparse table for input vector.
  void build(const Vec<T>& v, Op merge_op = Op{}) {
    op = merge_op;
    const I32 n = sz(v);
    lg.assign(n + 1, 0);
    FOR(i, 2, n + 1) lg[i] = lg[i / 2] + 1;

    if (n == 0) {
      table.clear();
      return;
    }

    const I32 max_log = lg[n] + 1;
    table.assign(max_log, Vec<T>(n));
    FOR(i, n) table[0][i] = v[i];

    FOR(k, 1, max_log) {
      FOR(i, n - (1 << k) + 1) {
        table[k][i] = op(table[k - 1][i], table[k - 1][i + (1 << (k - 1))]);
      }
    }
  }

  /// @brief Query over inclusive range [l, r].
  T query(I32 l, I32 r) const {
    const I32 k = lg[r - l + 1];
    return op(table[k][l], table[k][r - (1 << k) + 1]);
  }
};

/**
 * @brief Suffix array + Kasai LCP with O(1) LCP queries.
 *
 * Construction:
 *  - suffix array in O(n log n) via counting-sort doubling on cyclic shifts
 *  - LCP in O(n) via Kasai
 *  - RMQ over LCP in O(n log n)
 */
struct SuffixArray {
  std::string s;
  Vec<I32> sa;    // Sorted suffix start positions.
  Vec<I32> rank;  // rank[i] = position of suffix i in sa.
  Vec<I32> lcp;   // lcp[k] = LCP(sa[k], sa[k+1]), size n-1.

  Vec<I32> lg;
  Vec<Vec<I32>> st;

  SuffixArray() = default;
  explicit SuffixArray(const std::string& str) { build(str); }

  /// @brief Rebuilds all structures for input string.
  void build(const std::string& str) {
    s = str;
    build_sa();
    build_lcp();
    build_rmq();
  }

  /// @brief Returns LCP length of suffixes starting at i and j.
  I32 lcp_query(I32 i, I32 j) const {
    if (i == j) return sz(s) - i;
    I32 ri = rank[i];
    I32 rj = rank[j];
    if (ri > rj) std::swap(ri, rj);
    return rmq(ri, rj - 1);
  }

  /// @brief Finds all occurrences of pattern using binary search on SA.
  Vec<I32> find_pattern(const std::string& pattern) const {
    Vec<I32> result;
    const I32 n = sz(s);

    auto cmp_suffix_pattern = [&](I32 suffix_pos) -> I32 {
      I32 i = 0;
      while (suffix_pos + i < n && i < sz(pattern)) {
        char a = s[suffix_pos + i];
        char b = pattern[i];
        if (a < b) return -1;
        if (a > b) return 1;
        ++i;
      }
      return (i == sz(pattern)) ? 0 : -1;
    };

    I32 left = 0;
    I32 right = n;
    while (left < right) {
      I32 mid = (left + right) / 2;
      if (cmp_suffix_pattern(sa[mid]) < 0) {
        left = mid + 1;
      } else {
        right = mid;
      }
    }

    while (left < n && cmp_suffix_pattern(sa[left]) == 0) {
      result.push_back(sa[left]);
      ++left;
    }
    return result;
  }

private:
  /// @brief Builds suffix array and rank array for s.
  void build_sa() {
    const I32 n = sz(s);
    sa.clear();
    rank.clear();
    if (n == 0) return;

    // Build SA on s + sentinel(0) as cyclic shifts.
    const I32 m = n + 1;
    Vec<I32> a(m);
    FOR(i, n) a[i] = as<U8>(s[i]) + 1;
    a[n] = 0;

    Vec<I32> p(m), c(m);
    {
      const I32 alphabet = std::max<I32>(256 + 1, m);
      Vec<I32> cnt(alphabet, 0);
      FOR(i, m) cnt[a[i]]++;
      FOR(i, 1, alphabet) cnt[i] += cnt[i - 1];
      FOR_R(i, m) p[--cnt[a[i]]] = i;

      c[p[0]] = 0;
      I32 classes = 1;
      FOR(i, 1, m) {
        if (a[p[i]] != a[p[i - 1]]) ++classes;
        c[p[i]] = classes - 1;
      }
    }

    I32 k = 0;
    while ((1 << k) < m) {
      FOR(i, m) p[i] = (p[i] - (1 << k) + m) % m;

      I32 classes = 0;
      FOR(i, m) classes = std::max(classes, c[i] + 1);
      Vec<I32> cnt(classes, 0);
      FOR(i, m) cnt[c[p[i]]]++;
      FOR(i, 1, classes) cnt[i] += cnt[i - 1];

      Vec<I32> pn(m);
      FOR_R(i, m) pn[--cnt[c[p[i]]]] = p[i];
      p.swap(pn);

      Vec<I32> cn(m);
      cn[p[0]] = 0;
      classes = 1;
      FOR(i, 1, m) {
        auto cur = Pair<I32, I32>{c[p[i]], c[(p[i] + (1 << k)) % m]};
        auto prv = Pair<I32, I32>{c[p[i - 1]], c[(p[i - 1] + (1 << k)) % m]};
        if (cur != prv) ++classes;
        cn[p[i]] = classes - 1;
      }
      c.swap(cn);
      ++k;
    }

    // Drop sentinel suffix.
    sa.resize(n);
    FOR(i, n) sa[i] = p[i + 1];
    rank.assign(n, 0);
    FOR(i, n) rank[sa[i]] = i;
  }

  /// @brief Builds LCP array using Kasai algorithm.
  void build_lcp() {
    const I32 n = sz(s);
    lcp.assign(n > 0 ? n - 1 : 0, 0);
    if (n <= 1) return;

    I32 k = 0;
    FOR(i, n) {
      if (rank[i] == n - 1) {
        k = 0;
        continue;
      }
      I32 j = sa[rank[i] + 1];
      while (i + k < n && j + k < n && s[i + k] == s[j + k]) ++k;
      lcp[rank[i]] = k;
      if (k) --k;
    }
  }

  /// @brief Builds RMQ sparse table over LCP array.
  void build_rmq() {
    const I32 n = sz(lcp);
    lg.assign(n + 1, 0);
    FOR(i, 2, n + 1) lg[i] = lg[i / 2] + 1;
    if (n == 0) {
      st.clear();
      return;
    }

    const I32 max_log = lg[n] + 1;
    st.assign(max_log, Vec<I32>(n));
    FOR(i, n) st[0][i] = lcp[i];
    FOR(k, 1, max_log) {
      FOR(i, n - (1 << k) + 1) {
        st[k][i] = std::min(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
      }
    }
  }

  /// @brief RMQ query on LCP array for range [l, r].
  I32 rmq(I32 l, I32 r) const {
    if (l > r) return 0;
    I32 k = lg[r - l + 1];
    return std::min(st[k][l], st[k][r - (1 << k) + 1]);
  }
};

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n, m);
  STR(s, t);

  std::string all = s + "%" + t + "$";
  SuffixArray sa(all);
  SparseTable<I32> lcp_st(sa.lcp);

  auto get_lcp = [&](I32 i, I32 j) -> I32 {
    if (i == j) return as<I32>(all.size()) - i;
    I32 ri = sa.rank[i];
    I32 rj = sa.rank[j];
    if (ri > rj) std::swap(ri, rj);
    return lcp_st.query(ri, rj - 1);
  };

  Vec<I32> from_s;
  FOR(i, as<I32>(sa.sa.size())) {
    if (sa.sa[i] < n) from_s.eb(i);
  }

  auto compare_suffix = [&](I32 start, I32 pos, char ch, I32 idx) -> Pair<I32, I32> {
    if (idx == 0 || idx > n) return {1, 0};

    pos -= start;
    idx--;
    I32 p = sa.sa[from_s[idx]];

    const I32 lcp1 = get_lcp(p, n + 1 + start);
    if (lcp1 < pos) {
      const char a = all[p + lcp1];
      const char b = all[n + 1 + start + lcp1];
      return (a < b) ? Pair<I32, I32>{1, lcp1} : Pair<I32, I32>{-1, lcp1};
    }

    if (lcp1 > pos) {
      char a = all[p + pos];
      char b;
      I32 len = pos;
      if (a != ch) {
        b = ch;
      } else {
        len = lcp1;
        a = all[p + lcp1];
        b = all[n + 1 + start + lcp1];
      }
      return (a < b) ? Pair<I32, I32>{1, len} : Pair<I32, I32>{-1, len};
    }

    char a = all[p + lcp1];
    char b = ch;
    if (a < b) return {1, pos};
    if (a > b) return {-1, pos};

    const I32 lcp2 = get_lcp(p + lcp1 + 1, n + 1 + start + lcp1 + 1);
    a = all[p + lcp1 + 1 + lcp2];
    b = all[n + 1 + start + lcp1 + 1 + lcp2];
    return (a < b) ? Pair<I32, I32>{1, pos + 1 + lcp2} : Pair<I32, I32>{-1, pos + 1 + lcp2};
  };

  auto get_max_len = [&](I32 start, I32 pos, char ch) -> I32 {
    I32 l = 0;
    I32 r = as<I32>(from_s.size());
    while (l <= r) {
      const I32 mid = l + (r - l) / 2;
      if (compare_suffix(start, pos, ch, mid).first == -1) {
        r = mid - 1;
      } else {
        l = mid + 1;
      }
    }
    I32 res = 0;
    res = std::max(res, compare_suffix(start, pos, ch, l - 1).second);
    res = std::max(res, compare_suffix(start, pos, ch, l).second);
    return res;
  };

  I32 ans = 0;
  I32 start = 0;
  I32 best = -1;

  FOR(i, m) {
    I32 mx_len = 0;
    FOR(ch, 'a', 'z' + 1) mx_len = std::max(mx_len, get_max_len(start, i, as<char>(ch)));
    best = std::max(best, mx_len);

    if (mx_len < i - start + 1) {
      ans++;
      start += best;
      best = -1;
      i = start - 1;
    }
  }

  if (start != m) ans++;
  OUT(ans);
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
