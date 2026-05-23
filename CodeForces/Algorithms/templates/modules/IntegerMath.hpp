#pragma once
#include "Concepts.hpp"
#include "Macros.hpp"

//===----------------------------------------------------------------------===//
/* Integer Mathematical Utilities */

namespace cp::detail {

template <cp::UnsignedIntegral T>
[[gnu::always_inline]] constexpr T mul_mod_unsigned(T a, T b, T mod) {
#if HAS_INT128
  if constexpr (sizeof(T) <= sizeof(U64)) {
    return as<T>((as<U128>(a) * as<U128>(b)) % as<U128>(mod));
  }
#endif

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

template <cp::NonBoolIntegral T>
[[gnu::always_inline]] constexpr T div_floor(T a, T b) {
  my_assert(b != 0);
  if constexpr (cp::SignedIntegral<T>) {
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
  if constexpr (cp::SignedIntegral<T>) {
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

template <cp::NonBoolIntegral T>
[[gnu::always_inline]] constexpr T mod_pow(T base, T exp, T mod) {
  my_assert(mod != 0);
  if constexpr (cp::SignedIntegral<T>) {
    my_assert(mod > 0);
    my_assert(exp >= 0);
    if (mod <= 0) return 0;
    if (exp < 0) return 0;
  }

  using U = cp::make_unsigned_t<T>;
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

template <cp::NonBoolIntegral T>
[[gnu::always_inline]] inline T floor_sqrt(T x) {
  if constexpr (cp::SignedIntegral<T>) {
    my_assert(x >= 0);
    if (x < 0) return 0;
  }

  using U = cp::make_unsigned_t<T>;
  const U ux = as<U>(x);
  if (ux <= 1) return as<T>(ux);

  if constexpr (sizeof(U) > sizeof(U64)) {
    constexpr int bits = static_cast<int>(sizeof(U) * 8);
    U lo = 1;
    U hi = U{1} << ((bits + 1) / 2);
    while (lo + 1 < hi) {
      const U mid = lo + (hi - lo) / 2;
      if (mid <= ux / mid) lo = mid;
      else hi = mid;
    }
    return as<T>(lo);
  }

  U r = as<U>(std::sqrt(as<F80>(ux)));
  while ((r + 1) <= ux / (r + 1)) ++r;
  while (r > ux / r) --r;
  return as<T>(r);
}

template <cp::NonBoolIntegral T>
[[gnu::always_inline]] inline T ceil_sqrt(T x) {
  using U = cp::make_unsigned_t<T>;
  const T root = floor_sqrt(x);
  const U uf = as<U>(root);
  if (uf == 0) return 0;

  const U ux = as<U>(x);
  if (ux / uf == uf && ux % uf == 0) return root;
  return as<T>(uf + 1);
}
