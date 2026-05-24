#ifndef CP_MODULES_DATA_STRUCTURES_FENWICK_TREE_HPP
#define CP_MODULES_DATA_STRUCTURES_FENWICK_TREE_HPP

#include "GroupFenwickTree.hpp"

/// @brief Additive Fenwick tree on half-open ranges.
template <typename T>
struct FenwickTree : GroupFenwickTree<AddGroup<T>> {
  using Base  = GroupFenwickTree<AddGroup<T>>;
  using Value = typename Base::Value;

  using Base::Base;
  using Base::apply;
  using Base::build;
  using Base::get;
  using Base::get_all;
  using Base::max_right;
  using Base::min_left;
  using Base::n;
  using Base::prod;
  using Base::prod_all;
  using Base::query;
  using Base::set;
  using Base::tree;

  /// @brief Returns the sum on the prefix [0, r).
  [[nodiscard]] auto prefix_sum(I32 r) const -> Value { return Base::prefix_query(r); }

  /// @brief Alias of prefix_sum().
  [[nodiscard]] auto sum(I32 r) const -> Value { return prefix_sum(r); }

  /// @brief Returns the sum on the half-open range [l, r).
  [[nodiscard]] auto sum(I32 l, I32 r) const -> Value { return Base::query(l, r); }

  /// @brief Returns the sum on [0, n).
  [[nodiscard]] auto sum_all() const -> Value { return Base::prod_all(); }

  /// @brief Adds @p delta at position @p idx.
  void add(I32 idx, const Value& delta) { Base::apply(idx, delta); }

  /// @brief Alias of add().
  void multiply(I32 idx, const Value& delta) { Base::apply(idx, delta); }

  /// @brief Returns the smallest prefix length @p r with prefix_sum(r) >= target.
  ///
  /// This requires prefix sums to be monotone, which in practice means all
  /// point updates should preserve non-decreasing prefixes.
  [[nodiscard]] auto prefix_lower_bound(const Value& target) const -> I32 {
    if (target <= Value{}) return 0;
    if (Base::prod_all() < target) return n;
    Value acc{};
    I32 pos = 0;
    I32 step = 1;
    while ((step << 1) <= n) step <<= 1;
    while (step > 0) {
      const I32 next = pos + step;
      if (next <= n && acc + tree[next - 1] < target) {
        acc += tree[next - 1];
        pos = next;
      }
      step >>= 1;
    }
    return std::min<I32>(pos + 1, n);
  }

  /// @brief Returns the smallest prefix length @p r with prefix_sum(r) > target.
  [[nodiscard]] auto prefix_upper_bound(const Value& target) const -> I32 {
    if (Value{} > target) return 0;
    if (Base::prod_all() <= target) return n;
    Value acc{};
    I32 pos = 0;
    I32 step = 1;
    while ((step << 1) <= n) step <<= 1;
    while (step > 0) {
      const I32 next = pos + step;
      if (next <= n && acc + tree[next - 1] <= target) {
        acc += tree[next - 1];
        pos = next;
      }
      step >>= 1;
    }
    return std::min<I32>(pos + 1, n);
  }

  /// @brief Alias of prefix_lower_bound().
  [[nodiscard]] auto lower_bound(const Value& target) const -> I32 {
    return prefix_lower_bound(target);
  }

  /// @brief Alias of prefix_upper_bound().
  [[nodiscard]] auto upper_bound(const Value& target) const -> I32 {
    return prefix_upper_bound(target);
  }
};

#endif
