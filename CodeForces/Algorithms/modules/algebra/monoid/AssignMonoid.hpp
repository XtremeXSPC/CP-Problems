#ifndef CP_MODULES_ALGEBRA_MONOID_ASSIGN_MONOID_HPP
#define CP_MODULES_ALGEBRA_MONOID_ASSIGN_MONOID_HPP

#include "modules/algebra/_Common.hpp"

/// @brief Assignment action monoid, where 'std::nullopt' means no pending write.
template <typename T>
struct AssignMonoid {
  using value_type = Optional<T>;

  static constexpr bool commute = false;

  static constexpr auto identity() -> value_type { return std::nullopt; }
  static constexpr auto combine(const value_type& existing, const value_type& fresh) -> value_type {
    return fresh.has_value() ? fresh : existing;
  }

  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
};

#endif
