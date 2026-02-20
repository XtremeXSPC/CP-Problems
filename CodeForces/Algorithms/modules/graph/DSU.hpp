#ifndef CP_MODULES_GRAPH_DSU_HPP
#define CP_MODULES_GRAPH_DSU_HPP

#include "_Common.hpp"

/**
 * @brief Disjoint Set Union (Union-Find) with union by rank + path compression.
 */
struct DSU {
  Vec<I32> parent, rank, size;
  I32 components;

  DSU(I32 n) : parent(n), rank(n, 0), size(n, 1), components(n) {
    std::iota(all(parent), 0);
  }

  /**
   * @brief Finds representative of the set containing x.
   */
  I32 find(I32 x) {
    if (parent[x] != x) parent[x] = find(parent[x]);
    return parent[x];
  }

  /**
   * @brief Unites sets containing x and y.
   * @return True if merge happened; false if already in same set.
   */
  bool unite(I32 x, I32 y) {
    x = find(x);
    y = find(y);

    if (x == y) return false;

    if (rank[x] < rank[y]) std::swap(x, y);
    parent[y] = x;
    size[x] += size[y];

    if (rank[x] == rank[y]) rank[x]++;

    components--;
    return true;
  }

  /**
   * @brief Checks whether x and y are in the same connected component.
   */
  bool connected(I32 x, I32 y) { return find(x) == find(y); }
  /**
   * @brief Returns size of component containing x.
   */
  I32 component_size(I32 x) { return size[find(x)]; }
  /**
   * @brief Returns current number of components.
   */
  I32 num_components() const { return components; }
};

#endif
