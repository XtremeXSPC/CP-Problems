#ifndef CP_MODULES_ALGEBRA_MONOID_MIN_MONOID_HPP
#define CP_MODULES_ALGEBRA_MONOID_MIN_MONOID_HPP

#include "modules/algebra/_Common.hpp"

/// @brief Minimum monoid with 'infinity<T>' as identity.
template <typename T>
struct MinMonoid {
  using value_type = T;

  static constexpr bool commute = true;

  static constexpr auto identity() -> value_type { return infinity<T>; }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    return std::min(a, b);
  }

  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
};

#endif
