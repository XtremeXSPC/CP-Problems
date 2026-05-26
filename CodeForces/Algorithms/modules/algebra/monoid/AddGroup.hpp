#ifndef CP_MODULES_ALGEBRA_MONOID_ADD_GROUP_HPP
#define CP_MODULES_ALGEBRA_MONOID_ADD_GROUP_HPP

#include "modules/algebra/_Common.hpp"

/// @brief Additive group: '+', unary inverse, and repeated addition.
template <typename T>
struct AddGroup {
  using value_type = T;

  static constexpr bool commute = true;

  static constexpr auto identity() -> value_type { return value_type{}; }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    return a + b;
  }
  static constexpr auto inverse(const value_type& x) -> value_type { return -x; }

  /// @brief Repeats an additive value @p k times.
  static constexpr auto scale(const value_type& x, I64 k) -> value_type {
    return x * value_type(k);
  }

  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
  static constexpr auto power(const value_type& x, I64 k) -> value_type { return scale(x, k); }
};

#endif
