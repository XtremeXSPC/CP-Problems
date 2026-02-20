#ifndef CP_MODULES_DATA_STRUCTURES_SQRT_DECOMPOSITION_HPP
#define CP_MODULES_DATA_STRUCTURES_SQRT_DECOMPOSITION_HPP

#include "_Common.hpp"

/**
 * @brief Sqrt decomposition for point updates and range-sum queries.
 *
 * @details Maintains an array split into blocks of size about sqrt(n).
 * Supports:
 *  - point_set / point_add in O(1)
 *  - range_sum([l, r)) in O(sqrt(n))
 */
template <typename T = I64>
struct SqrtDecomposition {
  I32 n = 0;
  I32 block_size = 1;
  I32 block_count = 0;
  Vec<T> data;
  Vec<T> block_sum;

  SqrtDecomposition() = default;

  explicit SqrtDecomposition(I32 n, T initial_value = T{}) { init(n, initial_value); }

  explicit SqrtDecomposition(const Vec<T>& values) { build(values); }

  void init(I32 size, T initial_value = T{}) {
    n = std::max<I32>(size, 0);
    block_size = std::max<I32>(1, static_cast<I32>(std::sqrt(static_cast<F80>(n)) + 1));
    block_count = (n + block_size - 1) / block_size;
    data.assign(static_cast<Size>(n), initial_value);
    block_sum.assign(static_cast<Size>(block_count), T{});
    FOR(i, n) block_sum[static_cast<Size>(i / block_size)] += initial_value;
  }

  void build(const Vec<T>& values) {
    init(static_cast<I32>(values.size()));
    data = values;
    std::fill(all(block_sum), T{});
    FOR(i, n) block_sum[static_cast<Size>(i / block_size)] += data[static_cast<Size>(i)];
  }

  /// @brief Number of elements.
  I32 size() const { return n; }

  /// @brief Point assignment: a[idx] = value.
  void point_set(I32 idx, T value) {
    if (idx < 0 || idx >= n) return;
    I32 b = idx / block_size;
    block_sum[static_cast<Size>(b)] += value - data[static_cast<Size>(idx)];
    data[static_cast<Size>(idx)] = value;
  }

  /// @brief Point increment: a[idx] += delta.
  void point_add(I32 idx, T delta) {
    if (idx < 0 || idx >= n) return;
    I32 b = idx / block_size;
    data[static_cast<Size>(idx)] += delta;
    block_sum[static_cast<Size>(b)] += delta;
  }

  /// @brief Returns a[idx].
  T get(I32 idx) const {
    if (idx < 0 || idx >= n) return T{};
    return data[static_cast<Size>(idx)];
  }

  /// @brief Range sum on half-open interval [l, r).
  T range_sum(I32 l, I32 r) const {
    l = std::max<I32>(l, 0);
    r = std::min<I32>(r, n);
    if (l >= r) return T{};

    I32 left_block = l / block_size;
    I32 right_block = (r - 1) / block_size;
    T result = T{};

    if (left_block == right_block) {
      FOR(i, l, r) result += data[static_cast<Size>(i)];
      return result;
    }

    I32 left_end = (left_block + 1) * block_size;
    FOR(i, l, left_end) result += data[static_cast<Size>(i)];

    FOR(b, left_block + 1, right_block) result += block_sum[static_cast<Size>(b)];

    I32 right_start = right_block * block_size;
    FOR(i, right_start, r) result += data[static_cast<Size>(i)];

    return result;
  }
};

#endif
