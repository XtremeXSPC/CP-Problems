//===----------------------------------------------------------------------===//
/**
 * @file: problem_A_sub.cpp
 * @generated: 2026-02-14 19:08:15
 * @source: problem_A.cpp
 * @author: Costantino Lombardi
 *
 * @brief: Codeforces Round 1070 (Div. 2) - Problem A
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

// Legacy aliases for backward compatibility:
using ll  = I64;
using ull = U64;
using ld  = F80;

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

// Short aliases for competitive programming:
template <class T>
using VC = Vec<T>;
template <class T>
using VVC = VC<VC<T>>;
template <class T>
using VVVC = VC<VVC<T>>;

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

// Policy-based data structures:
#if defined(PBDS_AVAILABLE) && PBDS_AVAILABLE
  using namespace __gnu_pbds;
  template <typename T>
  using ordered_set = tree<T, null_type, std::less<T>, rb_tree_tag, tree_order_statistics_node_update>;
  template <typename T>
  using ordered_multiset = tree<T, null_type, std::less_equal<T>, rb_tree_tag, tree_order_statistics_node_update>;
  template <typename K, typename V>
  using ordered_map = tree<K, V, std::less<K>, rb_tree_tag, tree_order_statistics_node_update>;
  template <typename K, typename V>
  using gp_hash_table = __gnu_pbds::gp_hash_table<K, V, std::hash<K>, std::equal_to<K>, direct_mask_range_hashing<>, linear_probe_fn<>, hash_standard_resize_policy<hash_exponential_size_policy<>, hash_load_check_resize_trigger<>, true>>;
#endif

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

#ifdef __SIZEOF_INT128__
template <>
inline constexpr I128 infinity<I128> = I128(infinity<I64>) * 2'000'000'000'000'000'000LL;
#endif

constexpr I32 INF32 = infinity<I32>;
constexpr I64 INF64 = infinity<I64>;
constexpr I64 LINF  = INF64; // Legacy alias

// Modular arithmetic constants:
constexpr I64 MOD  = 1000000007;
constexpr I64 MOD2 = 998244353;
constexpr I64 MOD3 = 1000000009;

//===----------------------------------------------------------------------===//
/* Advanced Macro System */

// Multi-dimensional vector creation macros:
#define make_vec(type, name, ...) VC<type> name(__VA_ARGS__)
#define vv(type, name, h, ...) \
  VC<VC<type>> name(h, VC<type>(__VA_ARGS__))
#define vvv(type, name, h, w, ...) \
  VC<VC<VC<type>>> name(h, VC<VC<type>>(w, VC<type>(__VA_ARGS__)))
#define vvvv(type, name, a, b, c, ...) \
  VC<VC<VC<VC<type>>>> name(a, VC<VC<VC<type>>>(b, VC<VC<type>>(c, VC<type>(__VA_ARGS__))))

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

// Range-based iteration:
#define REP(i, n) for (I64 i : std::views::iota(0LL, (I64)(n)))
#define RREP(i, n) for (I64 i : std::views::iota(0LL, (I64)(n)) | std::views::reverse)
#define ALL(x) std::ranges::begin(x), std::ranges::end(x)
#define RALL(x) std::ranges::rbegin(x), std::ranges::rend(x)

// Container utility macros:
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x) (I64)(x).size()
#define len(x) sz(x)
#define pb push_back
#define eb emplace_back
#define mp make_pair
#define mt make_tuple
#define fi first
#define se second
#define elif else if

// Advanced container operations:
#define UNIQUE(x) (std::ranges::sort(x), x.erase(std::ranges::unique(x).begin(), x.end()), x.shrink_to_fit())
#define LB(c, x) (I64)std::distance((c).begin(), std::ranges::lower_bound(c, x))
#define UB(c, x) (I64)std::distance((c).begin(), std::ranges::upper_bound(c, x))
#define SUM(x) std::accumulate(all(x), 0LL)
#define MIN(x) *std::ranges::min_element(x)
#define MAX(x) *std::ranges::max_element(x)

//===----------------------------------------------------------------------===//
/* Mathematical Utilities */

// Generic mathematical functions:
template <typename T>
[[gnu::always_inline]] constexpr T _gcd(T a, T b) {
  if constexpr (std::is_integral_v<T>) {
    return b ? _gcd(b, a % b) : a;
  } else {
    return std::gcd(a, b);
  }
}

