//===----------------------------------------------------------------------===//
/**
 * @file: problem_F_sub.cpp
 * @generated: 2026-02-20 17:26:05
 * @source: problem_F.cpp
 * @author: Costantino Lombardi
 *
 * @brief: Codeforces Round 1079 (Div. 1) - Problem F
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
/* Main Solver Function */

void solve() {
  LL(n, m);
  using Edge = Pair<I32, I32>;

  auto encode = [](I32 u, I32 v) -> U64 {
    if (u > v) std::swap(u, v);
    return (as<U64>(u) << 32) | as<U64>(as<U32>(v));
  };

  auto minimal_odd_k_for_m = [&](I64 need) -> I32 {
    I64 d = as<I64>(std::sqrtl(1.0L + 8.0L * as<F80>(need)));
    while ((d + 1) * (d + 1) <= 1 + 8 * need) ++d;
    while (d * d > 1 + 8 * need) --d;
    I64 k = (1 + d) / 2;
    while (k * (k - 1) / 2 < need) ++k;
    if ((k & 1) == 0) ++k;
    return as<I32>(k);
  };

  auto build_circulant = [&](I32 k, I32 deg_even) -> Vec<Edge> {
    const I32 half = deg_even / 2;
    Vec<Edge> edges;
    edges.reserve(as<std::size_t>(as<I64>(k) * deg_even / 2));
    FOR(d, 1, half + 1) {
      FOR(i, 1, k + 1) {
        I32 j = i + d;
        if (j > k) j -= k;
        if (i < j) {
          edges.push_back({i, j});
        } else {
          edges.push_back({j, i});
        }
      }
    }
    return edges;
  };

  auto construct_on_k = [&](I32 k, I64 need) -> Vec<Edge> {
    const I64 q = (2 * need) / k;
    const I64 r = (2 * need) % k;

    if ((q & 1) == 0) {
      Vec<Edge> edges = build_circulant(k, as<I32>(q));
      const I32 shift = k / 2;
      edges.reserve(edges.size() + as<std::size_t>(r / 2));
      FOR(i, 1, as<I32>(r / 2) + 1) edges.push_back({i, i + shift});
      return edges;
    }

    const Vec<Edge> base = build_circulant(k, as<I32>(q + 1));
    const I32 limit = k - as<I32>(r);
    UnorderedSet<U64> removed;
    removed.reserve(as<std::size_t>(limit));
    for (I32 i = 1; i < limit; i += 2) removed.insert(encode(i, i + 1));

    Vec<Edge> edges;
    edges.reserve(base.size());
    for (auto [u, v] : base) {
      if (!removed.contains(encode(u, v))) edges.push_back({u, v});
    }
    return edges;
  };

  auto construct_odd_total = [&](I32 n_odd, I64 need) -> Vec<Edge> {
    const I32 k = minimal_odd_k_for_m(need);
    (void)n_odd;
    return construct_on_k(k, need);
  };

  auto edges_for_m12 = [&]() -> Vec<Edge> {
    Vec<Edge> edges;
    edges.reserve(12);
    FOR(i, 1, 7) {
      FOR(j, i + 1, 7) {
        if (i <= 3 && j <= 3) continue;
        edges.push_back({i, j});
      }
    }
    return edges;
  };

  auto complement_edges = [&](I32 n_all, const Vec<Edge>& small) -> Vec<Edge> {
    UnorderedSet<U64> s;
    s.reserve(small.size() * 2 + 1);
    for (auto [u, v] : small) s.insert(encode(u, v));

    Vec<Edge> edges;
    const I64 total = as<I64>(n_all) * (n_all - 1) / 2;
    edges.reserve(as<std::size_t>(total - as<I64>(small.size())));

    FOR(u, 1, n_all + 1) {
      FOR(v, u + 1, n_all + 1) {
        if (u == n_all || v == n_all) {
          edges.push_back({u, v});
        } else if (!s.contains(encode(u, v))) {
          edges.push_back({u, v});
        }
      }
    }
    return edges;
  };

  auto construct_dense_comp12 = [&](I32 n_all) -> Vec<Edge> {
    UnorderedSet<U64> missing;
    missing.reserve(32);
    FOR(i, 1, 7) {
      FOR(j, i + 1, 7) {
        if (i <= 3 && j <= 3) continue;
        missing.insert(encode(as<I32>(i), as<I32>(j)));
      }
    }

    Vec<Edge> edges;
    const I64 total = as<I64>(n_all) * (n_all - 1) / 2;
    edges.reserve(as<std::size_t>(total - 12));
    FOR(u, 1, n_all + 1) {
      FOR(v, u + 1, n_all + 1) {
        if (!missing.contains(encode(as<I32>(u), as<I32>(v)))) {
          edges.push_back({as<I32>(u), as<I32>(v)});
        }
      }
    }
    return edges;
  };

  auto bad_small = [](I64 x) -> bool { return x == 1 || x == 2 || x == 4 || x == 6; };
  auto bad_comp = [](I64 x) -> bool { return x == 0 || x == 1 || x == 2 || x == 4 || x == 6; };

  if (bad_small(m)) {
    OUT("No");
    return;
  }

  Vec<Edge> edges;
  if (m == 12) {
    edges = edges_for_m12();
  } else if ((n & 1) == 1) {
    edges = construct_odd_total(as<I32>(n), m);
  } else {
    const I64 threshold = (n - 1) * (n - 2) / 2;
    if (m <= threshold) {
      edges = construct_odd_total(as<I32>(n - 1), m);
    } else {
      const I64 m_comp = n * (n - 1) / 2 - m;
      if (m_comp == 12) {
        edges = construct_dense_comp12(as<I32>(n));
      } else
      if (bad_comp(m_comp)) {
        OUT("No");
        return;
      } else {
        const Vec<Edge> edges_comp = construct_odd_total(as<I32>(n - 1), m_comp);
        edges = complement_edges(as<I32>(n), edges_comp);
      }
    }
  }

  OUT("Yes");
  for (auto [u, v] : edges) OUT(u, v);
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
  I32 T = 0;
  if (!(std::cin >> T)) return 0;
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
