#ifndef CP_MODULES_GEOMETRY_CONVEX_DIAMETER_HPP
#define CP_MODULES_GEOMETRY_CONVEX_DIAMETER_HPP

#include "_Common.hpp"
#include "Point2D.hpp"

/**
 * @brief Diameter of convex polygon via rotating calipers.
 * @return Maximum pairwise distance between hull vertices.
 */
template <typename T>
T convex_diameter(const Vec<Point2D<T>>& hull) {
  I32 n = sz(hull);
  if (n <= 1) return 0;
  if (n == 2) return (hull[1] - hull[0]).norm();

  T max_dist = 0;
  I32 j = 1;

  FOR(i, n) {
    while ((hull[(i + 1) % n] - hull[i]).cross(hull[(j + 1) % n] - hull[j]) > 0) {
      j = (j + 1) % n;
    }
    max_dist = max(max_dist, (hull[i] - hull[j]).norm());
    max_dist = max(max_dist, (hull[(i + 1) % n] - hull[(j + 1) % n]).norm());
  }

  return max_dist;
}

#endif
