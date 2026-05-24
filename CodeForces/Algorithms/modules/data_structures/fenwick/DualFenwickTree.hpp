#ifndef CP_MODULES_DATA_STRUCTURES_DUAL_FENWICK_TREE_HPP
#define CP_MODULES_DATA_STRUCTURES_DUAL_FENWICK_TREE_HPP

#include "GroupFenwickTree.hpp"

/// @brief Dual Fenwick tree for range updates and point queries.
template <typename Group>
struct DualFenwickTree {
  using Value = typename Group::value_type;

  I32 n = 0;
  GroupFenwickTree<Group> diff;

  DualFenwickTree() = default;

  /// @brief Constructs an identity-filled tree of size @p size_.
  explicit DualFenwickTree(I32 size_) { build(size_); }

  /// @brief Constructs a tree from a point-value generator.
  template <typename F>
  DualFenwickTree(I32 size_, F init) {
    build(size_, init);
  }

  /// @brief Constructs a tree from explicit point values.
  explicit DualFenwickTree(const Vec<Value>& values) { build(values); }

  /// @brief Reinitializes the tree with @p size_ identity values.
  void build(I32 size_) {
    n = std::max<I32>(size_, 0);
    diff.build(n);
  }

  /// @brief Reinitializes the tree from explicit point values.
  void build(const Vec<Value>& values) {
    build(isz(values), [&](I32 i) -> Value { return values[i]; });
  }

  /// @brief Reinitializes the tree from a point-value generator.
  template <typename F>
  void build(I32 size_, F init) {
    n = std::max<I32>(size_, 0);
    Vec<Value> delta(n, Group::identity());
    Value prev = Group::identity();
    FOR(i, n) {
      const Value cur = init(i);
      delta[i] = Group::combine(Group::inverse(prev), cur);
      prev = cur;
    }
    diff.build(delta);
  }

  /// @brief Applies @p delta to the half-open range [l, r).
  void apply(I32 l, I32 r, const Value& delta) {
    l = std::clamp<I32>(l, 0, n);
    r = std::clamp<I32>(r, 0, n);
    if (l >= r) return;
    diff.apply(l, delta);
    if (r < n) diff.apply(r, Group::inverse(delta));
  }

  /// @brief Alias of apply().
  void add(I32 l, I32 r, const Value& delta) { apply(l, r, delta); }

  /// @brief Returns the point value at @p idx.
  [[nodiscard]] auto get(I32 idx) const -> Value {
    my_assert(0 <= idx && idx < n);
    return diff.prefix_query(idx + 1);
  }

  /// @brief Alias of get().
  [[nodiscard]] auto query(I32 idx) const -> Value { return get(idx); }

  /// @brief Materializes all point values.
  [[nodiscard]] auto get_all() const -> Vec<Value> {
    Vec<Value> values(n, Group::identity());
    Value cur = Group::identity();
    const Vec<Value> delta = diff.get_all();
    FOR(i, n) {
      cur = Group::combine(cur, delta[i]);
      values[i] = cur;
    }
    return values;
  }

  /// @brief Overwrites the point at @p idx.
  void set(I32 idx, const Value& value) {
    apply(idx, idx + 1, Group::combine(Group::inverse(get(idx)), value));
  }
};

#endif
