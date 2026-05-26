#ifndef CP_MODULES_ALGEBRA_ACTED_RANGE_ADD_RANGE_MIN_MAX_HPP
#define CP_MODULES_ALGEBRA_ACTED_RANGE_ADD_RANGE_MIN_MAX_HPP

#include "modules/algebra/ActedMonoid.hpp"
#include "modules/algebra/monoid/AddMonoid.hpp"
#include "modules/algebra/monoid/MaxMonoid.hpp"
#include "modules/algebra/monoid/MinMonoid.hpp"

template <typename T>
struct RangeAddRangeMinAction {
  static constexpr auto apply(const T& value, const T& action, I32) -> T {
    return value == infinity<T> ? value : value + action;
  }
};

/// @brief Lazy action for range add + range minimum.
template <typename T>
using RangeAddRangeMinActedMonoid = ActedMonoid<MinMonoid<T>, AddMonoid<T>, RangeAddRangeMinAction<T>>;

template <typename T>
struct RangeAddRangeMaxAction {
  static constexpr auto apply(const T& value, const T& action, I32) -> T {
    return value == neg_infinity<T> ? value : value + action;
  }
};

/// @brief Lazy action for range add + range maximum.
template <typename T>
using RangeAddRangeMaxActedMonoid = ActedMonoid<MaxMonoid<T>, AddMonoid<T>, RangeAddRangeMaxAction<T>>;

#endif
