#pragma once
#include "../templates/Types.hpp"
#include "../templates/Macros.hpp"
#include "../templates/Constants.hpp"

/**
 * @file Splay_Tree.hpp
 * @brief Implicit splay tree with lazy propagation for sequence operations.
 */

//===----------------------------------------------------------------------===//
//============================ Splay Tree Module =============================//

/**
 * @brief Implicit splay tree supporting split, merge, range queries and updates.
 * @tparam M Monoid defining aggregation and lazy propagation.
 *
 * Monoid interface requirements:
 *   using Val  = ...;                            - Value type
 *   using Lazy = ...;                            - Lazy tag type
 *   static Val  e();                             - Value identity
 *   static Lazy le();                            - Lazy identity
 *   static Val  op(Val a, Val b);                - Merge two values (left-to-right)
 *   static Val  mapping(Lazy f, Val x, I32 n);   - Apply lazy f to aggregate x over n elements
 *   static Lazy composition(Lazy f, Lazy g);     - Compose: apply f after g
 */
template <typename M>
struct SplayTree {
  using Val  = typename M::Val;
  using Lazy = typename M::Lazy;

  struct Node {
    I32 ch[2] = {-1, -1};
    I32 par = -1;
    I32 sz = 1;
    Val val = M::e();
    Val agg = M::e();
    Lazy lazy = M::le();
    bool rev = false;
  };

  Vec<Node> nodes;
  Vec<I32> free_list;

  SplayTree() = default;

  /**
   * @brief Pre-allocates node storage for anticipated element count.
   */
  explicit SplayTree(I32 capacity) { nodes.reserve(capacity); }

  /**
   * @brief Allocates a new node with the given value.
   * @return Node index.
   */
  I32 new_node(Val v = M::e()) {
    I32 idx;
    if (!free_list.empty()) {
      idx = free_list.back();
      free_list.pop_back();
      nodes[idx] = Node{};
    } else {
      idx = sz(nodes);
      nodes.eb();
    }
    nodes[idx].val = v;
    nodes[idx].agg = v;
    return idx;
  }

  /**
   * @brief Returns a node to the free list for reuse.
   */
  void del_node(I32 x) {
    if (x != -1) free_list.pb(x);
  }

  I32 size(I32 x) const { return x == -1 ? 0 : nodes[x].sz; }

  //===-------------------- CORE SPLAY TREE OPERATIONS --------------------===//

  /**
   * @brief Pushes pending lazy tags and reverse flag to children.
   */
  void push(I32 x) {
    if (x == -1) return;

    if (nodes[x].rev) {
      std::swap(nodes[x].ch[0], nodes[x].ch[1]);
      for (I32 d = 0; d < 2; ++d) {
        if (nodes[x].ch[d] != -1) nodes[nodes[x].ch[d]].rev ^= true;
      }
      nodes[x].rev = false;
    }

    if (!(nodes[x].lazy == M::le())) {
      for (I32 d = 0; d < 2; ++d) {
        I32 c = nodes[x].ch[d];
        if (c != -1) {
          nodes[c].agg = M::mapping(nodes[x].lazy, nodes[c].agg, nodes[c].sz);
          nodes[c].val = M::mapping(nodes[x].lazy, nodes[c].val, 1);
          nodes[c].lazy = M::composition(nodes[x].lazy, nodes[c].lazy);
        }
      }
      nodes[x].lazy = M::le();
    }
  }

  /**
   * @brief Recomputes size and aggregate from children.
   */
  void pull(I32 x) {
    if (x == -1) return;
    nodes[x].sz = 1;
    nodes[x].agg = nodes[x].val;
    I32 lc = nodes[x].ch[0], rc = nodes[x].ch[1];
    if (lc != -1) {
      nodes[x].sz += nodes[lc].sz;
      nodes[x].agg = M::op(nodes[lc].agg, nodes[x].agg);
    }
    if (rc != -1) {
      nodes[x].sz += nodes[rc].sz;
      nodes[x].agg = M::op(nodes[x].agg, nodes[rc].agg);
    }
  }

