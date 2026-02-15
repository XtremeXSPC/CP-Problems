#pragma once
#include "../templates/Types.hpp"
#include "../templates/Macros.hpp"
#include "../templates/Constants.hpp"
#include <cmath>
#include <complex>

/**
 * @file Geometry.hpp
 * @brief 2D/3D computational geometry primitives and algorithms.
 */

//===----------------------------------------------------------------------===//
//====================== Computational Geometry Module =======================//

/**
 * @brief 2D point/vector with standard vector operations.
 * @tparam T Coordinate type.
 */
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

  /**
   * @brief Rotates the vector by angle (radians) around origin.
   */
  Point2D rotate(T angle) const {
    T c = cos(angle), s = sin(angle);
    return Point2D(x * c - y * s, x * s + y * c);
  }

  /**
   * @brief Returns perpendicular vector (-y, x).
   */
  Point2D perp() const { return Point2D(-y, x); }

  bool operator<(const Point2D& p) const {
    return x != p.x ? x < p.x : y < p.y;
  }

  bool operator==(const Point2D& p) const {
    return abs(x - p.x) < EPS && abs(y - p.y) < EPS;
  }
};

/**
 * @brief Oriented area sign of triangle (a, b, c).
 * @return Positive for counterclockwise turn, negative for clockwise.
 */
template <typename T>
T orientation(const Point2D<T>& a, const Point2D<T>& b, const Point2D<T>& c) {
  return (b - a).cross(c - a);
}

/**
 * @brief Checks whether point p lies on segment [a, b].
 */
template <typename T>
bool on_segment(const Point2D<T>& p, const Point2D<T>& a, const Point2D<T>& b) {
  return abs((b - a).cross(p - a)) < EPS &&
         (p - a).dot(p - b) <= EPS;
}

/**
 * @brief 2D line in implicit form a*x + b*y = c.
 */
template <typename T = F64>
struct Line2D {
  T a, b, c;

  Line2D(T a = 0, T b = 0, T c = 0) : a(a), b(b), c(c) {}

  /**
   * @brief Builds line passing through points p1 and p2.
   */
  Line2D(const Point2D<T>& p1, const Point2D<T>& p2) {
    a = p2.y - p1.y;
    b = p1.x - p2.x;
    c = a * p1.x + b * p1.y;
  }

  /**
   * @brief Euclidean distance from point to line.
   */
  T distance(const Point2D<T>& p) const {
    return abs(a * p.x + b * p.y - c) / sqrt(a * a + b * b);
  }

  /**
   * @brief Orthogonal projection of point onto this line.
   */
  Point2D<T> project(const Point2D<T>& p) const {
    T d = a * a + b * b;
    T x = (b * (b * p.x - a * p.y) + a * c) / d;
    T y = (a * (-b * p.x + a * p.y) + b * c) / d;
    return Point2D<T>(x, y);
  }
};

/**
 * @brief Segment intersection test with intersection point extraction.
 * @tparam T Input coordinate type.
 * @tparam U Output coordinate type (must be floating point).
 * @return true if segments intersect (including collinear touching).
 */
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

  // Check for collinear cases
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

/**
 * @brief Polygon wrapper with common geometric predicates.
 */
template <typename T = F64>
struct Polygon {
  Vec<Point2D<T>> vertices;

  Polygon() {}
  Polygon(const Vec<Point2D<T>>& v) : vertices(v) {}

  /**
   * @brief Signed-area magnitude of polygon.
   */
  T area() const {
    T result = 0;
    I32 n = sz(vertices);
    FOR(i, n) {
      result += vertices[i].cross(vertices[(i + 1) % n]);
    }
    return abs(result) / 2;
  }

  /**
   * @brief Perimeter length.
   */
  T perimeter() const {
    T result = 0;
    I32 n = sz(vertices);
    FOR(i, n) {
      result += (vertices[(i + 1) % n] - vertices[i]).norm();
    }
    return result;
  }

  /**
   * @brief Checks polygon convexity.
   */
  bool is_convex() const {
    I32 n = sz(vertices);
    bool sign = false;
    FOR(i, n) {
      T o = orientation(vertices[i],
                       vertices[(i + 1) % n],
                       vertices[(i + 2) % n]);
      if (abs(o) < EPS) continue;
      if (i == 0) sign = o > 0;
      else if ((o > 0) != sign) return false;
    }
    return true;
  }

