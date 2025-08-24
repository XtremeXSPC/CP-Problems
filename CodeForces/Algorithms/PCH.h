//===---------------------------------------------------------------------===//
// PCH.h - Pre-compiled Header for Clang/MSVC compatibility
// Enhanced version that emulates <bits/stdc++.h> for competitive programming
// Compatible with GCC, Clang, MSVC, and other modern C++ compilers
// Optimized for use with sanitizers when GCC's sanitizer libraries are unavailable

// clang-format off
#ifndef PCH_H
#define PCH_H

// Prevent inclusion of bits/stdc++.h when using this header
#ifndef _GLIBCXX_NO_ASSERT
    #define _GLIBCXX_NO_ASSERT
#endif
#define _BITS_STDCPP_H 1

// Compiler detection and version checks
#if defined(__GNUC__) && !defined(__clang__)
    #ifndef COMPILER_GCC
        #define COMPILER_GCC
    #endif
    #ifndef COMPILER_NAME
        #define COMPILER_NAME "GCC"
    #endif
    #if __GNUC__ < 9
        #error "GCC 9 or later required"
    #endif
#elif defined(__clang__)
    #ifndef COMPILER_CLANG
        #define COMPILER_CLANG
    #endif
    #if defined(__apple_build_version__)
        #ifndef COMPILER_APPLE_CLANG
            #define COMPILER_APPLE_CLANG
        #endif
        #ifndef COMPILER_NAME
            #define COMPILER_NAME "Apple Clang"
        #endif
    #else
        #ifndef COMPILER_NAME
            #define COMPILER_NAME "LLVM Clang"
        #endif
    #endif
    #if __clang_major__ < 10
        #error "Clang 10 or later required"
    #endif
#elif defined(_MSC_VER)
    #ifndef COMPILER_MSVC
        #define COMPILER_MSVC
    #endif
    #ifndef COMPILER_NAME
        #define COMPILER_NAME "MSVC"
    #endif
    #if _MSC_VER < 1920
        #error "Visual Studio 2019 or later required"
    #endif
#else
    #ifndef COMPILER_UNKNOWN
        #define COMPILER_UNKNOWN
    #endif
    #ifndef COMPILER_NAME
        #define COMPILER_NAME "Unknown"
    #endif
#endif

// Ensure COMPILER_NAME is always defined
#ifndef COMPILER_NAME
    #define COMPILER_NAME "Unknown"
#endif

// C++ Standard version check - relaxed to C++20 for broader compatibility
#if __cplusplus < 202002L && !defined(__clang__)
    #pragma message("C++20 or later recommended for full feature support")
#endif
// clang-format on

//===---------------------------------------------------------------------===//
// C++ STANDARD LIBRARY HEADERS
//===---------------------------------------------------------------------===//
// clang-format off

// Input/Output Stream Library
#include <fstream>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <streambuf>

// Containers Library
#include <array>
#include <bitset>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Iterator Library
#include <iterator>

// Algorithm Library
#include <algorithm>
#include <numeric>

// C++17 Parallel algorithms (if available)
#if __cplusplus >= 201703L && __has_include(<execution>)
    #include <execution>
#endif

// Function Objects
#include <functional>

// String Library
#include <regex>
#include <string>
#include <string_view>

// Localization Library (conditionally include codecvt as it's deprecated in C++17)
#include <locale>
#if __cplusplus < 201703L
    #include <codecvt>
#endif

// Error Handling
#include <exception>
#include <stdexcept>
#include <system_error>

// Memory Management
#include <memory>
#include <new>
#include <scoped_allocator>
#if __cplusplus >= 201703L && __has_include(<memory_resource>)
    #include <memory_resource>
#endif

// Metaprogramming Utilities
#include <initializer_list>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <utility>

// C++17 features
#if __cplusplus >= 201703L
    #include <any>
    #include <optional>
    #include <variant>
    #if __has_include(<filesystem>)
        #include <filesystem>
    #endif
