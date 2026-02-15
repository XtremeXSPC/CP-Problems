#pragma once
#include "../templates/Types.hpp"
#include "../templates/Macros.hpp"
#include "../templates/Math.hpp"
#include "../templates/Constants.hpp"
#include <algorithm>

/**
 * @file Data_Structures.hpp
 * @brief Core advanced data structures for competitive programming.
 */

//===----------------------------------------------------------------------===//
//===================== Advanced Data Structures Module ======================//

/**
 * @brief Iterative segment tree for point updates and range queries.
 * @tparam T Value type.
 * @tparam F Associative merge function type.
 */
template <typename T, typename F = std::plus<T>>
struct SegmentTree {
  I32 n;
  Vec<T> tree;
  F op;
  T identity;

  /**
   * @brief Constructs empty segment tree with identity element.
   */
  SegmentTree(I32 size, T id = T{}, F func = F{})
    : n(1), op(func), identity(id) {
    while (n < size) n *= 2;
    tree.assign(2 * n, identity);
  }

  /**
   * @brief Constructs segment tree from initial array.
   */
  SegmentTree(const Vec<T>& v, T id = T{}, F func = F{})
    : SegmentTree(sz(v), id, func) {
    FOR(i, sz(v)) tree[n + i] = v[i];
    FOR_R(i, 1, n) tree[i] = op(tree[2*i], tree[2*i + 1]);
  }

  /**
   * @brief Point assignment at position @p pos.
   */
  void update(I32 pos, T val) {
    pos += n;
    tree[pos] = val;
    while (pos > 1) {
      tree[pos >> 1] = op(tree[pos], tree[pos ^ 1]);
      pos >>= 1;
    }
  }

  /**
   * @brief Range query over half-open interval [l, r).
   */
  T query(I32 l, I32 r) {  // [l, r)
    T res_left = identity, res_right = identity;
    l += n; r += n;
    while (l < r) {
      if (l & 1) res_left = op(res_left, tree[l++]);
      if (r & 1) res_right = op(tree[--r], res_right);
      l >>= 1; r >>= 1;
    }
    return op(res_left, res_right);
  }

  /**
   * @brief Returns current value at index @p pos.
   */
  T get(I32 pos) { return tree[n + pos]; }
};

/**
 * @brief Recursive lazy-propagation segment tree for range add / range sum.
 * @tparam T Node aggregate type.
 * @tparam U Lazy delta type.
 */
template <typename T, typename U>
struct LazySegTree {
  I32 n;
  Vec<T> tree;
  Vec<U> lazy;

  /**
   * @brief Constructs tree for given size (rounded up to power of two).
   */
  LazySegTree(I32 size) : n(1) {
    while (n < size) n *= 2;
    tree.assign(2 * n, T{});
    lazy.assign(2 * n, U{});
  }

  /**
   * @brief Pushes pending lazy value from node to children.
   */
  void push(I32 v, I32 tl, I32 tr) {
    if (lazy[v] == U{}) return;
    tree[v] += lazy[v] * (tr - tl);
    if (tl + 1 != tr) {
      lazy[2*v] += lazy[v];
      lazy[2*v + 1] += lazy[v];
    }
    lazy[v] = U{};
  }

  /**
   * @brief Adds @p val over interval [l, r).
   */
  void update(I32 l, I32 r, U val, I32 v = 1, I32 tl = 0, I32 tr = -1) {
    if (tr == -1) tr = n;
    push(v, tl, tr);
    if (l >= tr || r <= tl) return;
    if (l <= tl && tr <= r) {
      lazy[v] += val;
      push(v, tl, tr);
      return;
    }
    I32 tm = (tl + tr) / 2;
    update(l, r, val, 2*v, tl, tm);
    update(l, r, val, 2*v + 1, tm, tr);
    push(2*v, tl, tm);
    push(2*v + 1, tm, tr);
    tree[v] = tree[2*v] + tree[2*v + 1];
  }