template <typename T>
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

// Fast modular exponentiation:
template <typename T>
[[gnu::always_inline]] constexpr T _power(T base, T exp, T mod = 0) {
  T result = 1;
  if (mod) base %= mod;
  while (exp > 0) {
    if (exp & 1) {
      result = mod ? (result * base) % mod : result * base;
    }
    base = mod ? (base * base) % mod : base * base;
    exp >>= 1;
  }
  return result;
}

template <class T, class S>
[[gnu::always_inline]] inline bool chmax(T& a, const S& b) {
  return a < b ? (a = b, true) : false;
}

template <class T, class S>
[[gnu::always_inline]] inline bool chmin(T& a, const S& b) {
  return a > b ? (a = b, true) : false;
}

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
/* Optimized I/O System */

// Fast I/O
namespace fast_io {
  static constexpr U32 BUFFER_SIZE = 1 << 17; // 128KB buffer.
  alignas(64) inline char input_buffer[BUFFER_SIZE];
  alignas(64) inline char output_buffer[BUFFER_SIZE];
  alignas(64) inline char number_buffer[128];
  
  // Precomputed number strings for fast output:
  struct NumberLookup {
    char digits[10000][4];
    constexpr NumberLookup() : digits{} {
      for (I32 i = 0; i < 10000; ++i) {
        digits[i][3] = '0' + (i % 10);
        digits[i][2] = '0' + ((i / 10) % 10);
        digits[i][1] = '0' + ((i / 100) % 10);
        digits[i][0] = '0' + (i / 1000);
      }
    }
  };
  inline constexpr NumberLookup number_lookup;
  
  inline U32 input_pos = 0, input_end = 0, output_pos = 0;
  
  inline void load_input() {
    std::memmove(input_buffer, input_buffer + input_pos, input_end - input_pos);
    input_end = input_end - input_pos + 
                std::fread(input_buffer + input_end - input_pos, 1, 
                          BUFFER_SIZE - input_end + input_pos, stdin);
    input_pos = 0;
    if (input_end < BUFFER_SIZE) input_buffer[input_end++] = '\n';
  }
  
  inline void flush_output() {
    std::fwrite(output_buffer, 1, output_pos, stdout);
    output_pos = 0;
  }
  
  // Fast character reading:
  inline void read_char(char& c) {
    do {
      if (input_pos >= input_end) load_input();
      c = input_buffer[input_pos++];
    } while (std::isspace(c));
  }
  
  // Optimized integer reading with SIMD potential:
  template <typename T>
  inline void read_integer(T& x) {
    if (input_pos + 64 >= input_end) load_input();
    
    char c;
    do { c = input_buffer[input_pos++]; } while (c < '-');
    
    bool negative = false;
    if constexpr (std::is_signed_v<T>) {
      if (c == '-') {
        negative = true;
        c = input_buffer[input_pos++];
      }
    }
    
    x = 0;
    while (c >= '0') {
      x = x * 10 + (c - '0');
      c = input_buffer[input_pos++];
    }
    
    if constexpr (std::is_signed_v<T>) {
      if (negative) x = -x;
    }
  }
  
  // Fast string reading:
  inline void read_string(std::string& s) {
    s.clear();
    char c;
    do {
      if (input_pos >= input_end) load_input();
      c = input_buffer[input_pos++];
    } while (std::isspace(c));
    
    do {
      s.push_back(c);
      if (input_pos >= input_end) load_input();
      c = input_buffer[input_pos++];
    } while (!std::isspace(c));
  }
  
  // Optimized integer writing:
  template <typename T>
  inline void write_integer(T x) {
    if (output_pos + 64 >= BUFFER_SIZE) flush_output();

    using UnsignedT = std::make_unsigned_t<T>;
    UnsignedT ux;
    if constexpr (std::is_signed_v<T>) {
      if (x < 0) {
        output_buffer[output_pos++] = '-';
        // Avoid UB for minimum signed value.
        ux = static_cast<UnsignedT>(-(x + 1));
        ux += 1;
      } else {
        ux = static_cast<UnsignedT>(x);
      }
    } else {
      ux = static_cast<UnsignedT>(x);
    }

    I32 digits = 0;
    UnsignedT temp = ux;
    do {
      number_buffer[digits++] = '0' + (temp % 10);
      temp /= 10;
    } while (temp > 0);
    
    // Reverse and copy:
    for (I32 i = digits - 1; i >= 0; --i) {
      output_buffer[output_pos++] = number_buffer[i];
    }
  }
  
