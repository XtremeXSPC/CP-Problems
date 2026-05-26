#ifndef CP_MODULES_ALGEBRA_MONOID_BITWISE_MONOIDS_HPP
#define CP_MODULES_ALGEBRA_MONOID_BITWISE_MONOIDS_HPP

#include "modules/algebra/_Common.hpp"

/// @brief Bitwise xor monoid, useful for parity and xor range aggregates.
template <typename T>
struct XorMonoid {
  using value_type = T;

  static constexpr bool commute = true;

  static constexpr auto identity() -> value_type { return value_type{}; }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    return a ^ b;
  }

  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
};

/// @brief Bitwise and monoid, identity is all bits set for integral 'T'.
template <typename T>
struct AndMonoid {
  using value_type = T;

  static constexpr bool commute = true;

  static constexpr auto identity() -> value_type { return ~value_type{}; }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    return a & b;
  }

  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
};

/// @brief Bitwise or monoid, identity '0'.
template <typename T>
struct OrMonoid {
  using value_type = T;

  static constexpr bool commute = true;

  static constexpr auto identity() -> value_type { return value_type{}; }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    return a | b;
  }

  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
};

#endif
