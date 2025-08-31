#pragma once
#include "Types.hpp"

//===----------------------------------------------------------------------===//
/* Mathematical Utilities */

// Generic mathematical functions:
template <typename T>
[[gnu::always_inline]] constexpr T gcd(T a, T b) {
  if constexpr (std::is_integral_v<T>) {
    return b ? gcd(b, a % b) : a;
  } else {
    return std::gcd(a, b);
  }
}

template <typename T>
[[gnu::always_inline]] constexpr T lcm(T a, T b) {
  return a / gcd(a, b) * b;
}

// Advanced division operations:
template <typename T>
[[gnu::always_inline]] constexpr T div_floor(T a, T b) {
  return a / b - (a % b != 0 && (a ^ b) < 0);
}

template <typename T>
[[gnu::always_inline]] constexpr T div_ceil(T a, T b) {
  return div_floor(a + b - 1, b);
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
[[gnu::always_inline]] constexpr T power(T base, T exp, T mod = 0) {
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

// Min/Max update functions:
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
constexpr const T& min(const T& a, const T& b) { return (b < a) ? b : a; }

template <typename T>
constexpr const T& max(const T& a, const T& b) { return (a < b) ? b : a; }

template <typename T, typename... Args>
constexpr const T& min(const T& a, const T& b, const Args&... args) {
  return min(a, min(b, args...));
}

template <typename T, typename... Args>
constexpr const T& max(const T& a, const T& b, const Args&... args) {
  return max(a, max(b, args...));
}
