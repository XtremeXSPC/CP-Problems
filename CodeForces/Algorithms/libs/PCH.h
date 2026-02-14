//===----------------------------------------------------------------------===//
/* PCH.h - Ultra-Optimized Precompiled Header for C++23
 * Enhanced version that provides comprehensive standard library access
 *
 * Optimized for competitive programming with advanced compiler features
 * Compatible with GCC, Clang, MSVC, and Apple Clang
 */
//===----------------------------------------------------------------------===//
// clang-format off

#ifndef MODERN_PCH_H
#define MODERN_PCH_H

// Prevent multiple inclusions and conflicts.
#ifndef _GLIBCXX_NO_ASSERT
    #define _GLIBCXX_NO_ASSERT
#endif
#define _BITS_STDCPP_H 1

//===----------------------------------------------------------------------===//
//===================== COMPILER DETECTION AND VERSIONING ====================//

// Enhanced compiler detection with feature capabilities.
#if defined(__GNUC__) && !defined(__clang__)
    #define COMPILER_GCC
    #define COMPILER_NAME "GCC"
    #define COMPILER_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
    #if COMPILER_VERSION < 120000  // GCC 12.0
        #error "GCC 12 or later required for full C++23 support"
    #endif
    #define SUPPORTS_CONCEPTS 1
    #define SUPPORTS_RANGES 1
    #define SUPPORTS_COROUTINES (__GNUC__ >= 11)
    #define SUPPORTS_MODULES (__GNUC__ >= 13)
#elif defined(__clang__)
    #define COMPILER_CLANG
    #if defined(__apple_build_version__)
        #define COMPILER_APPLE_CLANG
        #define COMPILER_NAME "Apple Clang"
        #define COMPILER_VERSION __apple_build_version__
    #else
        #define COMPILER_NAME "LLVM Clang"
        #define COMPILER_VERSION (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
    #endif
    #if __clang_major__ < 15
        #warning "Clang 15 or later recommended for optimal C++23 support"
    #endif
    #define SUPPORTS_CONCEPTS (__clang_major__ >= 12)
    #define SUPPORTS_RANGES (__clang_major__ >= 13)
    #define SUPPORTS_COROUTINES (__clang_major__ >= 14)
    #define SUPPORTS_MODULES (__clang_major__ >= 16)
#elif defined(_MSC_VER)
    #define COMPILER_MSVC
    #define COMPILER_NAME "MSVC"
    #define COMPILER_VERSION _MSC_VER
    #if _MSC_VER < 1930  // Visual Studio 2022 17.0
        #error "Visual Studio 2022 or later required"
    #endif
    #define SUPPORTS_CONCEPTS (_MSC_VER >= 1929)
    #define SUPPORTS_RANGES (_MSC_VER >= 1930)
    #define SUPPORTS_COROUTINES (_MSC_VER >= 1928)
    #define SUPPORTS_MODULES (_MSC_VER >= 1930)
#else
    #define COMPILER_UNKNOWN
    #define COMPILER_NAME "Unknown"
    #define COMPILER_VERSION 0
    #define SUPPORTS_CONCEPTS 0
    #define SUPPORTS_RANGES 0
    #define SUPPORTS_COROUTINES 0
    #define SUPPORTS_MODULES 0
#endif

// C++ Standard version detection and feature availability.
#define CPP_STANDARD __cplusplus
#define HAS_CPP20 (CPP_STANDARD >= 202002L)
#define HAS_CPP23 (CPP_STANDARD >= 202302L)

// Feature test macros.
#ifdef __has_include
    #define HAS_HEADER(header) __has_include(header)
#else
    #define HAS_HEADER(header) 0
#endif

#ifdef __has_cpp_attribute
    #define HAS_ATTRIBUTE(attr) __has_cpp_attribute(attr)
#else
    #define HAS_ATTRIBUTE(attr) 0
#endif

//===----------------------------------------------------------------------===//
//===================== COMPILER-SPECIFIC OPTIMIZATIONS ======================//

