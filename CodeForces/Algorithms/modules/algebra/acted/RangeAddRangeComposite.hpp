#ifndef CP_MODULES_ALGEBRA_ACTED_RANGE_ADD_RANGE_COMPOSITE_HPP
#define CP_MODULES_ALGEBRA_ACTED_RANGE_ADD_RANGE_COMPOSITE_HPP

#include "modules/algebra/ActedMonoid.hpp"
#include "modules/algebra/monoid/AddMonoid.hpp"
#include "modules/algebra/monoid/MinMaxMonoid.hpp"
#include "modules/algebra/monoid/SumMinMaxMonoids.hpp"

template <typename T>
struct RangeAddRangeMinMaxAction {
  using Value = typename MinMaxMonoid<T>::value_type;

  static constexpr auto apply(const Value& value, const T& action, I32) -> Value {
    const T lo = value.first  == infinity<T> ? value.first : value.first + action;
    const T hi = value.second == neg_infinity<T> ? value.second : value.second + action;
    return {lo, hi};
  }
};

/// @brief Lazy action for range add + '{minimum, maximum}' query.
template <typename T>
using RangeAddRangeMinMaxActedMonoid =
    ActedMonoid<MinMaxMonoid<T>, AddMonoid<T>, RangeAddRangeMinMaxAction<T>>;

template <typename T>
struct RangeAddRangeSumMinAction {
  using Value = typename SumMinMonoid<T>::value_type;

  static constexpr auto apply(const Value& value, const T& action, I32 len) -> Value {
    const T mn = value.second == infinity<T> ? value.second : value.second + action;
    return {value.first + T(len) * action, mn};
  }
};

/// @brief Lazy action for range add + '{sum, minimum}' query.
template <typename T>
using RangeAddRangeSumMinActedMonoid =
    ActedMonoid<SumMinMonoid<T>, AddMonoid<T>, RangeAddRangeSumMinAction<T>>;

template <typename T>
struct RangeAddRangeSumMaxAction {
  using Value = typename SumMaxMonoid<T>::value_type;

  static constexpr auto apply(const Value& value, const T& action, I32 len) -> Value {
    const T mx = value.second == neg_infinity<T> ? value.second : value.second + action;
    return {value.first + T(len) * action, mx};
  }
};

/// @brief Lazy action for range add + '{sum, maximum}' query.
template <typename T>
using RangeAddRangeSumMaxActedMonoid =
    ActedMonoid<SumMaxMonoid<T>, AddMonoid<T>, RangeAddRangeSumMaxAction<T>>;

#endif
