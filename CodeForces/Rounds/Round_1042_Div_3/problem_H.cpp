//===----------------------------------------------------------------------===//
/**
 * @file: problem_H.cpp
 * @brief: Codeforces Round 1042 (Div. 3) - Problem H
 * @author: Costantino Lombardi
 *
 * @status: PASSED
 */
//===----------------------------------------------------------------------===//
/* Included library */

// clang-format off
// Sanitaze macro:
#ifdef USE_CLANG_SANITIZE
  #include "PCH.h"
#else
  #include <bits/stdc++.h>
#endif

using namespace std;

//===----------------------------------------------------------------------===//
/* Macros and Type Aliases */

// Debug macro:
#ifdef LOCAL
  #include "debug.h"
#else
  #define debug(...) 42
#endif
// clang-format on

// Type aliases
using ll   = long long;
using vi   = vector<int>;
using pii  = pair<int, int>;
using vll  = vector<ll>;
using vpii = vector<pii>;

// Constants
constexpr int MOD  = 1000000007;
constexpr int INF  = 1e9;
constexpr ll  LINF = 1e18;
constexpr int LIM  = 1000000 + 5;

//===----------------------------------------------------------------------===//
/* Data Types and Function Definitions */

int  spf[LIM];
int  mu[LIM];
bool sieve_done = false;

/**
 * @brief Initializes the sieve for calculating smallest prime factor (spf) and Mobius function (mu).
 */
void init_sieve() {
  if (sieve_done)
    return;
  vector<int> pr;
  mu[1] = 1;
  for (int i = 2; i < LIM; ++i) {
    if (!spf[i]) {
      spf[i] = i;
      pr.push_back(i);
      mu[i] = -1;
    }
    for (int p : pr) {
      ll x = 1LL * p * i;
      if (x >= LIM)
        break;
      spf[x] = p;
      if (p == spf[i]) {
        mu[x] = 0;
        break;
      } else {
        mu[x] = -mu[i];
      }
    }
  }
  sieve_done = true;
}

/**
 * @brief Gets the prime factors of a number x.
 * @param x The number.
 * @return A vector of prime factors.
 */
inline vi getpf(int x) {
  vi r;
  while (x > 1) {
    int p = spf[x];
    r.push_back(p);
    while (x % p == 0)
      x /= p;
  }
  return r;
}

/**
 * @brief Gets the square-free divisors from a vector of prime factors.
 * @param pf The vector of prime factors.
 * @return A vector of square-free divisors.
 */
inline vi getsqd(const vi& pf) {
  vi d = {1};
  for (int p : pf) {
    size_t sz = d.size();
    for (size_t i = 0; i < sz; ++i)
      d.push_back(d[i] * p);
  }
  return d;
}

