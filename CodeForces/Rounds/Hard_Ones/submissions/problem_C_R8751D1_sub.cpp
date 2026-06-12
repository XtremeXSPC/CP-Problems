//===----------------------------------------------------------------------===//
/**
 * @file: problem_C_R8751D1_sub.cpp
 * @generated: 2026-06-12 11:52:31
 * @source: problem_C_R8751D1.cpp
 * @author: C.L.
 *
 * @brief: Codeforces Round XXX (Div. X) - Problem_C_R8751D1
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

namespace detail {

template <class T, bool = std::is_integral_v<remove_cvref_t<T>> && !std::is_same_v<remove_cvref_t<T>, bool>>
struct loop_arg {
  using type = remove_cvref_t<T>;
};

template <class T>
struct loop_arg<T, true> {
  using type = std::make_signed_t<remove_cvref_t<T>>;
};

} // namespace detail

template <class... Ts>
using loop_t = std::common_type_t<I32, typename detail::loop_arg<Ts>::type...>;

template <class... Ts>
struct loop {
  using T = loop_t<Ts...>;

  template <class X>
  [[gnu::always_inline]] static constexpr T val(X x) noexcept {
    return x;
  }
};

template <class R>
[[gnu::always_inline]] constexpr I64 sz64(const R& x) noexcept(noexcept(std::ssize(x))) {
  return std::ssize(x);
}

template <class R>
[[gnu::always_inline]] constexpr I32 sz32(const R& x) noexcept(noexcept(std::ssize(x))) {
  return std::ssize(x);
}

} // namespace cp

//===----------------------------------------------------------------------===//
/* Advanced Macro System */

// Advanced FOR loop system:
#define CP_LOOP(...) cp::loop<__VA_ARGS__>
#define CP_VAL(x, ...) CP_LOOP(__VA_ARGS__)::val(x)

#define FOR1(a) \
  for (auto _ = CP_VAL(0, decltype(a)); _ < CP_VAL(a, decltype(a)); ++_)
#define FOR2(i, a) \
  for (auto i = CP_VAL(0, decltype(a)); i < CP_VAL(a, decltype(a)); ++i)
#define FOR3(i, a, b) \
  for (auto i = CP_VAL(a, decltype(a), decltype(b)); i < CP_VAL(b, decltype(a), decltype(b)); ++i)
#define FOR4(i, a, b, c)                                          \
  for (auto i = CP_VAL(a, decltype(a), decltype(b), decltype(c)); \
       i < CP_VAL(b, decltype(a), decltype(b), decltype(c));      \
       i += CP_VAL(c, decltype(a), decltype(b), decltype(c)))
#define FOR1_R(a) \
  for (auto _ = CP_VAL(a, decltype(a)); _-- > 0;)
#define FOR2_R(i, a) \
  for (auto i = CP_VAL(a, decltype(a)); i-- > 0;)
#define FOR3_R(i, a, b) \
  for (auto i = CP_VAL(b, decltype(a), decltype(b)); i-- > CP_VAL(a, decltype(a), decltype(b));)

// Overload resolution for FOR macros:
#define overload4(a, b, c, d, e, ...) e
#define overload3(a, b, c, d, ...) d
#define FOR(...) overload4(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)(__VA_ARGS__)
#define FOR_R(...) overload3(__VA_ARGS__, FOR3_R, FOR2_R, FOR1_R)(__VA_ARGS__)
#define ROF(...) FOR_R(__VA_ARGS__)
#define FORD3(i, a, b) \
  for (auto i = CP_VAL(a, decltype(a), decltype(b)); i >= CP_VAL(b, decltype(a), decltype(b)); --i)
#define FORD4(i, a, b, c)                                         \
  for (auto i = CP_VAL(a, decltype(a), decltype(b), decltype(c)); \
       i >= CP_VAL(b, decltype(a), decltype(b), decltype(c));     \
       i -= CP_VAL(c, decltype(a), decltype(b), decltype(c)))
