//===----------------------------------------------------------------------===//
/**
 * @file: problem_D2_sub.cpp
 * @generated: 2026-05-28 20:46:36
 * @source: problem_D2.cpp
 * @author: C.L.
 *
 * @brief: Codeforces Round 1098 (Div. 2) - Problem D2
 */
//===----------------------------------------------------------------------===//
/* Included library and Compiler Optimizations */

#ifndef CP_TEMPLATE_PROFILE_STRICT
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define CP_IO_PROFILE_FAST_MINIMAL

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

#if defined(__GNUC__) && !defined(__clang__)
  #if CP_ENABLE_AGGRESSIVE_OPTIMIZATIONS
    #pragma GCC optimize("Ofast,unroll-loops,fast-math,O3,inline-functions")
  #else
    #pragma GCC optimize("O3,unroll-loops,inline-functions")
  #endif
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-result"
  #ifdef CP_ENABLE_ARCH_TARGET_PRAGMAS
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
/* Lightweight Stopwatch Utility */

struct Stopwatch {
  using Clock = std::chrono::high_resolution_clock;
  Clock::time_point start;

  Stopwatch() : start(Clock::now()) {}
  void reset() { start = Clock::now(); }
  [[gnu::always_inline]] F64 elapsed() const { return std::chrono::duration<F64>(Clock::now() - start).count(); }
  [[gnu::always_inline]] bool within(F64 limit) const { return elapsed() < limit; }
};

// Minimal Fast_IO shim: selects the no-extension variant of Fast_IO.hpp.
  #define CP_FAST_IO_VARIANT 0

//===----------------------------------------------------------------------===//
/* Library Function Aliases */

namespace cp {

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
/* Buffered I/O (variant-driven) */

template <I64 MOD>
struct ModInt;

namespace cp {
template <class T, class Tag>
class StrongType;
} // namespace cp

namespace fast_io {

template <class T>
concept FastIntegral = cp::Int<T> && !cp::Same<T, bool> && !cp::Same<T, char>;

template <class T>
concept FastFloating = cp::Float<T>;

template <class T>
inline void read_integer(T&);

template <class T>
inline void write_integer(T);

} // namespace fast_io

namespace fast_io {

static constexpr U32 BUFFER_SIZE = 1U << 20; // 1 MB
alignas(64) inline char input_buffer[BUFFER_SIZE];
alignas(64) inline char output_buffer[BUFFER_SIZE];
alignas(64) inline char number_buffer[128];

inline U32 input_pos  = 0;
inline U32 input_end  = 0;
inline U32 output_pos = 0;

#if CP_FAST_IO_VARIANT == 1
inline bool input_eof = false;
#endif

/* ------------------------------- INPUT API -------------------------------- */

inline void load_input() {
#if CP_FAST_IO_VARIANT == 1
  // memmove + sentinel: preserves tokens spanning a refill boundary.
  if (input_eof && input_pos >= input_end) {
    input_pos = input_end = 0;
    return;
  }
  const U32 remaining = input_end - input_pos;
  std::memmove(input_buffer, input_buffer + input_pos, remaining);
  const U32 capacity   = BUFFER_SIZE - remaining;
  const U32 bytes_read = as<U32>(std::fread(input_buffer + remaining, 1, capacity, stdin));
  input_end = remaining + bytes_read;
  input_pos = 0;
  if (bytes_read < capacity) {
    input_eof = true;
    if (input_end < BUFFER_SIZE)
      input_buffer[input_end++] = '\n';
  }
#else
  // Full reload: assumes any single token fits within BUFFER_SIZE.
  input_end = as<U32>(std::fread(input_buffer, 1, BUFFER_SIZE, stdin));
  input_pos = 0;
#endif
}

[[gnu::always_inline]] inline char next_input_char() {
  if (input_pos >= input_end) {
    load_input();
    if (input_pos >= input_end)
      return 0;
  }
  return input_buffer[input_pos++];
}

inline void flush_output() {
  if (output_pos == 0)
    return;
  std::fwrite(output_buffer, 1, output_pos, stdout);
  output_pos = 0;
}

inline void read_char(char& c) {
  do {
    c = next_input_char();
  } while (c <= ' ' && c != 0);
}

template <typename T>
inline void read_integer(T& x) {
  char c;
  do {
    c = next_input_char();
  } while (c <= ' ' && c != 0);

  bool negative = false;
  if constexpr (cp::Signed<T>) {
    if (c == '-') {
      negative = true;
      c = next_input_char();
    }
  }

  x = 0;
  while (c > ' ') {
    x = x * 10 + (c - '0');
    c = next_input_char();
  }

  if constexpr (cp::Signed<T>) {
    if (negative)
      x = -x;
  }
}

inline void read_string(std::string& s) {
  s.clear();
  s.reserve(32);
  char c;
  do {
    c = next_input_char();
  } while (c <= ' ' && c != 0);

  while (c > ' ') {
    s.push_back(c);
    c = next_input_char();
  }
}

template <typename T>
inline void read_floating(T& x) {
  char token[64];
  U32 len = 0;
  char c;
  do {
    c = next_input_char();
  } while (c <= ' ' && c != 0);
  while (c > ' ' && len + 1 < sizeof(token)) {
    token[len++] = c;
    c = next_input_char();
  }
  token[len] = '\0';

  char* end = nullptr;
  if constexpr (cp::Same<T, F32>)
    x = std::strtof(token, &end);
  else if constexpr (cp::Same<T, F64>)
    x = std::strtod(token, &end);
  else
    x = std::strtold(token, &end);
}

template <FastIntegral T>
inline void read(T& x) { read_integer(x); }

template <FastFloating T>
inline void read(T& x) { read_floating(x); }
inline void read(char& x) { read_char(x); }
inline void read(std::string& x) { read_string(x); }

/* ------------------------------- OUTPUT API ------------------------------- */

template <typename T>
inline void write_integer(T x) {
  if (output_pos + 64 >= BUFFER_SIZE)
    flush_output();

  using UnsignedT = std::make_unsigned_t<T>;
  UnsignedT ux;
  if constexpr (cp::Signed<T>) {
    if (x < 0) {
      output_buffer[output_pos++] = '-';
      ux = as<UnsignedT>(-(x + 1));
      ux += 1;
    } else {
      ux = as<UnsignedT>(x);
    }
  } else {
    ux = as<UnsignedT>(x);
  }

  I32 digits = 0;
  do {
    number_buffer[digits++] = as<char>('0' + (ux % 10));
    ux /= 10;
  } while (ux > 0);

  for (I32 i = digits - 1; i >= 0; --i) {
    output_buffer[output_pos++] = number_buffer[i];
  }
}

