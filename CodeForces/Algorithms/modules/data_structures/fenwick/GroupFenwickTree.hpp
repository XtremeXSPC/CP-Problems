#ifndef CP_MODULES_DATA_STRUCTURES_GROUP_FENWICK_TREE_HPP
#define CP_MODULES_DATA_STRUCTURES_GROUP_FENWICK_TREE_HPP

#include "../_Common.hpp"

/// @brief Fenwick tree over a commutative group on half-open ranges.
template <typename Group>
struct GroupFenwickTree {
  using Value = typename Group::value_type;

  I32 n = 0;
  Vec<Value> tree;
  Value total = Group::identity();

  GroupFenwickTree() = default;

  /// @brief Constructs an identity-filled tree of size @p size_.
  explicit GroupFenwickTree(I32 size_) { build(size_); }

  /// @brief Constructs a tree from a point-value generator.
  template <typename F>
  GroupFenwickTree(I32 size_, F init) {
    build(size_, init);
  }

  /// @brief Constructs a tree from explicit point values.
  explicit GroupFenwickTree(const Vec<Value>& values) { build(values); }

  /// @brief Reinitializes the tree with @p size_ identity values.
  void build(I32 size_) {
    n = std::max<I32>(size_, 0);
    tree.assign(n, Group::identity());
    total = Group::identity();
  }

  /// @brief Reinitializes the tree from explicit point values.
  void build(const Vec<Value>& values) {
    build(isz(values), [&](I32 i) -> Value { return values[i]; });
  }

  /// @brief Reinitializes the tree from a point-value generator.
  template <typename F>
  void build(I32 size_, F init) {
    n = std::max<I32>(size_, 0);
    tree.assign(n, Group::identity());
    total = Group::identity();
    FOR(i, n) tree[i] = init(i);
    FOR(i, 1, n + 1) {
      const I32 j = i + (i & -i);
      if (j <= n) tree[j - 1] = Group::combine(tree[i - 1], tree[j - 1]);
    }
    total = prefix_query(n);
  }

  /// @brief Returns the group product on the prefix [0, r).
  [[nodiscard]] auto prefix_query(I32 r) const -> Value {
    r = std::clamp<I32>(r, 0, n);
    Value res = Group::identity();
    while (r > 0) {
      res = Group::combine(res, tree[r - 1]);
      r -= r & -r;
    }
    return res;
  }

  /// @brief Alias of prefix_query().
  [[nodiscard]] auto prefix_prod(I32 r) const -> Value { return prefix_query(r); }

  /// @brief Returns the group product on [l, r).
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

  /// @brief Applies a point delta through the group operation.
  void apply(I32 idx, const Value& delta) {
    my_assert(0 <= idx && idx < n);
    total = Group::combine(total, delta);
    for (++idx; idx <= n; idx += idx & -idx) {
      tree[idx - 1] = Group::combine(tree[idx - 1], delta);
    }
  }

  /// @brief Alias of apply().
  void add(I32 idx, const Value& delta) { apply(idx, delta); }

  /// @brief Alias of apply().
  void multiply(I32 idx, const Value& delta) { apply(idx, delta); }

  /// @brief Overwrites the value at @p idx.
  void set(I32 idx, const Value& value) {
    apply(idx, Group::combine(Group::inverse(get(idx)), value));
  }

  /// @brief Returns the total product on [0, n).
  [[nodiscard]] auto prod_all() const -> Value { return total; }

  /// @brief Alias of prod_all().
  [[nodiscard]] auto query_all() const -> Value { return prod_all(); }

  /// @brief Finds the largest @p r with @p l <= r <= n and check(query(l, r)) true.
  ///
  /// The predicate must be monotone over @p r and satisfy check(identity()).
  /// This implementation prioritizes a clean generic interface over the last
  /// logarithmic factor, so it runs in O(log^2 n).
  template <typename F>
  [[nodiscard]] auto max_right(F check, I32 l = 0) const -> I32 {
    l = std::clamp<I32>(l, 0, n);
    my_assert(check(Group::identity()));
    I32 valid_right = l;
    I32 invalid_right = n + 1;
    while (invalid_right - valid_right > 1) {
      const I32 mid = valid_right + (invalid_right - valid_right) / 2;
      if (check(query(l, mid))) {
        valid_right = mid;
      } else {
        invalid_right = mid;
      }
    }
    return valid_right;
  }

  /// @brief Finds the smallest @p l with 0 <= l <= r and check(query(l, r)) true.
  ///
  /// The predicate must be monotone over @p l and satisfy check(identity()).
  /// This implementation prioritizes a clean generic interface over the last
  /// logarithmic factor, so it runs in O(log^2 n).
  template <typename F>
  [[nodiscard]] auto min_left(F check, I32 r) const -> I32 {
    r = std::clamp<I32>(r, 0, n);
    my_assert(check(Group::identity()));
    I32 valid_left = r;
    I32 invalid_left = -1;
    while (valid_left - invalid_left > 1) {
      const I32 mid = invalid_left + (valid_left - invalid_left) / 2;
      if (check(query(mid, r))) {
        valid_left = mid;
      } else {
        invalid_left = mid;
      }
    }
    return valid_left;
  }
};

#endif
