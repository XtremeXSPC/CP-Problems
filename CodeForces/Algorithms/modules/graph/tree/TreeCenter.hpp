#ifndef CP_MODULES_GRAPH_TREE_TREE_CENTER_HPP
#define CP_MODULES_GRAPH_TREE_TREE_CENTER_HPP

#include "TreeDiameter.hpp"

/// @brief Center vertices of an unweighted tree component.
/**
 * @brief Center vertex/vertices of a tree by edge count.
 *
 * @details For an even-length diameter, @c first == @c second is the unique
 * center. For an odd-length diameter, @c first and @c second are the two middle
 * vertices. Edge weights are ignored intentionally; use @c tree_diameter when
 * the weighted diameter path is needed.
 */
struct TreeCenterResult {
  I32 first  = -1;
  I32 second = -1;
  I32 diameter_edges = 0;
};

struct TreeCenter {
  /// @brief Returns one center for even diameter, two adjacent centers for odd diameter.
  [[nodiscard]] static auto solve(const Graph<I64>& g, I32 root = 0) -> TreeCenterResult {
    const auto d = tree_diameter(g, root);
    const I32 len = isz(d.path) - 1;
    my_assert(len >= 0);
    if (len % 2 == 0) {
      const I32 c = d.path[len / 2];
      return {.first = c, .second = c, .diameter_edges = len};
    }
    return {.first = d.path[len / 2], .second = d.path[len / 2 + 1], .diameter_edges = len};
  }
};

[[nodiscard]] inline auto tree_center(const Graph<I64>& g, I32 root = 0) -> TreeCenterResult {
  return TreeCenter::solve(g, root);
}

#endif