  /**
   * @brief Point-in-polygon test (winding/ray crossing variant).
   */
  bool contains(const Point2D<T>& p) const {
    I32 n = sz(vertices);
    I32 count = 0;

    FOR(i, n) {
      Point2D<T> a = vertices[i];
      Point2D<T> b = vertices[(i + 1) % n];

      if (on_segment(p, a, b)) return true;

      if (a.y <= p.y) {
        if (b.y > p.y) {
          if (orientation(a, b, p) > 0) count++;
        }
      } else {
        if (b.y <= p.y) {
          if (orientation(a, b, p) < 0) count--;
        }
      }
    }

    return count != 0;
  }
};

/**
 * @brief Convex hull via monotonic chain (a.k.a. Andrew/Graham variant).
 * @return Hull vertices in counterclockwise order.
 */
template <typename T>
Vec<Point2D<T>> convex_hull(Vec<Point2D<T>> points) {
  I32 n = sz(points);
  if (n <= 3) return points;

  sort(all(points));

  Vec<Point2D<T>> hull;

  // Lower hull.
  FOR(i, n) {
    while (sz(hull) >= 2 &&
           orientation(hull[sz(hull) - 2], hull.back(), points[i]) <= 0) {
      hull.pop_back();
    }
    hull.pb(points[i]);
  }

  // Upper hull.
  I32 lower_size = sz(hull);
  FOR_R(i, n - 1) {
    while (sz(hull) > lower_size &&
           orientation(hull[sz(hull) - 2], hull.back(), points[i]) <= 0) {
      hull.pop_back();
    }
    hull.pb(points[i]);
  }

  hull.pop_back();  // Remove duplicate point.
  return hull;
}

/**
 * @brief Closest pair of points by divide-and-conquer.
 * @return Pair of points with minimum Euclidean distance.
 *
 * Complexity: O(n log^2 n) in this implementation due to per-level strip sorting.
 */
template <typename T>
std::pair<Point2D<T>, Point2D<T>> closest_pair(Vec<Point2D<T>> points) {
  I32 n = sz(points);
  sort(all(points));

  std::function<std::pair<Point2D<T>, Point2D<T>>(I32, I32)> solve =
    [&](I32 l, I32 r) -> std::pair<Point2D<T>, Point2D<T>> {

    if (r - l <= 3) {
      T min_dist = infinity<T>;
      std::pair<Point2D<T>, Point2D<T>> result;
      FOR(i, l, r) {
        FOR(j, i + 1, r) {
          T d = (points[i] - points[j]).norm();
          if (d < min_dist) {
            min_dist = d;
            result = {points[i], points[j]};
          }
        }
      }
      return result;
    }

    I32 mid = (l + r) / 2;
    T midx = points[mid].x;

    auto left_pair = solve(l, mid);
    auto right_pair = solve(mid, r);

    T d_left = (left_pair.first - left_pair.second).norm();
    T d_right = (right_pair.first - right_pair.second).norm();

    T d = min(d_left, d_right);
    auto best_pair = (d_left < d_right) ? left_pair : right_pair;

    Vec<Point2D<T>> strip;
    FOR(i, l, r) {
      if (abs(points[i].x - midx) < d) {
        strip.pb(points[i]);
      }
    }

    sort(all(strip), [](const Point2D<T>& a, const Point2D<T>& b) {
      return a.y < b.y;
    });

    FOR(i, sz(strip)) {
      FOR(j, i + 1, sz(strip)) {
        if (strip[j].y - strip[i].y >= d) break;
        T dist = (strip[i] - strip[j]).norm();
        if (dist < d) {
          d = dist;
          best_pair = {strip[i], strip[j]};
        }
      }
    }

    return best_pair;
  };

  return solve(0, n);
}

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
 * @brief 3D point/vector primitive.
 */
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
    return Point3D(y * p.z - z * p.y,
                   z * p.x - x * p.z,
                   x * p.y - y * p.x);
  }

  T norm2() const { return x * x + y * y + z * z; }
  T norm() const { return sqrt(norm2()); }

  bool operator<(const Point3D& p) const {
    if (x != p.x) return x < p.x;
    if (y != p.y) return y < p.y;
    return z < p.z;
  }
};

