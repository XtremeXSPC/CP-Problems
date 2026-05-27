#ifndef CP_MODULES_DATA_STRUCTURES_UTILITY_CUMSUM_2D_HPP
#define CP_MODULES_DATA_STRUCTURES_UTILITY_CUMSUM_2D_HPP

#include "../_Common.hpp"

/// @brief 2D prefix products over a commutative group.
template <typename Group>
struct Cumsum2D {
  using Value = typename Group::value_type;

  I32 rows = 0;
  I32 cols = 0;
  Vec<Value> pref;

  Cumsum2D() = default;

  explicit Cumsum2D(const Vec2D<Value>& a) { build(a); }

  template <typename F>
  Cumsum2D(I32 rows_, I32 cols_, F init) {
    build(rows_, cols_, init);
  }

  /// @brief Builds from a rectangular matrix.
  void build(const Vec2D<Value>& a) {
    const I32 h = isz(a);
    const I32 w = h == 0 ? 0 : isz(a[0]);
    build(h, w, [&](I32 i, I32 j) -> Value { return a[i][j]; });
  }

  /// @brief Builds from a cell generator.
  template <typename F>
  void build(I32 rows_, I32 cols_, F init) {
    my_assert(rows_ >= 0 && cols_ >= 0);
    rows = rows_;
    cols = cols_;
    pref.assign((rows + 1) * (cols + 1), Group::identity());
    FOR(i, rows) {
      FOR(j, cols) {
        const I32 p = index(i + 1, j + 1);
        pref[p] = Group::combine(
            Group::combine(pref[index(i, j + 1)], pref[index(i + 1, j)]),
            Group::combine(Group::inverse(pref[index(i, j)]), init(i, j)));
      }
    }
  }

  /// @brief Product on prefix rectangle [0, x) x [0, y).
  [[nodiscard]] auto prefix(I32 x, I32 y) const -> Value {
    my_assert(0 <= x && x <= rows);
    my_assert(0 <= y && y <= cols);
    return pref[index(x, y)];
  }

  /// @brief Product on half-open rectangle [x1, x2) x [y1, y2).
  [[nodiscard]] auto query(I32 x1, I32 y1, I32 x2, I32 y2) const -> Value {
    my_assert(0 <= x1 && x1 <= x2 && x2 <= rows);
    my_assert(0 <= y1 && y1 <= y2 && y2 <= cols);
    return Group::combine(
        Group::combine(prefix(x2, y2), prefix(x1, y1)),
        Group::inverse(Group::combine(prefix(x1, y2), prefix(x2, y1))));
  }

  /// @brief Alias for query().
  [[nodiscard]] auto sum(I32 x1, I32 y1, I32 x2, I32 y2) const -> Value {
    return query(x1, y1, x2, y2);
  }

private:
  [[nodiscard]] auto index(I32 x, I32 y) const -> I32 { return x * (cols + 1) + y; }
};

#endif
