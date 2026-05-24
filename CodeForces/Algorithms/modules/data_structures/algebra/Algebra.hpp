#ifndef CP_MODULES_DATA_STRUCTURES_ALGEBRA_HPP
#define CP_MODULES_DATA_STRUCTURES_ALGEBRA_HPP

#include "../_Common.hpp"

template <typename T>
struct AddMonoid {
  using value_type = T;

  static constexpr auto identity() -> value_type { return value_type{}; }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    return a + b;
  }
};

template <typename T>
struct AddGroup {
  using value_type = T;

  static constexpr auto identity() -> value_type { return value_type{}; }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    return a + b;
  }
  static constexpr auto inverse(const value_type& x) -> value_type { return -x; }

  /// @brief Repeats an additive value @p k times.
  static constexpr auto scale(const value_type& x, I64 k) -> value_type {
    return x * static_cast<value_type>(k);
  }
};

template <typename T>
struct InplaceAddMonoid {
  using value_type = T;

  static constexpr auto identity() -> value_type { return value_type{}; }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    value_type res = a;
    res += b;
    return res;
  }
};

template <typename T>
struct MinMonoid {
  using value_type = T;

  static constexpr auto identity() -> value_type { return infinity<T>; }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    return std::min(a, b);
  }
};

template <typename T>
struct MaxMonoid {
  using value_type = T;

  static constexpr auto identity() -> value_type { return neg_infinity<T>; }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    return std::max(a, b);
  }
};

/// @brief Bridges a value monoid, an action monoid, and one action applicator.
template <typename ValueMonoid, typename ActionMonoid, typename Action>
struct ActedMonoid {
  using value_type  = typename ValueMonoid::value_type;
  using action_type = typename ActionMonoid::value_type;

  static constexpr auto value_identity() -> value_type { return ValueMonoid::identity(); }
  static constexpr auto action_identity() -> action_type { return ActionMonoid::identity(); }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    return ValueMonoid::combine(a, b);
  }

  /// @brief Composes `fresh` after the already-pending `existing` action.
  static constexpr auto compose(const action_type& existing, const action_type& fresh) -> action_type {
    return ActionMonoid::combine(existing, fresh);
  }

  static constexpr auto apply(const value_type& value, const action_type& action, I32 len) -> value_type {
    return Action::apply(value, action, len);
  }
};

template <typename T>
struct RangeAddRangeSumAction {
  static constexpr auto apply(const T& value, const T& action, I32 len) -> T {
    return value + action * static_cast<T>(len);
  }
};

template <typename T>
using RangeAddRangeSumActedMonoid = ActedMonoid<AddMonoid<T>, AddMonoid<T>, RangeAddRangeSumAction<T>>;

#endif