  #define CP_FLOAT_PRECISION 10

template <typename T>
inline void write_floating(T x) {
  char local_buffer[128];
  const int n = std::snprintf(local_buffer, sizeof(local_buffer), "%.*Lf", CP_FLOAT_PRECISION, as<F80>(x));
  if (n <= 0)
    return;

  U32 len = as<U32>(std::min(n, as<int>(sizeof(local_buffer) - 1)));
  if (output_pos + len >= BUFFER_SIZE)
    flush_output();
  if (len >= BUFFER_SIZE) {
    std::fwrite(local_buffer, 1, len, stdout);
    return;
  }
  std::memcpy(output_buffer + output_pos, local_buffer, len);
  output_pos += len;
}

inline void write_char(char c) {
  if (output_pos >= BUFFER_SIZE)
    flush_output();
  output_buffer[output_pos++] = c;
}

inline void write_string(std::string_view s) {
  const char* data = s.data();
  U32 remaining = as<U32>(s.size());
  while (remaining > 0) {
    if (output_pos >= BUFFER_SIZE)
      flush_output();
    U32 space = BUFFER_SIZE - output_pos;
    U32 chunk = (remaining < space) ? remaining : space;
    std::memcpy(output_buffer + output_pos, data, chunk);
    output_pos += chunk;
    data += chunk;
    remaining -= chunk;
  }
}

template <FastIntegral T>
inline void write(T x) { write_integer(x); }

template <FastFloating T>
inline void write(T x) { write_floating(x); }
inline void write(char x) { write_char(x); }
inline void write(const std::string& x) { write_string(x); }
inline void write(const char* x) { write_string(x); }

} // namespace fast_io

/* ------------------------------- EXTENSIONS ------------------------------- */

#if CP_FAST_IO_VARIANT == 1
  #ifndef CP_FAST_IO_ENABLE_MODINT
    #ifdef NEED_MOD_INT
      #define CP_FAST_IO_ENABLE_MODINT 1
    #else
      #define CP_FAST_IO_ENABLE_MODINT 0
    #endif
  #endif

  #ifndef CP_FAST_IO_ENABLE_STRONG_TYPE
    #define CP_FAST_IO_ENABLE_STRONG_TYPE 0
  #endif
  #if CP_FAST_IO_ENABLE_STRONG_TYPE && !defined(CP_USE_ADVANCED)
    #undef CP_FAST_IO_ENABLE_STRONG_TYPE
    #define CP_FAST_IO_ENABLE_STRONG_TYPE 0
  #endif

  #if CP_FAST_IO_ENABLE_MODINT
    #include "Fast_IO_Ext_ModInt.hpp"
  #endif
  #if CP_FAST_IO_ENABLE_STRONG_TYPE
    #include "templates/advanced/Fast_IO_Ext_StrongType.hpp"
  #endif
#endif // CP_FAST_IO_VARIANT == 1

namespace fast_io {

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

// Single-arg fallbacks: error on types lacking a concrete overload.
template <class T>
  requires(!FastIntegral<T> && !FastFloating<T>)
void read(T&) = delete;

template <class T>
  requires(!FastIntegral<T> && !FastFloating<T>)
void write(const T&) = delete;

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

const int MAXN = 2'000'005;
const int INF  = 1'000'000'000;

I32 CMin[MAXN], CMax[MAXN];
I32 RMin[MAXN], RMax[MAXN];
I32 PrsY[MAXN];

void solve() {
  INT(n);

  FOR(i, 1, n + 2) {
    CMin[i] = INF;
    CMax[i] = -1;
    PrsY[i] = 0;
  }

  FOR(i, 0, n) {
    INT(x, y);
    if (y < CMin[x]) CMin[x] = y;
    if (y > CMax[x]) CMax[x] = y;
    PrsY[y] = 1;
  }

  FOR(i, 1, n + 1) {
    PrsY[i] += PrsY[i - 1];
  }

  RMin[n + 1] = INF;
  RMax[n + 1] = -1;

  for (I32 i = n; i >= 1; --i) {
    RMin[i] = min(RMin[i + 1], CMin[i]);
    RMax[i] = max(RMax[i + 1], CMax[i]);
  }

  I64 ans  = 0;
  I32 LMin = INF, LMax = -1;

  FOR(x, 1, n + 1) {
    if (CMax[x] != -1) {
      if (CMin[x] < LMin) LMin = CMin[x];
      if (CMax[x] > LMax) LMax = CMax[x];
    } else {
      continue;
    }

    if (RMin[x + 1] == INF) continue;

    I32 low  = max(LMin, RMin[x + 1]);
    I32 high = min(LMax, RMax[x + 1]);

    if (low < high) {
      ans += PrsY[high - 1] - PrsY[low - 1];
    }
  }

  OUT(ans);
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