// Function to solve a single test case
void solve() {
  // Start timer
  Timer timer;

  // Read input values
  int n, m;
  cin >> n >> m;
  vi a((size_t)n + 1);
  for (int i = 1; i <= n; ++i)
    cin >> a[(size_t)i];

  vi cnt((size_t)m + 1, 0);
  for (int i = 1; i <= n; ++i)
    ++cnt[(size_t)a[(size_t)i]];

  vi g((size_t)m + 1, 0);
  for (int d = 1; d <= m; ++d) {
    for (int k = d; k <= m; k += d)
      g[(size_t)d] += cnt[(size_t)k];
  }

  unordered_map<int, int> id;
  id.reserve((size_t)n * 2);
  vi         vals;
  vector<vi> pos;
  for (int i = 1; i <= n; ++i) {
    int  x  = a[(size_t)i];
    auto it = id.find(x);
    if (it == id.end()) {
      int idx = (int)vals.size();
      id[x]   = idx;
      vals.push_back(x);
      pos.push_back(vi());
      pos[(size_t)idx].push_back(i);
    } else {
      pos[(size_t)it->second].push_back(i);
    }
  }

  int        U = (int)vals.size();
  vector<vi> pfv((size_t)U), sqdv((size_t)U);
  for (int z = 0; z < U; ++z) {
    if (vals[(size_t)z] == 1) {
      pfv[(size_t)z]  = {};
      sqdv[(size_t)z] = {1};
    } else {
      pfv[(size_t)z]  = getpf(vals[(size_t)z]);
      sqdv[(size_t)z] = getsqd(pfv[(size_t)z]);
    }
  }

  auto F = [&](int z, vi& gg) -> int {
    int s = 0;
    for (int d : sqdv[(size_t)z])
      s += mu[(size_t)d] * gg[(size_t)d];
    return s;
  };

  vi ones;
  if (id.count(1)) {
    int z = id[1];
    for (int idx : pos[(size_t)z])
      ones.push_back(idx);
  }

  if ((int)ones.size() >= 4) {
    cout << ones[0] << " " << ones[1] << " " << ones[2] << " " << ones[3] << "\n";
    return;
  }
  if ((int)ones.size() == 3) {
    int x = -1;
    for (int i = 1; i <= n; ++i) {
      if (i != ones[0] && i != ones[1] && i != ones[2]) {
        x = i;
        break;
      }
    }
    if (x == -1) {
      cout << 0 << "\n";
      return;
    }
    cout << ones[0] << " " << ones[1] << " " << ones[2] << " " << x << "\n";
    return;
  }
  if ((int)ones.size() == 2) {
    int x = -1, y = -1;
    for (int i = 1; i <= n; ++i) {
      if (i == ones[0] || i == ones[1])
        continue;
      if (x == -1)
        x = i;
      else {
        y = i;
        break;
      }
    }
    if (y == -1) {
      cout << 0 << "\n";
      return;
    }
    cout << ones[0] << " " << x << " " << ones[1] << " " << y << "\n";
    return;
  }

  bool done = false;
  for (int z = 0; z < U && !done; ++z) {
    if (vals[(size_t)z] == 1)
      continue;
    if ((int)pos[(size_t)z].size() >= 2) {
      int cop = F(z, g);
      if (cop >= 2) {
        int v = vals[(size_t)z];
        int j = -1, k = -1;
        for (int i = 1; i <= n && (k == -1); ++i) {
          if (std::gcd(v, a[(size_t)i]) == 1) {
            if (j == -1)
              j = i;
            else
              k = i;
          }
        }
        if (j != -1 && k != -1) {
          cout << pos[(size_t)z][0] << " " << j << " " << pos[(size_t)z][1] << " " << k << "\n";
          done = true;
        }
      }
    }
  }
  if (done)
    return;

  if ((int)ones.size() == 1) {
    int  o  = ones[0];
    bool ok = false;
    for (int z = 0; z < U && !ok; ++z) {
      if (vals[(size_t)z] == 1)
        continue;
      int cop_no1 = F(z, g) - 1;
      if (cop_no1 >= 1) {
        int i_idx = pos[(size_t)z][0];
        int j_idx = -1;
        for (int t = 1; t <= n; ++t) {
          if (t == o || t == i_idx)
            continue;
          if (std::gcd(vals[(size_t)z], a[(size_t)t]) == 1) {
            j_idx = t;
            break;
          }
        }
        if (j_idx == -1)
          continue;
        int k_idx = -1;
        for (int t = 1; t <= n; ++t) {
          if (t == o || t == i_idx || t == j_idx)
            continue;
          k_idx = t;
          break;
        }
        if (k_idx == -1) {
          cout << 0 << "\n";
          ok = true;
          break;
        }
        cout << i_idx << " " << j_idx << " " << o << " " << k_idx << "\n";
        ok = true;
      }
    }
    if (!ok)
      cout << 0 << "\n";
    return;
  }

  vi deg_val((size_t)U, 0);
  for (int z = 0; z < U; ++z) {
    int fz = F(z, g);
    if (vals[(size_t)z] == 1)
      fz -= 1;
    deg_val[(size_t)z] = fz;
  }

  auto remove_idx = [&](int idx, int dlt) {
    int z = id[a[(size_t)idx]];
    for (int d : sqdv[(size_t)z])
      g[(size_t)d] += dlt;
  };

  auto find_second = [&](int ban1, int ban2) -> pii {
    for (int r = 1; r <= n; ++r) {
      if (r == ban1 || r == ban2)
        continue;
      int zr  = id[a[(size_t)r]];
      int can = 0;
      int s   = 0;
      for (int d : sqdv[(size_t)zr])
        s += mu[(size_t)d] * g[(size_t)d];
      if (a[(size_t)r] == 1)
        s -= 1;
      if (s >= 1) {
        for (int t = 1; t <= n; ++t) {
          if (t == ban1 || t == ban2 || t == r)
            continue;
          if (std::gcd(a[(size_t)r], a[(size_t)t]) == 1) {
            can = t;
            break;
          }
        }
        if (can) {
          return {r, can};
        }
      }
    }
    return {-1, -1};
  };

  int leaf_i = -1, leaf_j = -1;
  for (int z = 0; z < U; ++z) {
    if (deg_val[(size_t)z] == 1) {
      int ii = pos[(size_t)z][0];
      int jj = -1;
      for (int t = 1; t <= n; ++t) {
        if (t == ii)
          continue;
        if (std::gcd(a[(size_t)ii], a[(size_t)t]) == 1) {
          jj = t;
          break;
        }
      }
      if (jj != -1) {
        leaf_i = ii;
        leaf_j = jj;
        break;
      }
    }
  }
  if (leaf_i != -1) {
    remove_idx(leaf_i, -1);
    remove_idx(leaf_j, -1);
    auto pr = find_second(leaf_i, leaf_j);
    if (pr.first != -1) {
      cout << leaf_i << " " << leaf_j << " " << pr.first << " " << pr.second << "\n";
      done = true;
    } else {
      remove_idx(leaf_i, +1);
      remove_idx(leaf_j, +1);
      cout << 0 << "\n";
      return;
    }
  }
  if (done)
    return;

  vi order_idx;
  order_idx.reserve((size_t)n);
  for (int z = 0; z < U; ++z) {
    if (deg_val[(size_t)z] >= 1) {
      for (int idx : pos[(size_t)z])
        order_idx.push_back(idx);
    }
  }
  sort(order_idx.begin(), order_idx.end(), [&](int i, int j) {
    int zi = id[a[(size_t)i]], zj = id[a[(size_t)j]];
    if (deg_val[(size_t)zi] != deg_val[(size_t)zj])
      return deg_val[(size_t)zi] < deg_val[(size_t)zj];
    return i < j;
  });

  bool found = false;
  for (int ii : order_idx) {
    if (found)
      break;
    int take = min(30, n - 1);
    vi  cand;
    cand.reserve((size_t)take);
    for (int t = 1; t <= n && (int)cand.size() < take; ++t) {
      if (t == ii)
        continue;
      if (std::gcd(a[(size_t)ii], a[(size_t)t]) == 1)
        cand.push_back(t);
    }
    for (int jj : cand) {
      remove_idx(ii, -1);
      remove_idx(jj, -1);
      auto pr = find_second(ii, jj);
      if (pr.first != -1) {
        cout << ii << " " << jj << " " << pr.first << " " << pr.second << "\n";
        found = true;
        break;
      }
      remove_idx(ii, +1);
      remove_idx(jj, +1);
    }
  }
  if (!found)
    cout << 0 << "\n";
}

//===----------------------------------------------------------------------===//
/* Main function */

int main() {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  init_sieve();

  int t = 1;
  cin >> t;
  while (t--) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//