#ifndef CP_MODULES_DATA_STRUCTURES_MONOID_SPARSE_TABLE_HPP
#define CP_MODULES_DATA_STRUCTURES_MONOID_SPARSE_TABLE_HPP

#include "../_Common.hpp"

/// @brief Static sparse table for idempotent monoids.
template <typename Monoid>
struct MonoidSparseTable {
  using Value = typename Monoid::value_type;

  I32 n = 0;
  I32 levels = 0;
  VecI32 lg;
  Vec2D<Value> table;

  MonoidSparseTable() = default;
  explicit MonoidSparseTable(I32 size_) { build(size_); }

  template <typename F>
  MonoidSparseTable(I32 size_, F init) {
    build(size_, init);
  }

  explicit MonoidSparseTable(const Vec<Value>& values) { build(values); }

  /// @brief Initializes a table of size @p size_ filled with the monoid identity.
  void build(I32 size_) {
    build(size_, [](I32) -> Value { return Monoid::identity(); });
  }

  /// @brief Initializes a table from a vector.
  void build(const Vec<Value>& values) {
    build(isz(values), [&](I32 i) -> Value { return values[i]; });
  }

  /// @brief Initializes a table from a point-value generator.
  template <typename F>
  void build(I32 size_, F init) {
    n = std::max<I32>(size_, 0);
    levels = 0;
    while ((1 << levels) < std::max<I32>(n, 1)) ++levels;
    lg.assign(n + 1, 0);
    FOR(i, 2, n + 1) lg[i] = lg[i >> 1] + 1;
    table.assign(std::max<I32>(levels, 1), {});
    table[0].resize(n);
    FOR(i, n) table[0][i] = init(i);

    FOR(k, levels) {
      if (k + 1 >= isz(table)) break;
      const I32 len = isz(table[k]) - (1 << k);
      if (len <= 0) {
        table[k + 1].clear();
        continue;
      }
      table[k + 1].resize(len);
      FOR(i, len) {
        table[k + 1][i] = Monoid::combine(table[k][i], table[k][i + (1 << k)]);
      }
    }
  }

  /// @brief Half-open range query on [l, r). Requires an idempotent monoid.
  [[nodiscard]] auto query(I32 l, I32 r) const -> Value {
    my_assert(0 <= l && l <= r && r <= n);
    if (l == r) return Monoid::identity();
    if (r == l + 1) return table[0][l];
    const I32 k = lg[r - l];
    return Monoid::combine(table[k][l], table[k][r - (1 << k)]);
  }

  [[nodiscard]] auto prod(I32 l, I32 r) const -> Value { return query(l, r); }

  /// @brief Largest `r` such that `check(query(l, r))` holds.
  template <typename F>
  [[nodiscard]] auto max_right(I32 l, F check) const -> I32 {
    my_assert(0 <= l && l <= n);
    my_assert(check(Monoid::identity()));
    I32 last = l;
    I32 ng = n + 1;
    while (last + 1 < ng) {
      const I32 mid = (last + ng) >> 1;
      if (check(query(l, mid))) last = mid;
      else ng = mid;
    }
    return last;
  }

  /// @brief Smallest `l` such that `check(query(l, r))` holds.
  template <typename F>
  [[nodiscard]] auto min_left(I32 r, F check) const -> I32 {
    my_assert(0 <= r && r <= n);
    my_assert(check(Monoid::identity()));
    I32 first = r;
    I32 ng = -1;
    while (ng + 1 < first) {
      const I32 mid = (first + ng) >> 1;
      if (check(query(mid, r))) first = mid;
      else ng = mid;
    }
    return first;
  }
};

#endif
