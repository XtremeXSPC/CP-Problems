#ifndef CP_MODULES_GRAPH_TREE_TREE_MONOID_HPP
#define CP_MODULES_GRAPH_TREE_TREE_MONOID_HPP

#include "TreeCore.hpp"
#include "../../data_structures/segment_tree/MonoidSegmentTree.hpp"

/// @brief Monoid with reversed argument order for upward tree segments.
template <typename Monoid>
struct TreeMonoidReverse {
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
 * @brief Dynamic path/subtree products on a tree using TreeCore + segment tree.
 *
 * @tparam EdgeMode false for vertex values, true for edge values indexed by id.
 * @details Non-commutative monoids are supported by maintaining a reversed
 * segment tree for upward HLD segments.
 */
template <typename Core, typename Monoid, bool EdgeMode = false>
struct TreeMonoid {
  using Value = typename Monoid::value_type;

  const Core& tree;
  MonoidSegmentTree<Monoid> seg;
  MonoidSegmentTree<TreeMonoidReverse<Monoid>> seg_rev;

  explicit TreeMonoid(const Core& tree_) : tree(tree_) { build([](I32) { return Monoid::identity(); }); }

  explicit TreeMonoid(const Core& tree_, const Vec<Value>& values) : tree(tree_) {
    build([&](I32 i) { return values[i]; });
  }

  template <typename F>
  TreeMonoid(const Core& tree_, F init) : tree(tree_) {
    build(init);
  }

  /// @brief Rebuilds from a vertex index or edge id generator.
  template <typename F>
  void build(F init) {
    seg.build(tree.n, [&](I32 i) -> Value {
      const I32 v = tree.order[i];
      if constexpr (EdgeMode) {
        if (tree.parent[v] == -1) return Monoid::identity();
        return init(tree.edge_to_parent[v]);
      } else {
        return init(v);
      }
    });
    if constexpr (!Monoid::commute) {
      seg_rev.build(seg.get_all());
    }
  }

  /// @brief Point assignment by vertex or edge id, depending on EdgeMode.
  void set(I32 idx, const Value& value) {
    I32 v = idx;
    if constexpr (EdgeMode) {
      v = tree.edge_child(idx);
      my_assert(v != -1);
    }
    seg.set(tree.pos[v], value);
    if constexpr (!Monoid::commute) seg_rev.set(tree.pos[v], value);
  }

  /// @brief Point multiplication by vertex or edge id, depending on EdgeMode.
  void multiply(I32 idx, const Value& value) {
    I32 v = idx;
    if constexpr (EdgeMode) {
      v = tree.edge_child(idx);
      my_assert(v != -1);
    }
    seg.multiply(tree.pos[v], value);
    if constexpr (!Monoid::commute) seg_rev.multiply(tree.pos[v], value);
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
