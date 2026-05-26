#ifndef CP_MODULES_ALGEBRA_MONOID_GCD_LCM_MONOIDS_HPP
#define CP_MODULES_ALGEBRA_MONOID_GCD_LCM_MONOIDS_HPP

#include "modules/algebra/_Common.hpp"

/// @brief GCD monoid, identity '0'.
template <typename T>
struct GcdMonoid {
  using value_type = T;

  static constexpr bool commute = true;

  static constexpr auto identity() -> value_type { return value_type{}; }
  static constexpr auto combine(value_type a, value_type b) -> value_type {
    return std::gcd(a, b);
  }

  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
};

/// @brief LCM monoid, identity '1'.
template <typename T>
struct LcmMonoid {
  using value_type = T;

  static constexpr bool commute = true;

  static constexpr auto identity() -> value_type { return value_type(1); }
  static constexpr auto combine(value_type a, value_type b) -> value_type {
    return std::lcm(a, b);
  }

  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
};

#endif
