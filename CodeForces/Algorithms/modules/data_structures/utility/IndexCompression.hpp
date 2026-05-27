#ifndef CP_MODULES_DATA_STRUCTURES_INDEX_COMPRESSION_HPP
#define CP_MODULES_DATA_STRUCTURES_INDEX_COMPRESSION_HPP

#include "../_Common.hpp"

/// @brief Coordinate compression over the sorted set of seen values.
template <typename T>
struct IndexCompression {
  Vec<T> values;

  IndexCompression() = default;

  explicit IndexCompression(const Vec<T>& data) { build(data); }

  template <std::ranges::input_range Range>
  explicit IndexCompression(const Range& data) {
    build(data);
  }

  /// @brief Rebuilds from raw values; duplicates are removed.
  void build(const Vec<T>& data) {
    values = data;
    normalize();
  }

  /// @brief Rebuilds from any input range of values.
  template <std::ranges::input_range Range>
  void build(const Range& data) {
    values.assign(std::ranges::begin(data), std::ranges::end(data));
    normalize();
  }

  /// @brief Number of distinct compressed values.
  [[nodiscard]] auto size() const -> I32 { return isz(values); }

  /// @brief Returns whether no value is stored.
  [[nodiscard]] auto empty() const -> bool { return values.empty(); }

  /// @brief Returns the sorted distinct values used by the compressor.
  [[nodiscard]] auto keys() const -> const Vec<T>& { return values; }

  /// @brief Returns the original value for compressed index @p idx.
  [[nodiscard]] auto value(I32 idx) const -> const T& {
    my_assert(0 <= idx && idx < size());
    return values[idx];
  }

  /// @brief Alias for value().
  [[nodiscard]] auto operator[](I32 idx) const -> const T& { return value(idx); }

  /// @brief Number of stored keys strictly smaller than @p x.
  [[nodiscard]] auto rank(const T& x) const -> I32 {
    return I32(std::ranges::lower_bound(values, x) - values.begin());
  }

  /// @brief Alias for rank(), matching common coordinate-compression notation.
  [[nodiscard]] auto operator()(const T& x) const -> I32 { return rank(x); }

  /// @brief Returns whether @p x is present in the compressed set.
  [[nodiscard]] auto contains(const T& x) const -> bool {
    const I32 pos = rank(x);
    return pos < size() && values[pos] == x;
  }

  /// @brief Returns the exact compressed id of @p x.
  [[nodiscard]] auto id(const T& x) const -> I32 {
    const I32 pos = rank(x);
    my_assert(pos < size() && values[pos] == x);
    return pos;
  }

  /// @brief Compresses one vector using this compressor.
  [[nodiscard]] auto compress(const Vec<T>& data) const -> VecI32 {
    VecI32 res(isz(data));
    FOR(i, isz(data)) res[i] = id(data[i]);
    return res;
  }

  /// @brief Builds from @p data and returns its compressed ids.
  [[nodiscard]] auto build_and_compress(const Vec<T>& data) -> VecI32 {
    build(data);
    return compress(data);
  }

private:
  void normalize() {
    std::ranges::sort(values);
    values.erase(std::ranges::unique(values).begin(), values.end());
    values.shrink_to_fit();
  }
};

#endif