#endif

// Numerics Library
#include <cfloat>
#include <climits>
#include <cmath>
#include <complex>
#include <limits>
#include <numeric>
#include <random>
#include <ratio>
#include <valarray>

// Time Library
#include <chrono>
#include <ctime>

// Thread Support Library (conditionally for competitive programming)
#ifdef ENABLE_THREADING
    #include <thread>
    #include <mutex>
    #include <shared_mutex>
    #include <condition_variable>
    #include <atomic>
    #include <future>
#endif

// C++20 and later features
#if __cplusplus >= 202002L
    #ifdef __has_include
        #if __has_include(<semaphore>)
            #include <semaphore>
        #endif
        #if __has_include(<barrier>)
            #include <barrier>
        #endif
        #if __has_include(<latch>)
            #include <latch>
        #endif
        #if __has_include(<stop_token>)
            #include <stop_token>
        #endif
        #if __has_include(<source_location>)
            #include <source_location>
        #endif
        #if __has_include(<span>)
            #include <span>
        #endif
        #if __has_include(<ranges>)
            #include <ranges>
        #endif
        #if __has_include(<concepts>)
            #include <concepts>
        #endif
        #if __has_include(<coroutine>)
            #include <coroutine>
        #endif
        #if __has_include(<compare>)
            #include <compare>
        #endif
        #if __has_include(<version>)
            #include <version>
        #endif
        #if __has_include(<format>)
            #include <format>
        #endif
        #if __has_include(<numbers>)
            #include <numbers>
        #endif
        #if __has_include(<bit>)
            #include <bit>
        #endif
    #endif
#endif

// C++23 features
#if __cplusplus >= 202302L
    #ifdef __has_include
        #if __has_include(<expected>)
            #include <expected>
        #endif
        #if __has_include(<flat_map>)
            #include <flat_map>
        #endif
        #if __has_include(<flat_set>)
            #include <flat_set>
        #endif
        #if __has_include(<generator>)
            #include <generator>
        #endif
        #if __has_include(<mdspan>)
            #include <mdspan>
        #endif
        #if __has_include(<print>)
            #include <print>
        #endif
        #if __has_include(<spanstream>)
            #include <spanstream>
        #endif
        #if __has_include(<stacktrace>)
            #include <stacktrace>
        #endif
        #if __has_include(<stdfloat>)
            #include <stdfloat>
        #endif
    #endif
#endif

// C Standard Library Headers
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfenv>
#include <cfloat>
#include <cinttypes>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cuchar>
#include <cwchar>
#include <cwctype>

//===---------------------------------------------------------------------===//
// COMPETITIVE PROGRAMMING OPTIMIZATIONS AND UTILITIES
//===---------------------------------------------------------------------===//

// Fast I/O optimization
#ifndef FAST_IO
    #ifdef ONLINE_JUDGE
        #define FAST_IO ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr)
    #else
        #define FAST_IO ios_base::sync_with_stdio(false); cin.tie(nullptr)
    #endif
#endif

// Debug mode detection
#ifdef LOCAL
    #define DEBUG_MODE 1
#else
    #define DEBUG_MODE 0
#endif

// Sanitizer detection
#if defined(__has_feature)
    #if __has_feature(address_sanitizer)
        #define HAS_ASAN 1
    #endif
    #if __has_feature(undefined_behavior_sanitizer)
        #define HAS_UBSAN 1
    #endif
#endif

#ifdef __SANITIZE_ADDRESS__
    #define HAS_ASAN 1
#endif

// clang-format on

