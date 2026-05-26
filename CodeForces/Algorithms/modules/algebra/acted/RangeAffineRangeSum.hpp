#ifndef CP_MODULES_ALGEBRA_ACTED_RANGE_AFFINE_RANGE_SUM_HPP
#define CP_MODULES_ALGEBRA_ACTED_RANGE_AFFINE_RANGE_SUM_HPP

#include "modules/algebra/ActedMonoid.hpp"
#include "modules/algebra/monoid/AddMonoid.hpp"
#include "modules/algebra/monoid/AffineMonoid.hpp"

template <typename T>
struct RangeAffineRangeSumAction {
  using Action = typename AffineMonoid<T>::value_type;

  static constexpr auto apply(const T& value, const Action& action, I32 len) -> T {
    return value * action.first + T(len) * action.second;
  }
};

/// @brief Lazy action for range affine transform + range sum.
template <typename T>
using RangeAffineRangeSumActedMonoid =
    ActedMonoid<AddMonoid<T>, AffineMonoid<T>, RangeAffineRangeSumAction<T>>;

#endif
