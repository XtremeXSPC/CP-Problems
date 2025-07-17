//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Round 1009 (Div. 3)
 * @author: Costantino Lombardi
 * @file: problem_D.cc
 *
 * @status: NOT PASSED
 */
//===---------------------------------------------------------------------===//
/* Included library */

#include <bits/stdc++.h>
#include <iostream>

using namespace std;

#ifdef LOCAL
#include "algo/debug.h"
#else
#define debug(...) 42
#endif

//===---------------------------------------------------------------------===//
/* Function Definitions */

struct Range {
  int left, right;
  Range(int l, int r) : left(l), right(r) {}
};

//===---------------------------------------------------------------------===//
/* Main function */

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;

  while (t--) {
    int n, m;
    cin >> n >> m;

    vector<int> centers(n);
    for (int i = 0; i < n; ++i) {
      cin >> centers[i];
    }

    vector<int> radii(n);
    for (int i = 0; i < n; ++i) {
      cin >> radii[i];
    }

    int max_radius = 0;
    for (int r : radii) {
      max_radius = max(max_radius, r);
    }

    int total_points = 0;

    // For each y coordinate
    for (int y = -max_radius; y <= max_radius; ++y) {
      vector<Range> ranges;

      // For each circle, find the interval of x coordinates
      for (int i = 0; i < n; ++i) {
        int centerX = centers[i];
        int radius  = radii[i];

        long long y_squared = static_cast<long long>(y) * y;
        if (y_squared <= static_cast<long long>(radius) * radius) {
          double dx    = sqrt(static_cast<long long>(radius) * radius - y_squared);
          int    left  = ceil(centerX - dx);
          int    right = floor(centerX + dx);

          if (left <= right) {
            ranges.push_back(Range(left, right));
          }
        }
      }

      if (ranges.empty())
        continue;

      // Sort the intervals by the left endpoint
      sort(ranges.begin(), ranges.end(), [](const Range& a, const Range& b) { return a.left < b.left; });

      // Merge overlapping intervals
      vector<Range> merged;
      merged.push_back(ranges[0]);

      for (size_t i = 1; i < ranges.size(); ++i) {
        Range& last = merged.back();

        if (ranges[i].left <= last.right + 1) {
          // Overlap or adjacency, merge
          last.right = max(last.right, ranges[i].right);
        } else {
          // No overlap, add as a new interval
          merged.push_back(ranges[i]);
        }
      }

      // Count the integer points in the merged intervals
      for (const Range& r : merged) {
        total_points += r.right - r.left + 1;
      }
    }

    cout << total_points << "\n";
  }

  return 0;
}

//===---------------------------------------------------------------------===//