//===----------------------------------------------------------------------===//
/**
 * @file: problem_F2_sub.cpp
 * @generated: 2026-06-02 11:57:32
 * @source: problem_F2.cpp
 * @author: C.L.
 *
 * @brief: Codeforces Round 1101 (Div. 2) - Problem F2
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

//===----------------------------------------------------------------------===//
/* Compiler Pragmas */

#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC optimize("O3,unroll-loops,inline-functions")
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-result"
  #if defined(__x86_64__) && !defined(__MINGW32__) && !defined(__MINGW64__)
    #pragma GCC target("avx2,bmi,bmi2,popcnt,lzcnt,sse4.2,fma")
  #endif
  #if defined(__aarch64__) && !defined(__MINGW32__) && !defined(__MINGW64__)
    #pragma GCC target("+simd,+crypto,+fp16")
  #endif
#endif

#ifdef __clang__
  #pragma clang optimize on
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wunused-result"
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

#ifdef __clang__
  #pragma clang diagnostic pop
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
#define MIN(x)                                       \
  ([&]() -> decltype(auto) {                         \
    auto &&_cp_min_range = (x);                      \
    if (std::ranges::empty(_cp_min_range)) {         \
      my_assert(false && "MIN(): empty range.");     \
      std::abort();                                  \
    }                                                \
    return *std::ranges::min_element(_cp_min_range); \
  }())
#define MAX(x)                                       \
  ([&]() -> decltype(auto) {                         \
    auto &&_cp_max_range = (x);                      \
    if (std::ranges::empty(_cp_max_range)) {         \
      my_assert(false && "MAX(): empty range.");     \
      std::abort();                                  \
    }                                                \
    return *std::ranges::max_element(_cp_max_range); \
  }())

// Y-combinator for recursive lambdas:
template <class F>
struct YCombinator {
  F fn;

  template <class... Args>
  decltype(auto) operator()(Args &&...args) const {
    return fn(*this, std::forward<Args>(args)...);
  }
};

template <class F>
YCombinator(F) -> YCombinator<F>;

template <class F>
[[gnu::always_inline]] constexpr auto fix(F &&fn) {
  return YCombinator<std::decay_t<F>>{std::forward<F>(fn)};
}

