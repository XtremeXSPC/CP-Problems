#ifndef CP_MODULES_GEOMETRY_POLYGON_HPP
#define CP_MODULES_GEOMETRY_POLYGON_HPP

#include "_Common.hpp"
#include "Point2D.hpp"

/// @brief Polygon wrapper with common geometric predicates.
template <typename T = F64>
struct Polygon {
  Vec<Point2D<T>> vertices;

  Polygon() {}
  Polygon(const Vec<Point2D<T>>& v) : vertices(v) {}

  /// @brief Signed-area magnitude of polygon.
  T area() const {
    T result = 0;
    I32 n = sz(vertices);
    FOR(i, n) {
      result += vertices[i].cross(vertices[(i + 1) % n]);
    }
    return abs(result) / 2;
  }

  /// @brief Perimeter length.
  T perimeter() const {
    T result = 0;
    I32 n = sz(vertices);
    FOR(i, n) {
      result += (vertices[(i + 1) % n] - vertices[i]).norm();
    }
    return result;
  }

  /// @brief Checks polygon convexity.
  bool is_convex() const {
    I32 n = sz(vertices);
    bool sign = false;
    FOR(i, n) {
      T o = orientation(vertices[i], vertices[(i + 1) % n], vertices[(i + 2) % n]);
      if (abs(o) < EPS) continue;
      if (i == 0) sign = o > 0;
      else if ((o > 0) != sign) return false;
    }
    return true;
  }

  /// @brief Point-in-polygon test (winding/ray crossing variant).
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

/// @brief Convex hull via monotonic chain (a.k.a. Andrew/Graham variant).
template <typename T>
Vec<Point2D<T>> convex_hull(Vec<Point2D<T>> points) {
  I32 n = sz(points);
  if (n <= 3) return points;

  sort(all(points));

  Vec<Point2D<T>> hull;

  // Lower hull.
  FOR(i, n) {
    while (sz(hull) >= 2 && orientation(hull[sz(hull) - 2], hull.back(), points[i]) <= 0) {
      hull.pop_back();
    }
    hull.push_back(points[i]);
  }

  // Upper hull.
  I32 lower_size = sz(hull);
  FOR_R(i, n - 1) {
    while (sz(hull) > lower_size && orientation(hull[sz(hull) - 2], hull.back(), points[i]) <= 0) {
      hull.pop_back();
    }
    hull.push_back(points[i]);
  }

  hull.pop_back();  // Remove duplicate point.
  return hull;
}

/// @brief Closest pair of points by divide-and-conquer.
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
        strip.push_back(points[i]);
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

#endif
