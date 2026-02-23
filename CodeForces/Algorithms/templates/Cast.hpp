#pragma once
#include "Concepts.hpp"

//===----------------------------------------------------------------------===//
/* Explicit Cast Helpers and Narrowing Policy */

namespace cp::cast {

template <class To, class From>
[[gnu::always_inline]] constexpr To as(From&& value) noexcept(
    noexcept(static_cast<To>(std::forward<From>(value)))) {
  return static_cast<To>(std::forward<From>(value));
}

template <class To, class From>
[[gnu::always_inline]] constexpr To narrow(From value) {
  To converted = static_cast<To>(value);
#ifdef LOCAL
  if constexpr (std::is_integral_v<From> && std::is_integral_v<To>) {
    my_assert(static_cast<From>(converted) == value && "narrow(): lossy integral conversion detected.");
  }
#endif
  return converted;
}

template <Enum E>
[[gnu::always_inline]] constexpr auto to_underlying(E value) noexcept
    -> std::underlying_type_t<remove_cvref_t<E>> {
  using U = std::underlying_type_t<remove_cvref_t<E>>;
  return static_cast<U>(value);
}

template <Enum E, Integral I>
[[gnu::always_inline]] constexpr E enum_cast(I value) noexcept {
  return static_cast<E>(value);
}

} // namespace cp::cast
