#ifndef CP_MODULES_DATA_STRUCTURES_UTILITY_CSR_HPP
#define CP_MODULES_DATA_STRUCTURES_UTILITY_CSR_HPP

#include "../_Common.hpp"

/// @brief Compressed sparse row buckets preserving insertion order per row.
template <typename T>
struct CSR {
  I32 rows = 0;
  bool built = false;
  VecI32 ptr;
  VecI32 row_ids;
  Vec<T> data;

  CSR() = default;

  explicit CSR(I32 rows_) { reset(rows_); }

  /// @brief Clears all entries and sets the number of rows.
  void reset(I32 rows_) {
    my_assert(rows_ >= 0);
    rows = rows_;
    built = false;
    ptr.clear();
    row_ids.clear();
    data.clear();
  }

  /// @brief Adds one value to a row before build().
  void add(I32 row, const T& value) {
    my_assert(!built);
    my_assert(0 <= row && row < rows);
    row_ids.push_back(row);
    data.push_back(value);
  }

  /// @brief Packs entries into contiguous row slices.
  void build() {
    my_assert(!built);
    built = true;
    ptr.assign(rows + 1, 0);
    for (const I32 row : row_ids) ++ptr[row + 1];
    FOR(i, rows) ptr[i + 1] += ptr[i];

    VecI32 cur = ptr;
    Vec<T> packed(data.size());
    FOR(i, isz(data)) {
      const I32 row = row_ids[i];
      packed[cur[row]++] = data[i];
    }
    data.swap(packed);
    row_ids.clear();
    row_ids.shrink_to_fit();
  }

  /// @brief Returns values stored in one row.
  [[nodiscard]] auto operator[](I32 row) const -> Span<const T> {
    my_assert(built);
    my_assert(0 <= row && row < rows);
    return Span<const T>(data.data() + ptr[row], data.data() + ptr[row + 1]);
  }

  /// @brief Number of entries in one row.
  [[nodiscard]] auto row_size(I32 row) const -> I32 {
    my_assert(built);
    my_assert(0 <= row && row < rows);
    return ptr[row + 1] - ptr[row];
  }

  /// @brief Total number of stored entries.
  [[nodiscard]] auto size() const -> I32 { return isz(data); }
};

#endif
