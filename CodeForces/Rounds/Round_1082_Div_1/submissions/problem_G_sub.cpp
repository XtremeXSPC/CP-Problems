//===----------------------------------------------------------------------===//
/**
 * @file: problem_G_sub.cpp
 * @generated: 2026-02-24 00:46:25
 * @source: problem_G.cpp
 * @author: Costantino Lombardi
 *
 * @brief: Codeforces Round 1082 (Div. 1) - Problem G
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

using namespace std;

//===----------------------------------------------------------------------===//
/* Main Solver Function */

namespace {

constexpr array<I32, 8> DR = {2, 2, -2, -2, 3, 3, -3, -3};
constexpr array<I32, 8> DC = {3, -3, 3, -3, 2, -2, 2, -2};

[[nodiscard]] bool is_neighbor(I32 r1, I32 c1, I32 r2, I32 c2) {
  const I32 dr = r1 - r2;
  const I32 dc = c1 - c2;
  return dr * dr + dc * dc == 13;
}

struct Candidate {
  I32 v;
  bool fin;
  I64 key1;
  I64 key2;
};

} // namespace

void solve() {
  INT(N);

  if (N == 5) {
    OUT("01110");
    OUT("11011");
    OUT("10001");
    OUT("11011");
    OUT("01110");
    return;
  }

  const I32 target = as<I32>(floor(as<F64>(N) * as<F64>(N) / exp(1.0)));

  auto in_ring = [&](I32 M, I32 r, I32 c) -> bool {
    return (r < M || r >= N - M || c < M || c >= N - M);
  };

  auto used_neighbors = [&](const Vec<U8>& used, I32 r, I32 c) -> array<I32, 3> {
    array<I32, 3> res = {-1, -1, 0};
    I32 cnt = 0;
    FOR(k, 8) {
      const I32 nr = r + DR[k];
      const I32 nc = c + DC[k];
      if ((unsigned)nr >= (unsigned)N || (unsigned)nc >= (unsigned)N) continue;
      const I32 id = nr * N + nc;
      if (!used[id]) continue;
      if (cnt < 2) res[cnt] = id;
      ++cnt;
      if (cnt > 2) break;
    }
    res[2] = cnt;
    return res;
  };

  auto verify_cycle = [&](const Vec<U8>& used) -> bool {
    I32 start = -1;
    I32 total = 0;
    FOR(i, as<I64>(N) * N) {
      if (!used[i]) continue;
      ++total;
      if (start == -1) start = i;
    }
    if (total < target) return false;

    FOR(i, as<I64>(N) * N) {
      if (!used[i]) continue;
      const I32 r = i / N;
      const I32 c = i % N;
      I32 deg = 0;
      FOR(k, 8) {
        const I32 nr = r + DR[k];
        const I32 nc = c + DC[k];
        if ((unsigned)nr >= (unsigned)N || (unsigned)nc >= (unsigned)N) continue;
        if (used[nr * N + nc]) ++deg;
      }
      if (deg != 2) return false;
    }

    Vec<U8> vis(N * N, 0);
    Deque<I32> dq;
    dq.push_back(start);
    vis[start] = 1;
    I32 seen = 0;

    while (!dq.empty()) {
      const I32 v = dq.front();
      dq.pop_front();
      ++seen;
      const I32 r = v / N;
      const I32 c = v % N;
      FOR(k, 8) {
        const I32 nr = r + DR[k];
        const I32 nc = c + DC[k];
        if ((unsigned)nr >= (unsigned)N || (unsigned)nc >= (unsigned)N) continue;
        const I32 to = nr * N + nc;
        if (!used[to] || vis[to]) continue;
        vis[to] = 1;
        dq.push_back(to);
      }
    }

    return seen == total;
  };

  const Vec<I32> try_margin = {16, 18, 20};
  Vec<U8> final_used;

  for (const I32 M : try_margin) {
    const I32 L = M;
    const I32 R = N - M;
    const I32 core_side = R - L;
    if (core_side <= 0) continue;

    Vec<U8> sel(N * N, 0);
    FOR(r, L, R) {
      const I32 base = r * N;
      FOR(c, L, R) {
        const I32 v = (r + 2 * c) % 5;
        if (v == 0 || v == 1) sel[base + c] = 1;
      }
    }

    auto core_neighbors = [&](I32 r, I32 c, I32 out[2]) -> I32 {
      const I32 residue = (r + 2 * c) % 5;
      I32 cnt = 0;
      if (residue == 0) {
        I32 r1 = r + 2, c1 = c - 3;
        if (r1 >= L && r1 < R && c1 >= L && c1 < R) out[cnt++] = r1 * N + c1;
        I32 r2 = r - 3, c2 = c + 2;
        if (r2 >= L && r2 < R && c2 >= L && c2 < R) out[cnt++] = r2 * N + c2;
      } else {
        I32 r1 = r - 2, c1 = c + 3;
        if (r1 >= L && r1 < R && c1 >= L && c1 < R) out[cnt++] = r1 * N + c1;
        I32 r2 = r + 3, c2 = c - 2;
        if (r2 >= L && r2 < R && c2 >= L && c2 < R) out[cnt++] = r2 * N + c2;
      }
      return cnt;
    };

    Vec<U8> deg(N * N, 0);
    FOR(r, L, R) {
      FOR(c, L, R) {
        const I32 id = r * N + c;
        if (!sel[id]) continue;
        I32 nb[2];
        deg[id] = as<U8>(core_neighbors(r, c, nb));
      }
    }

    Vec<U8> vis(N * N, 0);
    Vec<Vec<I32>> paths;
    paths.reserve(2500);

    auto walk_path = [&](I32 start) -> Vec<I32> {
      Vec<I32> path;
      path.reserve(256);
      I32 cur = start, prev = -1;
      while (true) {
        vis[cur] = 1;
        path.push_back(cur);
        const I32 r = cur / N;
        const I32 c = cur % N;
        I32 nb[2];
        const I32 cnt = core_neighbors(r, c, nb);
        I32 next = -1;
        FOR(i, cnt) {
          if (nb[i] == prev || !sel[nb[i]] || vis[nb[i]]) continue;
          next = nb[i];
          break;
        }
        if (next == -1) break;
        prev = cur;
        cur = next;
      }
      return path;
    };

    FOR(r, L, R) {
      FOR(c, L, R) {
        const I32 id = r * N + c;
        if (!sel[id] || vis[id] || deg[id] > 1) continue;
        auto path = walk_path(id);
        if (as<I32>(path.size()) > 5) paths.push_back(std::move(path));
      }
    }

    FOR(r, L, R) {
      FOR(c, L, R) {
        const I32 id = r * N + c;
        if (!sel[id] || vis[id]) continue;
        auto cycle = walk_path(id);
        if (as<I32>(cycle.size()) <= 6) continue;
        cycle.pop_back();
        paths.push_back(std::move(cycle));
      }
    }

    for (auto& path : paths) {
      const I32 first_r = path.front() / N;
      const I32 last_r = path.back() / N;
      if (first_r > last_r) reverse(path.begin(), path.end());
    }

    sort(paths.begin(), paths.end(), [&](const auto& a, const auto& b) {
      const I32 ra = a.front() / N, ca = a.front() % N;
      const I32 rb = b.front() / N, cb = b.front() % N;
      return (ra + ca) < (rb + cb);
    });

    if (paths.size() % 2 == 1) paths.pop_back();
    const I32 K = as<I32>(paths.size());
    if (K < 2) continue;

    Vec<U8> used(N * N, 0);
    for (const auto& path : paths) {
      for (const I32 v : path) used[v] = 1;
    }

    auto connect_small = [&](I32 u, I32 v) -> bool {
      const I32 vr = v / N;
      const I32 vc = v % N;

      auto generate_candidates = [&](I32 cur) -> Vec<Candidate> {
        Vec<Candidate> candidates;
        candidates.reserve(8);
        const I32 cr = cur / N;
        const I32 cc = cur % N;

        FOR(k, 8) {
          const I32 nr = cr + DR[k];
          const I32 nc = cc + DC[k];
          if ((unsigned)nr >= (unsigned)N || (unsigned)nc >= (unsigned)N) continue;
          if (!in_ring(M, nr, nc)) continue;
          const I32 nid = nr * N + nc;
          if (used[nid]) continue;

          const auto nb = used_neighbors(used, nr, nc);
          const I32 cnt = nb[2];
          bool correct = false;
          bool fin = false;

          if (cnt == 1 && nb[0] == cur) {
            correct = true;
          } else if (cnt == 2) {
            const I32 a = nb[0], b = nb[1];
            if (((a == cur && b == v) || (a == v && b == cur)) && is_neighbor(nr, nc, vr, vc)) {
              correct = true;
              fin = true;
            }
          }
          if (!correct) continue;

          I32 onward = 0;
          FOR(t, 8) {
            const I32 rr = nr + DR[t];
            const I32 cc2 = nc + DC[t];
            if ((unsigned)rr >= (unsigned)N || (unsigned)cc2 >= (unsigned)N) continue;
            if (!in_ring(M, rr, cc2)) continue;
            const I32 id2 = rr * N + cc2;
            if (used[id2]) continue;
            const auto nb2 = used_neighbors(used, rr, cc2);
            if (nb2[2] == 1 && nb2[0] == nid) ++onward;
          }

          const I64 dist = as<I64>(nr - vr) * (nr - vr) + as<I64>(nc - vc) * (nc - vc);
          candidates.push_back({nid, fin, onward, dist});
        }

        sort(candidates.begin(), candidates.end(), [](const Candidate& a, const Candidate& b) {
          if (a.key1 != b.key1) return a.key1 < b.key1;
          if (a.key2 != b.key2) return a.key2 < b.key2;
          return a.v < b.v;
        });
        return candidates;
      };

      constexpr array<I32, 6> LIMITS = {24, 40, 64, 96, 140, 220};
      for (const I32 lim : LIMITS) {
        struct Frame {
          I32 cur;
          I32 prev;
          I32 it;
          Vec<Candidate> cand;
        };

        Vec<Frame> st;
        st.reserve(as<I64>(lim) + 2);
        st.push_back({u, -1, 0, {}});

        bool correct = false;
        while (!st.empty()) {
          auto& f = st.back();
          if (f.cand.empty()) {
            const I32 depth_used = as<I32>(st.size()) - 1;
            if (depth_used > lim) {
              if (f.cur != u) used[f.cur] = 0;
              st.pop_back();
              continue;
            }
            f.prev = (st.size() >= 2 ? st[st.size() - 2].cur : u);
            f.cand = generate_candidates(f.cur);
            f.it = 0;
          }

          if (f.it >= as<I32>(f.cand.size())) {
            if (f.cur != u) used[f.cur] = 0;
            st.pop_back();
            continue;
          }

          const Candidate cd = f.cand[f.it++];
          used[cd.v] = 1;
          if (cd.fin) {
            correct = true;
            break;
          }
          st.push_back({cd.v, f.cur, 0, {}});
        }

        if (correct) return true;
      }

      return false;
    };

    bool all_good = true;
    FOR(i, K) {
      I32 u, v;
      if (i % 2 == 0) {
        u = paths[i].back();
        v = paths[(i + 1) % K].back();
      } else {
        u = paths[i].front();
        v = paths[(i + 1) % K].front();
      }
      if (connect_small(u, v)) continue;
      all_good = false;
      break;
    }

    if (all_good && verify_cycle(used)) {
      final_used = std::move(used);
      break;
    }
  }

  if (final_used.empty()) {
    FOR(r, N) {
      string row(N, '0');
      OUT(row);
    }
    return;
  }

  FOR(r, N) {
    string row;
    row.reserve(N);
    const I32 base = r * N;
    FOR(c, N) {
      row.push_back(final_used[base + c] ? '1' : '0');
    }
    OUT(row);
  }
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  solve();
  return 0;
}

//===----------------------------------------------------------------------===//
