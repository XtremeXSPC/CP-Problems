#ifndef CP_MODULES_ALGEBRA_ACTED_RANGE_ADD_RANGE_SUM_HPP
#define CP_MODULES_ALGEBRA_ACTED_RANGE_ADD_RANGE_SUM_HPP

#include "modules/algebra/ActedMonoid.hpp"
#include "modules/algebra/monoid/AddMonoid.hpp"

template <typename T>
struct RangeAddRangeSumAction {
  static constexpr auto apply(const T& value, const T& action, I32 len) -> T {
    return value + action * T(len);
  }
};

/// @brief Lazy action for range add + range sum.
template <typename T>
using RangeAddRangeSumActedMonoid = ActedMonoid<AddMonoid<T>, AddMonoid<T>, RangeAddRangeSumAction<T>>;

#endif