  inline void write_char(char c) {
    if (output_pos >= BUFFER_SIZE) flush_output();
    output_buffer[output_pos++] = c;
  }
  
  inline void write_string(const std::string& s) {
    for (char c : s) write_char(c);
  }
  
  // Template-based readers:
  inline void read(I32& x) { read_integer(x); }
  inline void read(I64& x) { read_integer(x); }
  inline void read(U32& x) { read_integer(x); }
  inline void read(U64& x) { read_integer(x); }
  inline void read(char& x) { read_char(x); }
  inline void read(std::string& x) { read_string(x); }
  
  template <class T, class U>
  void read(std::pair<T, U>& p) { read(p.first); read(p.second); }
  
  template <class T>
  void read(VC<T>& v) { for (auto& x : v) read(x); }
  
  // Variadic read:
  template <class Head, class... Tail>
  void read(Head& head, Tail&... tail) {
    read(head);
    if constexpr (sizeof...(tail) > 0) read(tail...);
  }
  
  // Template-based writers:
  inline void write(I32 x) { write_integer(x); }
  inline void write(I64 x) { write_integer(x); }
  inline void write(U32 x) { write_integer(x); }
  inline void write(U64 x) { write_integer(x); }
  inline void write(char x) { write_char(x); }
  inline void write(const std::string& x) { write_string(x); }
  inline void write(const char* x) { write_string(std::string(x)); }
  
  template <class T, class U>
  void write(const std::pair<T, U>& p) {
    write(p.first); write(' '); write(p.second);
  }
  
  template <class T>
  void write(const VC<T>& v) {
    for (I64 i = 0; i < sz(v); ++i) {
      if (i) write(' ');
      write(v[i]);
    }
  }
  
  // Variadic write:
  template <class Head, class... Tail>
  void write(const Head& head, const Tail&... tail) {
    write(head);
    if constexpr (sizeof...(tail) > 0) {
      write(' ');
      write(tail...);
    }
  }
  
  inline void writeln() { write_char('\n'); }
  
  template <class... Args>
  void writeln(const Args&... args) {
    if constexpr (sizeof...(args) > 0) write(args...);
    write_char('\n');
  }
  
  // Destructor for automatic flushing:
  struct IOFlusher {
    ~IOFlusher() { flush_output(); }
  };
  inline IOFlusher io_flusher;
}

// Input/Output macros:
#define IN(...) fast_io::read(__VA_ARGS__)
#define OUT(...) fast_io::writeln(__VA_ARGS__)
#define FLUSH() fast_io::flush_output()

// Convenient input macros:
#define INT(...) I32 __VA_ARGS__; IN(__VA_ARGS__)
#define LL(...) I64 __VA_ARGS__; IN(__VA_ARGS__)
#define ULL(...) U64 __VA_ARGS__; IN(__VA_ARGS__)
#define STR(...) std::string __VA_ARGS__; IN(__VA_ARGS__)
#define CHR(...) char __VA_ARGS__; IN(__VA_ARGS__)
#define DBL(...) F64 __VA_ARGS__; IN(__VA_ARGS__)

#define VEC(type, name, size) VC<type> name(size); IN(name)
#define VV(type, name, h, w) VVC<type> name(h, VC<type>(w)); IN(name)

// Answer macros:
inline void YES(bool condition = true) { OUT(condition ? "YES" : "NO"); }
inline void NO(bool condition = true) { YES(!condition); }
inline void Yes(bool condition = true) { OUT(condition ? "Yes" : "No"); }
inline void No(bool condition = true) { Yes(!condition); }

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n);
  VC<I32> a(n);
  IN(a);

  I32 pref_max = a[0];
  I32 operations = 0;

  FOR(i, 1, n) {
    if (pref_max > a[i]) ++operations;
    chmax(pref_max, a[i]);
  }

  OUT(operations);
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