  /**
   * @brief Returns aggregate over interval [l, r).
   */
  T query(I32 l, I32 r, I32 v = 1, I32 tl = 0, I32 tr = -1) {
    if (tr == -1) tr = n;
    if (l >= tr || r <= tl) return T{};
    push(v, tl, tr);
    if (l <= tl && tr <= r) return tree[v];
    I32 tm = (tl + tr) / 2;
    return query(l, r, 2*v, tl, tm) + query(l, r, 2*v + 1, tm, tr);
  }
};

/**
 * @brief Fenwick tree (Binary Indexed Tree) for prefix sums.
 * @tparam T Value type supporting + and -.
 */
template <typename T>
struct FenwickTree {
  I32 n;
  Vec<T> tree;

  /**
   * @brief Constructs empty BIT of given size.
   */
  FenwickTree(I32 size) : n(size), tree(size + 1, T{}) {}

  /**
   * @brief Adds @p val at position @p idx.
   */
  void add(I32 idx, T val) {
    idx++;  // 1-indexed internally.
    while (idx <= n) {
      tree[idx] += val;
      idx += idx & -idx;
    }
  }

  /**
   * @brief Prefix sum on [0, idx].
   */
  T sum(I32 idx) {  // Sum [0, idx].
    idx++;
    T res{};
    while (idx > 0) {
      res += tree[idx];
      idx -= idx & -idx;
    }
    return res;
  }

  /**
   * @brief Range sum on [l, r].
   */
  T range_sum(I32 l, I32 r) {  // Sum [l, r].
    return sum(r) - (l > 0 ? sum(l - 1) : T{});
  }

  /**
   * @brief Finds minimum index with prefix sum >= @p val.
   * @param val Target prefix sum threshold.
   * @return First index @c idx in [0, n) such that sum(idx) >= val.
   *
   * Requires non-negative updates to preserve monotonic prefix sums.
   * Complexity: O(log n).
   */
  I32 lower_bound(T val) {
    if (val <= T{}) return 0;
    I32 pos = 0;
    I32 pw = 1;
    while (pw <= n) pw *= 2;
    pw /= 2;

    while (pw > 0) {
      I32 next = pos + pw;
      if (next <= n && tree[next] < val) {
        val -= tree[next];
        pos = next;
      }
      pw /= 2;
    }
    return pos;
  }
};

/**
 * @brief 2D Fenwick tree for point updates and rectangle sums.
 * @tparam T Value type.
 */
template <typename T>
struct FenwickTree2D {
  I32 n, m;
  Vec2<T> tree;

  /**
   * @brief Constructs rows x cols empty BIT.
   */
  FenwickTree2D(I32 rows, I32 cols) : n(rows), m(cols), tree(n + 1, Vec<T>(m + 1)) {}

  /**
   * @brief Adds @p val at cell (x, y).
   */
  void add(I32 x, I32 y, T val) {
    x++; y++;
    for (I32 i = x; i <= n; i += i & -i) {
      for (I32 j = y; j <= m; j += j & -j) {
        tree[i][j] += val;
      }
    }
  }

  /**
   * @brief Prefix sum over rectangle [0..x] x [0..y].
   */
  T sum(I32 x, I32 y) {
    x++; y++;
    T res{};
    for (I32 i = x; i > 0; i -= i & -i) {
      for (I32 j = y; j > 0; j -= j & -j) {
        res += tree[i][j];
      }
    }
    return res;
  }

  /**
   * @brief Rectangle sum over inclusive box [x1..x2] x [y1..y2].
   */
  T range_sum(I32 x1, I32 y1, I32 x2, I32 y2) {
    return sum(x2, y2) - sum(x1 - 1, y2) - sum(x2, y1 - 1) + sum(x1 - 1, y1 - 1);
  }
};

/**
 * @brief Sparse table for idempotent static range queries.
 * @tparam T Value type.
 * @tparam F Merge operation type.
 */
