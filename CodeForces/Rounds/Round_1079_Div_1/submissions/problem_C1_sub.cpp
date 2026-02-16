//===----------------------------------------------------------------------===//
/**
 * @file: problem_C1_sub.cpp
 * @generated: 2026-02-17 00:15:10
 * @source: problem_C1.cpp
 * @author: Costantino Lombardi
 *
 * @brief: Codeforces Round 1079 (Div. 1) - Problem C1
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
/* Advanced Type System and Aliases */
// clang-format off

// Fundamental type aliases with explicit sizes:
using I8  = std::int8_t;
using I16 = std::int16_t;
using I32 = std::int32_t;
using I64 = std::int64_t;
using U8  = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;

// Extended precision types (when available):
#ifdef __SIZEOF_INT128__
  using I128 = __int128;
  using U128 = unsigned __int128;
#define HAS_INT128 1
#else
  // Fallback for compilers that don't support 128-bit integers.
  using I128 = std::int64_t;
  using U128 = std::uint64_t;
#define HAS_INT128 0
#endif

// Floating point types:
using F32 = float;
using F64 = double;
using F80 = long double;

#ifdef __FLOAT128__
  using F128 = __float128;
#define HAS_FLOAT128 1
#else
  // Fallback for compilers that don't support 128-bit floats.
  using F128 = long double;
#define HAS_FLOAT128 0
#endif

// Legacy short aliases (deprecated -- use I64, U64, F80 instead):
using ll  [[deprecated("use I64 instead")]] = I64;
using ull [[deprecated("use U64 instead")]] = U64;
using ld  [[deprecated("use F80 instead")]] = F80;

// Container type aliases:
template <class T>
using Vec = std::vector<T>;
template <class T>
using Deque = std::deque<T>;
template <class T>
using List = std::list<T>;
template <class T>
using Set = std::set<T>;
template <class T>
using MultiSet = std::multiset<T>;
template <class T>
using UnorderedSet = std::unordered_set<T>;
template <class K, class V>
using Map = std::map<K, V>;
template <class K, class V>
using MultiMap = std::multimap<K, V>;
template <class K, class V>
using UnorderedMap = std::unordered_map<K, V>;
template <class T>
using Stack = std::stack<T, std::deque<T>>;
template <class T>
using Queue = std::queue<T, std::deque<T>>;
template <class T>
using PriorityQueue = std::priority_queue<T, std::vector<T>>;
template <class T>
using MinPriorityQueue = std::priority_queue<T, std::vector<T>, std::greater<T>>;

// Canonical multidimensional aliases:
template <class T>
using Vec2 = Vec<Vec<T>>;
template <class T>
using Vec3 = Vec<Vec2<T>>;

// Legacy container aliases (deprecated -- use Vec, Vec2, Vec3 instead):
template <class T>
using VC [[deprecated("use Vec<T> instead")]] = Vec<T>;
template <class T>
using VVC [[deprecated("use Vec2<T> instead")]] = Vec2<T>;
template <class T>
using VVVC [[deprecated("use Vec3<T> instead")]] = Vec3<T>;

// Pair and tuple aliases:
template <class T, class U>
using Pair = std::pair<T, U>;
template <class T, class U>
using P = Pair<T, U>;

using PII = Pair<I32, I32>;
using PLL = Pair<I64, I64>;
using PLD = Pair<F80, F80>;

// Specialized container aliases:
using VI   = Vec<I32>;
using VLL  = Vec<I64>;
using VVI  = Vec<VI>;
using VVLL = Vec<VLL>;
using VB   = Vec<bool>;
using VS   = Vec<std::string>;
using VU8  = Vec<U8>;
using VU32 = Vec<U32>;
using VU64 = Vec<U64>;
using VF   = Vec<F64>;

// Vector of pairs:
using VPII = Vec<PII>;
using VPLL = Vec<PLL>;
template <class T, class U>
using VP = Vec<P<T, U>>;

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

#define overload4(a, b, c, d, e, ...) e
#define overload3(a, b, c, d, ...) d
#define FOR(...) overload4(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)(__VA_ARGS__)
#define FOR_R(...) overload3(__VA_ARGS__, FOR3_R, FOR2_R, FOR1_R)(__VA_ARGS__)

// Range-based iteration (deprecated -- use FOR/FOR_R and all/rall instead):
#define REP(i, n) _Pragma("GCC warning \"REP is deprecated, use FOR(i, n)\"") for (I64 i = 0, _rep_n = static_cast<I64>(n); i < _rep_n; ++i)
#define RREP(i, n) _Pragma("GCC warning \"RREP is deprecated, use FOR_R(i, n)\"") for (I64 i = static_cast<I64>(n) - 1; i >= 0; --i)
#define ALL(x) std::ranges::begin(x), std::ranges::end(x)
#define RALL(x) std::ranges::rbegin(x), std::ranges::rend(x)

// Container utility macros:
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x) (I64)(x).size()
#define len(x) sz(x)
#define pb push_back
#define eb emplace_back
#define fi first
#define se second
// mp/mt are obsolete with C++17 CTAD -- prefer brace init or direct construction.
#define mp make_pair
#define mt make_tuple
#define elif else if

