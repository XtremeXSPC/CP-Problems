#ifndef CP_MODULES_DATA_STRUCTURES_FENWICK_TREE_2D_HPP
#define CP_MODULES_DATA_STRUCTURES_FENWICK_TREE_2D_HPP

#include "GroupFenwickTree2D.hpp"

/// @brief Additive dense 2D Fenwick tree on half-open rectangles.
template <typename T>
struct FenwickTree2D : GroupFenwickTree2D<AddGroup<T>> {
  using Base = GroupFenwickTree2D<AddGroup<T>>;
  using Value = typename Base::Value;

  using Base::Base;
  using Base::apply;
  using Base::build;
  using Base::get;
  using Base::get_all;
  using Base::prod;
  using Base::query;
  using Base::set;

  /// @brief Returns the sum on the prefix rectangle [0, rx) x [0, ry).
  [[nodiscard]] auto prefix_sum(I32 rx, I32 ry) const -> Value { return Base::prefix_query(rx, ry); }

  /// @brief Alias of prefix_sum().
  [[nodiscard]] auto sum(I32 rx, I32 ry) const -> Value { return prefix_sum(rx, ry); }

  /// @brief Returns the sum on [lx, rx) x [ly, ry).
  [[nodiscard]] auto sum(I32 lx, I32 rx, I32 ly, I32 ry) const -> Value {
    return Base::query(lx, rx, ly, ry);
  }

  /// @brief Adds @p delta at cell (@p x, @p y).
  void add(I32 x, I32 y, const Value& delta) { Base::apply(x, y, delta); }

  /// @brief Alias of add().
  void multiply(I32 x, I32 y, const Value& delta) { Base::apply(x, y, delta); }
};

#endif
