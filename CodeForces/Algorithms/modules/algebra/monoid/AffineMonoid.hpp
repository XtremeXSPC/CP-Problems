#ifndef CP_MODULES_ALGEBRA_MONOID_AFFINE_MONOID_HPP
#define CP_MODULES_ALGEBRA_MONOID_AFFINE_MONOID_HPP

#include "modules/algebra/_Common.hpp"

/// @brief Monoid over affine maps 'f(x) = a*x + b'.
///
/// 'combine(f, g)' means "apply 'f', then apply 'g'". This matches lazy action
/// composition in this library: an already pending action is the left argument,
/// the fresh action is the right argument.
template <typename T>
struct AffineMonoid {
  using value_type = Pair<T, T>;

  static constexpr bool commute = false;

  static constexpr auto identity() -> value_type { return {T(1), T(0)}; }
  static constexpr auto combine(const value_type& f, const value_type& g) -> value_type {
    return {f.first * g.first, f.second * g.first + g.second};
  }
  static constexpr auto eval(const value_type& f, const T& x) -> T {
    return f.first * x + f.second;
  }

  static constexpr auto unit() -> value_type { return identity(); }
  static constexpr auto op(const value_type& a, const value_type& b) -> value_type {
    return combine(a, b);
  }
};

#endif
