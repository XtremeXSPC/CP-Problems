#ifndef CP_MODULES_GRAPH_TREE_STATIC_TREE_MONOID_HPP
#define CP_MODULES_GRAPH_TREE_STATIC_TREE_MONOID_HPP

#include "TreeCore.hpp"
#include "../../data_structures/range_query/DisjointSparseTable.hpp"

/// @brief Reversed monoid for upward static tree path segments.
template <typename Monoid>
struct StaticTreeMonoidReverse {
  using value_type = typename Monoid::value_type;
  static constexpr bool commute = Monoid::commute;

  static constexpr auto identity() -> value_type { return Monoid::identity(); }
  static constexpr auto combine(const value_type& a, const value_type& b) -> value_type {
    return Monoid::combine(b, a);
  }
  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
};

/**
 * @brief Static path/subtree products on a tree.
 *
 * @tparam EdgeMode false for vertex values, true for edge values indexed by id.
 * @details Uses disjoint sparse tables, so each HLD segment is queried in O(1)
 * for arbitrary associative monoids. Whole path queries take O(log n).
 */
template <typename Core, typename Monoid, bool EdgeMode = false>
struct StaticTreeMonoid {
  using Value = typename Monoid::value_type;

  const Core& tree;
  DisjointSparseTable<Monoid> seg;
  DisjointSparseTable<StaticTreeMonoidReverse<Monoid>> seg_rev;

  explicit StaticTreeMonoid(const Core& tree_) : tree(tree_) {
    build([](I32) { return Monoid::identity(); });
  }

  explicit StaticTreeMonoid(const Core& tree_, const Vec<Value>& values) : tree(tree_) {
    build([&](I32 i) { return values[i]; });
  }

  template <typename F>
  StaticTreeMonoid(const Core& tree_, F init) : tree(tree_) {
    build(init);
  }

  /// @brief Rebuilds from a vertex index or edge id generator.
  template <typename F>
  void build(F init) {
    Vec<Value> values(tree.n, Monoid::identity());
    FOR(i, tree.n) {
      const I32 v = tree.order[i];
      if constexpr (EdgeMode) {
        if (tree.parent[v] != -1) values[i] = init(tree.edge_to_parent[v]);
      } else {
        values[i] = init(v);
      }
    }
    seg.build(values);
    if constexpr (!Monoid::commute) {
      seg_rev.build(values);
    }
  }

  /// @brief Product on path u -> v.
  [[nodiscard]] auto path_prod(I32 u, I32 v) const -> Value {
    Value acc = Monoid::identity();
    for (auto [l, r] : tree.path_segments(u, v, EdgeMode)) {
      acc = Monoid::combine(acc, segment_prod(l, r));
    }
    return acc;
  }

  [[nodiscard]] auto prod_path(I32 u, I32 v) const -> Value { return path_prod(u, v); }

  /// @brief Product over rooted subtree of v.
  [[nodiscard]] auto subtree_prod(I32 v) const -> Value {
    auto [l, r] = tree.subtree_range(v);
    if constexpr (EdgeMode) ++l;
    return l <= r ? seg.query(l, r) : Monoid::identity();
  }

  [[nodiscard]] auto prod_subtree(I32 v) const -> Value { return subtree_prod(v); }

private:
  [[nodiscard]] auto segment_prod(I32 l, I32 r) const -> Value {
    if (l <= r) return seg.query(l, r + 1);
    if constexpr (Monoid::commute) return seg.query(r, l + 1);
    else return seg_rev.query(r, l + 1);
  }
};

#endif