/**
 * @brief Plane in 3D represented as a*x + b*y + c*z = d.
 */
template <typename T = F64>
struct Plane3D {
  T a, b, c, d;

  Plane3D(T a = 0, T b = 0, T c = 0, T d = 0) : a(a), b(b), c(c), d(d) {}

  /**
   * @brief Builds plane from 3 non-collinear points.
   */
  Plane3D(const Point3D<T>& p1, const Point3D<T>& p2, const Point3D<T>& p3) {
    Point3D<T> v1 = p2 - p1;
    Point3D<T> v2 = p3 - p1;
    Point3D<T> normal = v1.cross(v2);

    a = normal.x;
    b = normal.y;
    c = normal.z;
    d = normal.dot(p1);
  }

  /**
   * @brief Distance from point to plane.
   */
  T distance(const Point3D<T>& p) const {
    return abs(a * p.x + b * p.y + c * p.z - d) / sqrt(a * a + b * b + c * c);
  }
};

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

/**
 * @brief Half-plane represented by directed line (left side is feasible).
 */
template <typename T>
struct HalfPlane {
  Point2D<T> p1, p2;  // Half-plane to the left of p1->p2.

  HalfPlane(const Point2D<T>& a, const Point2D<T>& b) : p1(a), p2(b) {}

  /**
   * @brief Checks if point lies in the half-plane.
   */
  bool contains(const Point2D<T>& p) const {
    return orientation(p1, p2, p) >= -EPS;
  }

  /**
   * @brief Direction angle of boundary line.
   */
  T angle() const { return atan2(p2.y - p1.y, p2.x - p1.x); }
};

/**
 * @brief Intersects a set of half-planes and returns feasible polygon.
 * @return Polygon of feasible region (possibly empty).
 */
template <typename T>
Polygon<T> half_plane_intersection(Vec<HalfPlane<T>> planes) {
  static_assert(std::is_floating_point_v<T>, "half_plane_intersection requires floating-point coordinates");
  sort(all(planes), [](const HalfPlane<T>& a, const HalfPlane<T>& b) {
    return a.angle() < b.angle();
  });

  if (planes.empty()) return Polygon<T>();

  auto intersect_segment_with_line = [](const Point2D<T>& a, const Point2D<T>& b,
                                        const HalfPlane<T>& hp) -> Point2D<T> {
    Point2D<T> ab = b - a;
    Point2D<T> dir = hp.p2 - hp.p1;
    T denom = ab.cross(dir);
    if (abs(denom) < EPS) return a;
    T t = (hp.p1 - a).cross(dir) / denom;
    return a + ab * t;
  };

  auto nearly_equal = [](const Point2D<T>& a, const Point2D<T>& b) {
    return abs(a.x - b.x) <= EPS && abs(a.y - b.y) <= EPS;
  };

  auto push_unique = [&](Vec<Point2D<T>>& poly, const Point2D<T>& p) {
    if (poly.empty() || !nearly_equal(poly.back(), p)) {
      poly.pb(p);
    }
  };

  const T BOUND = static_cast<T>(1e9);
  Vec<Point2D<T>> polygon = {
    {-BOUND, -BOUND},
    { BOUND, -BOUND},
    { BOUND,  BOUND},
    {-BOUND,  BOUND},
  };

  for (const auto& hp : planes) {
    if (polygon.empty()) break;
    Vec<Point2D<T>> clipped;
    I32 n = sz(polygon);

    FOR(i, n) {
      Point2D<T> curr = polygon[i];
      Point2D<T> next = polygon[(i + 1) % n];
      bool in_curr = hp.contains(curr);
      bool in_next = hp.contains(next);

      if (in_curr && in_next) {
        push_unique(clipped, next);
      } else if (in_curr && !in_next) {
        push_unique(clipped, intersect_segment_with_line(curr, next, hp));
      } else if (!in_curr && in_next) {
        push_unique(clipped, intersect_segment_with_line(curr, next, hp));
        push_unique(clipped, next);
      }
    }

    if (!clipped.empty() && nearly_equal(clipped.front(), clipped.back())) {
      clipped.pop_back();
    }
    polygon.swap(clipped);
  }

  return Polygon<T>(polygon);
}

//===----------------------------------------------------------------------===//
