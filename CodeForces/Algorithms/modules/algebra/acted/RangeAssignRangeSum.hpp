#ifndef CP_MODULES_ALGEBRA_ACTED_RANGE_ASSIGN_RANGE_SUM_HPP
#define CP_MODULES_ALGEBRA_ACTED_RANGE_ASSIGN_RANGE_SUM_HPP

#include "modules/algebra/ActedMonoid.hpp"
#include "modules/algebra/monoid/AddMonoid.hpp"
#include "modules/algebra/monoid/AssignMonoid.hpp"

template <typename T>
struct RangeAssignRangeSumAction {
  using Action = typename AssignMonoid<T>::value_type;

  static constexpr auto apply(const T& value, const Action& action, I32 len) -> T {
    return action.has_value() ? (*action) * T(len) : value;
  }
};

/// @brief Lazy action for range assign + range sum.
template <typename T>
using RangeAssignRangeSumActedMonoid =
    ActedMonoid<AddMonoid<T>, AssignMonoid<T>, RangeAssignRangeSumAction<T>>;

#endif