  /**
   * @brief Single tree rotation around node x.
   */
  void rot(I32 x) {
    I32 p = nodes[x].par;
    I32 g = nodes[p].par;
    I32 d = (nodes[p].ch[1] == x);

    nodes[p].ch[d] = nodes[x].ch[d ^ 1];
    if (nodes[x].ch[d ^ 1] != -1) nodes[nodes[x].ch[d ^ 1]].par = p;

    nodes[x].ch[d ^ 1] = p;
    nodes[p].par = x;
    nodes[x].par = g;

    if (g != -1) nodes[g].ch[nodes[g].ch[1] == p] = x;

    pull(p);
    pull(x);
  }

  /**
   * @brief Splays node x to the root of its tree.
   *
   * Pushes all pending lazy tags along the root-to-x path before rotating.
   */
  void splay(I32 x) {
    static Vec<I32> anc;
    anc.clear();
    for (I32 v = x; v != -1; v = nodes[v].par) anc.pb(v);
    FOR_R(i, sz(anc)) push(anc[i]);

    while (nodes[x].par != -1) {
      I32 p = nodes[x].par;
      I32 g = nodes[p].par;
      if (g != -1) {
        bool same = (nodes[g].ch[0] == p) == (nodes[p].ch[0] == x);
        rot(same ? p : x);
      }
      rot(x);
    }
  }

  /**
   * @brief Finds the node at position k (0-indexed) in the subtree.
   */
  I32 kth_node(I32 x, I32 k) {
    while (true) {
      push(x);
      I32 lsz = size(nodes[x].ch[0]);
      if (k < lsz) { x = nodes[x].ch[0]; }
      else if (k == lsz) { return x; }
      else { k -= lsz + 1; x = nodes[x].ch[1]; }
    }
  }

  //===-------------------- AUXILIARY TREE OPERATIONS ---------------------===//

  /**
   * @brief Splits tree into left (k elements) and right (rest).
   * @return {left_root, right_root}.
   */
  P<I32, I32> split(I32 root, I32 k) {
    if (k <= 0) return {-1, root};
    if (root == -1 || k >= size(root)) return {root, -1};

    I32 x = kth_node(root, k - 1);
    splay(x);

    I32 right = nodes[x].ch[1];
    if (right != -1) nodes[right].par = -1;
    nodes[x].ch[1] = -1;
    pull(x);

    return {x, right};
  }

  /**
   * @brief Merges two trees (all elements in l precede all elements in r).
   * @return Root of merged tree.
   */
  I32 merge(I32 l, I32 r) {
    if (l == -1) return r;
    if (r == -1) return l;

    I32 x = l;
    push(x);
    while (nodes[x].ch[1] != -1) {
      x = nodes[x].ch[1];
      push(x);
    }
    splay(x);

    nodes[x].ch[1] = r;
    nodes[r].par = x;
    pull(x);
    return x;
  }

  //===----------------------- SEQUENCE OPERATIONS ------------------------===//

  /**
   * @brief Inserts value v at position pos (0-indexed, pushes existing elements right).
   * @return New root.
   */
  I32 insert(I32 root, I32 pos, Val v) {
    I32 nd = new_node(v);
    auto [l, r] = split(root, pos);
    return merge(merge(l, nd), r);
  }

  /**
   * @brief Erases the element at position pos (0-indexed).
   * @return New root.
   */
  I32 erase(I32 root, I32 pos) {
    auto [l, mr] = split(root, pos);
    auto [m, r] = split(mr, 1);
    del_node(m);
    return merge(l, r);
  }

  /**
   * @brief Range aggregate query over half-open interval [l, r).
   */
  Val query(I32& root, I32 l, I32 r) {
    auto [ab, c] = split(root, r);
    auto [a, b] = split(ab, l);
    Val result = (b == -1) ? M::e() : nodes[b].agg;
    root = merge(merge(a, b), c);
    return result;
  }

