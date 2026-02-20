#ifndef CP_MODULES_GEOMETRY_POINT3D_HPP
#define CP_MODULES_GEOMETRY_POINT3D_HPP

#include "_Common.hpp"

/// @brief 3D point/vector primitive.
template <typename T = F64>
struct Point3D {
  T x, y, z;

  Point3D(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z) {}

  Point3D operator+(const Point3D& p) const { return Point3D(x + p.x, y + p.y, z + p.z); }
  Point3D operator-(const Point3D& p) const { return Point3D(x - p.x, y - p.y, z - p.z); }
  Point3D operator*(T k) const { return Point3D(x * k, y * k, z * k); }
  Point3D operator/(T k) const { return Point3D(x / k, y / k, z / k); }

  T dot(const Point3D& p) const { return x * p.x + y * p.y + z * p.z; }

  Point3D cross(const Point3D& p) const {
    return Point3D((y * p.z - z * p.y), (z * p.x - x * p.z), (x * p.y - y * p.x));
  }

  T norm2() const { return x * x + y * y + z * z; }
  T norm() const { return sqrt(norm2()); }

  bool operator<(const Point3D& p) const {
    if (x != p.x) return x < p.x;
    if (y != p.y) return y < p.y;
    return z < p.z;
  }
};

#endif