// Advanced container operations:
#define UNIQUE(x) (std::ranges::sort(x), x.erase(std::ranges::unique(x).begin(), x.end()), x.shrink_to_fit())
#define LB(c, x) (I64)std::distance((c).begin(), std::ranges::lower_bound(c, x))
#define UB(c, x) (I64)std::distance((c).begin(), std::ranges::upper_bound(c, x))
#define SUM(x) std::accumulate(all(x), 0LL)
#define MIN(x) *std::ranges::min_element(x)
#define MAX(x) *std::ranges::max_element(x)

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

#if !defined(CP_FAST_IO_NAMESPACE_DEFINED)
namespace fast_io {
template <class T>
inline void read_integer(T& x) { cp_io::read(x); }

template <class T>
inline void write_integer(T x) { cp_io::write_one(x); }

inline void read_char(char& x) { cp_io::read(x); }
inline void read_string(std::string& x) { cp_io::read(x); }
inline void write_char(char c) { std::cout.put(c); }
inline void write_string(const std::string& s) { cp_io::write_one(s); }
inline void flush_output() { std::cout.flush(); }

using cp_io::read;
using cp_io::write;
using cp_io::writeln;
} // namespace fast_io
#endif

// Input/Output macros.
#ifndef IN
  #define IN(...) cp_io::read(__VA_ARGS__)
#endif
#ifndef OUT
  #define OUT(...) cp_io::writeln(__VA_ARGS__)
#endif
#ifndef FLUSH
  #define FLUSH() std::cout.flush()
#endif

// Convenient input macros.
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

// Answer macros.
inline void YES(bool condition = true) { OUT(condition ? "YES" : "NO"); }
inline void NO(bool condition = true) { YES(!condition); }
inline void Yes(bool condition = true) { OUT(condition ? "Yes" : "No"); }
inline void No(bool condition = true) { Yes(!condition); }

//===----------------------------------------------------------------------===//
/* Type Aliases */

using Edge  = Pair<I32, I32>;
using Path  = Vec<I32>;
using MPath = std::optional<Path>;

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n);

  auto lex_compare = [&](const Path& a, const Path& b) -> I32 {
    const I32 common = std::min<I32>(a.size(), b.size());
    FOR(i, common) {
      if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    if (a.size() == b.size()) return 0;
    return a.size() < b.size() ? -1 : 1;
  };

  auto cmp_path_vs_seq = [&](const MPath& path, const Path& seq) -> I32 {
    if (!path.has_value()) return 1;
    return lex_compare(path.value(), seq);
  };

  std::map<I32, MPath> cache;
  auto ask = [&](I32 k) -> const MPath& {
    if (cache.contains(k)) return cache[k];

    OUT("?", k);
    FLUSH();

    I32 len = 0;
    if (!(std::cin >> len)) std::exit(0);
    if (len == -1) std::exit(0);
    if (len == 0) {
      cache[k] = std::nullopt;
      return cache[k];
    }

    Path path(len);
    FOR(i, len) IN(path[i]);
    cache[k] = path;
    return cache[k];
  };

  auto find_path_count = [&]() -> I32 {
    I32 lo = 1;
    I32 hi = (1 << 30) + 1;
    while (lo + 1 < hi) {
      const I32 mid = lo + (hi - lo) / 2;
      if (!ask(mid).has_value()) hi = mid;
      else lo = mid;
    }
    return lo;
  };

  auto lower_bound_path = [&](I32 path_count, const Path& seq) -> I32 {
    I32 lo = 1;
    I32 hi = path_count + 1;
    while (lo < hi) {
      const I32 mid = lo + (hi - lo) / 2;
      if (cmp_path_vs_seq(ask(mid), seq) < 0) lo = mid + 1;
      else hi = mid;
    }
    return lo;
  };

  const I32 path_count = find_path_count();
  Vec<I32> start(n + 2, 0), end(n + 2, 0);

  start[1] = 1;
  FOR(u, 1, n) {
    end[u] = lower_bound_path(path_count, Path{u + 1});
    start[u + 1] = end[u];
  }
  end[n] = path_count + 1;

  const I32 sentinel = n + 1;
  Vec<Edge> edges;
  Vec<Vec<bool>> seen(n + 1, Vec<bool>(n + 1, false));

  FOR(u, 1, n + 1) {
    I32 cur = start[u] + 1; // Skip singleton path [u].
    while (cur < end[u]) {
      const MPath& path_opt = ask(cur);
      if (!path_opt.has_value()) std::exit(0);
      const Path& path = path_opt.value();
      if (path.size() < 2) std::exit(0);

      const I32 v = path[1];
      if (!seen[u][v]) {
        seen[u][v] = true;
        edges.eb(u, v);
      }

      cur = lower_bound_path(path_count, Path{u, v, sentinel});
    }
  }

  OUT("!", static_cast<I32>(edges.size()));
  for (const auto& [u, v] : edges) OUT(u, v);
  FLUSH();
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  I32 T = 0;
  if (!(std::cin >> T)) return 0;
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
