#ifndef CP_MODULES_DATA_STRUCTURES_UTILITY_REMOVABLE_QUEUE_HPP
#define CP_MODULES_DATA_STRUCTURES_UTILITY_REMOVABLE_QUEUE_HPP

#include "../_Common.hpp"

/// @brief Queue-like adapter with lazy removals from the front/top.
template <typename Queue>
struct RemovableQueue {
  using Value = typename Queue::value_type;

  Queue data;
  Queue removed;

  RemovableQueue() = default;

  /// @brief Adds one value to the main queue.
  void push(const Value& x) { data.push(x); }

  /// @brief Lazily removes one matching value.
  void remove(const Value& x) { removed.push(x); }

  /// @brief Returns the visible front/top value.
  [[nodiscard]] auto top() -> Value {
    refresh();
    my_assert(!data.empty());
    return data.top();
  }

  /// @brief Pops and returns the visible front/top value.
  auto pop() -> Value {
    refresh();
    my_assert(!data.empty());
    Value x = data.top();
    data.pop();
    return x;
  }

  /// @brief Number of visible elements, assuming each removed value exists.
  [[nodiscard]] auto size() const -> I32 { return I32(data.size() - removed.size()); }

  /// @brief Returns whether no visible element remains.
  [[nodiscard]] auto empty() {
    refresh();
    return data.empty();
  }

private:
  void refresh() {
    while (!data.empty() && !removed.empty() && data.top() == removed.top()) {
      data.pop();
      removed.pop();
    }
  }
};

template <typename T>
using RemovablePriorityQueue = RemovableQueue<PriorityQueue<T>>;

template <typename T>
using RemovableMinPriorityQueue = RemovableQueue<MinPriorityQueue<T>>;

#endif
