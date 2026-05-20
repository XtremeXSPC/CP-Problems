#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_TIMER
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"
#include "modules/data_structures/LazySegTree.hpp"

//===----------------------------------------------------------------------===//
/* Mathematical Helpers and Constants */

inline auto add(I64 a, I64 b) -> I64 {
  a = (a + b) % MOD2;
  if (a < 0) a += MOD2;
  return a;
}

inline auto sub(I64 a, I64 b) -> I64 {
  a = (a - b) % MOD2;
  if (a < 0) a += MOD2;
  return a;
}

inline auto mul(I64 a, I64 b) -> I64 {
  a = (a % MOD2) * (b % MOD2) % MOD2;
  if (a < 0) a += MOD2;
  return a;
}

inline auto power(I64 base, I64 exp) -> I64 {
  I64 res = 1;
  base %= MOD2;
  while (exp > 0) {
    if (exp & 1) res = mul(res, base);
    base = mul(base, base);
    exp >>= 1;
  }
  return res;
}

constexpr I32 MAX_FACT = 1'300'005;
VecI64 fact(MAX_FACT), inv_fact(MAX_FACT);

void precompute() {
  fact[0] = 1;
  FOR(i, 1, MAX_FACT) {
    fact[i] = mul(fact[i - 1], i);
  }
  inv_fact[MAX_FACT - 1] = power(fact[MAX_FACT - 1], MOD2 - 2);
  for (I32 i = MAX_FACT - 2; i >= 0; --i) {
    inv_fact[i] = mul(inv_fact[i + 1], i + 1);
  }
}

inline auto C(I32 n, I32 k) -> I64 {
  if (k < 0 || k > n || n < 0) return 0;
  return mul(fact[n], mul(inv_fact[k], inv_fact[n - k]));
}

//===----------------------------------------------------------------------===//
/* Data Structures */

struct Node {
  I64 sf  = 0;
  I64 sf2 = 0;
  I64 su  = 0;
  I64 su2 = 0;
  I64 sfu = 0;
};

struct Tag {
  I64 x = 0;
  I64 y = 0;
};

struct ScaledTag {
  I64 x   = 0;
  I64 y   = 0;
  I64 len = 0;
};

inline auto operator+(const Node& left, const Node& right) -> Node {
  Node parent;
  parent.sf  = add(left.sf, right.sf);
  parent.sf2 = add(left.sf2, right.sf2);
  parent.su  = add(left.su, right.su);
  parent.su2 = add(left.su2, right.su2);
  parent.sfu = add(left.sfu, right.sfu);
  return parent;
}

inline auto operator*(const Tag& tag, I32 len) -> ScaledTag {
  return ScaledTag{tag.x, tag.y, as<I64>(len)};
}

inline auto operator+=(Node& node, const ScaledTag& stag) -> Node& {
  I64 x    = stag.x;
  I64 y    = stag.y;
  I64 len  = stag.len;
  node.sfu = add(node.sfu, add(mul(y, node.sf), add(mul(x, node.su), mul(len, mul(x, y)))));
  node.sf2 = add(node.sf2, add(mul(2, mul(x, node.sf)), mul(len, mul(x, x))));
  node.su2 = add(node.su2, add(mul(2, mul(y, node.su)), mul(len, mul(y, y))));
  node.sf  = add(node.sf, mul(len, x));
  node.su  = add(node.su, mul(len, y));
  return node;
}

inline auto operator+=(Tag& left, const Tag& right) -> Tag& {
  left.x = add(left.x, right.x);
  left.y = add(left.y, right.y);
  return left;
}

struct Fenwick {
  I32 n;
  VecI64 tree;

  explicit Fenwick(I32 n_) : n(n_), tree(n_ + 1, 0) {}

  void add(I32 idx, I64 val) {
    for (; idx <= n; idx += idx & -idx) {
      tree[idx] += val;
    }
  }

  auto query(I32 idx) -> I64 {
    I64 sum = 0;
    for (; idx > 0; idx -= idx & -idx) {
      sum += tree[idx];
    }
    return sum;
  }

  auto query(I32 L, I32 R) -> I64 {
    if (L > R) return 0;
    return query(R) - query(L - 1);
  }
};

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n, q);
  VecI64 a(n + 1);
  FOR(i, 1, n + 1) {
    cin >> a[i];
  }

  Fenwick bit_F(n), bit_u(n);
  VecI64 F(n + 1, 0), u(n + 1, 0);
  FOR(i, 1, n + 1) {
    if (a[i] >= 0) {
      F[i] = F[i - 1] + a[i];
      u[i] = u[i - 1];
      bit_F.add(i, a[i]);
    } else {
      F[i] = F[i - 1];
      u[i] = u[i - 1] + 1;
      bit_u.add(i, 1);
    }
  }

  LazySegTree<Node, Tag> st(n + 2);
  FOR(i, 1, n + 1) {
    st.update(i, i + 1, Tag{F[i] % MOD2, u[i] % MOD2});
  }

  FOR(q_idx, q) {
    INT(op);
    if (op == 1) {
      INT(p, v);
      I64 old_f = (a[p] >= 0 ? a[p] : 0);
      I64 old_u = (a[p] == -1 ? 1 : 0);
      I64 new_f = (v >= 0 ? v : 0);
      I64 new_u = (v == -1 ? 1 : 0);
      I64 df = new_f - old_f;
      I64 du = new_u - old_u;

      bit_F.add(p, df);
      bit_u.add(p, du);

      I64 x = (df % MOD2 + MOD2) % MOD2;
      I64 y = (du % MOD2 + MOD2) % MOD2;
      st.update(p, n + 1, Tag{x, y});

      a[p] = v;
    } else {
      INT(l, r, m);
      I64 fl1 = 0, ul1 = 0;
      if (l > 1) {
        Node res = st.query(l - 1, l);
        fl1 = res.sf;
        ul1 = res.su;
      }

      I64 S_fixed = bit_F.query(l, r);
      I64 u_count = bit_u.query(l, r);
      I64 M_free = m - S_fixed;

      if (M_free < 0) {
        OUT(0);
        continue;
      }

      I64 len = r - l + 1;
      Node nodes = st.query(l, r + 1);

      if (u_count == 0) {
        if (M_free == 0) {
          I64 Sum_F2 = add(sub(nodes.sf2, mul(2, mul(fl1, nodes.sf))), mul(len, mul(fl1, fl1)));
          OUT(Sum_F2);
        } else {
          OUT(0);
        }
        continue;
      }

      I64 Sum_F2  = add(sub(nodes.sf2, mul(2, mul(fl1, nodes.sf))), mul(len, mul(fl1, fl1)));
      I64 term_Fu = add(sub(nodes.sfu, add(mul(ul1, nodes.sf), mul(fl1, nodes.su))), mul(len, mul(fl1, ul1)));
      I64 term_u  = sub(nodes.su, mul(len, ul1));
      I64 Sum_Fu_linear = add(mul(2, term_Fu), term_u);

      I64 term_u2 = add(sub(nodes.su2, mul(2, mul(ul1, nodes.su))), mul(len, mul(ul1, ul1)));
      I64 Sum_u2_linear = add(term_u2, term_u);

      I64 C0 = C(M_free + u_count - 1, u_count - 1);
      I64 C1 = C(M_free + u_count - 1, u_count);
      I64 C2 = C(M_free + u_count - 1, u_count + 1);

      I64 ans = add(mul(Sum_F2, C0), add(mul(Sum_Fu_linear, C1), mul(Sum_u2_linear, C2)));
      OUT(ans);
    }
  }
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
#endif

  precompute();

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
