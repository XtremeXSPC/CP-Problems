#ifndef CP_MODULES_DATA_STRUCTURES_DISJOINT_SPARSE_TABLE_HPP
#define CP_MODULES_DATA_STRUCTURES_DISJOINT_SPARSE_TABLE_HPP

#include "../_Common.hpp"

/**
 * @brief Static range product for any associative monoid.
 *
 * @details Answers half-open range queries [l, r) in O(1) after O(n log n)
 * preprocessing. Unlike a classic sparse table, the monoid need not be
 * idempotent.
 */
template <typename Monoid>
struct DisjointSparseTable {
  using Value = typename Monoid::value_type;

  I32 n = 0;
  I32 levels = 0;
  VecI32 lg;
  Vec2D<Value> table;

  DisjointSparseTable() = default;
  explicit DisjointSparseTable(I32 size_) { build(size_); }

  template <typename F>
  DisjointSparseTable(I32 size_, F init) {
    build(size_, init);
  }

  explicit DisjointSparseTable(const Vec<Value>& values) { build(values); }

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
    lg.assign((1 << levels) + 1, 0);
    FOR(i, 2, isz(lg)) lg[i] = lg[i >> 1] + 1;

    table.assign(std::max<I32>(levels, 1), Vec<Value>(n, Monoid::identity()));
    if (n == 0) return;

    FOR(i, n) table[0][i] = init(i);
    Vec<Value> base = table[0];

    FOR(k, levels) {
      const I32 half = 1 << k;
      const I32 block = half << 1;
      for (I32 left = 0; left < n; left += block) {
        const I32 mid = std::min(left + half, n);
        const I32 right = std::min(left + block, n);
        if (mid == left) continue;

        table[k][mid - 1] = base[mid - 1];
        for (I32 i = mid - 2; i >= left; --i) {
          table[k][i] = Monoid::combine(base[i], table[k][i + 1]);
        }

        if (mid < right) {
          table[k][mid] = base[mid];
          FOR(i, mid + 1, right) {
            table[k][i] = Monoid::combine(table[k][i - 1], base[i]);
          }
        }
      }
    }
  }

  /// @brief Half-open range product on [l, r).
  [[nodiscard]] auto query(I32 l, I32 r) const -> Value {
    my_assert(0 <= l && l <= r && r <= n);
    if (l == r) return Monoid::identity();
    if (r == l + 1) return table[0][l];
    const I32 k = lg[l ^ (r - 1)];
    return Monoid::combine(table[k][l], table[k][r - 1]);
  }

  [[nodiscard]] auto prod(I32 l, I32 r) const -> Value { return query(l, r); }
};

#endif
