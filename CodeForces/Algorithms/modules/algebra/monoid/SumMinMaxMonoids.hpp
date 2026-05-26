#ifndef CP_MODULES_ALGEBRA_MONOID_SUM_MIN_MAX_MONOIDS_HPP
#define CP_MODULES_ALGEBRA_MONOID_SUM_MIN_MAX_MONOIDS_HPP

#include "modules/algebra/_Common.hpp"

/// @brief Pair '{sum, minimum}' for one segment.
template <typename T>
struct SumMinMonoid {
  using value_type = Pair<T, T>;

  static constexpr bool commute = true;

  static constexpr auto identity() -> value_type { return {T(0), infinity<T>}; }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    return {a.first + b.first, std::min(a.second, b.second)};
  }
  static constexpr auto from_element(const T& x) -> value_type { return {x, x}; }

  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
};

/// @brief Pair '{sum, maximum}' for one segment.
template <typename T>
struct SumMaxMonoid {
  using value_type = Pair<T, T>;

  static constexpr bool commute = true;

  static constexpr auto identity() -> value_type { return {T(0), neg_infinity<T>}; }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    return {a.first + b.first, std::max(a.second, b.second)};
  }
  static constexpr auto from_element(const T& x) -> value_type { return {x, x}; }

  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
};

#endif