// Type-safe cast alias:
template <typename To>
[[gnu::always_inline]] constexpr To as(auto &&x) noexcept {
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

// clang-format off
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
// clang-format on

template <class T>
concept Unsigned = Int<T> && !Signed<T>;

template <class T>
concept Enum = std::is_enum_v<cvref_t<T>>;

template <class F, class... Args>
concept Predicate = std::predicate<F, Args...>;

template <class T>
concept Hashable = requires(const cvref_t<T> &value) {
  { std::hash<cvref_t<T>>{}(value) } -> std::convertible_to<std::size_t>;
};

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
struct make_unsigned {
  using type = std::make_unsigned_t<T>;
};

template <>
struct make_unsigned<bool> {
  using type = U8;
};

#if HAS_INT128
template <>
inline constexpr bool is_extended_integral_v<I128> = true;
template <>
inline constexpr bool is_extended_integral_v<U128> = true;

template <>
struct make_unsigned<I128> {
  using type = U128;
};
template <>
struct make_unsigned<U128> {
  using type = U128;
};
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

  T s = b, t = a;
  T m0 = 0, m1 = 1;

  while (t) {
    T u = s / t;
    s -= t * u;
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
[[gnu::always_inline]] constexpr bool merge_congruences(T &r1, T &m1, T r2, T m2) {
  my_assert(m1 > 0 && m2 > 0);
  r1 = safe_mod(r1, m1);
  r2 = safe_mod(r2, m2);

  T target    = safe_mod(r2 - r1, m2);
  auto [g, x] = inv_gcd(m1, m2);
  if (target % g != 0)
    return false;

  const T m2_g = m2 / g;
  const T lhs  = safe_mod(target / g, m2_g);
  const T rhs  = safe_mod(x, m2_g);
  using U      = cp::make_unsigned_t<T>;
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

  using U    = cp::make_unsigned_t<T>;
  const U ux = as<U>(x);
  if (ux <= 1)
    return as<T>(ux);

  if constexpr (sizeof(U) > sizeof(U64)) {
    constexpr int bits = int(sizeof(U) * 8);
    U lo = 1, hi = U{1} << ((bits + 1) / 2);
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
void read(T &x) { std::cin >> x; }

template <class T>
void write(const T &x) { std::cout << x; }

#define CP_IO_COMPOSITE_CONTEXT 1
template <class T, class U>
inline void read(Pair<T, U> &p) {
  read(p.first);
  read(p.second);
}

template <class T>
inline void read(Vec<T> &v) {
  for (auto &x : v)
    read(x);
}

template <typename... Args>
inline void read(std::tuple<Args...> &t) {
  std::apply([](auto &...args) { (read(args), ...); }, t);
}

template <class T, class U>
inline void write(const Pair<T, U> &p) {
  write(p.first);
  write(' ');
  write(p.second);
}

template <class T>
inline void write(const Vec<T> &v) {
  for (I64 i = 0; i < sz(v); ++i) {
    if (i)
      write(' ');
    write(v[i]);
  }
}

template <typename... Args>
inline void write(const std::tuple<Args...> &t) {
  I32 i = 0;
  std::apply([&i](const auto &...args) { ((i++ > 0 ? (write(' '), 0) : 0, write(args)), ...); }, t);
}
#undef CP_IO_COMPOSITE_CONTEXT

template <class Head, class... Tail>
  requires(sizeof...(Tail) > 0)
inline void read(Head &head, Tail &...tail) {
  read(head);
  read(tail...);
}

template <class Head, class... Tail>
  requires(sizeof...(Tail) > 0)
inline void write(const Head &head, const Tail &...tail) {
  write(head);
  write(' ');
  write(tail...);
}

inline void writeln() { write('\n'); }

template <class... Args>
inline void writeln(const Args &...args) {
  if constexpr (sizeof...(args) > 0)
    write(args...);
  writeln();
}

} // namespace cp_io

namespace fast_io {
#define CP_IO_COMPAT_FAST_IO_NAMESPACE_DEFINED 1

template <class T>
inline void read_integer(T &x) { cp_io::read(x); }
inline void read_char(char &x) { cp_io::read(x); }
inline void read_string(std::string &x) { cp_io::read(x); }

template <class T>
inline void write_integer(T x) { cp_io::write(x); }
inline void write_char(char c) { std::cout.put(c); }
inline void write_string(const std::string &s) { cp_io::write(s); }
inline void flush_output() { std::cout.flush(); }

using cp_io::read;
using cp_io::write;
using cp_io::writeln;
} // namespace fast_io

#define CP_IO_IMPL_READ(...) cp_io::read(__VA_ARGS__)
#define CP_IO_IMPL_WRITELN(...) cp_io::writeln(__VA_ARGS__)
#define CP_IO_IMPL_FLUSH() std::cout.flush()

//===----------------------------------------------------------------------===//
/* Shared I/O Macro and Answer Helper Definitions */

#define IN(...) CP_IO_IMPL_READ(__VA_ARGS__)
#define OUT(...) CP_IO_IMPL_WRITELN(__VA_ARGS__)
#define FLUSH() CP_IO_IMPL_FLUSH()

#define CP_IO_DECL_MACROS_DEFINED 1
#define INT(...)   \
  I32 __VA_ARGS__; \
  IN(__VA_ARGS__)
#define LL(...)    \
  I64 __VA_ARGS__; \
  IN(__VA_ARGS__)
#define ULL(...)   \
  U64 __VA_ARGS__; \
  IN(__VA_ARGS__)
#define STR(...)           \
  std::string __VA_ARGS__; \
  IN(__VA_ARGS__)
#define CHR(...)    \
  char __VA_ARGS__; \
  IN(__VA_ARGS__)
#define DBL(...)   \
  F64 __VA_ARGS__; \
  IN(__VA_ARGS__)

#define CP_IO_ANSWER_HELPERS_DEFINED 1
inline void YES(bool condition = true) {
  if (condition)
    OUT("YES");
  else
    OUT("NO");
}
inline void NO(bool condition = true) { YES(!condition); }
inline void Yes(bool condition = true) {
  if (condition)
    OUT("Yes");
  else
    OUT("No");
}
inline void No(bool condition = true) { Yes(!condition); }

//===----------------------------------------------------------------------===//
/* Helpers for the problem */

/*
 * A perfectly cooked chain uses pan times T_0, T_1, ... tied by
 *   b*T_{i-1} + a*T_i = k    =>    T_i = (k - b*T_{i-1}) / a.
 * Unrolling the recurrence gives a closed form for every step:
 *   a^i * T_i = (-b)^i * T_0 + off_i,   off_i = sum_{j<i} (-b)^{i-1-j} k a^j.
 * Hence each step constrains the starting time T_0 in two ways:
 *   - non-negativity:  (-b)^i * T_0 + off_i >= 0      (a window [lo, hi]),
 *   - integrality:     (-b)^i * T_0 + off_i ≡ 0 (mod a^i)  (a congruence).
 */

// Steps of the "golden" chain that starts already cooked (T_0 = k/a), so no
// pancake is sacrificed. Returns -1 when it does not exist (k % a != 0);
// otherwise the count of extra steps (total cooked pancakes = result + 1).
I64 golden_chain_steps(I64 a, I64 b, I64 k) {
  if (k % a != 0)
    return -1;
  I64 steps = 0;
  for (I64 t = k / a;;) {
    I64 next = k - b * t;
    if (next < 0 || next % a != 0)
      break;
    t = next / a;
    ++steps;
  }
  return steps;
}

// Longest feasible chain length L in [1, 60] for the forest case (a != b),
// or 0 when none exists.
I64 longest_chain(I64 a, I64 b, I64 k) {
  // Reduce by gcd(a, b): the T_i sequence is unchanged, but a smaller a keeps a^i
  // (and k*a^i) inside __int128 long enough to reach the true maximum length.
  // If gcd(a, b) does not divide k no pair is perfect, so the length is 0 anyway.
  const I64 g_ab = std::gcd(a, b);
  if (k % g_ab == 0) {
    a /= g_ab;
    b /= g_ab;
    k /= g_ab;
  }

  // Roll out (-b)^i, a^i and off_i while they stay inside __int128 range.
  Vec<I128> negb_pow{1}; // (-b)^i, also the coefficient of T_0 at step i
  Vec<I128> a_pow{1};    // a^i, the modulus at step i
  Vec<I128> off{0};      // off_i

  I64 reach = 0;
  while (reach < 60) {
    I128 next_negb, next_a, scaled, k_term, next_off;
    if (__builtin_mul_overflow(negb_pow[reach], -(I128)b, &next_negb))
      break;
    if (__builtin_mul_overflow(a_pow[reach], (I128)a, &next_a))
      break;
    if (__builtin_mul_overflow(off[reach], -(I128)b, &scaled))
      break;
    if (__builtin_mul_overflow((I128)k, a_pow[reach], &k_term))
      break;
    if (__builtin_add_overflow(scaled, k_term, &next_off))
      break;
    negb_pow.push_back(next_negb);
    a_pow.push_back(next_a);
    off.push_back(next_off);
    ++reach;
  }

  for (I64 L = reach; L >= 1; --L) {
    // 1) Non-negativity of every T_i pins T_0 to an integer window [lo, hi].
    //    T_0 <= floor(k/b) keeps T_1 = (k - b*T_0)/a non-negative.
    I128 lo = 0, hi = k / b;
    FOR(i, 1, L + 1) {
      const I128 coeff = negb_pow[i];
      if (coeff > 0)
        lo = std::max(lo, div_ceil(-off[i], coeff));
      else if (coeff < 0)
        hi = std::min(hi, div_floor(off[i], -coeff));
    }
    if (lo > hi)
      continue;

    // 2) Integrality intersects the congruences  coeff*T_0 ≡ -off_i (mod a^i).
    I128 rem = 0, mod = 1;
    bool feasible = true;
    FOR(i, 1, L + 1) {
      const I128 step_mod = a_pow[i];
      const I128 coeff    = safe_mod(negb_pow[i], step_mod);
      const I128 target   = safe_mod(-off[i], step_mod);

      auto [g, coeff_inv] = inv_gcd(coeff, step_mod);
      if (target % g != 0) {
        feasible = false;
        break;
      }

      const I128 class_mod = step_mod / g;
      I128 residue         = safe_mod(target / g, class_mod);
      residue              = safe_mod(residue * safe_mod(coeff_inv, class_mod), class_mod);

      if (!merge_congruences(rem, mod, residue, class_mod)) {
        feasible = false;
        break;
      }
    }
    if (!feasible)
      continue;

    // 3) Smallest T_0 >= lo in the congruence class; the length is usable iff <= hi.
    I128 t0 = rem;
    if (t0 < lo)
      t0 += div_ceil(lo - rem, mod) * mod;
    if (t0 <= hi)
      return L;
  }
  return 0;
}

//===----------------------------------------------------------------------===//
/* Main Solver Function */

using namespace std;

void solve() {
  I64 n, a, b, k;
  IN(n, a, b, k);

  // a == b: the map is the involution x <-> k/a - x; cook everything or nothing.
  if (a == b) {
    OUT(k % a == 0 ? n : 0);
    return;
  }

  // Self-loop at x* = k/(a+b): cook n-1 pancakes after sacrificing the first.
  I64 best = (k % (a + b) == 0) ? n - 1 : 0;

  // A prefix of the golden chain may already cook all n pancakes.
  I64 golden = golden_chain_steps(a, b, k);
  if (n <= golden + 1) {
    OUT(n);
    return;
  }

  // Otherwise: one golden block (if any) plus repeated longest-chain blocks,
  // each block cooking L of its L+1 pancakes.
  I64 golden_pts = max<I64>(0, golden + 1);
  I64 budget     = n - golden_pts;

  I64 path      = longest_chain(a, b, k);
  I64 block_pts = 0;
  if (path > 0) {
    I64 blocks   = budget / (path + 1);
    I64 leftover = budget % (path + 1);
    block_pts    = blocks * path + max<I64>(0, leftover - 1);
  }

  OUT(max(best, golden_pts + block_pts));
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Stopwatch timer;
#endif

  INT(T);
  FOR(T)
  solve();

  return 0;
}

//===----------------------------------------------------------------------===//
