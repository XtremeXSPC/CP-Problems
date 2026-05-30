//===----------------------------------------------------------------------===//
/**
 * @file: problem_F_sub.cpp
 * @generated: 2026-05-31 00:59:03
 * @source: problem_F.cpp
 * @author: C.L.
 *
 * @brief: Codeforces Round 1101 (Div. 2) - Problem F
 */
//===----------------------------------------------------------------------===//
/* Included library and Compiler Optimizations */

#ifndef CP_TEMPLATE_PROFILE_STRICT
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define CP_IO_PROFILE_SIMPLE

// Generated from profiles.toml. Do not edit by hand.

#if defined(CP_TEMPLATE_PROFILE_STRICT)
  #ifndef CP_STRICT_TEMPLATE_NEEDS
    #define CP_STRICT_TEMPLATE_NEEDS 1
  #endif
  #ifndef CP_CORE_ENABLE_MATH
    #define CP_CORE_ENABLE_MATH 0
  #endif
  #ifndef CP_USE_GLOBAL_STD_NAMESPACE
    #define CP_USE_GLOBAL_STD_NAMESPACE 0
  #endif
  #ifndef CP_ENABLE_LEGACY_IO_VEC_MACROS
    #define CP_ENABLE_LEGACY_IO_VEC_MACROS 0
  #endif
#endif

#ifndef CP_IO_ENABLE_COMPOSITE
  #if CP_ENABLE_LEGACY_IO_VEC_MACROS
    #define CP_IO_ENABLE_COMPOSITE 1
  #else
    #define CP_IO_ENABLE_COMPOSITE 0
  #endif
#endif

//===----------------------------------------------------------------------===//
/* Compiler Pragmas */

  #define CP_ENABLE_GCC_OPTIMIZE_PRAGMAS 1

#if defined(__GNUC__) && !defined(__clang__)
  #if CP_ENABLE_GCC_OPTIMIZE_PRAGMAS
    #if CP_ENABLE_AGGRESSIVE_OPTIMIZATIONS
      #pragma GCC optimize("Ofast,unroll-loops,fast-math,O3,inline-functions")
    #else
      #pragma GCC optimize("O3,unroll-loops,inline-functions")
    #endif
  #endif
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-result"
  #if CP_ENABLE_ARCH_TARGET_PRAGMAS
    #if defined(__x86_64__) && !defined(__MINGW32__) && !defined(__MINGW64__)
      #pragma GCC target("avx2,bmi,bmi2,popcnt,lzcnt,sse4.2,fma")
    #endif
    #if defined(__aarch64__) && !defined(__MINGW32__) && !defined(__MINGW64__)
      #pragma GCC target("+simd,+crypto,+fp16")
    #endif
  #endif
#endif

//===----------------------------------------------------------------------===//
/* Standard Library Includes */

  #include <bits/stdc++.h>

//===----------------------------------------------------------------------===//
/* Toolchain Feature Detection */

#ifndef HAS_INT128
  #ifdef __SIZEOF_INT128__
    #define HAS_INT128 1
  #else
    #define HAS_INT128 0
  #endif
#endif

#ifndef HAS_FLOAT128
  #ifdef __FLOAT128__
    #define HAS_FLOAT128 1
  #else
    #define HAS_FLOAT128 0
  #endif
#endif

#if defined(__has_include) && __has_include(<ext/pb_ds/assoc_container.hpp>) \
 && __has_include(<ext/pb_ds/tree_policy.hpp>)
  #include <ext/pb_ds/assoc_container.hpp>
  #include <ext/pb_ds/tree_policy.hpp>
  #ifndef PBDS_AVAILABLE
    #define PBDS_AVAILABLE 1
  #endif
#elif !defined(PBDS_AVAILABLE)
  #define PBDS_AVAILABLE 0
#endif

//===----------------------------------------------------------------------===//
/* Debug and Assertion Policy */

#ifdef LOCAL
  #include "debug.h"
#else
  #define debug(...)
  #define debug_if(...)
  #define debug_tree(...)
  #define debug_tree_verbose(...)
  #define debug_line()
  #define my_assert(cond) ((void)0)
  #define COUNT_CALLS(...)
