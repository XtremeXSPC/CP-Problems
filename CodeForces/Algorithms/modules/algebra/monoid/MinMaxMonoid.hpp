#ifndef CP_MODULES_ALGEBRA_MONOID_MIN_MAX_MONOID_HPP
#define CP_MODULES_ALGEBRA_MONOID_MIN_MAX_MONOID_HPP

#include "modules/algebra/_Common.hpp"

/// @brief Pair '{minimum, maximum}' for one segment.
template <typename T>
struct MinMaxMonoid {
  using value_type = Pair<T, T>;

  static constexpr bool commute = true;

  static constexpr auto identity() -> value_type { return {infinity<T>, neg_infinity<T>}; }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    return {std::min(a.first, b.first), std::max(a.second, b.second)};
  }
  static constexpr auto from_element(const T& x) -> value_type { return {x, x}; }

  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
};

#endif
