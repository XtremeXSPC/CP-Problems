#ifndef CP_MODULES_ALGEBRA_MONOID_POWER_HPP
#define CP_MODULES_ALGEBRA_MONOID_POWER_HPP

#include "modules/algebra/_Common.hpp"

/// @brief Generic binary exponentiation for monoids.
template <typename Monoid>
auto monoid_power(typename Monoid::value_type x, I64 exp) -> typename Monoid::value_type {
  my_assert(exp >= 0);
  typename Monoid::value_type res = Monoid::identity();
  while (exp > 0) {
    if (exp & 1) res = Monoid::combine(res, x);
    x = Monoid::combine(x, x);
    exp >>= 1;
  }
  return res;
}

#endif
