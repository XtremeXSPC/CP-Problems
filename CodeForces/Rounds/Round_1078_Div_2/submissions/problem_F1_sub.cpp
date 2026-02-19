//===----------------------------------------------------------------------===//
/**
 * @file: problem_F1_sub.cpp
 * @generated: 2026-02-19 02:25:25
 * @source: problem_F1.cpp
 * @author: Costantino Lombardi
 *
 * @brief: Codeforces Round 1078 (Div. 2) - Problem F1
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

#define Timer CP_MATH_TIMER

#include <numeric>

//===----------------------------------------------------------------------===//
/* Mathematical Utilities */

// Generic mathematical functions:
template <typename T>
[[deprecated("use std::gcd() instead")]]
[[gnu::always_inline]] constexpr T _gcd(T a, T b) {
  static_assert(std::is_integral_v<T>, "_gcd requires an integral type.");
  return b ? _gcd(b, a % b) : a;
}

template <typename T>
[[deprecated("use std::lcm() instead")]]
[[gnu::always_inline]] constexpr T _lcm(T a, T b) {
  return a / _gcd(a, b) * b;
}

// Advanced division operations:
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

// Legacy alias:
template <typename T>
[[deprecated("use power(base, exp) or mod_pow(base, exp, mod) instead")]]
[[gnu::always_inline]] constexpr T _power(T base, T exp, T mod = 0) {
  return mod ? mod_pow(base, exp, mod) : power(base, exp);
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
struct Timer {
  using Clock = std::chrono::high_resolution_clock;
  Clock::time_point start;

  Timer() : start(Clock::now()) {}

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
#undef Timer

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n, k);

  Vec<Vec<I32>> adj(n + 1);
  FOR(n - 1) {
    INT(v, u);
    adj[v].push_back(u);
    adj[u].push_back(v);
  }

  Vec<I32> a(n + 1);
  FOR(v, 1, n + 1) IN(a[v]);

  Vec<I32> b(k);
  IN(b);

  auto build_space = [&]() -> std::tuple<I32, Vec<I32>, std::unordered_map<I32, I32>> {
    Vec<I32> basis;
    for (I32 x : b) {
      for (I32 y : basis) {
        if ((x ^ y) < x) x ^= y;
      }
      if (x > 0) {
        basis.push_back(x);
        std::sort(basis.begin(), basis.end(), std::greater<>());
      }
    }

    const I32 sz = 1 << static_cast<I32>(basis.size());
    Vec<I32> val(sz, 0);
    std::unordered_map<I32, I32> id;
    id.reserve(static_cast<std::size_t>(sz) * 2);

    FOR(msk, sz) {
      I32 x = 0;
      FOR(bit, static_cast<I32>(basis.size())) {
        if (msk & (1 << bit)) x ^= basis[bit];
      }
      val[msk] = x;
      id[x] = msk;
    }
    return {sz, std::move(val), std::move(id)};
  };

  auto [sz, val, id] = build_space();
  std::unordered_set<I32> in_b;
  in_b.reserve(static_cast<std::size_t>(k) * 2);
  for (I32 x : b) in_b.insert(x);

  auto fwht = [&](Vec<I32>& f, bool inv) -> void {
    for (I32 len = 1; len < sz; len <<= 1) {
      for (I32 i = 0; i < sz; i += (len << 1)) {
        FOR(j, len) {
          const I32 x = f[i + j];
          const I32 y = f[i + j + len];
          I32 s = x + y;
          if (s >= MOD) s -= MOD;
          I32 d = x - y;
          if (d < 0) d += MOD;
          f[i + j] = s;
          f[i + j + len] = d;
        }
      }
    }
    if (inv) {
      const I64 inv_sz = mod_pow<I64>(sz, MOD - 2, MOD);
      FOR(i, sz) f[i] = static_cast<I32>((static_cast<I64>(f[i]) * inv_sz) % MOD);
    }
  };

  Vec<I32> par(n + 1, -1), ord;
  ord.reserve(n);
  Vec<I32> st = {1};
  par[1] = 0;
  while (!st.empty()) {
    const I32 u = st.back();
    st.pop_back();
    ord.push_back(u);
    for (I32 v : adj[u]) {
      if (v == par[u]) continue;
      par[v] = u;
      st.push_back(v);
    }
  }

  Vec<I32> post = ord;
  std::reverse(post.begin(), post.end());

  Vec<I32> sx(n + 1, 0);
  for (I32 u : post) {
    I32 x = a[u];
    for (I32 v : adj[u]) {
      if (v == par[u]) continue;
      x ^= sx[v];
    }
    sx[u] = x;
  }

  Vec<I32> dp(static_cast<std::size_t>(n + 1) * static_cast<std::size_t>(sz), 0);
  auto row = [&](I32 u) -> I32* {
    return dp.data() + static_cast<std::size_t>(u) * static_cast<std::size_t>(sz);
  };

  for (I32 u : post) {
    Vec<I32> cur(sz, 1);

    for (I32 v : adj[u]) {
      if (v == par[u]) continue;

      I32* d = row(v);
      Vec<I32> nxt(sz);
      std::memcpy(nxt.data(), d, static_cast<std::size_t>(sz) * sizeof(I32));

      I32 add = 0;
      FOR(msk, sz) {
        if (in_b.contains(sx[v] ^ val[msk])) {
          add += d[msk];
          if (add >= MOD) add -= MOD;
        }
      }

      auto it = id.find(sx[v]);
      if (it != id.end()) {
        nxt[it->second] += add;
        if (nxt[it->second] >= MOD) nxt[it->second] -= MOD;
      }

      fwht(nxt, false);
      FOR(msk, sz) {
        cur[msk] = static_cast<I32>((static_cast<I64>(cur[msk]) * nxt[msk]) % MOD);
      }
    }

    fwht(cur, true);
    std::memcpy(row(u), cur.data(), static_cast<std::size_t>(sz) * sizeof(I32));
  }

  I32 ans = 0;
  I32* root = row(1);
  FOR(msk, sz) {
    if (in_b.contains(sx[1] ^ val[msk])) {
      ans += root[msk];
      if (ans >= MOD) ans -= MOD;
    }
  }
  OUT(ans);
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
