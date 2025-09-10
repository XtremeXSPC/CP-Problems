#include <bits/stdc++.h>

using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;

using u128 = unsigned __int128;
using i128 = __int128;

template <class T>
constexpr T power(T a, u64 b, T res = 1) {
  for (; b != 0; b /= 2, a *= a) {
    if (b & 1) {
      res *= a;
    }
  }
  return res;
}

template <u32 P>
constexpr u32 mulMod(u32 a, u32 b) {
  return u64(a) * b % P;
}

template <u64 P>
constexpr u64 mulMod(u64 a, u64 b) {
  u64 res = a * b - u64(1.L * a * b / P - 0.5L) * P;
  res %= P;
  return res;
}

constexpr i64 safeMod(i64 x, i64 m) {
  x %= m;
  if (x < 0) {
    x += m;
  }
  return x;
}

constexpr std::pair<i64, i64> invGcd(i64 a, i64 b) {
  a = safeMod(a, b);
  if (a == 0) {
    return {b, 0};
  }

  i64 s = b, t = a;
  i64 m0 = 0, m1 = 1;

  while (t) {
    i64 u = s / t;
    s -= t * u;
    m0 -= m1 * u;

    std::swap(s, t);
    std::swap(m0, m1);
  }

  if (m0 < 0) {
    m0 += b / s;
  }

  return {s, m0};
}

template <std::unsigned_integral U, U P>
struct ModIntBase {
 public:
  constexpr ModIntBase() : x(0) {}
  template <std::unsigned_integral T>
  constexpr ModIntBase(T x_) : x(x_ % mod()) {}
  template <std::signed_integral T>
  constexpr ModIntBase(T x_) {
    using S = std::make_signed_t<U>;
    S v     = x_ % S(mod());
    if (v < 0) {
      v += mod();
    }
    x = v;
  }

  constexpr static U mod() { return P; }

  constexpr U val() const { return x; }

  constexpr ModIntBase operator-() const {
    ModIntBase res;
    res.x = (x == 0 ? 0 : mod() - x);
    return res;
  }

  constexpr ModIntBase inv() const { return power(*this, mod() - 2); }

  constexpr ModIntBase& operator*=(const ModIntBase& rhs) & {
    x = mulMod<mod()>(x, rhs.val());
    return *this;
  }
  constexpr ModIntBase& operator+=(const ModIntBase& rhs) & {
    x += rhs.val();
    if (x >= mod()) {
      x -= mod();
    }
    return *this;
  }
  constexpr ModIntBase& operator-=(const ModIntBase& rhs) & {
    x -= rhs.val();
    if (x >= mod()) {
      x += mod();
    }
    return *this;
  }
  constexpr ModIntBase& operator/=(const ModIntBase& rhs) & { return *this *= rhs.inv(); }

  friend constexpr ModIntBase operator*(ModIntBase lhs, const ModIntBase& rhs) {
    lhs *= rhs;
    return lhs;
  }
  friend constexpr ModIntBase operator+(ModIntBase lhs, const ModIntBase& rhs) {
    lhs += rhs;
    return lhs;
  }
  friend constexpr ModIntBase operator-(ModIntBase lhs, const ModIntBase& rhs) {
    lhs -= rhs;
    return lhs;
  }
  friend constexpr ModIntBase operator/(ModIntBase lhs, const ModIntBase& rhs) {
    lhs /= rhs;
    return lhs;
  }

  friend constexpr std::istream& operator>>(std::istream& is, ModIntBase& a) {
    i64 i;
    is >> i;
    a = i;
    return is;
  }
  friend constexpr std::ostream& operator<<(std::ostream& os, const ModIntBase& a) {
    return os << a.val();
  }

  friend constexpr bool operator==(const ModIntBase& lhs, const ModIntBase& rhs) {
    return lhs.val() == rhs.val();
  }
  friend constexpr std::strong_ordering operator<=>(const ModIntBase& lhs, const ModIntBase& rhs) {
    return lhs.val() <=> rhs.val();
  }

 private:
  U x;
};

template <u32 P>
using ModInt = ModIntBase<u32, P>;
template <u64 P>
using ModInt64 = ModIntBase<u64, P>;

struct Barrett {
 public:
  Barrett(u32 m_) : m(m_), im((u64)(-1) / m_ + 1) {}

  constexpr u32 mod() const { return m; }

  constexpr u32 mul(u32 a, u32 b) const {
    u64 z = a;
    z *= b;

    u64 x = u64((u128(z) * im) >> 64);

    u32 v = u32(z - x * m);
    if (m <= v) {
      v += m;
    }
    return v;
  }

 private:
  u32 m;
  u64 im;
};

template <u32 Id>
struct DynModInt {
 public:
  constexpr DynModInt() : x(0) {}
  template <std::unsigned_integral T>
  constexpr DynModInt(T x_) : x(x_ % mod()) {}
  template <std::signed_integral T>
  constexpr DynModInt(T x_) {
    int v = x_ % int(mod());
    if (v < 0) {
      v += mod();
    }
    x = v;
  }

  constexpr static void setMod(u32 m) { bt = m; }

  static u32 mod() { return bt.mod(); }

  constexpr u32 val() const { return x; }

  constexpr DynModInt operator-() const {
    DynModInt res;
    res.x = (x == 0 ? 0 : mod() - x);
    return res;
  }

  constexpr DynModInt inv() const {
    auto v = invGcd(x, mod());
    assert(v.first == 1);
    return v.second;
  }

