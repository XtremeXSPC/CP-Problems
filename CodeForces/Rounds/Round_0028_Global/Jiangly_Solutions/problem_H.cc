//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Global Round 28
 * @author: Codeforces User "Jiangly"
 * @file: problem_H.cc
 *
 * @status: PASSED
 */
//===---------------------------------------------------------------------===//
/* Included library */

#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "algo/debug.h"
#else
#define debug(...) 42
#endif

using i64  = long long;
using u64  = unsigned long long;
using u32  = unsigned;
using u128 = unsigned __int128;

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
  friend constexpr std::ostream& operator<<(std::ostream& os, const ModIntBase& a) { return os << a.val(); }

  friend constexpr bool operator==(const ModIntBase& lhs, const ModIntBase& rhs) { return lhs.val() == rhs.val(); }

  friend constexpr std::strong_ordering operator<=>(const ModIntBase& lhs, const ModIntBase& rhs) { return lhs.val() <=> rhs.val(); }

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
  friend constexpr std::ostream& operator<<(std::ostream& os, const DynModInt& a) { return os << a.val(); }

  friend constexpr bool operator==(const DynModInt& lhs, const DynModInt& rhs) { return lhs.val() == rhs.val(); }

  friend constexpr std::strong_ordering operator<=>(const DynModInt& lhs, const DynModInt& rhs) { return lhs.val() <=> rhs.val(); }

private:
  u32            x;
  static Barrett bt;
};

template <u32 Id>
Barrett DynModInt<Id>::bt = 998244353;

using Z = ModInt<998244353>;

template <typename T>
struct Fenwick {
  int            n;
  std::vector<T> a;

  Fenwick(int n_ = 0) { init(n_); }

  void init(int n_) {
    n = n_;
    a.assign(n, T{});
  }

  void add(int x, const T& v) {
    for (int i = x + 1; i <= n; i += i & -i) {
      a[i - 1] = a[i - 1] + v;
    }
  }

  T sum(int x) {
    T ans{};
    for (int i = x; i > 0; i -= i & -i) {
      ans = ans + a[i - 1];
    }
    return ans;
  }

  T rangeSum(int l, int r) { return sum(r) - sum(l); }

  int select(const T& k) {
    int x = 0;
    T   cur{};
    for (int i = 1 << std::__lg(n); i; i /= 2) {
      if (x + i <= n && cur + a[x + i - 1] <= k) {
        x += i;
        cur = cur + a[x - 1];
      }
    }
    return x;
  }
};

void solve() {
  std::string s;
  std::cin >> s;

  int n = s.size();

  Fenwick<Z> fen(n);

  std::vector<int> l(n);
  for (int i = 0; i < n; i++) {
    l[i] = s[i] == '1' ? i + 1 : i == 0 ? 0 : l[i - 1];
  }

  for (int i = 0; i < n; i++) {
    fen.add(i, 1);
    if (l[i] <= i) {
      fen.add(i - l[i], fen.rangeSum(i - l[i] + 1, n));
    }
  }

  std::cout << fen.sum(n) << "\n";
}

//===---------------------------------------------------------------------===//
/* Main function */

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }

  return 0;
}

//===---------------------------------------------------------------------===//