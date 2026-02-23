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
  using Src = remove_cvref_t<From>;
  using Dst = remove_cvref_t<To>;
  if constexpr (std::integral<Src> && std::integral<Dst>) {
#ifdef LOCAL
    my_assert(std::in_range<Dst>(value) && "narrow(): integral value out of destination range.");
#endif
  }
  return static_cast<To>(value);
}

template <class To, class From>
[[nodiscard]] constexpr auto try_narrow(From value) -> Optional<remove_cvref_t<To>> {
  using Src = remove_cvref_t<From>;
  using Dst = remove_cvref_t<To>;
  if constexpr (std::integral<Src> && std::integral<Dst>) {
    if (!std::in_range<Dst>(value)) return std::nullopt;
  }
  return static_cast<Dst>(value);
}

template <class To, class From>
[[gnu::always_inline]] constexpr To saturate(From value) {
  using Src = remove_cvref_t<From>;
  using Dst = remove_cvref_t<To>;
  if constexpr (std::integral<Src> && std::integral<Dst>) {
    if (std::in_range<Dst>(value)) return static_cast<To>(value);
    if (std::cmp_less(value, std::numeric_limits<Dst>::min())) {
      return static_cast<To>(std::numeric_limits<Dst>::min());
    }
    return static_cast<To>(std::numeric_limits<Dst>::max());
  }
  return static_cast<To>(value);
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
