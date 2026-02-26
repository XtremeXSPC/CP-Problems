#pragma once
#include "Concepts.hpp"
#include "Timer.hpp"

//===----------------------------------------------------------------------===//
/* Mathematical Utilities */

namespace cp::detail {

template <std::unsigned_integral T>
[[gnu::always_inline]] constexpr T mul_mod_unsigned(T a, T b, T mod) {
#if HAS_INT128
  if constexpr (sizeof(T) <= sizeof(U64)) {
    return static_cast<T>((static_cast<U128>(a) * static_cast<U128>(b)) % static_cast<U128>(mod));
  }
#endif

  // Portable fallback for toolchains without native 128-bit multiplication.
  T result = 0;
  while (b > 0) {
    if ((b & 1U) != 0U) {
      if (result >= mod - a) result -= (mod - a);
      else result += a;
    }
    b >>= 1U;
    if (b == 0) break;
    if (a >= mod - a) a -= (mod - a);
    else a += a;
  }
  return result;
}

} // namespace cp::detail

// Integer division and modulus with floor/ceil semantics:
template <cp::NonBoolIntegral T>
[[gnu::always_inline]] constexpr T div_floor(T a, T b) {
  my_assert(b != 0);
  if constexpr (std::is_signed_v<T>) {
    T q = a / b;
    T r = a % b;
    if (r != 0 && ((r > 0) != (b > 0))) --q;
    return q;
  } else {
    return a / b;
  }
}

template <cp::NonBoolIntegral T>
[[gnu::always_inline]] constexpr T div_ceil(T a, T b) {
  my_assert(b != 0);
  if constexpr (std::is_signed_v<T>) {
    T q = a / b;
    T r = a % b;
    if (r != 0 && ((r > 0) == (b > 0))) ++q;
    return q;
  } else {
    return a / b + (a % b != 0);
  }
}

template <cp::NonBoolIntegral T>
[[gnu::always_inline]] constexpr T mod_floor(T a, T b) {
  return a - b * div_floor(a, b);
}

template <cp::NonBoolIntegral T>
[[gnu::always_inline]] constexpr std::pair<T, T> divmod(T a, T b) {
  T q = div_floor(a, b);
  return {q, a - q * b};
}

// Exponentiation without modulus:
template <cp::NonBoolIntegral T>
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
template <cp::NonBoolIntegral T>
[[gnu::always_inline]] constexpr T mod_pow(T base, T exp, T mod) {
  my_assert(mod != 0);
  if constexpr (std::is_signed_v<T>) {
    my_assert(mod > 0);
    my_assert(exp >= 0);
    if (mod <= 0) return 0;
    if (exp < 0) return 0;
  }

  using U = std::make_unsigned_t<T>;
  U umod = static_cast<U>(mod);
  U uexp = static_cast<U>(exp);
  U ubase = static_cast<U>(mod_floor(base, mod));
  U result = static_cast<U>(1) % umod;

  while (uexp > 0) {
    if ((uexp & 1U) != 0U) {
      result = cp::detail::mul_mod_unsigned(result, ubase, umod);
    }
    ubase = cp::detail::mul_mod_unsigned(ubase, ubase, umod);
    uexp >>= 1U;
  }
  return static_cast<T>(result);
}

template <cp::NonBoolIntegral T>
[[gnu::always_inline]] inline T floor_sqrt(T x) {
  if constexpr (std::is_signed_v<T>) {
    my_assert(x >= 0);
    if (x < 0) return 0;
  }

  using U = std::make_unsigned_t<T>;
  const U ux = static_cast<U>(x);
  if (ux <= 1) return static_cast<T>(ux);

  U r = static_cast<U>(std::sqrt(static_cast<F80>(ux)));
  while ((r + 1) <= ux / (r + 1)) ++r;
  while (r > ux / r) --r;
  return static_cast<T>(r);
}

template <cp::NonBoolIntegral T>
[[gnu::always_inline]] inline T ceil_sqrt(T x) {
  using U = std::make_unsigned_t<T>;
  const T root = floor_sqrt(x);
  const U uf = static_cast<U>(root);
  if (uf == 0) return 0;

  const U ux = static_cast<U>(x);
  if (ux / uf == uf && ux % uf == 0) return root;
  return static_cast<T>(uf + 1);
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
inline std::mt19937_64 rng(static_cast<U64>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

template <cp::Integral T>
inline T rnd(T a, T b) { return std::uniform_int_distribution<T>(a, b)(rng); }

template <cp::Floating T>
inline T rnd(T a, T b) { return std::uniform_real_distribution<T>(a, b)(rng); }

// Variadic min/max:
template <typename T>
constexpr const T& _min(const T& a, const T& b) { return (b < a) ? b : a; }

template <typename T>
constexpr const T& _max(const T& a, const T& b) { return (a < b) ? b : a; }

template <typename T, typename... Args>
constexpr const T& _min(const T& a, const T& b, const Args&... args) { return _min(a, _min(b, args...)); }

template <typename T, typename... Args>
constexpr const T& _max(const T& a, const T& b, const Args&... args) { return _max(a, _max(b, args...)); }
