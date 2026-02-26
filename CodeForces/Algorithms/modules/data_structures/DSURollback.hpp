#ifndef CP_MODULES_DATA_STRUCTURES_DSU_ROLLBACK_HPP
#define CP_MODULES_DATA_STRUCTURES_DSU_ROLLBACK_HPP

#include "_Common.hpp"

/**
 * @brief Disjoint Set Union with rollback/snapshot support.
 *
 * @details Uses union by size and intentionally avoids path compression so that
 * rollbacks stay O(1) per reverted union. Useful for offline dynamic
 * connectivity and divide-and-conquer on time.
 */
struct DSURollback {
private:
  struct Change {
    I32 a = -1;      // Parent after merge.
    I32 b = -1;      // Child root attached to 'a'.
    I32 size_a = 0;  // Size[a] before merge.
  };

  Vec<Change> history;

public:
  VecI32 parent;
  VecI32 size;
  I32 components = 0;

  explicit DSURollback(I32 n = 0) { init(n); }

  void init(I32 n) {
    n = std::max<I32>(n, 0);
    parent.resize(as<Size>(n));
    std::iota(all(parent), 0);
    size.assign(as<Size>(n), 1);
    history.clear();
    components = n;
  }

  I32 find(I32 v) const {
    while (parent[as<Size>(v)] != v) {
      v = parent[as<Size>(v)];
    }
    return v;
  }

  bool unite(I32 a, I32 b) {
    a = find(a);
    b = find(b);
    if (a == b) {
      history.push_back(Change{});
      return false;
    }

    if (size[as<Size>(a)] < size[as<Size>(b)]) {
      std::swap(a, b);
    }

    history.push_back(Change{a, b, size[as<Size>(a)]});
    parent[as<Size>(b)] = a;
    size[as<Size>(a)] += size[as<Size>(b)];
    --components;
    return true;
  }

  /// @brief Current rollback token.
  Size snapshot() const { return history.size(); }

  /// @brief Rolls back all merges performed after @p snap.
  void rollback(Size snap) {
    while (history.size() > snap) {
      Change ch = history.back();
      history.pop_back();
      if (ch.a == -1) continue;  // no-op merge

      parent[as<Size>(ch.b)] = ch.b;
      size[as<Size>(ch.a)] = ch.size_a;
      ++components;
    }
  }

  /// @brief Reverts the last unite() call (if any).
  void rollback_one() {
    if (history.empty()) return;
    rollback(history.size() - 1);
  }

  bool connected(I32 a, I32 b) const { return find(a) == find(b); }

  I32 component_size(I32 v) const { return size[as<Size>(find(v))]; }

  I32 num_components() const { return components; }
};

#endif