#define FORD(...) overload4(__VA_ARGS__, FORD4, FORD3)(__VA_ARGS__)

// Range-based iteration macros:
#define ALL(x) std::ranges::begin(x), std::ranges::end(x)
#define RALL(x) std::ranges::rbegin(x), std::ranges::rend(x)

// Container utility macros:
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x) cp::sz64(x)
#define isz(x) cp::sz32(x)
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
    auto&& _cp_min_range = (x);                      \
    if (std::ranges::empty(_cp_min_range)) {         \
      my_assert(false && "MIN(): empty range.");     \
      std::abort();                                  \
    }                                                \
    return *std::ranges::min_element(_cp_min_range); \
  }())
#define MAX(x)                                       \
  ([&]() -> decltype(auto) {                         \
    auto&& _cp_max_range = (x);                      \
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
concept Hashable = requires(const cvref_t<T>& value) {
  { std::hash<cvref_t<T>>{}(value) } -> std::convertible_to<std::size_t>;
};

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
alignas(64) inline char number_buffer[160];

struct FourDigitTable {
  char digits[10'000][4];

  constexpr FourDigitTable() : digits{} {
    for (U32 value = 0; value < 10'000; ++value) {
      U32 x = value;
      for (I32 pos = 3; pos >= 0; --pos) {
        digits[value][pos] = as<char>('0' + (x % 10));
        x /= 10;
      }
    }
  }
};

inline constexpr FourDigitTable four_digit_table{};

inline U32 input_pos  = 0;
inline U32 input_end  = 0;
inline U32 output_pos = 0;

/* ------------------------------- INPUT API -------------------------------- */

inline void load_input() {
  // Full reload: assumes any single token fits within BUFFER_SIZE.
  input_end = as<U32>(std::fread(input_buffer, 1, BUFFER_SIZE, stdin));
  input_pos = 0;
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
      c        = next_input_char();
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
    c            = next_input_char();
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
  using UnsignedT = cp::make_unsigned_t<T>;
  UnsignedT ux;
  if constexpr (cp::Signed<T>) {
    ux = x < 0 ? as<UnsignedT>(-(x + 1)) + 1 : as<UnsignedT>(x);
  } else {
    ux = as<UnsignedT>(x);
  }

  I32 begin = I32(sizeof(number_buffer));
  while (ux >= 10'000) {
    const U32 chunk = as<U32>(ux % 10'000);
    ux /= 10'000;
    begin -= 4;
    std::memcpy(number_buffer + begin, four_digit_table.digits[chunk], 4);
  }

  const U32 head = as<U32>(ux);
  if (head >= 1'000) {
    begin -= 4;
    std::memcpy(number_buffer + begin, four_digit_table.digits[head], 4);
  } else if (head >= 100) {
    begin -= 3;
    std::memcpy(number_buffer + begin, four_digit_table.digits[head] + 1, 3);
  } else if (head >= 10) {
    begin -= 2;
    number_buffer[begin]     = as<char>('0' + head / 10);
    number_buffer[begin + 1] = as<char>('0' + head % 10);
  } else {
    number_buffer[--begin] = as<char>('0' + head);
  }

  if constexpr (cp::Signed<T>) {
    if (x < 0)
      number_buffer[--begin] = '-';
  }

  [[assume(begin >= 0)]];
  const U32 len = as<U32>(I32(sizeof(number_buffer)) - begin);
  if (output_pos + len >= BUFFER_SIZE)
    flush_output();
  std::memcpy(output_buffer + output_pos, number_buffer + begin, len);
  output_pos += len;
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
  U32 remaining    = as<U32>(s.size());
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

namespace fast_io {

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
/* Main Solver Function */

using namespace std;

constexpr I32 INF = 1 << 29;

enum Tag : I32 { BAD, PATH, PINT, FD, FS, NIL };

struct Prof {
  Tag t = BAD;
  I32 a = 0, b = 0;
};

struct Lat {
  bool fit = false;
  I32 lam = 0, s = 0;
};

Prof bad() { return {BAD, 0, 0}; }
Prof path(I32 len) { return {PATH, len, 0}; }
Prof pint(I32 a, I32 b) { return {PINT, a, b}; }
Prof fd(I32 d) { return {FD, min(d, INF), 0}; }
Prof fs(I32 s) { return s == 0 ? fd(0) : Prof{FS, s, 0}; }
Prof nil() { return {NIL, 0, 0}; }
Lat no_lat() { return {}; }
Lat lat(I32 lam, I32 s) { return {true, lam, s}; }

Prof shift(const Prof& p) {
  switch (p.t) {
  case PATH:
    return path(p.a + 1);
  case PINT:
    return fd(1);
  case FD:
    return fd(p.a + 1);
  case FS:
    return fs(p.a - 1);
  default:
    return bad();
  }
}

I32 free_top(const Prof& p) {
  switch (p.t) {
  case PATH:
    return INF;
  case FD:
    return p.a + 1;
  case PINT:
    return 1;
  default:
    return -1;
  }
}

struct Dec {
  I32 up   = -1;
  I32 side = 0;
  I32 len  = 0;
  bool both_path = false;
};

struct Pack {
  I32 n = 0;
  I32 v[3]{};
  Prof p[3];
  Lat l[3];

  void add(I32 id, Prof prof, Lat lat = {}) {
    v[n] = id;
    p[n] = prof;
    l[n] = lat;
    ++n;
  }
};

I32 lat_lam(const Prof& p) {
  if (p.t == PINT)
    return 1;
  if (p.t == FD)
    return min(p.a + 1, INF);
  return -1;
}

Lat side_prof(const Pack& ps) {
  I32 id[2], n = 0;
  FOR(i, ps.n) {
    if (ps.p[i].t != NIL) {
      if (n == 2)
        return no_lat();
      id[n++] = i;
    }
  }

  if (n == 0)
    return lat(INF, 0);

  if (n == 1) {
    const Prof& p = ps.p[id[0]];
    if (p.t == PATH)
      return lat(INF, 0);
    const I32 lam = lat_lam(p);
    return lam < 0 ? no_lat() : lat(lam, 0);
  }

  const Prof& A = ps.p[id[0]];
  const Prof& B = ps.p[id[1]];
  const bool p0 = A.t == PATH;
  const bool p1 = B.t == PATH;
  if (p0 && p1)
    return lat(INF, min(A.a, B.a));
  if (p0 == p1)
    return no_lat();

  const Prof& P = p0 ? A : B;
  const Prof& X = p0 ? B : A;
  const I32 lam = lat_lam(X);
  return lam < 0 ? no_lat() : lat(lam, P.a);
}

Prof merge2(const Prof& A, const Lat& LA, const Prof& B, const Lat& LB, Dec& dec) {
  if (A.t == BAD || B.t == BAD)
    return bad();
  if (A.t == PATH && B.t == PATH) {
    dec = {0, 0, 0, true};
    return pint(A.a, B.a);
  }

  I32 best = INF;
  Dec take;
  const Prof* p[2] = {&A, &B};

  FOR(up, 2) {
    const Prof& U = *p[up];
    const Prof& S = *p[1 - up];
    const I32 lim = free_top(U);
    if (lim < 0)
      continue;

    if (S.t == PATH) {
      if (S.a <= lim && 0 < best) {
        best = 0;
        take = {up, 0, 0, false};
      }
      if (S.a - 1 < best) {
        best = S.a - 1;
        take = {up, 1, 0, false};
      }
    } else if (S.t == PINT) {
      if (S.a <= lim - 1 && S.b < best) {
        best = S.b;
        take = {up, 2, S.a, false};
      }
      if (S.b <= lim - 1 && S.a < best) {
        best = S.a;
        take = {up, 2, S.b, false};
      }
    }

    const Lat& L = (up == 0 ? LB : LA);
    if (U.t == PATH && L.fit && U.a < L.lam && L.s < best) {
      best = L.s;
      take = {up, 3, 0, false};
    }
  }

  if (take.up < 0)
    return bad();
  dec = take;
  return fs(best);
}

Prof merge_all(const Pack& ps, Dec& dec) {
  I32 id[2], n = 0;
  FOR(i, ps.n) {
    if (ps.p[i].t != NIL) {
      if (n == 2)
        return bad();
      id[n++] = i;
    }
  }

  if (n == 0)
    return path(1);
  if (n == 1)
    return shift(ps.p[id[0]]);
  return merge2(ps.p[id[0]], ps.l[id[0]], ps.p[id[1]], ps.l[id[1]], dec);
}

bool top_one(const Prof& p) {
  return p.t == PATH || p.t == FD || p.t == PINT;
}

bool top_fit(const Prof& A, const Prof& B) {
  const bool hasA = A.t != NIL;
  const bool hasB = B.t != NIL;
  if (!hasA && !hasB)
    return true;
  if (!hasA)
    return top_one(B);
  if (!hasB)
    return top_one(A);
  if (A.t == PATH && B.t == PATH)
    return true;
  if (A.t == FD && B.t == PATH)
    return B.a <= A.a;
  if (B.t == FD && A.t == PATH)
    return A.a <= B.a;
  return false;
}

struct Solver {
  I32 n = 0, m = 0;
  Vec2D<I32> adj, cyc, att;
  VecI32 deg, par, dep, ord;
  VecI32 cid, pos, ext, entry;
  Vec<Prof> down, up;
  Vec<Lat> ldown, lup;
  VecI32 X, Y;
  bool fail = false;

  Prof beyond_att(I32 v) {
    const I32 h = ext[v];
    return par[h] == v ? down[h] : up[v];
  }

  Prof beyond(I32 u, I32 v) {
    return par[v] == u ? down[v] : up[u];
  }

  Lat side_beyond(I32 u, I32 v) {
    return par[v] == u ? ldown[v] : lup[u];
  }

  void build() {
    par.assign(n + 1, 0);
    dep.assign(n + 1, -1);
    cid.assign(n + 1, -1);
    pos.assign(n + 1, 0);
    ord.clear();
    ord.reserve(n);

    VecI32 it(n + 1, 0), st{1};
    dep[1] = 0;

    while (!st.empty()) {
      const I32 v = st.back();
      if (it[v] == 0)
        ord.push_back(v);

      if (it[v] == isz(adj[v])) {
        st.pop_back();
        continue;
      }

      const I32 to = adj[v][it[v]++];
      if (to == par[v])
        continue;

      if (dep[to] == -1) {
        dep[to] = dep[v] + 1;
        par[to] = v;
        st.push_back(to);
      } else if (dep[to] < dep[v]) {
        VecI32 vs;
        for (I32 x = v; x != to; x = par[x])
          vs.push_back(x);
        vs.push_back(to);
        reverse(all(vs));

        const I32 id = isz(cyc);
        if (isz(vs) % 2)
          fail = true;
        FOR(i, isz(vs)) {
          if (cid[vs[i]] != -1)
            fail = true;
          cid[vs[i]] = id;
          pos[vs[i]] = i;
        }
        cyc.push_back(std::move(vs));
        entry.push_back(to);
      }
    }

    ext.assign(n + 1, 0);
    att.assign(isz(cyc), {});
    FOR(v, 1, n + 1) {
      if (cid[v] < 0)
        continue;

      const VecI32& vs = cyc[cid[v]];
      const I32 L      = isz(vs);
      const I32 p      = pos[v];
      const I32 prev   = vs[(p - 1 + L) % L];
      const I32 next   = vs[(p + 1) % L];

      for (const I32 to : adj[v]) {
        if (to != prev && to != next)
          ext[v] = to;
      }
      if (ext[v])
        att[cid[v]].push_back(v);
    }
  }

  bool phi_fit(I32 c, I32 root, I32 dir, I32& s) {
    const VecI32& vs = cyc[c];
    const I32 L      = isz(vs);
    const I32 k      = L / 2;
    const I32 p      = pos[root];
    const I32 low    = ((p + dir) % L + L) % L;
    const I32 top1   = ((p + dir * k) % L + L) % L;
    const I32 top2   = ((p + dir * (k + 1)) % L + L) % L;

    Prof A = nil(), B = nil();
    s = 0;

    for (const I32 v : att[c]) {
      if (v == root)
        continue;
      const I32 q     = pos[v];
      const Prof prof = beyond_att(v);
      if (prof.t == BAD)
        return false;

      if (q == low) {
        if (prof.t != PATH)
          return false;
        s = prof.a;
      } else if (q == top1) {
        A = prof;
      } else if (q == top2) {
        B = prof;
      } else {
        return false;
      }
    }

    return top_fit(A, B);
  }

  Prof best_phi(I32 c, I32 root, I32* out = nullptr) {
    if (isz(att[c]) > 4)
      return bad();

    I32 best = INF, take = 0;
    FOR(d, 2) {
      const I32 dir = (d == 0 ? 1 : -1);
      I32 s = 0;
      if (phi_fit(c, root, dir, s) && s < best) {
        best = s;
        take = dir;
      }
    }

    if (take == 0)
      return bad();
    if (out)
      *out = take;
    return fs(best);
  }

  void calc() {
    down.assign(n + 1, bad());
    up.assign(n + 1, nil());
    ldown.assign(n + 1, no_lat());
    lup.assign(n + 1, no_lat());
    Dec dec;

    for (I32 i = n - 1; i >= 0; --i) {
      const I32 v = ord[i];
      if (cid[v] >= 0) {
        if (entry[cid[v]] == v)
          down[v] = best_phi(cid[v], v);
        continue;
      }

      Pack ps;
      for (const I32 to : adj[v]) {
        if (to != par[v])
          ps.add(to, down[to], ldown[to]);
      }
      down[v]  = merge_all(ps, dec);
      ldown[v] = side_prof(ps);
    }

    FOR(i, 1, n) {
      const I32 v = ord[i];
      const I32 p = par[v];

      if (cid[v] >= 0 && cid[v] == (p ? cid[p] : -2)) {
        up[v] = bad();
        continue;
      }
      if (cid[p] >= 0) {
        up[v]  = best_phi(cid[p], p);
        lup[v] = no_lat();
        continue;
      }

      Pack ps;
      for (const I32 to : adj[p]) {
        if (to != v && to != par[p])
          ps.add(to, down[to], ldown[to]);
      }
      if (p != ord[0]) {
        ps.add(par[p], up[p], lup[p]);
      }
      up[v]  = merge_all(ps, dec);
      lup[v] = side_prof(ps);
    }
  }

  struct Job {
    I32 u, from, col, row, dir;
    bool side = false;
  };

  Vec<Job> jobs;

  void push(I32 u, I32 from, I32 col, I32 row, I32 dir) {
    jobs.push_back({u, from, col, row, dir, false});
  }

  void push_side(I32 u, I32 from, I32 col, I32 row, I32 dir) {
    jobs.push_back({u, from, col, row, dir, true});
  }

  void place_cycle(I32 c, I32 p, I32 d, I32 col, I32 row, I32 dir) {
    const VecI32& vs = cyc[c];
    const I32 L = isz(vs);
    const I32 k = L / 2;

    FOR(t, L) {
      const I32 v = vs[((p + d * t) % L + L) % L];
      if (t == 0) {
        X[v] = col;
        Y[v] = row;
      } else if (t <= k) {
        X[v] = 1 - col;
        Y[v] = row + (t - 1) * dir;
      } else {
        X[v] = col;
        Y[v] = row + (2 * k - t) * dir;
      }
    }
  }

  void expand_cycle(I32 root, I32 col, I32 row, I32 dir) {
    const I32 c = cid[root];
    const I32 p = pos[root];
    const VecI32& vs = cyc[c];
    const I32 L = isz(vs);
    const I32 k = L / 2;

    I32 best = INF, take = 0;
    FOR(d, 2) {
      const I32 od = (d == 0 ? 1 : -1);
      I32 s        = 0;
      if (phi_fit(c, root, od, s) && s < best) {
        best = s;
        take = od;
      }
    }

    place_cycle(c, p, take, col, row, dir);

    const I32 low  = ((p + take) % L + L) % L;
    const I32 top1 = ((p + take * k) % L + L) % L;
    const I32 top2 = ((p + take * (k + 1)) % L + L) % L;
    const I32 a = vs[low], b = vs[top1], e = vs[top2];

    if (ext[a] && a != root)
      push(ext[a], a, 1 - col, row - dir, -dir);
    if (ext[b])
      push(ext[b], b, 1 - col, row + k * dir, dir);
    if (ext[e])
      push(ext[e], e, col, row + k * dir, dir);
  }

  void expand_tree(I32 u, I32 from, I32 col, I32 row, I32 dir) {
    X[u] = col;
    Y[u] = row;

    Pack ch;
    for (const I32 v : adj[u]) {
      if (v == from)
        continue;
      ch.add(v, beyond(u, v), side_beyond(u, v));
    }

    if (ch.n == 0)
      return;
    if (ch.n == 1) {
      push(ch.v[0], u, col, row + dir, dir);
      return;
    }

    Dec dec;
    if (merge2(ch.p[0], ch.l[0], ch.p[1], ch.l[1], dec).t == BAD)
      return;

    if (dec.both_path) {
      push(ch.v[0], u, col, row + dir, dir);
      push(ch.v[1], u, 1 - col, row, dir);
      return;
    }

    const I32 U = ch.v[dec.up];
    const I32 S = ch.v[1 - dec.up];
    push(U, u, col, row + dir, dir);

    if (dec.side == 0) {
      push(S, u, 1 - col, row, dir);
    } else if (dec.side == 1) {
      push(S, u, 1 - col, row, -dir);
    } else if (dec.side == 3) {
      push_side(S, u, 1 - col, row, dir);
    } else {
      X[S] = 1 - col;
      Y[S] = row;

      I32 a = -1, b = -1;
      for (const I32 v : adj[S]) {
        if (v == u)
          continue;
        (a < 0 ? a : b) = v;
      }

      const I32 len        = beyond(S, a).a;
      const I32 up_child   = (len == dec.len ? a : b);
      const I32 down_child = (up_child == a ? b : a);
      push(up_child, S, 1 - col, row + dir, dir);
      push(down_child, S, 1 - col, row - dir, -dir);
    }
  }

  void expand_side(I32 u, I32 from, I32 col, I32 row, I32 dir) {
    X[u] = col;
    Y[u] = row;

    Pack ch;
    for (const I32 v : adj[u]) {
      if (v == from)
        continue;
      ch.add(v, beyond(u, v));
    }

    if (ch.n == 0)
      return;
    if (ch.n == 1) {
      push(ch.v[0], u, col, row + dir, dir);
      return;
    }

    const bool p0 = ch.p[0].t == PATH;
    const bool p1 = ch.p[1].t == PATH;
    if (p0 && p1) {
      const I32 dn  = ch.p[0].a <= ch.p[1].a ? 0 : 1;
      const I32 upi = 1 - dn;
      push(ch.v[upi], u, col, row + dir, dir);
      push(ch.v[dn], u, col, row - dir, -dir);
    } else if (p0 != p1) {
      const I32 dn  = p0 ? 0 : 1;
      const I32 upi = 1 - dn;
      push(ch.v[upi], u, col, row + dir, dir);
      push(ch.v[dn], u, col, row - dir, -dir);
    }
  }

  void run() {
    while (!jobs.empty()) {
      const Job j = jobs.back();
      jobs.pop_back();

      if (j.side)
        expand_side(j.u, j.from, j.col, j.row, j.dir);
      else if (cid[j.u] >= 0)
        expand_cycle(j.u, j.col, j.row, j.dir);
      else
        expand_tree(j.u, j.from, j.col, j.row, j.dir);
    }
  }

  bool valid() {
    Set<PairI32> seen;
    FOR(v, 1, n + 1) {
      if (X[v] < 0 || X[v] > 1)
        return false;
      if (Y[v] < -200000 || Y[v] > 200000)
        return false;
      if (!seen.insert({X[v], Y[v]}).second)
        return false;
    }

    FOR(v, 1, n + 1) {
      for (const I32 to : adj[v]) {
        if (v < to && abs(X[v] - X[to]) + abs(Y[v] - Y[to]) != 1)
          return false;
      }
    }
    return true;
  }

  void reset() {
    jobs.clear();
    X.assign(n + 1, -1);
    Y.assign(n + 1, 0);
  }

  bool accept() {
    run();
    return valid();
  }

  bool solve_case() {
    X.assign(n + 1, 0);
    Y.assign(n + 1, 0);

    if (n == 1)
      return true;

    FOR(v, 1, n + 1) {
      if (deg[v] >= 4)
        fail = true;
    }
    if (fail)
      return false;

    build();
    if (fail)
      return false;

    calc();

    Dec dec;
    FOR(v, 1, n + 1) {
      if (cid[v] >= 0 || deg[v] > 2)
        continue;

      Pack ps;
      for (const I32 to : adj[v]) {
        ps.add(to, beyond(v, to), side_beyond(v, to));
      }
      if (merge_all(ps, dec).t == BAD)
        continue;

      reset();
      push(v, 0, 0, 0, 1);
      if (accept())
        return true;
    }

    FOR(c, isz(cyc)) {
      const VecI32& vs = cyc[c];
      const I32 L = isz(vs);
      const I32 k = L / 2;
      if (isz(att[c]) > 4)
        continue;

      FOR(i, L) {
        bool fits = true;
        Prof A = nil(), B = nil();

        for (const I32 v : att[c]) {
          const I32 d     = (pos[v] - i + L) % L;
          const Prof prof = beyond_att(v);
          if (prof.t == BAD) {
            fits = false;
            break;
          }

          if (d == 0 || d == 1) {
            if (prof.t != PATH) {
              fits = false;
              break;
            }
          } else if (d == k) {
            A = prof;
          } else if (d == k + 1) {
            B = prof;
          } else {
            fits = false;
            break;
          }
        }

        if (!fits || !top_fit(A, B))
          continue;

        reset();
        place_cycle(c, i, 1, 0, 0, 1);

        const I32 a = vs[i];
        const I32 b = vs[(i + 1) % L];
        const I32 x = vs[(i + k) % L];
        const I32 y = vs[(i + k + 1) % L];

        if (ext[a])
          push(ext[a], a, 0, -1, -1);
        if (ext[b])
          push(ext[b], b, 1, -1, -1);
        if (ext[x])
          push(ext[x], x, 1, k, 1);
        if (ext[y])
          push(ext[y], y, 0, k, 1);

        if (accept())
          return true;
      }
    }

    return false;
  }
};

void solve() {
  INT(n, m);

  Solver S;
  S.n = n;
  S.m = m;
  S.adj.assign(n + 1, {});
  S.deg.assign(n + 1, 0);

  FOR(m) {
    INT(u, v);
    S.adj[u].push_back(v);
    S.adj[v].push_back(u);
    ++S.deg[u];
    ++S.deg[v];
  }

  if (!S.solve_case()) {
    OUT("No");
    return;
  }

  OUT("Yes");
  FOR(i, 1, n + 1)
  OUT(S.X[i], S.Y[i]);
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
