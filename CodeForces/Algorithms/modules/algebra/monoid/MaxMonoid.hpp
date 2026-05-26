#ifndef CP_MODULES_ALGEBRA_MONOID_MAX_MONOID_HPP
#define CP_MODULES_ALGEBRA_MONOID_MAX_MONOID_HPP

#include "modules/algebra/_Common.hpp"

/// @brief Maximum monoid with 'neg_infinity<T>' as identity.
template <typename T>
struct MaxMonoid {
  using value_type = T;

  static constexpr bool commute = true;

  static constexpr auto identity() -> value_type { return neg_infinity<T>; }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    return std::max(a, b);
  }

  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
};

#endif