/*
// Type aliases
using ll   = long long;
using ull  = unsigned long long;
using ld   = long double;
using pii  = std::pair<int, int>;
using pll  = std::pair<long long, long long>;
using vi   = std::vector<int>;
using vll  = std::vector<long long>;
using vvi  = std::vector<std::vector<int>>;
using vvll = std::vector<std::vector<long long>>;
using vs   = std::vector<std::string>;
using vpii = std::vector<std::pair<int, int>>;
using vpll = std::vector<std::pair<long long, long long>>;

// Mathematical constants
constexpr long double PI   = 3.141592653589793238462643383279502884L;
constexpr long double E    = 2.718281828459045235360287471352662498L;
constexpr long double EPS  = 1e-9L;
constexpr int         INF  = 0x3f3f3f3f;
constexpr long long   LINF = 0x3f3f3f3f3f3f3f3fLL;
constexpr int         MOD  = 1000000007;
constexpr int         MOD2 = 998244353;
*/

// Utility functions
template <typename T>
inline T gcd(T a, T b) {
  return b ? gcd(b, a % b) : a;
}

template <typename T>
inline T lcm(T a, T b) {
  return a / gcd(a, b) * b;
}

template <typename T>
inline T power(T a, T b, T mod = 0) {
  T result = 1;
  if (mod)
    a %= mod;
  while (b > 0) {
    if (b & 1)
      result = (mod ? (result * a) % mod : result * a);
    a = (mod ? (a * a) % mod : a * a);
    b >>= 1;
  }
  return result;
}

// Min/Max variadic functions
template <typename T>
inline T min(T a, T b) {
  return (a < b) ? a : b;
}

template <typename T>
inline T max(T a, T b) {
  return (a > b) ? a : b;
}

template <typename T, typename... Args>
inline T min(T a, T b, Args... args) {
  return min(a, min(b, args...));
}

template <typename T, typename... Args>
inline T max(T a, T b, Args... args) {
  return max(a, max(b, args...));
}

// clang-format off

//===---------------------------------------------------------------------===//
// NAMESPACE USAGE
//===---------------------------------------------------------------------===//

using namespace std;

// Additional useful namespaces for competitive programming
namespace chrono_literals = std::chrono_literals;

//===---------------------------------------------------------------------===//
// COMPILER-SPECIFIC OPTIMIZATIONS
//===---------------------------------------------------------------------===//

// GCC specific optimizations
#if defined(COMPILER_GCC) && defined(__GNUC__) && !defined(__clang__)
  #pragma GCC optimize("O3,unroll-loops")
  // Only enable these for x86 architectures:
  #if defined(__x86_64__) || defined(__i386__)
    #pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
  #elif defined(__aarch64__) || defined(__arm__)
    #pragma GCC target("neon,+simd")
  #endif
  #define likely(x)   __builtin_expect(!!(x), 1)
  #define unlikely(x) __builtin_expect(!!(x), 0)
  #define __builtin_popcount __builtin_popcount
  #define __builtin_popcountll __builtin_popcountll
#endif

// Clang specific optimizations  
#if defined(COMPILER_CLANG) && defined(__clang__)
  #pragma clang optimize on
  #define likely(x)   __builtin_expect(!!(x), 1)
  #define unlikely(x) __builtin_expect(!!(x), 0)
  
  // Ensure builtin functions are available
  #ifndef __builtin_popcount
    #define __builtin_popcount __builtin_popcount
  #endif
  #ifndef __builtin_popcountll
    #define __builtin_popcountll __builtin_popcountll
  #endif
#endif

// MSVC specific optimizations
#if defined(COMPILER_MSVC) && defined(_MSC_VER)
  #pragma optimize("gt", on)
  #define likely(x)   (x)
  #define unlikely(x) (x)
  #include <intrin.h>
  #define __builtin_popcount __popcnt
  #define __builtin_popcountll __popcnt64
#endif

// Default definitions if not provided by any compiler
#ifndef likely
  #define likely(x) (x)
#endif
#ifndef unlikely
  #define unlikely(x) (x)
#endif

// Fallback for popcount if not available
#ifndef __builtin_popcount
  #define __builtin_popcount(x) std::bitset<32>(x).count()
