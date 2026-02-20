#ifndef CP_MODULES_GEOMETRY_LINE2D_HPP
#define CP_MODULES_GEOMETRY_LINE2D_HPP

#include "_Common.hpp"
#include "Point2D.hpp"

/// @brief 2D line in implicit form a*x + b*y = c.
template <typename T = F64>
struct Line2D {
  T a, b, c;

  Line2D(T a = 0, T b = 0, T c = 0) : a(a), b(b), c(c) {}

  /// @brief Builds line passing through points p1 and p2.
  Line2D(const Point2D<T>& p1, const Point2D<T>& p2) {
    a = p2.y - p1.y;
    b = p1.x - p2.x;
    c = a * p1.x + b * p1.y;
  }

  /// @brief Euclidean distance from point to line.
  T distance(const Point2D<T>& p) const {
    return abs(a * p.x + b * p.y - c) / sqrt(a * a + b * b);
  }

  /// @brief Orthogonal projection of point onto this line.
  Point2D<T> project(const Point2D<T>& p) const {
    T d = a * a + b * b;
    T x = (b * (b * p.x - a * p.y) + a * c) / d;
    T y = (a * (-b * p.x + a * p.y) + b * c) / d;
    return Point2D<T>(x, y);
  }
};

/// @brief Segment intersection test with intersection point extraction.
template <typename T, typename U>
bool segment_intersect(const Point2D<T>& a, const Point2D<T>& b,
                       const Point2D<T>& c, const Point2D<T>& d,
                       Point2D<U>& intersection) {
  static_assert(std::is_floating_point_v<U>, "intersection output type must be floating-point");
  using Calc = std::common_type_t<T, U, F64>;
  Calc d1 = static_cast<Calc>(orientation(c, d, a));
  Calc d2 = static_cast<Calc>(orientation(c, d, b));
  Calc d3 = static_cast<Calc>(orientation(a, b, c));
  Calc d4 = static_cast<Calc>(orientation(a, b, d));

  if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
      ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
    Calc t = d1 / (d1 - d2);
    intersection = Point2D<U>(
      static_cast<U>(a.x + (b.x - a.x) * t),
      static_cast<U>(a.y + (b.y - a.y) * t)
    );
    return true;
  }

  // Check for collinear cases.
  if (abs(d1) < EPS && on_segment(a, c, d)) {
    intersection = Point2D<U>(static_cast<U>(a.x), static_cast<U>(a.y));
    return true;
  }
  if (abs(d2) < EPS && on_segment(b, c, d)) {
    intersection = Point2D<U>(static_cast<U>(b.x), static_cast<U>(b.y));
    return true;
  }
  if (abs(d3) < EPS && on_segment(c, a, b)) {
    intersection = Point2D<U>(static_cast<U>(c.x), static_cast<U>(c.y));
    return true;
  }
  if (abs(d4) < EPS && on_segment(d, a, b)) {
    intersection = Point2D<U>(static_cast<U>(d.x), static_cast<U>(d.y));
    return true;
  }

  return false;
}

#endif