template <typename T, typename F = std::function<T(const T&, const T&)>>
struct SparseTable {
  Vec2<T> table;
  Vec<I32> lg;
  F op;

  /**
   * @brief Precomputes sparse table for input vector.
   */
  SparseTable(const Vec<T>& v, F func = [](const T& a, const T& b) { return _min(a, b); }) : op(func) {
    I32 n = sz(v);
    I32 max_log = 0;
    while ((1 << max_log) <= n) max_log++;

    table.assign(max_log, Vec<T>(n));
    lg.assign(n + 1, 0);

    FOR(i, 2, n + 1) lg[i] = lg[i / 2] + 1;

    FOR(i, n) table[0][i] = v[i];

    FOR(k, 1, max_log) {
      FOR(i, n - (1 << k) + 1) {
        table[k][i] = op(table[k-1][i], table[k-1][i + (1 << (k-1))]);
      }
    }
  }

  /**
   * @brief Query over inclusive range [l, r].
   */
  T query(I32 l, I32 r) {  // [l, r]
    I32 k = lg[r - l + 1];
    return op(table[k][l], table[k][r - (1 << k) + 1]);
  }
};

/**
 * @brief Persistent segment tree supporting versioned point updates.
 */
struct PersistentSegTree {
  /**
   * @brief Node in persistent segment tree arena.
   */
  struct Node {
    I64 val;
    I32 left, right;
    Node(I64 v = 0) : val(v), left(-1), right(-1) {}
  };

  Vec<Node> nodes;
  Vec<I32> roots;
  I32 n;

  /**
   * @brief Creates base version initialized with zeros.
   */
  PersistentSegTree(I32 size) : n(1) {
    while (n < size) n *= 2;
    nodes.reserve(n * 20);  // Pre-allocate for efficiency.
    build(0, n);
    roots.pb(0);
  }

  /**
   * @brief Builds initial tree structure.
   */
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

  /**
   * @brief Creates updated node path from previous version root.
   * @return New version root index.
   */
  I32 update(I32 prev, I32 pos, I64 val, I32 tl = 0, I32 tr = -1) {
    if (tr == -1) tr = n;
    I32 idx = sz(nodes);
    nodes.pb(nodes[prev]);

    if (tl + 1 == tr) {
      nodes[idx].val = val;
    } else {
      I32 tm = (tl + tr) / 2;
      if (pos < tm) {
        nodes[idx].left = update(nodes[prev].left, pos, val, tl, tm);
      } else {
        nodes[idx].right = update(nodes[prev].right, pos, val, tm, tr);
      }
      nodes[idx].val = nodes[nodes[idx].left].val + nodes[nodes[idx].right].val;
    }
    return idx;
  }

  /**
   * @brief Range sum query on version root over [l, r).
   */
  I64 query(I32 root, I32 l, I32 r, I32 tl = 0, I32 tr = -1) {
    if (tr == -1) tr = n;
    if (l >= tr || r <= tl) return 0;
    if (l <= tl && tr <= r) return nodes[root].val;
    I32 tm = (tl + tr) / 2;
    return query(nodes[root].left, l, r, tl, tm) +
           query(nodes[root].right, l, r, tm, tr);
  }

  /**
   * @brief Appends a new version after setting one position.
   */
  void new_version(I32 pos, I64 val) {
    roots.pb(update(roots.back(), pos, val));
  }
};

/**
 * @brief Randomized treap supporting duplicates and order statistics.
 */
struct Treap {
  /**
   * @brief Treap node storing key, multiplicity and subtree aggregates.
   */
  struct Node {
    I64 key, priority;
    I32 left, right, size, cnt;
    I64 sum;

    Node(I64 k)
        : key(k), priority((static_cast<I64>(rand()) << 32) ^ rand()),
          left(-1), right(-1), size(1), cnt(1), sum(k) {}
  };

  Vec<Node> nodes;
  I32 root;

