#ifndef CP_MODULES_DATA_STRUCTURES_ROLLBACK_ARRAY_HPP
#define CP_MODULES_DATA_STRUCTURES_ROLLBACK_ARRAY_HPP

#include "../_Common.hpp"

/// @brief Rollback-friendly random-access array with snapshot support.
template <typename T>
struct RollbackArray {
  Vec<T> values;
  Vec<Pair<I32, T>> history;

  RollbackArray() = default;
  explicit RollbackArray(Vec<T> values_) : values(std::move(values_)) {}

  [[nodiscard]] auto get(I32 idx) const -> const T& { return values[idx]; }

  [[nodiscard]] auto get_all() const -> const Vec<T>& { return values; }

  void set(I32 idx, const T& value) {
    history.push_back({idx, values[idx]});
    values[idx] = value;
  }

  [[nodiscard]] auto snapshot() const -> Size { return history.size(); }

  void rollback(Size snap) {
    while (history.size() > snap) {
      const auto [idx, old_value] = history.back();
      history.pop_back();
      values[idx] = old_value;
    }
  }
};

#endif