#endif
#ifndef __builtin_popcountll
  #define __builtin_popcountll(x) std::bitset<64>(x).count()
#endif

//===---------------------------------------------------------------------===//
// ADDITIONAL COMPETITIVE PROGRAMMING UTILITIES
//===---------------------------------------------------------------------===//

// Custom hash for pairs (useful for unordered_map<pair<int,int>>)
struct pair_hash {
    template<class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        // Better hash combination to reduce collisions
        return h1 ^ (h2 << 1) ^ (h2 >> 1);
    }
};

// Custom hash for tuples
template<class T>
inline void hash_combine(std::size_t& seed, const T& v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
struct tuple_hash_impl {
    static void apply(size_t& seed, const Tuple& tuple) {
        tuple_hash_impl<Tuple, Index - 1>::apply(seed, tuple);
        hash_combine(seed, std::get<Index>(tuple));
    }
};

template<class Tuple>
struct tuple_hash_impl<Tuple, 0> {
    static void apply(size_t& seed, const Tuple& tuple) {
        hash_combine(seed, std::get<0>(tuple));
    }
};

template<typename... T>
struct tuple_hash {
    size_t operator()(const std::tuple<T...>& t) const {
        size_t seed = 0;
        tuple_hash_impl<std::tuple<T...>>::apply(seed, t);
        return seed;
    }
};

// Fast input/output for large datasets
#ifndef FAST_IO_INSTANCE_DEFINED
    #define FAST_IO_INSTANCE_DEFINED
    struct FastIO {
        FastIO() {
            std::ios_base::sync_with_stdio(false);
            std::cin.tie(nullptr);
            std::cout.tie(nullptr);
            
            // Set fixed precision for floating point output
            std::cout << std::fixed << std::setprecision(10);
            
            // Increase buffer size for better performance
            #ifdef COMPILER_GCC
                std::cin.rdbuf()->pubsetbuf(nullptr, 1 << 20);
                std::cout.rdbuf()->pubsetbuf(nullptr, 1 << 20);
            #endif
        }
    } fast_io_instance;
#endif

// Random number generator
#ifndef RNG_DEFINED
    #define RNG_DEFINED
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::mt19937_64 rng64(std::chrono::steady_clock::now().time_since_epoch().count());
    
    template<typename T>
    T random(T l, T r) {
        if constexpr (sizeof(T) <= 4) {
            return std::uniform_int_distribution<T>(l, r)(rng);
        } else {
            return std::uniform_int_distribution<T>(l, r)(rng64);
        }
    }
    
    template<>
    double random<double>(double l, double r) {
        return std::uniform_real_distribution<double>(l, r)(rng);
    }
    
    template<>
    float random<float>(float l, float r) {
        return std::uniform_real_distribution<float>(l, r)(rng);
    }
#endif

// Sanitizer-friendly assertions
#ifdef HAS_ASAN
    #define SANITIZER_ASSERT(x) assert(x)
#else
    #define SANITIZER_ASSERT(x) ((void)0)
#endif

// Print diagnostic information about the compilation environment
#if DEBUG_MODE
    namespace {
        struct CompilationInfo {
            CompilationInfo() {
                std::cerr << "/===------ Compilation Environment ------===/" << std::endl;
                std::cerr << "Compiler: " << COMPILER_NAME << std::endl;
                std::cerr << "C++ Standard: " << __cplusplus << std::endl;
                #ifdef HAS_ASAN
                    std::cerr << "AddressSanitizer: ENABLED" << std::endl;
                #endif
                #ifdef HAS_UBSAN
                    std::cerr << "UBSanitizer: ENABLED" << std::endl;
                #endif
                std::cerr << "/===-------------------------------------===/" << std::endl;
            }
        };
        #ifdef SHOW_COMPILATION_INFO
            CompilationInfo compilation_info;
        #endif
    }
#endif

#endif // PCH_H

//===---------------------------------------------------------------------===//