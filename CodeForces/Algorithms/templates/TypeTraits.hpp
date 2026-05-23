#pragma once
#include "ScalarTypes.hpp"

//===----------------------------------------------------------------------===//
/* Extended Type Traits */

namespace cp {

template <class T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

namespace detail {

template <class T>
inline constexpr bool is_extended_integral_v = false;

template <class T>
struct make_unsigned { using type = std::make_unsigned_t<T>; };

template <>
struct make_unsigned<bool> { using type = U8; };

#if HAS_INT128
  template <>
  inline constexpr bool is_extended_integral_v<I128> = true;
  template <>
  inline constexpr bool is_extended_integral_v<U128> = true;

  template <>
  struct make_unsigned<I128> { using type = U128; };
  template <>
  struct make_unsigned<U128> { using type = U128; };
#endif

} // namespace detail

template <class T>
using make_unsigned_t = typename detail::make_unsigned<remove_cvref_t<T>>::type;

} // namespace cp
