#ifndef CP_MODULES_DATA_STRUCTURES_PERSISTENT_SEG_TREE_HPP
#define CP_MODULES_DATA_STRUCTURES_PERSISTENT_SEG_TREE_HPP

#include "_Common.hpp"

/// @brief Persistent segment tree supporting versioned point updates.
struct PersistentSegTree {
  /// @brief Node in persistent segment tree arena.
  struct Node {
    I64 val;
    I32 left, right;
    Node(I64 v = 0) : val(v), left(-1), right(-1) {}
  };

  Vec<Node> nodes;
  VecI32 roots;
  I32 n;

  /// @brief Creates base version initialized with zeros.
  PersistentSegTree(I32 size) : n(1) {
    while (n < size) n *= 2;
    nodes.reserve(n * 20);  // Pre-allocate for efficiency.
    build(0, n);
    roots.push_back(0);
  }

  /// @brief Builds initial tree structure.
  I32 build(I32 tl, I32 tr) {
    I32 idx = sz(nodes);
    nodes.eb();
    if (tl + 1 != tr) {
      I32 tm = (tl + tr) / 2;
      nodes[idx].left = build(tl, tm);
      nodes[idx].right = build(tm, tr);
    }
    return idx;
  }

  /// @brief Creates updated node path from previous version root.
  I32 update(I32 prev, I32 pos, I64 val, I32 tl = 0, I32 tr = -1) {
    if (tr == -1) tr = n;
    if (tl + 1 == tr) {
      if (nodes[prev].val == val) return prev;
      I32 idx = sz(nodes);
      nodes.push_back(nodes[prev]);
      nodes[idx].val = val;
      return idx;
    }

    I32 tm = (tl + tr) / 2;
    I32 next_left = nodes[prev].left;
    I32 next_right = nodes[prev].right;
    if (pos < tm) {
      next_left = update(nodes[prev].left, pos, val, tl, tm);
    } else {
      next_right = update(nodes[prev].right, pos, val, tm, tr);
    }

    if (next_left == nodes[prev].left && next_right == nodes[prev].right) { return prev; }

    I32 idx = sz(nodes);
    nodes.push_back(nodes[prev]);
    nodes[idx].left = next_left;
    nodes[idx].right = next_right;
    nodes[idx].val = nodes[nodes[idx].left].val + nodes[nodes[idx].right].val;
    return idx;
  }

  /// @brief Range sum query on version root over [l, r).
  I64 query(I32 root, I32 l, I32 r, I32 tl = 0, I32 tr = -1) {
    if (tr == -1) tr = n;
    if (l >= tr || r <= tl) return 0;
    if (l <= tl && tr <= r) return nodes[root].val;
    I32 tm = (tl + tr) / 2;
    return query(nodes[root].left, l, r, tl, tm) + query(nodes[root].right, l, r, tm, tr);
  }

  /// @brief Appends a new version after setting one position.
  void new_version(I32 pos, I64 val) { roots.push_back(update(roots.back(), pos, val)); }
};

#endif
