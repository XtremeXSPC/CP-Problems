//===----------------------------------------------------------------------===//
/**
 * @file: problem_D.cpp
 * @brief Codeforces Round 1048 (Div. 2) - Problem D
 * @author: Costantino Lombardi
 *
 * @status: In Progress
 */
//===----------------------------------------------------------------------===//
/* Included library */

// clang-format off
// Compiler optimizations:
#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC optimize("Ofast,unroll-loops,fast-math,O3")
  // Apple Silicon optimizations:
  #ifdef __aarch64__
    #pragma GCC target("+simd")
  #endif
#endif

#ifdef __clang__
  #pragma clang optimize on
#endif

// Sanitaze macro:
#ifdef USE_CLANG_SANITIZE
  #include "PCH.h"
#else
  #include <bits/stdc++.h>
#endif

// Debug macro:
#ifdef LOCAL
  #include "debug.h"
#else
  #define debug(...) 42
#endif
// clang-format on

//===----------------------------------------------------------------------===//
/* Type Aliases and Constants */

// Type aliases:
using ll       = long long;
using ull      = unsigned long long;
using ld       = long double;
using P_ii     = std::pair<int, int>;
using P_ll     = std::pair<long long, long long>;
using V_b      = std::vector<bool>;
using V_i      = std::vector<int>;
using V_ui     = std::vector<unsigned int>;
using V_ll     = std::vector<long long>;
using VV_i     = std::vector<std::vector<int>>;
using VV_ll    = std::vector<std::vector<long long>>;
using V_s      = std::vector<std::string>;
using VV_s     = std::vector<std::vector<std::string>>;
using VP_ii    = std::vector<std::pair<int, int>>;
using VVP_ii   = std::vector<std::vector<std::pair<int, int>>>;
using VP_ll    = std::vector<std::pair<long long, long long>>;
using VT_iii   = std::vector<std::tuple<int, int, int>>;
using Map_ll   = std::map<long long, long long>;
using VUMap_il = std::vector<std::unordered_map<int, ll>>;

// Mathematical constants:
constexpr long double PI   = 3.141592653589793238462643383279502884L;
constexpr long double E    = 2.718281828459045235360287471352662498L;
constexpr long double EPS  = 1e-9L;
constexpr int         INF  = 0x3f3f3f3f;
constexpr long long   LINF = 0x3f3f3f3f3f3f3f3fLL;
constexpr int         LIM  = 1000000 + 5;
constexpr int         MOD  = 1000000007;
constexpr int         MOD2 = 998244353;

const int INF_ST = 0x3f3f3f3f;

using namespace std;

//===----------------------------------------------------------------------===//
/* Data Types and Function Definitions */

// Generic Segment Tree for range operations.
template <typename T, typename U>
struct SegTree {
  int       n;
  vector<T> tree;

  T neutral;
  U op;

  SegTree(int size, T neutral_val, U merge_op) : n(size), neutral(neutral_val), op(merge_op) {
    tree.assign(4 * n, neutral);
  }

  void update_recursive(int node, int start, int end, int idx, T val) {
    if (start == end) {
      tree[node] = op(tree[node], val);
      return;
    }
    int mid = start + (end - start) / 2;
    if (start <= idx && idx <= mid) {
      update_recursive(2 * node, start, mid, idx, val);
    } else {
      update_recursive(2 * node + 1, mid + 1, end, idx, val);
    }
    tree[node] = op(tree[2 * node], tree[2 * node + 1]);
  }

  // Range query from l to r (1-indexed).
  T query_recursive(int node, int start, int end, int l, int r) {
    if (r < start || end < l || l > r) {
      return neutral;
    }
    if (l <= start && end <= r) {
      return tree[node];
    }
    int mid = start + (end - start) / 2;
    T   p1  = query_recursive(2 * node, start, mid, l, r);
    T   p2  = query_recursive(2 * node + 1, mid + 1, end, l, r);
    return op(p1, p2);
  }

  void update(int idx, T val) { update_recursive(1, 1, n, idx, val); }
  T    query(int l, int r) { return query_recursive(1, 1, n, l, r); }
};

// Max operation for segment tree.
auto max_op = [](int a, int b) { return max(a, b); };

// Function to solve a single test case.
void solve() {
  int n, q;
  cin >> n >> q;
  V_i a(n + 1);
  for (int i = 1; i <= n; ++i) cin >> a[i];

  // Calculate L[j]: index of first element to the left > a[j].
  V_i        L(n + 1, 0);
  stack<int> st;
  for (int i = 1; i <= n; ++i) {
    while (!st.empty() && a[st.top()] <= a[i]) {
      st.pop();
    }
    if (!st.empty()) {
      L[i] = st.top();
    }
    st.push(i);
  }

  // Calculate start[k]: max start index of decreasing subsequence (i,j,k) ending at k.
  V_i start_indices(n + 1, 0);

  // Build segment tree on L for fast queries.
  SegTree<int, decltype(max_op)> max_l_tree(n, 0, max_op);
  for (int k = 1; k <= n; ++k) {
    // Find max(L[j]) for all j < k with a[j] > a[k].
    if (a[k] < n) {
      start_indices[k] = max_l_tree.query(a[k] + 1, n);
    }
    if (L[k] > 0) {
      max_l_tree.update(a[k], L[k]);
    }
  }

  // Build segment tree on start_indices for fast queries.
  SegTree<int, decltype(max_op)> max_start_tree(n, 0, max_op);
  for (int k = 1; k <= n; ++k) {
    if (start_indices[k] > 0) {
      max_start_tree.update(k, start_indices[k]);
    }
  }

  // Answer queries.
  for (int i = 0; i < q; ++i) {
    int l, r;
    cin >> l >> r;

    if (r - l < 2) {
      cout << "YES\n";
      continue;
    }

    int max_start_in_range = max_start_tree.query(l + 2, r);

    if (max_start_in_range >= l) {
      cout << "NO\n";
    } else {
      cout << "YES\n";
    }
  }
}

//===----------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int T = 1;
  cin >> T;
  for ([[maybe_unused]] auto _ : views::iota(0, T)) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//