// GCC optimizations with advanced features.
#ifdef COMPILER_GCC
    #pragma GCC optimize("Ofast,unroll-loops,fast-math,inline-functions,tree-vectorize")
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-result"
    #pragma GCC diagnostic ignored "-Wunused-variable"
    #pragma GCC diagnostic ignored "-Wunused-function"

    // Architecture-specific optimizations.
    #if defined(__x86_64__) || defined(__i386__)
        #pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt,sse4.2,fma,f16c")
    #elif defined(__aarch64__)
        #pragma GCC target("+simd,+crypto,+fp16,+sve")
    #elif defined(__arm__)
        #pragma GCC target("+neon,+vfp4")
    #endif

    // Branch prediction hints.
    #define LIKELY(x)   __builtin_expect(!!(x), 1)
    #define UNLIKELY(x) __builtin_expect(!!(x), 0)
    #define UNREACHABLE() __builtin_unreachable()

    // Function attributes.
    #define ALWAYS_INLINE __attribute__((always_inline)) inline
    #define NEVER_INLINE __attribute__((noinline))
    #define PURE_FUNCTION __attribute__((const))
    #define HOT_FUNCTION __attribute__((hot))
    #define COLD_FUNCTION __attribute__((cold))

#elif defined(COMPILER_CLANG)
    #pragma clang optimize on
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-result"
    #pragma clang diagnostic ignored "-Wunused-variable"
    #pragma clang diagnostic ignored "-Wunused-function"

    // Branch prediction hints.
    #define LIKELY(x)   __builtin_expect(!!(x), 1)
    #define UNLIKELY(x) __builtin_expect(!!(x), 0)
    #define UNREACHABLE() __builtin_unreachable()
    #define ALWAYS_INLINE __attribute__((always_inline)) inline
    #define NEVER_INLINE __attribute__((noinline))
    #define PURE_FUNCTION __attribute__((const))
    #define HOT_FUNCTION __attribute__((hot))
    #define COLD_FUNCTION __attribute__((cold))

    // Architecture-specific optimizations for Clang.
    #if defined(__x86_64__) || defined(__i386__)
        #define CLANG_TARGET_X86
    #elif defined(__aarch64__)
        #define CLANG_TARGET_ARM64
    #endif

    // Architecture-specific function attributes.
    #ifdef CLANG_TARGET_X86
        #define TARGET_OPTIMIZED __attribute__((target("avx2,bmi,bmi2,lzcnt,popcnt,sse4.2,fma")))
    #elif defined(CLANG_TARGET_ARM64)
        #define TARGET_OPTIMIZED __attribute__((target("+simd,+crypto,+fp16")))
    #else
        #define TARGET_OPTIMIZED
    #endif

#elif defined(COMPILER_MSVC)
    #pragma optimize("gt", on)
    #include <intrin.h>

    // Branch prediction hints.
    #define LIKELY(x)   (x)
    #define UNLIKELY(x) (x)
    #define UNREACHABLE() __assume(0)
    #define ALWAYS_INLINE __forceinline
    #define NEVER_INLINE __declspec(noinline)
    #define PURE_FUNCTION
    #define HOT_FUNCTION
    #define COLD_FUNCTION

#else
    // Fallback definitions for unknown compilers.
    #define LIKELY(x) (x)
    #define UNLIKELY(x) (x)
    #define UNREACHABLE()
    #define ALWAYS_INLINE inline
    #define NEVER_INLINE
    #define PURE_FUNCTION
    #define HOT_FUNCTION
    #define COLD_FUNCTION
#endif

//===----------------------------------------------------------------------===//
//====================== COMPREHENSIVE HEADER INCLUDES =======================//

// Core C++ headers - always available.
#include <iostream>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <streambuf>

// Container headers.
#include <vector>
#include <array>
#include <deque>
#include <list>
#include <forward_list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <bitset>

// Algorithm and iterator headers.
#include <algorithm>
#include <numeric>
#include <iterator>
#include <functional>

// String and localization.
#include <string>
#include <string_view>
#include <regex>
#include <locale>

// Memory management.
#include <memory>
#include <new>
#include <scoped_allocator>

