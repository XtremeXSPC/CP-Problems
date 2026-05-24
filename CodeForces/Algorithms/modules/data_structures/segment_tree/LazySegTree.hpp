#ifndef CP_MODULES_DATA_STRUCTURES_LAZY_SEG_TREE_HPP
#define CP_MODULES_DATA_STRUCTURES_LAZY_SEG_TREE_HPP

#include "ActedLazySegTree.hpp"

template <typename Value, typename Action>
struct LegacyLazySegTreeAction {
  /// @brief Applies the legacy "value += action * len" convention.
  static auto apply(const Value& value, const Action& action, I32 len) -> Value {
    Value res = value;
    res += action * len;
    return res;
  }
};

template <typename Value, typename Action>
using LegacyLazySegTreeActedMonoid =
    ActedMonoid<AddMonoid<Value>, InplaceAddMonoid<Action>, LegacyLazySegTreeAction<Value, Action>>;

/// @brief Backward-compatible lazy segment tree keeping the old operator-driven API.
template <typename Value, typename Action>
using LazySegTree = ActedLazySegTree<LegacyLazySegTreeActedMonoid<Value, Action>>;

#endif
