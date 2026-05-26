#ifndef CP_MODULES_ALGEBRA_MONOID_INPLACE_ADD_MONOID_HPP
#define CP_MODULES_ALGEBRA_MONOID_INPLACE_ADD_MONOID_HPP

#include "modules/algebra/_Common.hpp"

/// @brief Additive monoid for values whose update is naturally 'res += b'.
template <typename T>
struct InplaceAddMonoid {
  using value_type = T;

  static constexpr bool commute = true;

  static constexpr auto identity() -> value_type { return value_type{}; }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    value_type res = a;
    res += b;
    return res;
  }

  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
};

#endif