#endif

#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC diagnostic pop
#endif

//===----------------------------------------------------------------------===//
/* Scalar Type Aliases */

using I8  = std::int8_t;
using I16 = std::int16_t;
using I32 = std::int32_t;
using I64 = std::int64_t;
using U8  = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;

#if HAS_INT128
  __extension__ using I128 = __int128;
  __extension__ using U128 = unsigned __int128;
#else
  using I128 = std::int64_t;
  using U128 = std::uint64_t;
#endif

using F32 = float;
using F64 = double;
using F80 = long double;

#if HAS_FLOAT128
  using F128 = __float128;
#else
  using F128 = long double;
#endif

template <typename T>
using Limits = std::numeric_limits<T>;

using Size = std::size_t;
using Diff = std::ptrdiff_t;
using Byte = std::byte;

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
#define isz(x) (I32)(x).size()
#define len(x) sz(x)
#define eb emplace_back
#define elif else if

// Advanced container operations:
#define UNIQUE(x) (std::ranges::sort(x), x.erase(std::ranges::unique(x).begin(), x.end()), x.shrink_to_fit())
#define LB(c, x) (I64) std::distance((c).begin(), std::ranges::lower_bound(c, x))
#define UB(c, x) (I64) std::distance((c).begin(), std::ranges::upper_bound(c, x))
#define SUM(x) std::accumulate(all(x), std::iter_value_t<decltype((x).begin())>{})
#define MIN(x)                                          \
  ([&]() -> decltype(auto) {                            \
    auto&& _cp_min_range = (x);                         \
    if (std::ranges::empty(_cp_min_range)) {            \
      my_assert(false && "MIN(): empty range.");        \
      std::abort();                                     \
    }                                                   \
    return *std::ranges::min_element(_cp_min_range);    \
  }())
