#ifndef CP_MODULES_GEOMETRY_CIRCLE_HPP
#define CP_MODULES_GEOMETRY_CIRCLE_HPP

#include "_Common.hpp"
#include "Point2D.hpp"

/**
 * @brief Circle primitive with utility constructors and predicates.
 */
template <typename T = F64>
struct Circle {
  Point2D<T> center;
  T radius;

  Circle(Point2D<T> c = Point2D<T>(), T r = 0) : center(c), radius(r) {}

  /**
   * @brief Circle area.
   */
  T area() const { return PI * radius * radius; }
  /**
   * @brief Circle circumference.
   */
  T circumference() const { return 2 * PI * radius; }

  /**
   * @brief Checks whether point lies inside or on boundary.
   */
  bool contains(const Point2D<T>& p) const {
    return (p - center).norm2() <= radius * radius + EPS;
  }

  /**
   * @brief Circumcircle through three non-collinear points.
   */
  static Circle from_points(const Point2D<T>& a, const Point2D<T>& b, const Point2D<T>& c) {
    T d = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
    if (abs(d) < EPS) return Circle();  // Points are collinear

    T a2 = a.norm2(), b2 = b.norm2(), c2 = c.norm2();
    T x = (a2 * (b.y - c.y) + b2 * (c.y - a.y) + c2 * (a.y - b.y)) / d;
    T y = (a2 * (c.x - b.x) + b2 * (a.x - c.x) + c2 * (b.x - a.x)) / d;

    Point2D<T> center(x, y);
    return Circle(center, (center - a).norm());
  }
};

/**
 * @brief Circle through two points (diameter).
 */
template <typename T>
Circle<T> circle_from_two(const Point2D<T>& a, const Point2D<T>& b) {
  Point2D<T> center((a.x + b.x) / 2, (a.y + b.y) / 2);
  return Circle<T>(center, (a - b).norm() / 2);
}

/**
 * @brief Minimum enclosing circle via Welzl's randomized incremental algorithm.
 * @param points Input points (taken by value for internal shuffle).
 * @return Smallest circle containing all points. O(n) expected time.
 */
template <typename T>
Circle<T> min_enclosing_circle(Vec<Point2D<T>> points) {
  static_assert(std::is_floating_point_v<T>, "min_enclosing_circle requires floating-point type");

  I32 n = (I32)points.size();
  if (n == 0) return Circle<T>();
  if (n == 1) return Circle<T>(points[0], 0);

  std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
  std::shuffle(all(points), rng);

  Circle<T> c(points[0], 0);
  FOR(i, 1, n) {
    if (c.contains(points[i])) continue;
    c = Circle<T>(points[i], 0);
    FOR(j, i) {
      if (c.contains(points[j])) continue;
      c = circle_from_two(points[i], points[j]);
      FOR(k, j) {
        if (c.contains(points[k])) continue;
        c = Circle<T>::from_points(points[i], points[j], points[k]);
      }
    }
  }
  return c;
}

#endif
