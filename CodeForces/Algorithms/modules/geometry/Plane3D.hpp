#ifndef CP_MODULES_GEOMETRY_PLANE3D_HPP
#define CP_MODULES_GEOMETRY_PLANE3D_HPP

#include "_Common.hpp"
#include "Point3D.hpp"

/// @brief Plane in 3D represented as 'a * x + b * y + c * z = d'.
template <typename T = F64>
struct Plane3D {
  T a, b, c, d;

  Plane3D(T a = 0, T b = 0, T c = 0, T d = 0) : a(a), b(b), c(c), d(d) {}

  /// @brief Builds plane from 3 non-collinear points.
  Plane3D(const Point3D<T>& p1, const Point3D<T>& p2, const Point3D<T>& p3) {
    Point3D<T> v1 = p2 - p1;
    Point3D<T> v2 = p3 - p1;
    Point3D<T> normal = v1.cross(v2);

    a = normal.x;
    b = normal.y;
    c = normal.z;
    d = normal.dot(p1);
  }

  /// @brief Distance from point to plane.
  T distance(const Point3D<T>& p) const {
    return abs(a * p.x + b * p.y + c * p.z - d) / sqrt(a * a + b * b + c * c);
  }
};

#endif