  /**
   * @brief Constructs empty treap.
   */
  Treap() : root(-1) { nodes.reserve(200000); }

  I32 get_size(I32 t) { return t == -1 ? 0 : nodes[t].size; }
  I64 get_sum(I32 t) { return t == -1 ? 0 : nodes[t].sum; }

  /**
   * @brief Recomputes subtree aggregates for node @p t.
   */
  void update(I32 t) {
    if (t == -1) return;
    nodes[t].size = nodes[t].cnt + get_size(nodes[t].left) + get_size(nodes[t].right);
    nodes[t].sum = nodes[t].key * nodes[t].cnt + get_sum(nodes[t].left) + get_sum(nodes[t].right);
  }

  /**
   * @brief Splits tree into keys < key and keys >= key.
   */
  void split_lt(I32 t, I64 key, I32& l, I32& r) {
    if (t == -1) {
      l = r = -1;
      return;
    }
    if (nodes[t].key < key) {
      split_lt(nodes[t].right, key, nodes[t].right, r);
      l = t;
    } else {
      split_lt(nodes[t].left, key, l, nodes[t].left);
      r = t;
    }
    update(t);
  }

  /**
   * @brief Splits tree into keys <= key and keys > key.
   */
  void split_le(I32 t, I64 key, I32& l, I32& r) {
    if (t == -1) {
      l = r = -1;
      return;
    }
    if (nodes[t].key <= key) {
      split_le(nodes[t].right, key, nodes[t].right, r);
      l = t;
    } else {
      split_le(nodes[t].left, key, l, nodes[t].left);
      r = t;
    }
    update(t);
  }

  /**
   * @brief Merges two treaps preserving BST and heap properties.
   */
  void merge(I32& t, I32 l, I32 r) {
    if (l == -1 || r == -1) {
      t = l == -1 ? r : l;
      return;
    }
    if (nodes[l].priority > nodes[r].priority) {
      merge(nodes[l].right, nodes[l].right, r);
      t = l;
    } else {
      merge(nodes[r].left, l, nodes[r].left);
      t = r;
    }
    update(t);
  }

  /**
   * @brief Inserts one occurrence of @p key.
   */
  void insert(I64 key) {
    I32 l, m, r;
    split_lt(root, key, l, m);
    split_le(m, key, m, r);
    if (m == -1) {
      m = sz(nodes);
      nodes.eb(key);
    } else {
      nodes[m].cnt++;
      update(m);
    }
    merge(root, l, m);
    merge(root, root, r);
  }

  /**
   * @brief Removes one occurrence of @p key if present.
   */
  void erase(I64 key) {
    I32 l, m, r;
    split_lt(root, key, l, m);
    split_le(m, key, m, r);

    if (m != -1) {
      if (nodes[m].cnt > 1) {
        nodes[m].cnt--;
        update(m);
      } else {
        merge(m, nodes[m].left, nodes[m].right);
      }
    }

    merge(root, l, m);
    merge(root, root, r);
  }

  /**
   * @brief Returns k-th smallest key (0-indexed).
   * @return Key value or -1 if k is out of bounds.
   */
  I64 kth_element(I32 k) {  // 0-indexed
    I32 t = root;
    while (t != -1) {
      I32 left_size = get_size(nodes[t].left);
      if (k == left_size) return nodes[t].key;
      if (k < left_size) {
        t = nodes[t].left;
      } else {
        if (k < left_size + nodes[t].cnt) return nodes[t].key;
        k -= left_size + nodes[t].cnt;
        t = nodes[t].right;
      }
    }
    return -1;
  }
};

/**
 * @brief Link-Cut Tree (splay-based) for dynamic forest path queries.
 *
 * This implementation maintains path sums over node values.
 */
struct LinkCutTree {
  /**
   * @brief Node in auxiliary splay trees.
   */
  struct Node {
    I32 left, right, parent;
    I64 value, sum;
    bool reversed;

    Node(I64 v = 0) : left(-1), right(-1), parent(-1),
                       value(v), sum(v), reversed(false) {}
  };

