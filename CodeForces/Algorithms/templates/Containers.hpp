#pragma once
#include <queue>
#include <utility>

#include "Macros.hpp"
#include "Types.hpp"

//===----------------------------------------------------------------------===//
/* Container Utilities and Algorithms */

// Enhanced binary search:
template <typename F>
I64 binary_search(F&& predicate, I64 left, I64 right) {
  while (std::abs(left - right) > 1) {
    I64 mid                         = left + (right - left) / 2;  // Avoid overflow.
    (predicate(mid) ? left : right) = mid;
  }
  return left;
}

template <typename F>
F64 binary_search_real(F&& predicate, F64 left, F64 right, I32 iterations = 100) {
  FOR(iterations) {
    F64 mid                         = left + (right - left) / 2;
    (predicate(mid) ? left : right) = mid;
  }
  return left + (right - left) / 2;
}

// Container manipulation utilities:
template <typename T>
Vec<I32> argsort(const Vec<T>& v, bool reverse = false) {
  Vec<I32> indices(sz(v));
  std::iota(all(indices), 0);
  if (reverse) {
    std::ranges::sort(indices, [&](I32 i, I32 j) { return v[i] == v[j] ? i < j : v[i] > v[j]; });
  } else {
    std::ranges::sort(indices, [&](I32 i, I32 j) { return v[i] == v[j] ? i < j : v[i] < v[j]; });
  }
  return indices;
}

template <typename T>
Vec<T> rearrange(const Vec<T>& v, const Vec<I32>& indices) {
  Vec<T> result(sz(indices));
  FOR(i, sz(indices)) result[i] = v[indices[i]];
  return result;
}

template <typename T>
Vec<T> cumsum(const Vec<T>& v, bool include_zero = true) {
  Vec<T> result(sz(v) + (include_zero ? 1 : 0));
  if (v.empty()) return result;
  if (include_zero) {
    FOR(i, sz(v)) result[i + 1] = result[i] + v[i];
  } else {
    result[0]                  = v[0];
    FOR(i, 1, sz(v)) result[i] = result[i - 1] + v[i];
  }
  return result;
}

// pop_val utilities for different containers (move-aware):
template <typename T>
T pop_val(std::deque<T>& container) {
  my_assert(!container.empty());
  T element = std::move(container.front());
  container.pop_front();
  return element;
}

template <typename T>
T pop_val(MinPriorityQueue<T>& container) {
  my_assert(!container.empty());
  T element = std::move(container.top());
  container.pop();
  return element;
}

template <typename T>
T pop_val(PriorityQueue<T>& container) {
  my_assert(!container.empty());
  T element = std::move(container.top());
  container.pop();
  return element;
}

template <typename T>
T pop_val(Vec<T>& container) {
  my_assert(!container.empty());
  T element = std::move(container.back());
  container.pop_back();
  return element;
}

// Legacy POP wrappers for backward compatibility:
template <typename T>
T POP(std::deque<T>& container) {
  return pop_val(container);
}

template <typename T>
T POP(MinPriorityQueue<T>& container) {
  return pop_val(container);
}

template <typename T>
T POP(PriorityQueue<T>& container) {
  return pop_val(container);
}

template <typename T>
T POP(Vec<T>& container) {
  return pop_val(container);
}

// String utilities:
inline Vec<I32> string_to_ints(const std::string& s, char base_char = 'a') {
  Vec<I32> result(sz(s));
  FOR(i, sz(s)) {
    result[i] = s[i] == '?' ? -1 : s[i] - base_char;
  }
  return result;
}