  /**
   * @brief Applies lazy update to half-open interval [l, r).
   */
  void apply(I32& root, I32 l, I32 r, Lazy lz) {
    auto [ab, c] = split(root, r);
    auto [a, b] = split(ab, l);
    if (b != -1) {
      nodes[b].agg = M::mapping(lz, nodes[b].agg, nodes[b].sz);
      nodes[b].val = M::mapping(lz, nodes[b].val, 1);
      nodes[b].lazy = M::composition(lz, nodes[b].lazy);
    }
    root = merge(merge(a, b), c);
  }

  /**
   * @brief Reverses the half-open interval [l, r).
   */
  void reverse(I32& root, I32 l, I32 r) {
    auto [ab, c] = split(root, r);
    auto [a, b] = split(ab, l);
    if (b != -1) nodes[b].rev ^= true;
    root = merge(merge(a, b), c);
  }

  /**
   * @brief Returns the value at position pos (0-indexed).
   */
  Val at(I32& root, I32 pos) {
    I32 x = kth_node(root, pos);
    splay(x);
    root = x;
    return nodes[x].val;
  }

  /**
   * @brief Builds a balanced tree from an array of values.
   * @return Root index.
   */
  I32 build(const Vec<Val>& a) {
    return build_rec(a, 0, sz(a));
  }

  /**
   * @brief Collects all values in-order into a vector.
   */
  void to_vec(I32 x, Vec<Val>& out) {
    if (x == -1) return;
    push(x);
    to_vec(nodes[x].ch[0], out);
    out.pb(nodes[x].val);
    to_vec(nodes[x].ch[1], out);
  }

private:
  I32 build_rec(const Vec<Val>& a, I32 l, I32 r) {
    if (l >= r) return -1;
    I32 mid = (l + r) / 2;
    I32 x = new_node(a[mid]);
    nodes[x].ch[0] = build_rec(a, l, mid);
    nodes[x].ch[1] = build_rec(a, mid + 1, r);
    if (nodes[x].ch[0] != -1) nodes[nodes[x].ch[0]].par = x;
    if (nodes[x].ch[1] != -1) nodes[nodes[x].ch[1]].par = x;
    pull(x);
    return x;
  }
};

//===--------------------- COMMON MONOIDS DEFINITIONS ---------------------===//

/**
 * @brief Range sum query with range addition.
 */
struct SumAdd {
  using Val  = I64;
  using Lazy = I64;
  static Val  e()                              { return 0; }
  static Lazy le()                             { return 0; }
  static Val  op(Val a, Val b)                 { return a + b; }
  static Val  mapping(Lazy f, Val x, I32 n)    { return x + f * n; }
  static Lazy composition(Lazy f, Lazy g)      { return f + g; }
};

/**
 * @brief Range minimum query with range addition.
 */
struct MinAdd {
  using Val  = I64;
  using Lazy = I64;
  static Val  e()                              { return infinity<I64>; }
  static Lazy le()                             { return 0; }
  static Val  op(Val a, Val b)                 { return std::min(a, b); }
  static Val  mapping(Lazy f, Val x, [[maybe_unused]] I32 n) { return x + f; }
  static Lazy composition(Lazy f, Lazy g)      { return f + g; }
};

/**
 * @brief Range maximum query with range addition.
 */
struct MaxAdd {
  using Val  = I64;
  using Lazy = I64;
  static Val  e()                              { return -infinity<I64>; }
  static Lazy le()                             { return 0; }
  static Val  op(Val a, Val b)                 { return std::max(a, b); }
  static Val  mapping(Lazy f, Val x, [[maybe_unused]] I32 n) { return x + f; }
  static Lazy composition(Lazy f, Lazy g)      { return f + g; }
};

/**
 * @brief Range sum query with range assignment.
 *
 * Uses std::optional<I64> as the lazy type; std::nullopt denotes no pending update.
 */
struct SumAssign {
  using Val  = I64;
  using Lazy = std::optional<I64>;
  static Val  e()                              { return 0; }
  static Lazy le()                             { return std::nullopt; }
  static Val  op(Val a, Val b)                 { return a + b; }
  static Val  mapping(Lazy f, Val x, I32 n)    { return f.has_value() ? *f * n : x; }
  static Lazy composition(Lazy f, Lazy g)      { return f.has_value() ? f : g; }
};

//===----------------------------------------------------------------------===//
