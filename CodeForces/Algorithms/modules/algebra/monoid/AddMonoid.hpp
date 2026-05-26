#ifndef CP_MODULES_ALGEBRA_MONOID_ADD_MONOID_HPP
#define CP_MODULES_ALGEBRA_MONOID_ADD_MONOID_HPP

#include "modules/algebra/_Common.hpp"

/// @brief Additive monoid: aggregate by 'a + b', identity '0'.
template <typename T>
struct AddMonoid {
  using value_type = T;

  static constexpr bool commute = true;

  static constexpr auto identity() -> value_type { return value_type{}; }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    return a + b;
  }

  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
};

#endif
