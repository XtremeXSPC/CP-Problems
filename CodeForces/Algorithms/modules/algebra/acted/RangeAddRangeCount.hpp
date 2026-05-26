#ifndef CP_MODULES_ALGEBRA_ACTED_RANGE_ADD_RANGE_COUNT_HPP
#define CP_MODULES_ALGEBRA_ACTED_RANGE_ADD_RANGE_COUNT_HPP

#include "modules/algebra/ActedMonoid.hpp"
#include "modules/algebra/monoid/AddMonoid.hpp"
#include "modules/algebra/monoid/CountMonoids.hpp"

template <typename T>
struct RangeAddRangeMinCountAction {
  using Value = typename MinCountMonoid<T>::value_type;

  static constexpr auto apply(const Value& value, const T& action, I32) -> Value {
    return value.first == infinity<T> ? value : Value{value.first + action, value.second};
  }
};

/// @brief Lazy action for range add + '{minimum, count}' query.
template <typename T>
using RangeAddRangeMinCountActedMonoid =
    ActedMonoid<MinCountMonoid<T>, AddMonoid<T>, RangeAddRangeMinCountAction<T>>;

template <typename T>
struct RangeAddRangeMaxCountAction {
  using Value = typename MaxCountMonoid<T>::value_type;

  static constexpr auto apply(const Value& value, const T& action, I32) -> Value {
    return value.first == neg_infinity<T> ? value : Value{value.first + action, value.second};
  }
};

/// @brief Lazy action for range add + '{maximum, count}' query.
template <typename T>
using RangeAddRangeMaxCountActedMonoid =
    ActedMonoid<MaxCountMonoid<T>, AddMonoid<T>, RangeAddRangeMaxCountAction<T>>;

#endif