  constexpr DynModInt& operator*=(const DynModInt& rhs) & {
    x = bt.mul(x, rhs.val());
    return *this;
  }
  constexpr DynModInt& operator+=(const DynModInt& rhs) & {
    x += rhs.val();
    if (x >= mod()) {
      x -= mod();
    }
    return *this;
  }
  constexpr DynModInt& operator-=(const DynModInt& rhs) & {
    x -= rhs.val();
    if (x >= mod()) {
      x += mod();
    }
    return *this;
  }
  constexpr DynModInt& operator/=(const DynModInt& rhs) & { return *this *= rhs.inv(); }

  friend constexpr DynModInt operator*(DynModInt lhs, const DynModInt& rhs) {
    lhs *= rhs;
    return lhs;
  }
  friend constexpr DynModInt operator+(DynModInt lhs, const DynModInt& rhs) {
    lhs += rhs;
    return lhs;
  }
  friend constexpr DynModInt operator-(DynModInt lhs, const DynModInt& rhs) {
    lhs -= rhs;
    return lhs;
  }
  friend constexpr DynModInt operator/(DynModInt lhs, const DynModInt& rhs) {
    lhs /= rhs;
    return lhs;
  }

  friend constexpr std::istream& operator>>(std::istream& is, DynModInt& a) {
    i64 i;
    is >> i;
    a = i;
    return is;
  }
  friend constexpr std::ostream& operator<<(std::ostream& os, const DynModInt& a) {
    return os << a.val();
  }

  friend constexpr bool operator==(const DynModInt& lhs, const DynModInt& rhs) {
    return lhs.val() == rhs.val();
  }
  friend constexpr std::strong_ordering operator<=>(const DynModInt& lhs, const DynModInt& rhs) {
    return lhs.val() <=> rhs.val();
  }

 private:
  u32            x;
  static Barrett bt;
};

template <u32 Id>
Barrett DynModInt<Id>::bt = 998244353;

using Z = ModInt<998244353>;

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

const Z Base = rng();

constexpr int L = 8E6;
constexpr int N = 1 << 18;

struct Node {
  int l;
  int r;
  Z   h;
} t[L];

int tot;

Z pw[N + 1];

void pull(int p, int len) {
  t[p].h = t[t[p].l].h * pw[len] + t[t[p].r].h;
}

int add(int p, int l, int r, int x) {
  int q = ++tot;
  t[q]  = t[p];
  if (r - l == 1) {
    t[q].h += 1;
    return q;
  }
  int m = (l + r) / 2;
  if (x < m) {
    t[q].l = add(t[q].l, l, m, x);
  } else {
    t[q].r = add(t[q].r, m, r, x);
  }
  pull(q, r - m);
  return q;
}

void dfs(int p, int l, int r) {
  if (r - l == 1) {
    int v = t[p].h.val();
    while (v--) {
      std::cerr << l << " ";
    }
    return;
  }
  int m = (l + r) / 2;
  dfs(t[p].r, m, r);
  dfs(t[p].l, l, m);
}

void solve() {
  int n;
  std::cin >> n;

  std::vector<int> l(n), r(n);
  for (int i = 0; i < n; i++) {
    std::cin >> l[i] >> r[i];
    l[i]--;
    r[i]--;
  }

  std::vector<int> layer(n), dep(n);
  [&](this auto&& self, int x) -> void {
    if (l[x] == -1) {
      layer[x] = 0;
    } else {
      dep[l[x]] = dep[x] + 1;
      dep[r[x]] = dep[x] + 1;
      self(l[x]);
      self(r[x]);
      layer[x] = std::max(layer[l[x]], layer[r[x]] + 1);
    }
  }(0);

  std::vector<std::vector<int>> vec(n);
  for (int i = 0; i < n; i++) {
    vec[layer[i]].push_back(i);
  }

  std::vector<int> rt(n), rk(n);

  std::vector<int> ans;

  int cur = -1;
  for (int d = 0; d < n; d++) {
    if (vec[d].empty()) {
      break;
    }

    std::sort(vec[d].begin(), vec[d].end(), [&](int i, int j) { return dep[i] > dep[j]; });
    for (auto x : vec[d]) {
      if (l[x] != -1) {
        rt[x] = add(rt[l[x]], 0, N, rk[r[x]]);
      }
    }

    std::sort(vec[d].begin(), vec[d].end(), [&](int i, int j) {
      if (t[rt[i]].h == t[rt[j]].h) {
        return i < j;
      }
      int u = rt[i], v = rt[j];
      int len = N;
      while (len > 1) {
        if (t[t[u].r].h == t[t[v].r].h) {
          u = t[u].l;
          v = t[v].l;
        } else {
          u = t[u].r;
          v = t[v].r;
        }
        len /= 2;
      }
      return t[u].h < t[v].h;
    });
    for (int i = 0; i < vec[d].size(); i++) {
      if (i == 0 || t[rt[vec[d][i]]].h != t[rt[vec[d][i - 1]]].h) {
        cur++;
      }
      rk[vec[d][i]] = cur;
    }
    ans.insert(ans.end(), vec[d].begin(), vec[d].end());
  }
  for (auto x : ans) {
    std::cout << x + 1 << " \n"[x == ans.back()];
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  pw[0] = 1;
  for (int i = 1; i <= N; i++) {
    pw[i] = pw[i - 1] * Base;
  }

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }

  return 0;
}