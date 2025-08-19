//===---------------------------------------------------------------------===//
// PCH.h - Pre-compiled Header for Clang/MSVC compatibility
// Enhanced version that emulates <bits/stdc++.h> for competitive programming
// Compatible with GCC, Clang, MSVC, and other modern C++ compilers

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
    #define COMPILER_GCC
    #if __GNUC__ < 9
        #error "GCC 9 or later required"
    #endif
#elif defined(__clang__)
    #define COMPILER_CLANG
    #if __clang_major__ < 10
        #error "Clang 10 or later required"
    #endif
#elif defined(_MSC_VER)
    #define COMPILER_MSVC
    #if _MSC_VER < 1920
        #error "Visual Studio 2019 or later required"
    #endif
#endif

// C++ Standard version check
#if __cplusplus < 202302L
    #error "C++23 or later required"
#endif
// clang-format on

//===---------------------------------------------------------------------===//
// C++ STANDARD LIBRARY HEADERS
//===---------------------------------------------------------------------===//

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
#include <execution>
#include <numeric>

// Function Objects
#include <functional>

// String Library
#include <regex>
#include <string>
#include <string_view>

// Localization Library
#include <codecvt>
#include <locale>

// Input/Output Manipulators
#include <iomanip>

// Forward Declarations
#include <iosfwd>

// Error Handling
#include <exception>
#include <stdexcept>
#include <system_error>

// Memory Management
#include <memory>
#include <memory_resource>
#include <new>
#include <scoped_allocator>

// Metaprogramming Utilities
#include <any>
#include <initializer_list>
#include <optional>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <utility>
#include <variant>

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

// clang-format off
// Filesystem Library (C++17 and later)
#if __cplusplus >= 201703L
    #include <filesystem>
#endif
// clang-format om

// Thread Support Library
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <atomic>
#include <future>

// C++20 and later features
#if __cplusplus >= 202002L
    #include <semaphore>
    #include <barrier>
    #include <latch>
    #include <stop_token>
    #include <source_location>
    #include <span>
    #include <ranges>
    #include <concepts>
    #include <coroutine>
    #include <compare>
    #include <version>
    #include <format>
    #include <numbers>
    #include <bit>
#endif

// clang-format off
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
// clang-format on

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
// COMPETITIVE PROGRAMMING OPTIMIZATIONS
//===---------------------------------------------------------------------===//
// clang-format off
// Fast I/O optimization (only define if not already defined)
#ifndef FAST_IO
    #ifdef ONLINE_JUDGE
        #define FAST_IO ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr)
    #else
        #define FAST_IO ios_base::sync_with_stdio(false); cin.tie(nullptr)
    #endif
#endif
// clang-format on

// Common shortcuts for competitive programming
#define ll long long
#define ull unsigned long long
#define ld long double
#define pii pair<int, int>
#define pll pair<long long, long long>
#define vi vector<int>
#define vll vector<long long>
#define vvi vector<vector<int>>
#define vvll vector<vector<long long>>
#define vs vector<string>
#define vpii vector<pair<int, int>>
#define vpll vector<pair<long long, long long>>

// Loop shortcuts
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define RFOR(i, a, b) for (int i = (a); i >= (b); --i)
#define REP(i, n) for (int i = 0; i < (n); ++i)
#define RREP(i, n) for (int i = (n) - 1; i >= 0; --i)
#define FOREACH(x, v) for (auto& x : v)

// Container operations
#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define sz(x) ((int)(x).size())
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sort_unique(x)                                                                                                                     \
  sort(all(x));                                                                                                                            \
  (x).erase(unique(all(x)), (x).end())

// Mathematical constants and functions
const long double PI   = acos(-1.0L);
const long double EPS  = 1e-9L;
const int         INF  = 0x3f3f3f3f;
const long long   LINF = 0x3f3f3f3f3f3f3f3fLL;
const int         MOD  = 1000000007;
const int         MOD2 = 998244353;

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
  a %= mod;
  while (b > 0) {
    if (b & 1)
      result = (mod ? (result * a) % mod : result * a);
    a = (mod ? (a * a) % mod : a * a);
    b >>= 1;
  }
  return result;
}

// Min/Max functions
template <typename T>
inline T min(T a, T b) {
  return (a < b) ? a : b;
}
template <typename T>
inline T max(T a, T b) {
  return (a > b) ? a : b;
}
template <typename T>
inline T min(T a, T b, T c) {
  return min(a, min(b, c));
}
template <typename T>
inline T max(T a, T b, T c) {
  return max(a, max(b, c));
}

// clang-format off
// Debug macros (only define if not already defined)
#ifndef DEBUG_MACROS_DEFINED
    #define DEBUG_MACROS_DEFINED
    #ifndef ONLINE_JUDGE
        #ifndef debug
            #define debug(x) cerr << #x << " = " << (x) << endl
        #endif
        #ifndef debug2
            #define debug2(x, y) cerr << #x << " = " << (x) << ", " << #y << " = " << (y) << endl
        #endif
        #ifndef debug_vec
            #define debug_vec(v) { cerr << #v << " = ["; for(auto& x : v) cerr << x << " "; cerr << "]" << endl; }
        #endif
    #else
        #ifndef debug
            #define debug(x)
        #endif
        #ifndef debug2
            #define debug2(x, y)
        #endif
        #ifndef debug_vec
            #define debug_vec(v)
        #endif
    #endif
#endif

//===---------------------------------------------------------------------===//
// NAMESPACE USAGE
//===---------------------------------------------------------------------===//

using namespace std;

// Additional useful namespaces for competitive programming
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;

//===---------------------------------------------------------------------===//
// COMPILER-SPECIFIC OPTIMIZATIONS
//===---------------------------------------------------------------------===//

// GCC specific optimizations
#ifdef COMPILER_GCC
    #pragma GCC optimize("O3,unroll-loops")
    #pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
    #define likely(x)   __builtin_expect(!!(x), 1)
    #define unlikely(x) __builtin_expect(!!(x), 0)
#endif

// Clang specific optimizations  
#ifdef COMPILER_CLANG
    #pragma clang optimize on
    #define likely(x)   __builtin_expect(!!(x), 1)
    #define unlikely(x) __builtin_expect(!!(x), 0)
#endif

// MSVC specific optimizations
#ifdef COMPILER_MSVC
    #pragma optimize("gt", on)
    #define likely(x)   (x)
    #define unlikely(x) (x)
#endif

//===---------------------------------------------------------------------===//
// ADDITIONAL COMPETITIVE PROGRAMMING UTILITIES
//===---------------------------------------------------------------------===//

// Custom hash for pairs (useful for unordered_map<pair<int,int>>)
struct pair_hash {
    template<class T1, class T2>
    size_t operator () (const pair<T1, T2>& p) const {
        auto h1 = hash<T1>{}(p.first);
        auto h2 = hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

// Fast input/output for large datasets
#ifndef FAST_IO_INSTANCE_DEFINED
    #define FAST_IO_INSTANCE_DEFINED
    struct FastIO {
        FastIO() {
            ios_base::sync_with_stdio(false);
            cin.tie(nullptr);
            cout.tie(nullptr);
        }
    } fast_io_instance;
#endif

// Random number generator
#ifndef RNG_DEFINED
    #define RNG_DEFINED
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    template<typename T> T random(T l, T r) {
        return uniform_int_distribution<T>(l, r)(rng);
    }
#endif

#endif // PCH_H

//===---------------------------------------------------------------------===//
