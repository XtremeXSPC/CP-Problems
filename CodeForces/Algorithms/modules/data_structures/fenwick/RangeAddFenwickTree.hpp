#ifndef CP_MODULES_DATA_STRUCTURES_RANGE_ADD_FENWICK_TREE_HPP
#define CP_MODULES_DATA_STRUCTURES_RANGE_ADD_FENWICK_TREE_HPP

#include "GroupFenwickTree.hpp"

/// @brief Fenwick pair for range updates and range queries on additive groups.
template <typename Group>
struct RangeAddFenwickTree {
  using Value = typename Group::value_type;

  I32 n = 0;
  GroupFenwickTree<Group> bit0;
  GroupFenwickTree<Group> bit1;

  RangeAddFenwickTree() = default;

  /// @brief Constructs an identity-filled tree of size @p size_.
  explicit RangeAddFenwickTree(I32 size_) { build(size_); }

  /// @brief Constructs a tree from a point-value generator.
  template <typename F>
  RangeAddFenwickTree(I32 size_, F init) {
    build(size_, init);
  }

  /// @brief Constructs a tree from explicit point values.
  explicit RangeAddFenwickTree(const Vec<Value>& values) { build(values); }

  /// @brief Reinitializes the tree with @p size_ identity values.
  void build(I32 size_) {
    n = std::max<I32>(size_, 0);
    bit0.build(n);
    bit1.build(n);
  }

  /// @brief Reinitializes the tree from explicit point values.
  void build(const Vec<Value>& values) {
    build(isz(values), [&](I32 i) -> Value { return values[i]; });
  }

  /// @brief Reinitializes the tree from a point-value generator.
  template <typename F>
  void build(I32 size_, F init) {
    n = std::max<I32>(size_, 0);
    bit0.build(n, init);
    bit1.build(n);
  }

  /// @brief Returns the aggregate on the prefix [0, r).
  [[nodiscard]] auto prefix_query(I32 r) const -> Value {
    r = std::clamp<I32>(r, 0, n);
    return Group::combine(Group::scale(bit1.prefix_query(r), r), bit0.prefix_query(r));
  }

  /// @brief Alias of prefix_query().
  [[nodiscard]] auto prefix_prod(I32 r) const -> Value { return prefix_query(r); }

  /// @brief Returns the aggregate on the half-open range [l, r).
  [[nodiscard]] auto query(I32 l, I32 r) const -> Value {
    l = std::clamp<I32>(l, 0, n);
    r = std::clamp<I32>(r, 0, n);
    if (l >= r) return Group::identity();
    return Group::combine(Group::inverse(prefix_query(l)), prefix_query(r));
  }

  /// @brief Alias of query().
  [[nodiscard]] auto prod(I32 l, I32 r) const -> Value { return query(l, r); }

  /// @brief Returns the point value at @p idx.
  [[nodiscard]] auto get(I32 idx) const -> Value { return query(idx, idx + 1); }

  /// @brief Materializes all point values.
  [[nodiscard]] auto get_all() const -> Vec<Value> {
    Vec<Value> values(n);
    FOR(i, n) values[i] = get(i);
    return values;
  }

  /// @brief Applies @p delta to the half-open range [l, r).
  void apply(I32 l, I32 r, const Value& delta) {
    l = std::clamp<I32>(l, 0, n);
    r = std::clamp<I32>(r, 0, n);
    if (l >= r) return;
    bit0.apply(l, Group::inverse(Group::scale(delta, l)));
    if (r < n) bit0.apply(r, Group::scale(delta, r));
    bit1.apply(l, delta);
    if (r < n) bit1.apply(r, Group::inverse(delta));
  }

  /// @brief Alias of apply().
  void add(I32 l, I32 r, const Value& delta) { apply(l, r, delta); }

  /// @brief Applies @p delta at a single point.
  void add_at(I32 idx, const Value& delta) { apply(idx, idx + 1, delta); }

  /// @brief Overwrites the point at @p idx.
  void set(I32 idx, const Value& value) {
    apply(idx, idx + 1, Group::combine(Group::inverse(get(idx)), value));
  }

  /// @brief Returns the aggregate on [0, n).
  [[nodiscard]] auto prod_all() const -> Value { return query(0, n); }
};

#endif
