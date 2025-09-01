#pragma once
#include "../templates/Types.hpp"
#include "../templates/Macros.hpp"
#include "../templates/Math.hpp"
#include "../templates/Constants.hpp"
#include <algorithm>

//===----------------------------------------------------------------------===//
//===================== Advanced Data Structures Module ======================//

// Segment Tree for range queries and updates.
template <typename T, typename F = std::plus<T>>
struct SegmentTree {
  I32 n;
  VC<T> tree;
  F op;
  T identity;
  
  SegmentTree(I32 size, T id = T{}, F func = F{}) 
    : n(1), op(func), identity(id) {
    while (n < size) n *= 2;
    tree.assign(2 * n, identity);
  }
  
  SegmentTree(const VC<T>& v, T id = T{}, F func = F{}) 
    : SegmentTree(sz(v), id, func) {
    FOR(i, sz(v)) tree[n + i] = v[i];
    FOR_R(i, 1, n) tree[i] = op(tree[2*i], tree[2*i + 1]);
  }
  
  void update(I32 pos, T val) {
    pos += n;
    tree[pos] = val;
    while (pos > 1) {
      tree[pos >> 1] = op(tree[pos], tree[pos ^ 1]);
      pos >>= 1;
    }
  }
  
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
  
  T get(I32 pos) { return tree[n + pos]; }
};

// Lazy Propagation Segment Tree.
template <typename T, typename U>
struct LazySegTree {
  I32 n;
  VC<T> tree;
  VC<U> lazy;
  
  LazySegTree(I32 size) : n(1) {
    while (n < size) n *= 2;
    tree.assign(2 * n, T{});
    lazy.assign(2 * n, U{});
  }
  
  void push(I32 v, I32 tl, I32 tr) {
    if (lazy[v] == U{}) return;
    tree[v] += lazy[v] * (tr - tl);
    if (tl + 1 != tr) {
      lazy[2*v] += lazy[v];
      lazy[2*v + 1] += lazy[v];
    }
    lazy[v] = U{};
  }
  
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
  
  T query(I32 l, I32 r, I32 v = 1, I32 tl = 0, I32 tr = -1) {
    if (tr == -1) tr = n;
    if (l >= tr || r <= tl) return T{};
    push(v, tl, tr);
    if (l <= tl && tr <= r) return tree[v];
    I32 tm = (tl + tr) / 2;
    return query(l, r, 2*v, tl, tm) + query(l, r, 2*v + 1, tm, tr);
  }
};

// Fenwick Tree (Binary Indexed Tree).
template <typename T>
struct FenwickTree {
  I32 n;
  VC<T> tree;
  
  FenwickTree(I32 size) : n(size), tree(size + 1, T{}) {}
  
  void add(I32 idx, T val) {
    idx++;  // 1-indexed internally.
    while (idx <= n) {
      tree[idx] += val;
      idx += idx & -idx;
    }
  }
  
  T sum(I32 idx) {  // Sum [0, idx].
    idx++;
    T res{};
    while (idx > 0) {
      res += tree[idx];
      idx -= idx & -idx;
    }
    return res;
  }
  
  T range_sum(I32 l, I32 r) {  // Sum [l, r].
    return sum(r) - (l > 0 ? sum(l - 1) : T{});
  }
  
  // Find first index where prefix sum >= val (binary lifting).
  I32 lower_bound(T val) {
    I32 pos = 0;
    I32 pw = 1;
    while (pw <= n) pw *= 2;
    pw /= 2;
    
    while (pw > 0) {
      if (pos + pw <= n && tree[pos + pw] < val) {
        pos += pw;
        val -= tree[pos];
      }
      pw /= 2;
    }
    return pos;
  }
};

// 2D Fenwick Tree.
template <typename T>
struct FenwickTree2D {
  I32 n, m;
  VVC<T> tree;
  
  FenwickTree2D(I32 rows, I32 cols) : n(rows), m(cols), tree(n + 1, VC<T>(m + 1)) {}
  
  void add(I32 x, I32 y, T val) {
    x++; y++;
    for (I32 i = x; i <= n; i += i & -i) {
      for (I32 j = y; j <= m; j += j & -j) {
        tree[i][j] += val;
      }
    }
  }
  
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
  
  T range_sum(I32 x1, I32 y1, I32 x2, I32 y2) {
    return sum(x2, y2) - sum(x1 - 1, y2) - sum(x2, y1 - 1) + sum(x1 - 1, y1 - 1);
  }
};

// Sparse Table for O(1) range minimum/maximum queries.
template <typename T, typename F = std::function<T(const T&, const T&)>>
struct SparseTable {
  VVC<T> table;
  VC<I32> lg;
  F op;
  
  SparseTable(const VC<T>& v, F func = [](const T& a, const T& b) { return _min(a, b); }) : op(func) {
    I32 n = sz(v);
    I32 max_log = 0;
    while ((1 << max_log) <= n) max_log++;
    
    table.assign(max_log, VC<T>(n));
    lg.assign(n + 1, 0);
    
    FOR(i, 2, n + 1) lg[i] = lg[i / 2] + 1;
    
    FOR(i, n) table[0][i] = v[i];
    
    FOR(k, 1, max_log) {
      FOR(i, n - (1 << k) + 1) {
        table[k][i] = op(table[k-1][i], table[k-1][i + (1 << (k-1))]);
      }
    }
  }
  
