#ifndef CP_MODULES_ALGEBRA_MONOID_INDEX_MONOIDS_HPP
#define CP_MODULES_ALGEBRA_MONOID_INDEX_MONOIDS_HPP

#include "modules/algebra/_Common.hpp"

/// @brief Minimum with its index. Ties prefer the smaller index by default.
template <typename T, bool TieLeft = true>
struct MinIndexMonoid {
  using value_type = Pair<T, I32>;

  static constexpr bool commute = true;

  static constexpr auto identity() -> value_type { return {infinity<T>, -1}; }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    if (a.first < b.first) return a;
    if (a.first > b.first) return b;
    if constexpr (TieLeft) {
      if (a.second == -1) return b;
      if (b.second == -1) return a;
      return a.second <= b.second ? a : b;
    } else {
      return a.second >= b.second ? a : b;
    }
  }

  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
};

/// @brief Maximum with its index. Ties prefer the smaller index by default.
template <typename T, bool TieLeft = true>
struct MaxIndexMonoid {
  using value_type = Pair<T, I32>;

  static constexpr bool commute = true;

  static constexpr auto identity() -> value_type { return {neg_infinity<T>, -1}; }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    if (a.first > b.first) return a;
    if (a.first < b.first) return b;
    if constexpr (TieLeft) {
      if (a.second == -1) return b;
      if (b.second == -1) return a;
      return a.second <= b.second ? a : b;
    } else {
      return a.second >= b.second ? a : b;
    }
  }

  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
};

#endif
