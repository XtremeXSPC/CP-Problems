#ifndef CP_MODULES_GEOMETRY_POINT2D_HPP
#define CP_MODULES_GEOMETRY_POINT2D_HPP

#include "_Common.hpp"

/// @brief 2D point/vector with standard vector operations.
template <typename T = F64>
struct Point2D {
  T x, y;

  Point2D(T x = 0, T y = 0) : x(x), y(y) {}

  Point2D operator+(const Point2D& p) const { return Point2D(x + p.x, y + p.y); }
  Point2D operator-(const Point2D& p) const { return Point2D(x - p.x, y - p.y); }
  Point2D operator*(T k) const { return Point2D(x * k, y * k); }
  Point2D operator/(T k) const { return Point2D(x / k, y / k); }

  T dot(const Point2D& p) const { return x * p.x + y * p.y; }
  T cross(const Point2D& p) const { return x * p.y - y * p.x; }
  T norm2() const { return x * x + y * y; }
  T norm() const { return sqrt(norm2()); }

  /// @brief Rotates the vector by angle (radians) around origin.
  Point2D rotate(T angle) const {
    T c = cos(angle), s = sin(angle);
    return Point2D(x * c - y * s, x * s + y * c);
  }

  ///@brief Returns perpendicular vector (-y, x).
  Point2D perp() const { return Point2D(-y, x); }

  bool operator<(const Point2D& p) const {
    return x != p.x ? x < p.x : y < p.y;
  }

  bool operator==(const Point2D& p) const {
    return abs(x - p.x) < EPS && abs(y - p.y) < EPS;
  }
};

/// @brief Oriented area sign of triangle (a, b, c).
template <typename T>
T orientation(const Point2D<T>& a, const Point2D<T>& b, const Point2D<T>& c) {
  return (b - a).cross(c - a);
}

/// @brief Checks whether point p lies on segment [a, b].
template <typename T>
bool on_segment(const Point2D<T>& p, const Point2D<T>& a, const Point2D<T>& b) {
  return abs((b - a).cross(p - a)) < EPS &&
         (p - a).dot(p - b) <= EPS;
}

#endif
