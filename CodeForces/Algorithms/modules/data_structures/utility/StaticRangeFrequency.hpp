#ifndef CP_MODULES_DATA_STRUCTURES_UTILITY_STATIC_RANGE_FREQUENCY_HPP
#define CP_MODULES_DATA_STRUCTURES_UTILITY_STATIC_RANGE_FREQUENCY_HPP

#include "IndexCompression.hpp"

/// @brief Static frequency queries: count value `x` in A[l, r).
template <typename T>
struct StaticRangeFrequency {
  I32 n = 0;
  IndexCompression<T> comp;
  Vec<VecI32> pos;

  StaticRangeFrequency() = default;

  explicit StaticRangeFrequency(const Vec<T>& a) { build(a); }

  template <typename F>
  StaticRangeFrequency(I32 size_, F init) {
    build(size_, init);
  }

  /// @brief Builds from explicit values.
  void build(const Vec<T>& a) {
    n = isz(a);
    comp.build(a);
    pos.assign(comp.size(), {});
    FOR(i, n) pos[comp.id(a[i])].push_back(i);
  }

  /// @brief Builds from a value generator.
  template <typename F>
  void build(I32 size_, F init) {
    my_assert(size_ >= 0);
    Vec<T> a(size_);
    FOR(i, size_) a[i] = init(i);
    build(a);
  }

  /// @brief Count occurrences of `x` in [l, r).
  [[nodiscard]] auto query(I32 l, I32 r, const T& x) const -> I32 {
    my_assert(0 <= l && l <= r && r <= n);
    if (!comp.contains(x)) return 0;
    const VecI32& p = pos[comp.id(x)];
    return I32(std::ranges::lower_bound(p, r) - std::ranges::lower_bound(p, l));
  }

  /// @brief Count values in [lo, hi) inside index range [l, r).
  [[nodiscard]] auto range_query(I32 l, I32 r, const T& lo, const T& hi) const -> I32 {
    my_assert(0 <= l && l <= r && r <= n);
    if (!(lo < hi)) return 0;
    I32 ans = 0;
    for (I32 id = comp.rank(lo); id < comp.rank(hi); ++id) {
      const VecI32& p = pos[id];
      ans += I32(std::ranges::lower_bound(p, r) - std::ranges::lower_bound(p, l));
    }
    return ans;
  }
};

#endif