// Utilities.
#include <utility>
#include <tuple>
#include <initializer_list>
#include <type_traits>
#include <typeinfo>
#include <typeindex>
#include <limits>
#include <climits>
#include <cfloat>

// Error handling.
#include <exception>
#include <stdexcept>
#include <system_error>
#include <cassert>

// Math and numerics
#include <cmath>
#include <complex>
#include <valarray>
#include <random>
#include <ratio>
#include <cinttypes>
#include <cstdint>

// Time utilities.
#include <chrono>
#include <ctime>

// C standard library.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cerrno>
#include <cfenv>
#include <clocale>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cwchar>
#include <cwctype>

// C++17 features.
#if HAS_CPP20
    #if HAS_HEADER(<optional>)
        #include <optional>
        #define HAS_OPTIONAL 1
    #endif
    #if HAS_HEADER(<variant>)
        #include <variant>
        #define HAS_VARIANT 1
    #endif
    #if HAS_HEADER(<any>)
        #include <any>
        #define HAS_ANY 1
    #endif
    #if HAS_HEADER(<filesystem>)
        #include <filesystem>
        #define HAS_FILESYSTEM 1
    #endif
    #if HAS_HEADER(<execution>)
        #include <execution>
        #define HAS_EXECUTION 1
    #endif
#endif

// C++20 features.
#if HAS_CPP20
    #if HAS_HEADER(<concepts>) && SUPPORTS_CONCEPTS
        #include <concepts>
        #define HAS_CONCEPTS 1
    #endif
    #if HAS_HEADER(<ranges>) && SUPPORTS_RANGES
        #include <ranges>
        #define HAS_RANGES 1
    #endif
    #if HAS_HEADER(<coroutine>) && SUPPORTS_COROUTINES
        #include <coroutine>
        #define HAS_COROUTINES 1
    #endif
    #if HAS_HEADER(<compare>)
        #include <compare>
        #define HAS_COMPARE 1
    #endif
    #if HAS_HEADER(<span>)
        #include <span>
        #define HAS_SPAN 1
    #endif
    #if __has_include(<source_location>) && !defined(HAS_SOURCE_LOCATION)
        #include <source_location>
        #define HAS_SOURCE_LOCATION 1
    #endif
    #if HAS_HEADER(<version>)
        #include <version>
        #define HAS_VERSION_HEADER 1
    #endif
    #if HAS_HEADER(<bit>)
        #include <bit>
        #define HAS_BIT_HEADER 1
    #endif
    #if HAS_HEADER(<numbers>)
        #include <numbers>
        #define HAS_NUMBERS 1
    #endif

    // Thread support (conditional for competitive programming).
    #ifdef ENABLE_THREADING
        #if HAS_HEADER(<atomic>)
            #include <atomic>
        #endif
        #if HAS_HEADER(<thread>)
            #include <thread>
        #endif
        #if HAS_HEADER(<mutex>)
            #include <mutex>
        #endif
        #if HAS_HEADER(<condition_variable>)
            #include <condition_variable>
        #endif
        #if HAS_HEADER(<future>)
            #include <future>
        #endif
        #if HAS_HEADER(<shared_mutex>)
            #include <shared_mutex>
        #endif
        #if HAS_HEADER(<semaphore>)
            #include <semaphore>
            #define HAS_SEMAPHORE 1
        #endif
        #if HAS_HEADER(<latch>)
            #include <latch>
            #define HAS_LATCH 1
        #endif
        #if HAS_HEADER(<barrier>)
            #include <barrier>
            #define HAS_BARRIER 1
        #endif
        #if HAS_HEADER(<stop_token>)
            #include <stop_token>
            #define HAS_STOP_TOKEN 1
        #endif
    #endif
#endif

