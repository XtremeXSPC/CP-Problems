#ifndef CP_MODULES_NUMBER_THEORY_LINEAR_DIOPHANTINE_HPP
#define CP_MODULES_NUMBER_THEORY_LINEAR_DIOPHANTINE_HPP

#include "_Common.hpp"
#include "Euclid.hpp"

/// @brief Helpers for linear Diophantine equations: 'a * x + b * y = c'.
namespace linear_diophantine {

/// @brief Finds any solution (x0, y0) and gcd g for a*x + b*y = c.
inline bool find_any_solution(I64 a, I64 b, I64 c, I64& x0, I64& y0, I64& g) {
  auto [gg, x, y] = extended_gcd<I64>(std::llabs(a), std::llabs(b));
  g = std::llabs(gg);
  if (g == 0) {
    if (c != 0) return false;
    x0 = 0;
    y0 = 0;
    return true;
  }
  if (c % g != 0) return false;

  x0 = x * (c / g);
  y0 = y * (c / g);
  if (a < 0) x0 = -x0;
  if (b < 0) y0 = -y0;
  return true;
}

/// @brief Shifts one solution by k along the full solution family.
inline void shift_solution(I64& x, I64& y, I64 a, I64 b, I64 k) {
  x += k * b;
  y -= k * a;
}

/// @brief Counts integer solutions in box constraints.
inline I64 count_solutions(
    I64 a, I64 b, I64 c, I64 min_x, I64 max_x, I64 min_y, I64 max_y) {
  if (min_x > max_x || min_y > max_y) return 0;
  if (a == 0 && b == 0) {
    if (c != 0) return 0;
    return (max_x - min_x + 1) * (max_y - min_y + 1);
  }
  if (a == 0) {
    if (c % b != 0) return 0;
    I64 y = c / b;
    return (min_y <= y && y <= max_y) ? (max_x - min_x + 1) : 0;
  }
  if (b == 0) {
    if (c % a != 0) return 0;
    I64 x = c / a;
    return (min_x <= x && x <= max_x) ? (max_y - min_y + 1) : 0;
  }

  I64 x = 0, y = 0, g = 0;
  if (!find_any_solution(a, b, c, x, y, g)) return 0;

  a /= g;
  b /= g;

  auto sign = [](I64 v) { return v < 0 ? -1 : 1; };
  I64 sign_a = sign(a);
  I64 sign_b = sign(b);

  shift_solution(x, y, a, b, (min_x - x) / b);
  if (x < min_x) shift_solution(x, y, a, b, sign_b);
  if (x > max_x) return 0;
  I64 lx1 = x;

  shift_solution(x, y, a, b, (max_x - x) / b);
  if (x > max_x) shift_solution(x, y, a, b, -sign_b);
  I64 rx1 = x;

  shift_solution(x, y, a, b, -(min_y - y) / a);
  if (y < min_y) shift_solution(x, y, a, b, -sign_a);
  if (y > max_y) return 0;
  I64 lx2 = x;

  shift_solution(x, y, a, b, -(max_y - y) / a);
  if (y > max_y) shift_solution(x, y, a, b, sign_a);
  I64 rx2 = x;

  if (lx2 > rx2) std::swap(lx2, rx2);
  I64 lx = std::max(lx1, lx2);
  I64 rx = std::min(rx1, rx2);

  if (lx > rx) return 0;
  return (rx - lx) / std::llabs(b) + 1;
}

}  // namespace linear_diophantine

#endif
