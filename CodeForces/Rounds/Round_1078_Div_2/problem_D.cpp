#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n, m);

  Vec<Vec<I32>> grid(n, Vec<I32>(m));
  I64 total_ones = 0;
  FOR(i, n) {
    I64 row_sum = 0;
    FOR(j, m) {
      INT(v);
      grid[i][j] = v;
      row_sum += v;
    }
    total_ones += row_sum;
  }

  const I64 target = total_ones / 2;
  const I64 best_product = (total_ones / 2) * ((total_ones + 1) / 2);

  I32 x = 0;
  I32 t_part = 0;

  if (target == 0) {
    x = 0;
    t_part = 0;
  } else {
    I64 seen = 0;
    x = m;
    t_part = 0;
    bool found = false;

    FOR(col, m) {
      for (I32 row = n - 1; row >= 0; --row) {
        if (grid[row][col] == 1) {
          ++seen;
          if (seen == target) {
            x = col;
            t_part = n - row;
            found = true;
            break;
          }
        }
      }
      if (found) break;
    }

    if (!found) {
      x = m;
      t_part = 0;
    }
  }

  std::string path;
  path.reserve(static_cast<std::size_t>(n + m));

  if (t_part == 0 || x == m) {
    path.append(static_cast<std::size_t>(x), 'R');
    path.append(static_cast<std::size_t>(n), 'D');
    path.append(static_cast<std::size_t>(m - x), 'R');
  } else {
    path.append(static_cast<std::size_t>(x), 'R');
    path.append(static_cast<std::size_t>(n - t_part), 'D');
    path.push_back('R');
    path.append(static_cast<std::size_t>(t_part), 'D');
    path.append(static_cast<std::size_t>(m - x - 1), 'R');
  }

  std::cout << best_product << '\n' << path << '\n';
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
  I32 T = 0;
  if (!(std::cin >> T)) return 0;
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