  Vec<Node> nodes;

  /**
   * @brief Initializes forest with @p n isolated nodes.
   */
  LinkCutTree(I32 n) : nodes(n) {
    FOR(i, n) nodes[i] = Node(0);
  }

  /**
   * @brief Checks whether node is root of its auxiliary splay.
   */
  bool is_root(I32 v) {
    return nodes[v].parent == -1 ||
           (nodes[nodes[v].parent].left != v && nodes[nodes[v].parent].right != v);
  }

  /**
   * @brief Pushes lazy reverse tag.
   */
  void push(I32 v) {
    if (!nodes[v].reversed) return;
    std::swap(nodes[v].left, nodes[v].right);
    if (nodes[v].left != -1) nodes[nodes[v].left].reversed ^= 1;
    if (nodes[v].right != -1) nodes[nodes[v].right].reversed ^= 1;
    nodes[v].reversed = false;
  }

  /**
   * @brief Recomputes aggregate for node @p v.
   */
  void update(I32 v) {
    nodes[v].sum = nodes[v].value;
    if (nodes[v].left != -1) nodes[v].sum += nodes[nodes[v].left].sum;
    if (nodes[v].right != -1) nodes[v].sum += nodes[nodes[v].right].sum;
  }

  /**
   * @brief Single rotation in splay tree.
   */
  void rotate(I32 v) {
    I32 p = nodes[v].parent;
    I32 g = nodes[p].parent;

    if (!is_root(p)) {
      if (nodes[g].left == p) nodes[g].left = v;
      else nodes[g].right = v;
    }

    if (nodes[p].left == v) {
      nodes[p].left = nodes[v].right;
      if (nodes[v].right != -1) nodes[nodes[v].right].parent = p;
      nodes[v].right = p;
    } else {
      nodes[p].right = nodes[v].left;
      if (nodes[v].left != -1) nodes[nodes[v].left].parent = p;
      nodes[v].left = p;
    }

    nodes[v].parent = g;
    nodes[p].parent = v;
    update(p);
    update(v);
  }

  /**
   * @brief Splays node to root of auxiliary tree.
   */
  void splay(I32 v) {
    while (!is_root(v)) {
      I32 p = nodes[v].parent;
      if (!is_root(p)) push(nodes[p].parent);
      push(p);
      push(v);

      if (!is_root(p)) {
        if ((nodes[nodes[p].parent].left == p) == (nodes[p].left == v)) {
          rotate(p);
        } else {
          rotate(v);
        }
      }
      rotate(v);
    }
    push(v);
    update(v);
  }

  /**
   * @brief Exposes preferred path root->v and returns last accessed node.
   */
  I32 access(I32 v) {
    I32 last = -1;
    for (I32 w = v; w != -1; w = nodes[w].parent) {
      splay(w);
      I32 old_right = nodes[w].right;
      if (old_right != -1) nodes[old_right].parent = -1;
      nodes[w].right = last;
      if (last != -1) nodes[last].parent = w;
      update(w);
      last = w;
    }
    splay(v);
    return last;
  }

  /**
   * @brief Makes node @p v the represented-tree root.
   */
  void make_root(I32 v) {
    access(v);
    nodes[v].reversed ^= 1;
  }

  /**
   * @brief Links two trees by edge (u, v).
   */
  void link(I32 u, I32 v) {
    make_root(u);
    nodes[u].parent = v;
  }

  /**
   * @brief Cuts edge (u, v) assuming it exists.
   */
  void cut(I32 u, I32 v) {
    make_root(u);
    access(v);
    nodes[v].left = -1;
    nodes[u].parent = -1;
    update(v);
  }

  /**
   * @brief Returns sum of node values on path u-v.
   */
  I64 path_sum(I32 u, I32 v) {
    make_root(u);
    access(v);
    return nodes[v].sum;
  }
};

//===----------------------------------------------------------------------===//