// C++23 features.
#if HAS_CPP23
    #if HAS_HEADER(<format>)
        #include <format>
        #define HAS_FORMAT 1
    #endif
    #if HAS_HEADER(<print>)
        #include <print>
        #define HAS_PRINT 1
    #endif
    #if HAS_HEADER(<expected>)
        #include <expected>
        #define HAS_EXPECTED 1
    #endif
    #if HAS_HEADER(<flat_map>)
        #include <flat_map>
        #define HAS_FLAT_MAP 1
    #endif
    #if HAS_HEADER(<flat_set>)
        #include <flat_set>
        #define HAS_FLAT_SET 1
    #endif
    #if HAS_HEADER(<mdspan>)
        #include <mdspan>
        #define HAS_MDSPAN 1
    #endif
    #if HAS_HEADER(<generator>)
        #include <generator>
        #define HAS_GENERATOR 1
    #endif
    #if HAS_HEADER(<spanstream>)
        #include <spanstream>
        #define HAS_SPANSTREAM 1
    #endif
    #if HAS_HEADER(<stacktrace>)
        #include <stacktrace>
        #define HAS_STACKTRACE 1
    #endif
    #if HAS_HEADER(<stdfloat>)
        #include <stdfloat>
        #define HAS_STDFLOAT 1
    #endif
#endif

//===----------------------------------------------------------------------===//
//==================== COMPETITIVE PROGRAMMING UTILITIES =====================//

// Fast I/O optimization.
#ifndef FAST_IO_SETUP
    #define FAST_IO_SETUP() \
        std::ios_base::sync_with_stdio(false); \
        std::cin.tie(nullptr); \
        std::cout.tie(nullptr); \
        std::cout << std::fixed << std::setprecision(10)
#endif

// Debug mode detection.
#ifdef LOCAL
    #define DEBUG_MODE 1
    #define DEBUG_BUILD
#else
    #define DEBUG_MODE 0
    #define RELEASE_BUILD
#endif

// Sanitizer detection for better debugging.
#if defined(__has_feature)
    #if __has_feature(address_sanitizer)
        #define HAS_ASAN 1
    #endif
    #if __has_feature(undefined_behavior_sanitizer)
        #define HAS_UBSAN 1
    #endif
    #if __has_feature(memory_sanitizer)
        #define HAS_MSAN 1
    #endif
    #if __has_feature(thread_sanitizer)
        #define HAS_TSAN 1
    #endif
#endif

#ifdef __SANITIZE_ADDRESS__
    #define HAS_ASAN 1
#endif

//===----------------------------------------------------------------------===//
//================== ENHANCED COMPETITIVE PROGRAMMING TYPES ==================//

#ifndef __TYPES__
#define __TYPES__
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
  using LL  = I64;
  using ULL = U64;
  using LD  = F80;

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
  using VI   = VC<I32>;
  using VVI  = VVC<I32>;
  using VVVI = VVVC<I32>;
  using VL   = VC<I64>;
  using VVL  = VVC<I64>;
  using VVVL = VVVC<I64>;
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
#endif

//===----------------------------------------------------------------------===//
//========================== MATHEMATICAL CONSTANTS ==========================//

#ifndef __CONSTANTS__
#define __CONSTANTS__
  // High precision mathematical constants.
  #ifdef HAS_NUMBERS
    using std::numbers::pi_v;
    using std::numbers::e_v;
    using std::numbers::log2e_v;
    using std::numbers::log10e_v;
    using std::numbers::ln2_v;
    using std::numbers::ln10_v;
    using std::numbers::sqrt2_v;
    using std::numbers::sqrt3_v;
    using std::numbers::inv_sqrt3_v;
    using std::numbers::egamma_v;
    using std::numbers::phi_v;

    template<class T = F80>
    constexpr T PI   = pi_v<T>;
    template<class T = F80>
    constexpr T E    = e_v<T>;
    template<class T = F80>
    constexpr T PHI  = phi_v<T>;
  #else
    // Fallback constants with high precision.
    template<class T = F80>
    constexpr T PI   = static_cast<T>(3.1415926535897932384626433832795028841971693993751L);
    template<class T = F80>
    constexpr T E    = static_cast<T>(2.7182818284590452353602874713526624977572470937000L);
    template<class T = F80>
    constexpr T PHI  = static_cast<T>(1.6180339887498948482045868343656381177203091798058L);
  #endif
    // Epsilon values for floating point comparisons.
    template<class T = F80>
    constexpr T EPS  = static_cast<T>(1e-9L);
    template<class T = F80>
    constexpr T DEPS = static_cast<T>(1e-12L);

    // Specialized infinity values.
    constexpr I32 INF32  = 1'010'000'000;
    constexpr I64 INF64  = 2'020'000'000'000'000'000LL;
    constexpr U32 UINF32 = 2'020'000'000U;
    constexpr U64 UINF64 = 4'040'000'000'000'000'000ULL;

    #if HAS_INT128
        constexpr I128 INF128 = static_cast<I128>(INF64) * 2'000'000'000'000'000'000LL;
    #endif

    // Infinity values with overflow protection.
    template<class T>
    constexpr T INF = std::numeric_limits<T>::max() / 4;

    // Legacy aliases.
    constexpr I64 LINF = INF64;

    // Modular arithmetic constants.
    constexpr I64 MOD  = 1000000007;
    constexpr I64 MOD2 = 998244353;
    constexpr I64 MOD3 = 1000000009;
