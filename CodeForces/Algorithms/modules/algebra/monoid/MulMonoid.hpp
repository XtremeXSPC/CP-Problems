#ifndef CP_MODULES_ALGEBRA_MONOID_MUL_MONOID_HPP
#define CP_MODULES_ALGEBRA_MONOID_MUL_MONOID_HPP

#include "modules/algebra/_Common.hpp"

/// @brief Multiplicative monoid: aggregate by 'a * b', identity '1'.
template <typename T>
struct MulMonoid {
  using value_type = T;

  static constexpr bool commute = true;

  static constexpr auto identity() -> value_type { return value_type(1); }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    return a * b;
  }

  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
};

#endif