  T query(I32 l, I32 r) {  // [l, r]
    I32 k = lg[r - l + 1];
    return op(table[k][l], table[k][r - (1 << k) + 1]);
  }
};

// Persistent Segment Tree (for version history).
struct PersistentSegTree {
  struct Node {
    I64 val;
    I32 left, right;
    Node(I64 v = 0) : val(v), left(-1), right(-1) {}
  };
  
  VC<Node> nodes;
  VC<I32> roots;
  I32 n;
  
  PersistentSegTree(I32 size) : n(1) {
    while (n < size) n *= 2;
    nodes.reserve(n * 20);  // Pre-allocate for efficiency.
    build(0, n);
    roots.pb(0);
  }
  
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
  
  I64 query(I32 root, I32 l, I32 r, I32 tl = 0, I32 tr = -1) {
    if (tr == -1) tr = n;
    if (l >= tr || r <= tl) return 0;
    if (l <= tl && tr <= r) return nodes[root].val;
    I32 tm = (tl + tr) / 2;
    return query(nodes[root].left, l, r, tl, tm) + 
           query(nodes[root].right, l, r, tm, tr);
  }
  
  void new_version(I32 pos, I64 val) {
    roots.pb(update(roots.back(), pos, val));
  }
};

// Treap (Randomized Binary Search Tree).
struct Treap {
  struct Node {
    I64 key, priority;
    I32 left, right, size;
    I64 sum;
    
    Node(I64 k) : key(k), priority(rand()), left(-1), right(-1), size(1), sum(k) {}
  };
  
  VC<Node> nodes;
  I32 root;
  
  Treap() : root(-1) { nodes.reserve(200000); }
  
  I32 get_size(I32 t) { return t == -1 ? 0 : nodes[t].size; }
  I64 get_sum(I32 t) { return t == -1 ? 0 : nodes[t].sum; }
  
  void update(I32 t) {
    if (t == -1) return;
    nodes[t].size = 1 + get_size(nodes[t].left) + get_size(nodes[t].right);
    nodes[t].sum = nodes[t].key + get_sum(nodes[t].left) + get_sum(nodes[t].right);
  }
  
  void split(I32 t, I64 key, I32& l, I32& r) {
    if (t == -1) {
      l = r = -1;
      return;
    }
    if (nodes[t].key <= key) {
      split(nodes[t].right, key, nodes[t].right, r);
      l = t;
    } else {
      split(nodes[t].left, key, l, nodes[t].left);
      r = t;
    }
    update(t);
  }
  
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
  
  void insert(I64 key) {
    I32 idx = sz(nodes);
    nodes.eb(key);
    I32 l, r;
    split(root, key, l, r);
    merge(l, l, idx);
    merge(root, l, r);
  }
  
  void erase(I64 key) {
    I32 l, m, r;
    split(root, key - 1, l, m);
    split(m, key, m, r);
    merge(root, l, r);
  }
  
  I64 kth_element(I32 k) {  // 0-indexed
    I32 t = root;
    while (t != -1) {
      I32 left_size = get_size(nodes[t].left);
      if (k == left_size) return nodes[t].key;
      if (k < left_size) {
        t = nodes[t].left;
      } else {
        k -= left_size + 1;
        t = nodes[t].right;
      }
    }
    return -1;
  }
};

// Link-Cut Tree for dynamic tree operations.
struct LinkCutTree {
  struct Node {
    I32 left, right, parent;
    I64 value, sum;
    bool reversed;
    
    Node(I64 v = 0) : left(-1), right(-1), parent(-1), 
                       value(v), sum(v), reversed(false) {}
  };
  
  VC<Node> nodes;
  
  LinkCutTree(I32 n) : nodes(n) {
    FOR(i, n) nodes[i] = Node(0);
  }
  
  bool is_root(I32 v) {
    return nodes[v].parent == -1 || 
           (nodes[nodes[v].parent].left != v && nodes[nodes[v].parent].right != v);
  }
  
  void push(I32 v) {
    if (!nodes[v].reversed) return;
    std::swap(nodes[v].left, nodes[v].right);
    if (nodes[v].left != -1) nodes[nodes[v].left].reversed ^= 1;
    if (nodes[v].right != -1) nodes[nodes[v].right].reversed ^= 1;
    nodes[v].reversed = false;
  }
  
  void update(I32 v) {
    nodes[v].sum = nodes[v].value;
    if (nodes[v].left != -1) nodes[v].sum += nodes[nodes[v].left].sum;
    if (nodes[v].right != -1) nodes[v].sum += nodes[nodes[v].right].sum;
  }
  
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
  
  I32 access(I32 v) {
    I32 last = -1;
    for (I32 w = v; w != -1; w = nodes[w].parent) {
      splay(w);
      nodes[w].right = last;
      update(w);
      last = w;
    }
    splay(v);
    return last;
  }
  
  void make_root(I32 v) {
    access(v);
    nodes[v].reversed ^= 1;
  }
  
  void link(I32 u, I32 v) {
    make_root(u);
    nodes[u].parent = v;
  }
  
  void cut(I32 u, I32 v) {
    make_root(u);
    access(v);
    nodes[v].left = -1;
    nodes[u].parent = -1;
    update(v);
  }
  
  I64 path_sum(I32 u, I32 v) {
    make_root(u);
    access(v);
    return nodes[v].sum;
  }
};