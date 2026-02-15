#pragma once
#include "Types.hpp"
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
