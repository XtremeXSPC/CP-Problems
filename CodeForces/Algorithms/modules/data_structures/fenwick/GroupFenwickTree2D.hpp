#ifndef CP_MODULES_DATA_STRUCTURES_GROUP_FENWICK_TREE_2D_HPP
#define CP_MODULES_DATA_STRUCTURES_GROUP_FENWICK_TREE_2D_HPP

#include "../_Common.hpp"

/// @brief Dense 2D Fenwick tree over a commutative group on half-open rectangles.
template <typename Group>
struct GroupFenwickTree2D {
  using Value = typename Group::value_type;

  I32 rows = 0;
  I32 cols = 0;
  Vec<Value> tree;

  GroupFenwickTree2D() = default;

  /// @brief Constructs an identity-filled tree with @p rows_ x @p cols_ cells.
  GroupFenwickTree2D(I32 rows_, I32 cols_) { build(rows_, cols_); }

  /// @brief Constructs a tree from a cell-value generator.
  template <typename F>
  GroupFenwickTree2D(I32 rows_, I32 cols_, F init) {
    build(rows_, cols_, init);
  }

  /// @brief Constructs a tree from explicit cell values.
  explicit GroupFenwickTree2D(const Vec2D<Value>& values) { build(values); }

  /// @brief Reinitializes the tree with @p rows_ x @p cols_ identity values.
  void build(I32 rows_, I32 cols_) {
    rows = std::max<I32>(rows_, 0);
    cols = std::max<I32>(cols_, 0);
    tree.assign(rows * cols, Group::identity());
  }

  /// @brief Reinitializes the tree from explicit rectangular cell values.
  void build(const Vec2D<Value>& values) {
    const I32 rows_ = isz(values);
    const I32 cols_ = rows_ == 0 ? 0 : isz(values[0]);
    FOR(x, rows_) my_assert(isz(values[x]) == cols_);
    build(rows_, cols_, [&](I32 x, I32 y) -> Value { return values[x][y]; });
  }

  /// @brief Reinitializes the tree from a cell-value generator.
  template <typename F>
  void build(I32 rows_, I32 cols_, F init) {
    build(rows_, cols_);
    FOR(x, rows) {
      FOR(y, cols) {
        tree[index(x + 1, y + 1)] = init(x, y);
      }
    }
    FOR(x, 1, rows + 1) {
      FOR(y, 1, cols + 1) {
        const I32 ny = y + (y & -y);
        if (ny <= cols) {
          tree[index(x, ny)] = Group::combine(tree[index(x, ny)], tree[index(x, y)]);
        }
      }
    }
    FOR(x, 1, rows + 1) {
      FOR(y, 1, cols + 1) {
        const I32 nx = x + (x & -x);
        if (nx <= rows) {
          tree[index(nx, y)] = Group::combine(tree[index(nx, y)], tree[index(x, y)]);
        }
      }
    }
  }

  /// @brief Applies a point delta at cell (@p x, @p y).
  void apply(I32 x, I32 y, const Value& delta) {
    my_assert(0 <= x && x < rows);
    my_assert(0 <= y && y < cols);
    for (I32 i = x + 1; i <= rows; i += i & -i) {
      for (I32 j = y + 1; j <= cols; j += j & -j) {
        tree[index(i, j)] = Group::combine(tree[index(i, j)], delta);
      }
    }
  }

  /// @brief Alias of apply().
  void add(I32 x, I32 y, const Value& delta) { apply(x, y, delta); }

  /// @brief Alias of apply().
  void multiply(I32 x, I32 y, const Value& delta) { apply(x, y, delta); }

  /// @brief Returns the group product on the prefix rectangle [0, rx) x [0, ry).
  [[nodiscard]] auto prefix_query(I32 rx, I32 ry) const -> Value {
    rx = std::clamp<I32>(rx, 0, rows);
    ry = std::clamp<I32>(ry, 0, cols);
    Value res = Group::identity();
    for (I32 i = rx; i > 0; i -= i & -i) {
      for (I32 j = ry; j > 0; j -= j & -j) {
        res = Group::combine(res, tree[index(i, j)]);
      }
    }
    return res;
  }

  /// @brief Alias of prefix_query().
  [[nodiscard]] auto prefix_prod(I32 rx, I32 ry) const -> Value { return prefix_query(rx, ry); }

  /// @brief Returns the group product on [lx, rx) x [ly, ry).
  [[nodiscard]] auto query(I32 lx, I32 rx, I32 ly, I32 ry) const -> Value {
    lx = std::clamp<I32>(lx, 0, rows);
    rx = std::clamp<I32>(rx, 0, rows);
    ly = std::clamp<I32>(ly, 0, cols);
    ry = std::clamp<I32>(ry, 0, cols);
    if (lx >= rx || ly >= ry) return Group::identity();
    return Group::combine(
      Group::combine(
        prefix_query(rx, ry),
        Group::inverse(prefix_query(lx, ry))
      ),
      Group::combine(
        Group::inverse(prefix_query(rx, ly)),
        prefix_query(lx, ly)
      )
    );
  }

  /// @brief Alias of query().
  [[nodiscard]] auto prod(I32 lx, I32 rx, I32 ly, I32 ry) const -> Value {
    return query(lx, rx, ly, ry);
  }

  /// @brief Returns the point value at cell (@p x, @p y).
  [[nodiscard]] auto get(I32 x, I32 y) const -> Value { return query(x, x + 1, y, y + 1); }

  /// @brief Materializes all point values.
  [[nodiscard]] auto get_all() const -> Vec2D<Value> {
    Vec2D<Value> values(rows, Vec<Value>(cols));
    FOR(x, rows) FOR(y, cols) values[x][y] = get(x, y);
    return values;
  }

  /// @brief Overwrites the value at cell (@p x, @p y).
  void set(I32 x, I32 y, const Value& value) {
    apply(x, y, Group::combine(Group::inverse(get(x, y)), value));
  }

private:
  [[nodiscard]] auto index(I32 x, I32 y) const -> I32 { return (x - 1) * cols + (y - 1); }
};

#endif