#endif

//===----------------------------------------------------------------------===//
//================== ENHANCED UTILITY FUNCTIONS AND MACROS ===================//

#ifndef __UTILITY_FUNCTIONS__
#define __UTILITY_FUNCTIONS__
  // Bitwise operations with modern implementations.
  template<std::integral T>
  ALWAYS_INLINE constexpr I32 popcount(T x) noexcept {
      #ifdef HAS_BIT_HEADER
          return std::popcount(x);
      #else
          if constexpr (sizeof(T) <= sizeof(U32)) {
              return __builtin_popcount(static_cast<U32>(x));
          } else {
              return __builtin_popcountll(static_cast<U64>(x));
          }
      #endif
  }

  template<std::integral T>
  ALWAYS_INLINE constexpr I32 countl_zero(T x) noexcept {
      #ifdef HAS_BIT_HEADER
          return std::countl_zero(x);
      #else
          if (x == 0) return sizeof(T) * 8;
          if constexpr (sizeof(T) <= sizeof(U32)) {
              return __builtin_clz(static_cast<U32>(x)) - (32 - sizeof(T) * 8);
          } else {
              return __builtin_clzll(static_cast<U64>(x)) - (64 - sizeof(T) * 8);
          }
      #endif
  }

  template<std::integral T>
  ALWAYS_INLINE constexpr I32 countr_zero(T x) noexcept {
      #ifdef HAS_BIT_HEADER
          return std::countr_zero(x);
      #else
          if (x == 0) return sizeof(T) * 8;
          if constexpr (sizeof(T) <= sizeof(U32)) {
              return __builtin_ctz(static_cast<U32>(x));
          } else {
              return __builtin_ctzll(static_cast<U64>(x));
          }
      #endif
  }

  template<std::integral T>
  ALWAYS_INLINE constexpr I32 bit_width(T x) noexcept {
      #ifdef HAS_BIT_HEADER
          return std::bit_width(x);
      #else
          return sizeof(T) * 8 - countl_zero(x);
      #endif
  }

  // Legacy aliases for compatibility.
  template<std::integral T> constexpr I32 popcnt(T x) { return popcount(x); }
  template<std::integral T> constexpr I32 topbit(T x) { return bit_width(x) - 1; }
  template<std::integral T> constexpr I32 lowbit(T x) { return countr_zero(x); }

  // Mathematical utility functions.
  template<std::integral T>
  ALWAYS_INLINE constexpr T gcd(T a, T b) noexcept {
      #if HAS_CPP20
          return std::gcd(a, b);
      #else
          while (b != 0) {
              T temp = b;
              b = a % b;
              a = temp;
          }
          return a;
      #endif
  }

  template<std::integral T>
  ALWAYS_INLINE constexpr T lcm(T a, T b) noexcept {
      #if HAS_CPP20
          return std::lcm(a, b);
      #else
          return a / gcd(a, b) * b;
      #endif
  }

  // Fast power function with modular arithmetic support.
  template<std::integral T>
  ALWAYS_INLINE constexpr T power(T base, T exp, T mod = 0) noexcept {
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

  // Min/Max functions with variadic support.
  template<class T>
  ALWAYS_INLINE constexpr const T& cp_min(const T& a, const T& b) noexcept {
      return (b < a) ? b : a;
  }

  template<class T>
  ALWAYS_INLINE constexpr const T& cp_max(const T& a, const T& b) noexcept {
      return (a < b) ? b : a;
  }

  template<class T, class... Args>
  ALWAYS_INLINE constexpr const T& cp_min(const T& a, const T& b, const Args&... args) noexcept {
      return cp_min(a, cp_min(b, args...));
  }

  template<class T, class... Args>
  ALWAYS_INLINE constexpr const T& cp_max(const T& a, const T& b, const Args&... args) noexcept {
      return cp_max(a, cp_max(b, args...));
  }

  // Efficient min/max update functions.
  template<class T, class S>
  ALWAYS_INLINE constexpr bool chmin(T& a, const S& b) noexcept {
      return (a > b) ? (a = b, true) : false;
  }

  template<class T, class S>
  ALWAYS_INLINE constexpr bool chmax(T& a, const S& b) noexcept {
      return (a < b) ? (a = b, true) : false;
  }
#endif

//===----------------------------------------------------------------------===//
//======================== ADVANCED HASHING UTILITIES ========================//

// Modern hash combiners for custom types.
template<class T>
ALWAYS_INLINE void hash_combine(std::size_t& seed, const T& v) noexcept {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

// Enhanced pair hash.
struct PairHash {
    template<class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const noexcept {
        std::size_t h1 = std::hash<T1>{}(p.first);
        std::size_t h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1) ^ (h2 >> 1);
    }
};

// Tuple hash implementation.
template<class Tuple, std::size_t Index = std::tuple_size_v<Tuple> - 1>
struct TupleHashImpl {
    static void apply(std::size_t& seed, const Tuple& tuple) {
        TupleHashImpl<Tuple, Index - 1>::apply(seed, tuple);
        hash_combine(seed, std::get<Index>(tuple));
    }
};

template<class Tuple>
struct TupleHashImpl<Tuple, 0> {
    static void apply(std::size_t& seed, const Tuple& tuple) {
        hash_combine(seed, std::get<0>(tuple));
    }
};

template<typename... T>
struct TupleHash {
    std::size_t operator()(const std::tuple<T...>& t) const noexcept {
        std::size_t seed = 0;
        TupleHashImpl<std::tuple<T...>>::apply(seed, t);
        return seed;
    }
};

//===----------------------------------------------------------------------===//
//===================== RANDOM NUMBER GENERATION SYSTEM ======================//

#ifndef DISABLE_RANDOM_SYSTEM
// Thread-safe random number generation.
class RandomEngine {
private:
    mutable std::mt19937 gen32;
    mutable std::mt19937_64 gen64;

public:
    RandomEngine() : gen32(std::chrono::steady_clock::now().time_since_epoch().count()),
                     gen64(std::chrono::steady_clock::now().time_since_epoch().count()) {}

    template<std::integral T>
    T randint(T min_val, T max_val) const {
        if constexpr (sizeof(T) <= 4) {
            return std::uniform_int_distribution<T>(min_val, max_val)(gen32);
        } else {
            return std::uniform_int_distribution<T>(min_val, max_val)(gen64);
        }
    }

    template<std::floating_point T>
    T randreal(T min_val, T max_val) const {
        return std::uniform_real_distribution<T>(min_val, max_val)(gen32);
    }

    bool randbool() const {
        return std::bernoulli_distribution(0.5)(gen32);
    }

    void seed(U64 s) {
        gen32.seed(static_cast<U32>(s));
        gen64.seed(s);
    }
};

// Global random engine instance.
inline RandomEngine& random_engine() {
    static RandomEngine engine;
    return engine;
}

// Convenient random functions.
template<std::integral T>
T randint(T min_val, T max_val) {
    return random_engine().randint(min_val, max_val);
}

template<std::floating_point T>
T randreal(T min_val = T(0), T max_val = T(1)) {
    return random_engine().randreal(min_val, max_val);
}

inline bool randbool() {
    return random_engine().randbool();
}

inline void set_random_seed(U64 seed) {
    random_engine().seed(seed);
}
#endif

//===----------------------------------------------------------------------===//
//===================== AUTOMATIC I/O OPTIMIZATION SETUP =====================//

// RAII I/O optimizer.
struct IOOptimizer {
    IOOptimizer() {
        FAST_IO_SETUP();

        #ifdef ONLINE_JUDGE
            // Additional optimizations for online judges.
            std::cin.rdbuf()->pubsetbuf(nullptr, 0);
            std::cout.rdbuf()->pubsetbuf(nullptr, 0);
        #endif

        #ifdef HAS_ASAN
            // Disable I/O optimizations under AddressSanitizer for better debugging.
            std::ios_base::sync_with_stdio(true);
        #endif
    }

    ~IOOptimizer() {
        std::cout.flush();
        std::cerr.flush();
    }
};

// Global instance for automatic setup.
#ifndef DISABLE_AUTO_IO_OPTIMIZATION
    [[maybe_unused]] static IOOptimizer io_optimizer_instance;
#endif

//===----------------------------------------------------------------------===//
//============================= NAMESPACE USAGE ==============================//

// Standard namespace usage for competitive programming.
using namespace std;

// Conditional namespace usage for C++20/23 features.
#ifdef HAS_RANGES
    namespace ranges = std::ranges;
    namespace views = std::ranges::views;
#endif

#ifdef HAS_CONCEPTS
    // Import useful concepts.
    using std::integral;
    using std::floating_point;
    using std::signed_integral;
    using std::unsigned_integral;
#endif

//===----------------------------------------------------------------------===//
//===================== FEATURE SUMMARY AND DIAGNOSTICS ======================//

// Compilation diagnostics in debug mode.
#if DEBUG_MODE
    namespace {
        struct CompilationDiagnostics {
            CompilationDiagnostics() {
                #ifdef SHOW_COMPILATION_INFO
                    std::cerr << "╭─── Modern PCH Compilation Info ───╮\n";
                    std::cerr << "│ Compiler: " << COMPILER_NAME;
                    #ifdef COMPILER_VERSION
                        std::cerr << " (version " << COMPILER_VERSION << ")";
                    #endif
                    std::cerr << "\n";
                    std::cerr << "│ C++ Standard: " << CPP_STANDARD << "\n";
                    std::cerr << "│ Features Available:\n";
                    std::cerr << "│   • Concepts: " << (HAS_CONCEPTS ? "✓" : "✗") << "\n";
                    std::cerr << "│   • Ranges: " << (HAS_RANGES ? "✓" : "✗") << "\n";
                    std::cerr << "│   • Coroutines: " << (HAS_COROUTINES ? "✓" : "✗") << "\n";
                    std::cerr << "│   • Format: " << (HAS_FORMAT ? "✓" : "✗") << "\n";
                    std::cerr << "│   • Print: " << (HAS_PRINT ? "✓" : "✗") << "\n";
                    std::cerr << "│   • int128: " << (HAS_INT128 ? "✓" : "✗") << "\n";
                    std::cerr << "│   • float128: " << (HAS_FLOAT128 ? "✓" : "✗") << "\n";
                    #ifdef HAS_ASAN
                        std::cerr << "│ ⚠️  AddressSanitizer: ENABLED\n";
                    #endif
                    #ifdef HAS_UBSAN
                        std::cerr << "│ ⚠️  UBSanitizer: ENABLED\n";
                    #endif
                    std::cerr << "╰───────────────────────────────────╯\n";
                #endif
            }
        } diagnostics_instance;
    }
#endif

// Restore compiler diagnostics.
#ifdef COMPILER_GCC
    #pragma GCC diagnostic pop
#elif defined(COMPILER_CLANG)
    #pragma clang diagnostic pop
#endif

#endif // MODERN_PCH_H

//===----------------------------------------------------------------------===//
