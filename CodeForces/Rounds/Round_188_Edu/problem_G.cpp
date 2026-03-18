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

//===----------------------------------------------------------------------===//
/* Main Solver Function */

struct Matrix {
  I32 n = 0;
  VecU64 a;

  Matrix() = default;

  explicit Matrix(I32 size) : n(size), a(as<Size>(size) * size, 0) {}

  auto at(I32 row, I32 col) -> U64& { return a[as<Size>(row) * n + col]; }

  auto at(I32 row, I32 col) const -> U64 { return a[as<Size>(row) * n + col]; }
};

auto multiply(const Matrix& lhs, const Matrix& rhs, U64 mod) -> Matrix {
  Matrix res(lhs.n);
  Vec<U128> acc(as<Size>(lhs.n) * lhs.n, 0);

  for (I32 i = 0; i < lhs.n; ++i) {
    const Size lhs_row = as<Size>(i) * lhs.n;
    for (I32 k = 0; k < lhs.n; ++k) {
      const U64 val = lhs.a[lhs_row + k];
      if (val == 0) {
        continue;
      }

      const Size rhs_row = as<Size>(k) * lhs.n;
      const U128 wide = val;
      for (I32 j = 0; j < lhs.n; ++j) {
        acc[lhs_row + j] += wide * rhs.a[rhs_row + j];
      }
    }
  }

  FOR(i, sz(acc)) {
    res.a[i] = as<U64>(acc[i] % mod);
  }
  return res;
}

auto apply(const Matrix& mat, const VecU64& vec, U64 mod) -> VecU64 {
  VecU64 res(mat.n, 0);

  for (I32 i = 0; i < mat.n; ++i) {
    U128 acc = 0;
    const Size row = as<Size>(i) * mat.n;

    for (I32 j = 0; j < mat.n; ++j) {
      const U64 coeff = mat.a[row + j];
      if (coeff == 0 || vec[j] == 0) {
        continue;
      }
      acc += as<U128>(coeff) * vec[j];
    }

    res[i] = as<U64>(acc % mod);
  }

  return res;
}

void solve() {
  I64 n;
  I32 m;
  U64 mod;
  IN(n, m, mod);

  const I32 state_size = 2 * m + 2;
  const I32 total_idx  = 2 * m;
  const I32 ans_idx    = 2 * m + 1;

  auto e_idx = [](I32 r) -> I32 { return r - 1; };
  auto s_idx = [m](I32 l) -> I32 { return m + l - 1; };

  VecU64 state(state_size, 0);
  for (I32 r = 1; r <= m; ++r) {
    state[e_idx(r)] = 1 % mod;
  }
  state[s_idx(1)]  = as<U64>(m) % mod;
  state[total_idx] = as<U64>(m) % mod;
  state[ans_idx]   = as<U64>(m) % mod;

  if (n == 1) {
    OUT(state[ans_idx]);
    return;
  }

  Matrix trans(state_size);

  auto add_coeff = [&](I32 row, I32 col, I64 value) -> void {
    I64 normalized = value % as<I64>(mod);
    if (normalized < 0) {
      normalized += as<I64>(mod);
    }
    trans.at(row, col) = (trans.at(row, col) + as<U64>(normalized)) % mod;
  };

  // State layout: [E(1..m), S(1..m), Total, Ans].
  for (I32 r = 1; r <= m; ++r) {
    const I32 row = e_idx(r);
    add_coeff(row, total_idx, r);

    for (I32 prev_r = 1; prev_r < r; ++prev_r) {
      add_coeff(row, e_idx(prev_r), -(r - prev_r));
    }

    for (I32 prev_l = r + 1; prev_l <= m; ++prev_l) {
      add_coeff(row, s_idx(prev_l), -r);
    }
  }

  for (I32 l = 1; l <= m; ++l) {
    const I32 row = s_idx(l);
    const I32 width = m - l + 1;
    add_coeff(row, total_idx, width);

    for (I32 prev_r = 1; prev_r < l; ++prev_r) {
      add_coeff(row, e_idx(prev_r), -width);
    }

    for (I32 prev_l = l + 1; prev_l <= m; ++prev_l) {
      add_coeff(row, s_idx(prev_l), -(prev_l - l));
    }
  }

  for (I32 col = 0; col < state_size; ++col) {
    U64 total_coeff = 0;
    for (I32 r = 1; r <= m; ++r) {
      total_coeff += trans.at(e_idx(r), col);
      if (total_coeff >= mod) {
        total_coeff -= mod;
      }
    }
    trans.at(total_idx, col) = total_coeff;
    trans.at(ans_idx, col)   = total_coeff;
  }
  trans.at(ans_idx, ans_idx) = (trans.at(ans_idx, ans_idx) + 1) % mod;

  I64 steps = n - 1;
  while (steps > 0) {
    if (steps & 1LL) {
      state = apply(trans, state, mod);
    }
    steps >>= 1LL;
    if (steps > 0) {
      trans = multiply(trans, trans, mod);
    }
  }

  OUT(state[ans_idx]);
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  solve();

  return 0;
}

//===----------------------------------------------------------------------===//
