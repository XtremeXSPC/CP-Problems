#ifndef CP_MODULES_ALGEBRA_ACTED_MONOID_HPP
#define CP_MODULES_ALGEBRA_ACTED_MONOID_HPP

#include "modules/algebra/_Common.hpp"

/// @brief Bridges a value monoid, an action monoid, and one action applicator.
///
/// Actions are composed as 'compose(existing, fresh)': the fresh action happens
/// after the already pending action.
template <typename ValueMonoid, typename ActionMonoid, typename Action>
struct ActedMonoid {
  using value_type  = typename ValueMonoid::value_type;
  using action_type = typename ActionMonoid::value_type;

  static constexpr auto value_identity() -> value_type { return ValueMonoid::identity(); }
  static constexpr auto action_identity() -> action_type { return ActionMonoid::identity(); }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    return ValueMonoid::combine(a, b);
  }

  static constexpr auto compose(const action_type& existing, const action_type& fresh) -> action_type {
    return ActionMonoid::combine(existing, fresh);
  }

  static constexpr auto apply(const value_type& value, const action_type& action, I32 len) -> value_type {
    return Action::apply(value, action, len);
  }
};

#endif
