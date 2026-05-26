#ifndef CP_MODULES_ALGEBRA_MONOID_COUNT_MONOIDS_HPP
#define CP_MODULES_ALGEBRA_MONOID_COUNT_MONOIDS_HPP

#include "modules/algebra/_Common.hpp"

/// @brief Minimum and number of occurrences of that minimum.
template <typename T>
struct MinCountMonoid {
  using value_type = Pair<T, I32>;

  static constexpr bool commute = true;

  static constexpr auto identity() -> value_type { return {infinity<T>, 0}; }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    if (a.first < b.first) return a;
    if (a.first > b.first) return b;
    return {a.first, a.second + b.second};
  }
  static constexpr auto from_element(const T& x) -> value_type { return {x, 1}; }

  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
};

/// @brief Maximum and number of occurrences of that maximum.
template <typename T>
struct MaxCountMonoid {
  using value_type = Pair<T, I32>;

  static constexpr bool commute = true;

  static constexpr auto identity() -> value_type { return {neg_infinity<T>, 0}; }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    if (a.first > b.first) return a;
    if (a.first < b.first) return b;
    return {a.first, a.second + b.second};
  }
  static constexpr auto from_element(const T& x) -> value_type { return {x, 1}; }

  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
};

#endif
