#pragma once
#include "Concepts.hpp"
#include "templates/core/Macros.hpp"
#include "templates/core/ContainerAliases.hpp"
#include "templates/core/Debug.hpp"

//===----------------------------------------------------------------------===//
/* Explicit Cast Helpers and Narrowing Policy */

namespace cp::cast {

template <class To, class From>
[[gnu::always_inline]] constexpr To as(From&& value) noexcept(noexcept(::as<To>(std::forward<From>(value)))) {
  return ::as<To>(std::forward<From>(value));
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
  return as<To>(value);
}

template <class To, class From>
[[nodiscard]] constexpr auto try_narrow(From value) -> Optional<remove_cvref_t<To>> {
  using Src = remove_cvref_t<From>;
  using Dst = remove_cvref_t<To>;
  if constexpr (std::integral<Src> && std::integral<Dst>) {
    if (!std::in_range<Dst>(value))
      return std::nullopt;
  }
  return as<Dst>(value);
}

template <class To, class From>
[[gnu::always_inline]] constexpr To saturate(From value) {
  using Src = remove_cvref_t<From>;
  using Dst = remove_cvref_t<To>;
  if constexpr (std::integral<Src> && std::integral<Dst>) {
    if (std::in_range<Dst>(value))
      return as<To>(value);
    if (std::cmp_less(value, Limits<Dst>::min())) {
      return as<To>(Limits<Dst>::min());
    }
    return as<To>(Limits<Dst>::max());
  }
  return as<To>(value);
}

template <Enum E>
[[gnu::always_inline]] constexpr auto to_underlying(E value) noexcept -> std::underlying_type_t<remove_cvref_t<E>> {
  using U = std::underlying_type_t<remove_cvref_t<E>>;
  return as<U>(value);
}

template <Enum E, Integral I>
[[gnu::always_inline]] constexpr E enum_cast(I value) noexcept {
  return as<E>(value);
}

} // namespace cp::cast

template <typename To>
[[gnu::always_inline]] constexpr To narrow_as(auto x) {
  To converted = as<To>(x);
#ifdef LOCAL
  using From = std::remove_cvref_t<decltype(x)>;
  if constexpr (std::is_integral_v<From> && std::is_integral_v<To>) {
    my_assert(as<From>(converted) == x && "narrow_as(): lossy integral conversion detected.");
  }
#endif
  return converted;
}