#define MAX(x)                                          \
  ([&]() -> decltype(auto) {                            \
    auto&& _cp_max_range = (x);                         \
    if (std::ranges::empty(_cp_max_range)) {            \
      my_assert(false && "MAX(): empty range.");        \
      std::abort();                                     \
    }                                                   \
    return *std::ranges::max_element(_cp_max_range);    \
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

//===----------------------------------------------------------------------===//
/* Container and Utility Aliases */

using String     = std::string;
using StringView = std::string_view;

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

template <class T>
using Stack = std::stack<T, std::deque<T>>;
template <class T>
using Queue = std::queue<T, std::deque<T>>;
template <class T>
using PriorityQueue = std::priority_queue<T, std::vector<T>>;
template <class T>
using MinPriorityQueue = std::priority_queue<T, std::vector<T>, std::greater<T>>;

template <class T, class Container>
using StackIn = std::stack<T, Container>;
template <class T, class Container>
using QueueIn = std::queue<T, Container>;
template <class T, class Container, class Compare>
using PriorityQueueBy = std::priority_queue<T, Container, Compare>;
template <class T, class Container = std::vector<T>>
using MinPriorityQueueIn = std::priority_queue<T, Container, std::greater<T>>;

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

template <class T>
using Vec2D = Vec<Vec<T>>;
template <class T>
using Vec3D = Vec<Vec2D<T>>;
template <class T>
using Vec4D = Vec<Vec3D<T>>;

using VecU8   = Vec<U8>;
using VecU16  = Vec<U16>;
using VecU32  = Vec<U32>;
using VecU64  = Vec<U64>;
using VecI8   = Vec<I8>;
using VecI16  = Vec<I16>;
using VecI32  = Vec<I32>;
using VecI64  = Vec<I64>;
using VecF64  = Vec<F64>;
using VecF80  = Vec<F80>;
using VecBool = Vec<bool>;
using VecStr  = Vec<String>;

template <class T, class U>
using VecPair = Vec<Pair<T, U>>;

using PairI32    = Pair<I32, I32>;
using PairI64    = Pair<I64, I64>;
using PairF80    = Pair<F80, F80>;
using VecPairI32 = Vec<PairI32>;
using VecPairI64 = Vec<PairI64>;

//===----------------------------------------------------------------------===//
/* Library Function Aliases */

namespace cp {

template <bool B, class T, class F>
using Conditional = std::conditional_t<B, T, F>;

template <class T>
using cvref_t = std::remove_cvref_t<T>;

template <class T, class U>
concept Same = std::same_as<cvref_t<T>, cvref_t<U>>;

template <class T>
concept Int = std::integral<cvref_t<T>>
#if HAS_INT128
    || std::same_as<cvref_t<T>, I128>
    || std::same_as<cvref_t<T>, U128>
#endif
    ;

template <class T>
concept Float = std::floating_point<cvref_t<T>>;

template <class T>
concept Signed = Int<T> && (std::is_signed_v<cvref_t<T>>
#if HAS_INT128
  || std::same_as<cvref_t<T>, I128>
#endif
);

template <class T>
concept Unsigned = Int<T> && !Signed<T>;

} // namespace cp

//===----------------------------------------------------------------------===//
/* Extended Type Traits */

namespace cp {

template <class T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

namespace detail {

template <class T>
inline constexpr bool is_extended_integral_v = false;

template <class T>
struct make_unsigned { using type = std::make_unsigned_t<T>; };

template <>
struct make_unsigned<bool> { using type = U8; };

#if HAS_INT128
  template <>
  inline constexpr bool is_extended_integral_v<I128> = true;
  template <>
  inline constexpr bool is_extended_integral_v<U128> = true;

  template <>
  struct make_unsigned<I128> { using type = U128; };
  template <>
  struct make_unsigned<U128> { using type = U128; };
#endif

} // namespace detail

template <class T>
using make_unsigned_t = typename detail::make_unsigned<remove_cvref_t<T>>::type;

} // namespace cp

//===----------------------------------------------------------------------===//
/* Integer Mathematical Utilities */

namespace cp::detail {

template <cp::Unsigned T>
[[gnu::always_inline]] constexpr T mul_mod_unsigned(T a, T b, T mod) {
#if HAS_INT128
  if constexpr (sizeof(T) <= sizeof(U64)) {
    return as<T>((as<U128>(a) * as<U128>(b)) % as<U128>(mod));
  }
#endif

  T result = 0;
  while (b > 0) {
    if ((b & 1U) != 0U) {
      if (result >= mod - a)
        result -= (mod - a);
      else
        result += a;
    }
    b >>= 1U;
    if (b == 0)
      break;
    if (a >= mod - a)
      a -= (mod - a);
    else
      a += a;
  }
  return result;
}

} // namespace cp::detail

template <cp::Signed T>
[[gnu::always_inline]] constexpr T safe_mod(T x, T mod) {
  my_assert(mod > 0);
  x %= mod;
  if (x < 0)
    x += mod;
  return x;
}

template <cp::Signed T>
[[gnu::always_inline]] constexpr std::pair<T, T> inv_gcd(T a, T b) {
  my_assert(b > 0);
  a = safe_mod(a, b);
  if (a == 0)
    return {b, 0};

  T s  = b, t = a;
  T m0 = 0, m1 = 1;

  while (t) {
    T u = s / t;
    s  -= t * u;
    m0 -= m1 * u;
    std::swap(s, t);
    std::swap(m0, m1);
  }

  if (m0 < 0)
    m0 += b / s;
  return {s, m0};
}

template <cp::Signed T>
[[gnu::always_inline]] constexpr T mod_inv(T a, T mod) {
  auto [g, x] = inv_gcd(a, mod);
  my_assert(g == 1 && "mod_inv(): inverse does not exist when gcd(value, mod) != 1.");
  return g == 1 ? x : 0;
}

template <cp::Signed T>
[[gnu::always_inline]] constexpr bool merge_congruences(T& r1, T& m1, T r2, T m2) {
  my_assert(m1 > 0 && m2 > 0);
  r1 = safe_mod(r1, m1);
  r2 = safe_mod(r2, m2);

  T target = safe_mod(r2 - r1, m2);
  auto [g, x] = inv_gcd(m1, m2);
  if (target % g != 0)
    return false;

  const T m2_g = m2 / g;
  const T lhs  = safe_mod(target / g, m2_g);
  const T rhs  = safe_mod(x, m2_g);
  using U = cp::make_unsigned_t<T>;
  const T step = as<T>(cp::detail::mul_mod_unsigned(as<U>(lhs), as<U>(rhs), as<U>(m2_g)));

  r1 += step * m1;
  m1 *= m2_g;
  r1 = safe_mod(r1, m1);
  return true;
}

template <cp::Int T>
[[gnu::always_inline]] constexpr T div_floor(T a, T b) {
  my_assert(b != 0);
  if constexpr (cp::Signed<T>) {
    T q = a / b;
    T r = a % b;
    if (r != 0 && ((r > 0) != (b > 0)))
      --q;
    return q;
  } else {
    return a / b;
  }
}

template <cp::Int T>
[[gnu::always_inline]] constexpr T div_ceil(T a, T b) {
  my_assert(b != 0);
  if constexpr (cp::Signed<T>) {
    T q = a / b;
    T r = a % b;
    if (r != 0 && ((r > 0) == (b > 0)))
      ++q;
    return q;
  } else {
    return a / b + (a % b != 0);
  }
}

template <cp::Int T>
[[gnu::always_inline]] constexpr T mod_floor(T a, T b) {
  return a - b * div_floor(a, b);
}

template <cp::Int T>
[[gnu::always_inline]] constexpr std::pair<T, T> divmod(T a, T b) {
  T q = div_floor(a, b);
  return {q, a - q * b};
}

template <cp::Int T>
[[gnu::always_inline]] constexpr T power(T base, T exp) {
  T result = 1;
  while (exp > 0) {
    if (exp & 1)
      result *= base;
    base *= base;
    exp >>= 1;
  }
  return result;
}

template <cp::Int T>
[[gnu::always_inline]] constexpr T mod_pow(T base, T exp, T mod) {
  my_assert(mod != 0);
  if constexpr (cp::Signed<T>) {
    my_assert(mod > 0);
    my_assert(exp >= 0);
    if (mod <= 0)
      return 0;
    if (exp < 0)
      return 0;
  }

  using U  = cp::make_unsigned_t<T>;
  U umod   = as<U>(mod);
  U uexp   = as<U>(exp);
  U ubase  = as<U>(mod_floor(base, mod));
  U result = as<U>(1) % umod;

  while (uexp > 0) {
    if ((uexp & 1U) != 0U) {
      result = cp::detail::mul_mod_unsigned(result, ubase, umod);
    }
    ubase = cp::detail::mul_mod_unsigned(ubase, ubase, umod);
    uexp >>= 1U;
  }
  return as<T>(result);
}

template <cp::Int T>
[[gnu::always_inline]] inline T floor_sqrt(T x) {
  if constexpr (cp::Signed<T>) {
    my_assert(x >= 0);
    if (x < 0)
      return 0;
  }

  using U = cp::make_unsigned_t<T>;
  const U ux = as<U>(x);
  if (ux <= 1)
    return as<T>(ux);

  if constexpr (sizeof(U) > sizeof(U64)) {
    constexpr int bits = int(sizeof(U) * 8);
    U lo = 1;
    U hi = U{1} << ((bits + 1) / 2);
    while (lo + 1 < hi) {
      const U mid = lo + (hi - lo) / 2;
      if (mid <= ux / mid)
        lo = mid;
      else
        hi = mid;
    }
    return as<T>(lo);
  }

  U r = as<U>(std::sqrt(as<F80>(ux)));
  while ((r + 1) <= ux / (r + 1))
    ++r;
  while (r > ux / r)
    --r;
  return as<T>(r);
}

template <cp::Int T>
[[gnu::always_inline]] inline T ceil_sqrt(T x) {
  using U      = cp::make_unsigned_t<T>;
  const T root = floor_sqrt(x);
  const U uf   = as<U>(root);
  if (uf == 0)
    return 0;

  const U ux = as<U>(x);
  if (ux / uf == uf && ux % uf == 0)
    return root;
  return as<T>(uf + 1);
}

//===----------------------------------------------------------------------===//
/* Lightweight Stopwatch Utility */

struct Stopwatch {
  using Clock = std::chrono::high_resolution_clock;
  Clock::time_point start;

  Stopwatch() : start(Clock::now()) {}
  void reset() { start = Clock::now(); }
  [[gnu::always_inline]] F64 elapsed() const { return std::chrono::duration<F64>(Clock::now() - start).count(); }
  [[gnu::always_inline]] bool within(F64 limit) const { return elapsed() < limit; }
};

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
void read(T& x) { std::cin >> x; }

template <class T>
void write(const T& x) { std::cout << x; }

#if CP_IO_ENABLE_COMPOSITE
  #define CP_IO_COMPOSITE_CONTEXT 1
template <class T, class U>
inline void read(Pair<T, U>& p) {
  read(p.first);
  read(p.second);
}

template <class T>
inline void read(Vec<T>& v) {
  for (auto& x : v)
    read(x);
}

template <typename... Args>
inline void read(std::tuple<Args...>& t) {
  std::apply([](auto&... args) { (read(args), ...); }, t);
}

template <class T, class U>
inline void write(const Pair<T, U>& p) {
  write(p.first);
  write(' ');
  write(p.second);
}

template <class T>
inline void write(const Vec<T>& v) {
  for (I64 i = 0; i < sz(v); ++i) {
    if (i)
      write(' ');
    write(v[i]);
  }
}

template <typename... Args>
inline void write(const std::tuple<Args...>& t) {
  I32 i = 0;
  std::apply([&i](const auto&... args) { ((i++ > 0 ? (write(' '), 0) : 0, write(args)), ...); }, t);
}
  #undef CP_IO_COMPOSITE_CONTEXT
#endif

template <class Head, class... Tail>
  requires(sizeof...(Tail) > 0)
inline void read(Head& head, Tail&... tail) {
  read(head);
  read(tail...);
}

template <class Head, class... Tail>
  requires(sizeof...(Tail) > 0)
inline void write(const Head& head, const Tail&... tail) {
  write(head);
  write(' ');
  write(tail...);
}

inline void writeln() { write('\n'); }

template <class... Args>
inline void writeln(const Args&... args) {
  if constexpr (sizeof...(args) > 0)
    write(args...);
  writeln();
}

} // namespace cp_io

namespace fast_io {
#define CP_IO_COMPAT_FAST_IO_NAMESPACE_DEFINED 1

template <class T>
inline void read_integer(T& x) { cp_io::read(x); }
inline void read_char(char& x) { cp_io::read(x); }
inline void read_string(std::string& x) { cp_io::read(x); }

template <class T>
inline void write_integer(T x) { cp_io::write(x); }
inline void write_char(char c) { std::cout.put(c); }
inline void write_string(const std::string& s) { cp_io::write(s); }
inline void flush_output() { std::cout.flush(); }

using cp_io::read;
using cp_io::write;
using cp_io::writeln;
} // namespace fast_io
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
    #define VV(type, name, h, w) Vec2D<type> name(h, Vec<type>(w)); IN(name)
  #endif
#endif

  #define CP_IO_ANSWER_HELPERS_DEFINED 1
  inline void YES(bool condition = true) {
    if (condition) OUT("YES");
    else OUT("NO");
  }
  inline void NO(bool condition = true) { YES(!condition); }
  inline void Yes(bool condition = true) {
    if (condition) OUT("Yes");
    else OUT("No");
  }
  inline void No(bool condition = true) { Yes(!condition); }


//===----------------------------------------------------------------------===//
/* Main Solver Function */

using namespace std;

const I128 MAX_I128 = ~((I128)1 << 127);

void solve() {
  I64 n, a, b, k;
  IN(n, a, b, k);

  if (a == b) {
    OUT(k % a == 0 ? n : 0);
    return;
  }

  I64 max_ans = (k % (a + b) == 0) ? n - 1 : 0;

  // Golden path evaluation.
  I64 l_gold = -1;
  if (k % a == 0) {
    I64 cur = k / a;
    while (cur >= 0) {
      l_gold++;
      I64 nxt = k - b * cur;
      if (nxt < 0 || nxt % a != 0)
        break;
      cur = nxt / a;
    }
  }

  if (n <= l_gold + 1) {
    OUT(n);
    return;
  }

  // Reduce by gab = gcd(a, b):
  I64 gab = std::gcd(a, b);
  if (k % gab == 0) {
    a /= gab;
    b /= gab;
    k /= gab;
  }

  // Precompute bounds to avoid overflow.
  Vec<I128> A_arr(1, 1);
  Vec<I128> B_arr(1, 0);
  Vec<I128> PA_arr(1, 1);

  I64 l_safe = 0;
  while (l_safe < 60) {
    I128 A = A_arr.back(), B = B_arr.back(), pa = PA_arr.back();

    I128 abs_A = A < 0 ? -A : A;
    if (abs_A > 0 && MAX_I128 / b < abs_A)
      break;
    if (pa > 0 && MAX_I128 / a < pa)
      break;

    I128 abs_B = B < 0 ? -B : B;
    if (abs_B > 0 && MAX_I128 / b < abs_B)
      break;
    if (MAX_I128 / k < pa)
      break;

    I128 t1 = -(I128)b * B;
    I128 t2 = (I128)k * pa;
    if (t1 > 0 && t2 > 0 && MAX_I128 - t1 < t2)
      break;
    if (t1 < 0 && t2 < 0 && -t1 > MAX_I128 - (-t2))
      break;

    A_arr.push_back(-(I128)b * A);
    B_arr.push_back(t1 + t2);
    PA_arr.push_back(pa * a);
    l_safe++;
  }

  // Max path length search.
  I64 l_max = 0;
  for (I64 L = l_safe; L >= 1; --L) {
    I128 LB = 0, UB = k / b; // T0 <= floor(k/b) so that T1 = (k - b*T0)/a >= 0
    bool VL = true;

    FOR(i, 1, L + 1) {
      I128 A = A_arr[i], B = B_arr[i];
      if (A > 0)
        LB = max(LB, div_ceil(-B, A));
      else if (A < 0)
        UB = min(UB, div_floor(B, -A));
    }

    if (LB > UB)
      continue;

    I128 R = 0, M = 1;
    FOR(i, 1, L + 1) {
      I128 MOD_i = PA_arr[i];
      I128 X = A_arr[i] % MOD_i;
      if (X < 0)
        X += MOD_i;
      I128 Y = (-B_arr[i]) % MOD_i;
      if (Y < 0)
        Y += MOD_i;

      auto [g, inv] = inv_gcd(X, MOD_i);
      if (Y % g != 0) {
        VL = false;
        break;
      }

      I128 m_i = MOD_i / g;
      I128 r_i = (Y / g) % m_i;
      if (r_i < 0)
        r_i += m_i;
      inv %= m_i;
      if (inv < 0)
        inv += m_i;
      r_i = (r_i * inv) % m_i;
      if (r_i < 0)
        r_i += m_i;

      if (!merge_congruences(R, M, r_i, m_i)) {
        VL = false;
        break;
      }
    }

    if (VL) {
      I128 T0 = R;
      if (T0 < LB) {
        I128 q = (LB - R + M - 1) / M;
        T0 = R + q * M;
      }
      if (T0 <= UB) {
        l_max = L;
        break;
      }
    }
  }

  // Score combination.
  I64 v_gold = max((I64)0, l_gold + 1);
  I64 n_rem  = n - v_gold;

  I64 p_blk = 0;
  if (l_max > 0) {
    I64 blk = n_rem / (l_max + 1);
    I64 rem = n_rem % (l_max + 1);
    p_blk = blk * l_max + max((I64)0, rem - 1);
  }

  I64 p_tot = v_gold + p_blk;

  OUT(max(max_ans, p_tot));
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Stopwatch timer;
#endif

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
