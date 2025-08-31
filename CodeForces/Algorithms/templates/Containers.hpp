#pragma once
#include "Types.hpp"
#include "Macros.hpp"

//===----------------------------------------------------------------------===//
/* Container Utilities and Algorithms */

// Enhanced binary search:
template <typename F>
I64 binary_search(F&& predicate, I64 left, I64 right) {
  while (std::abs(left - right) > 1) {
    I64 mid = left + (right - left) / 2;  // Avoid overflow
    (predicate(mid) ? left : right) = mid;
  }
  return left;
}

template <typename F>
F64 binary_search_real(F&& predicate, F64 left, F64 right, I32 iterations = 100) {
  FOR(iterations) {
    F64 mid = left + (right - left) / 2;
    (predicate(mid) ? left : right) = mid;
  }
  return left + (right - left) / 2;
}

// Container manipulation utilities:
template <typename T>
VC<I32> argsort(const VC<T>& v, bool reverse = false) {
  VC<I32> indices(sz(v));
  std::iota(all(indices), 0);
  if (reverse) {
    std::ranges::sort(indices, [&](I32 i, I32 j) {
      return v[i] == v[j] ? i < j : v[i] > v[j];
    });
  } else {
    std::ranges::sort(indices, [&](I32 i, I32 j) {
      return v[i] == v[j] ? i < j : v[i] < v[j];
    });
  }
  return indices;
}

template <typename T>
VC<T> rearrange(const VC<T>& v, const VC<I32>& indices) {
  VC<T> result(sz(indices));
  FOR(i, sz(indices)) result[i] = v[indices[i]];
  return result;
}

template <typename T>
VC<T> cumsum(const VC<T>& v, bool include_zero = true) {
  VC<T> result(sz(v) + (include_zero ? 1 : 0));
  if (include_zero) {
    FOR(i, sz(v)) result[i + 1] = result[i] + v[i];
  } else {
    result[0] = v[0];
    FOR(i, 1, sz(v)) result[i] = result[i - 1] + v[i];
  }
  return result;
}

// POP utilities for different containers:
template <typename T>
T POP(std::deque<T>& container) {
  T element = container.front();
  container.pop_front();
  return element;
}

template <typename T>
T POP(PQ_min<T>& container) {
  T element = container.top();
  container.pop();
  return element;
}

template <typename T>
T POP(PQ_max<T>& container) {
  T element = container.top();
  container.pop();
  return element;
}

template <typename T>
T POP(VC<T>& container) {
  T element = container.back();
  container.pop_back();
  return element;
}

// String utilities:
VC<I32> string_to_ints(const std::string& s, char base_char = 'a') {
  VC<I32> result(sz(s));
  FOR(i, sz(s)) {
    result[i] = s[i] == '?' ? -1 : s[i] - base_char;
  }
  return result;
}
