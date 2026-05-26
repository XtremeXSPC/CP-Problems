#ifndef CP_MODULES_ALGEBRA_ACTED_RANGE_ASSIGN_RANGE_MIN_MAX_HPP
#define CP_MODULES_ALGEBRA_ACTED_RANGE_ASSIGN_RANGE_MIN_MAX_HPP

#include "modules/algebra/ActedMonoid.hpp"
#include "modules/algebra/monoid/AssignMonoid.hpp"
#include "modules/algebra/monoid/MaxMonoid.hpp"
#include "modules/algebra/monoid/MinMonoid.hpp"

template <typename T>
struct RangeAssignRangeMinAction {
  using Action = typename AssignMonoid<T>::value_type;

  static constexpr auto apply(const T& value, const Action& action, I32) -> T {
    return action.has_value() ? *action : value;
  }
};

/// @brief Lazy action for range assign + range minimum.
template <typename T>
using RangeAssignRangeMinActedMonoid =
    ActedMonoid<MinMonoid<T>, AssignMonoid<T>, RangeAssignRangeMinAction<T>>;

template <typename T>
struct RangeAssignRangeMaxAction {
  using Action = typename AssignMonoid<T>::value_type;

  static constexpr auto apply(const T& value, const Action& action, I32) -> T {
    return action.has_value() ? *action : value;
  }
};

/// @brief Lazy action for range assign + range maximum.
template <typename T>
using RangeAssignRangeMaxActedMonoid =
    ActedMonoid<MaxMonoid<T>, AssignMonoid<T>, RangeAssignRangeMaxAction<T>>;

#endif
