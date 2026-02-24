#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

namespace {

constexpr array<I32, 8> DR = {2, 2, -2, -2, 3, 3, -3, -3};
constexpr array<I32, 8> DC = {3, -3, 3, -3, 2, -2, 2, -2};

[[nodiscard]] bool is_neighbor(I32 r1, I32 c1, I32 r2, I32 c2) {
  const I32 dr = r1 - r2;
  const I32 dc = c1 - c2;
  return dr * dr + dc * dc == 13;
}

struct Candidate {
  I32 v;
  bool fin;
  I64 key1;
  I64 key2;
};

} // namespace

void solve() {
  INT(N);

  if (N == 5) {
    OUT("01110");
    OUT("11011");
    OUT("10001");
    OUT("11011");
    OUT("01110");
    return;
  }

  const I32 target = as<I32>(floor(as<F64>(N) * as<F64>(N) / exp(1.0)));

  auto in_ring = [&](I32 M, I32 r, I32 c) -> bool {
    return (r < M || r >= N - M || c < M || c >= N - M);
  };

  auto used_neighbors = [&](const Vec<U8>& used, I32 r, I32 c) -> array<I32, 3> {
    array<I32, 3> res = {-1, -1, 0};
    I32 cnt = 0;
    FOR(k, 8) {
      const I32 nr = r + DR[k];
      const I32 nc = c + DC[k];
      if ((unsigned)nr >= (unsigned)N || (unsigned)nc >= (unsigned)N) continue;
      const I32 id = nr * N + nc;
      if (!used[id]) continue;
      if (cnt < 2) res[cnt] = id;
      ++cnt;
      if (cnt > 2) break;
    }
    res[2] = cnt;
    return res;
  };

  auto verify_cycle = [&](const Vec<U8>& used) -> bool {
    I32 start = -1;
    I32 total = 0;
    FOR(i, as<I64>(N) * N) {
      if (!used[i]) continue;
      ++total;
      if (start == -1) start = i;
    }
    if (total < target) return false;

    FOR(i, as<I64>(N) * N) {
      if (!used[i]) continue;
      const I32 r = i / N;
      const I32 c = i % N;
      I32 deg = 0;
      FOR(k, 8) {
        const I32 nr = r + DR[k];
        const I32 nc = c + DC[k];
        if ((unsigned)nr >= (unsigned)N || (unsigned)nc >= (unsigned)N) continue;
        if (used[nr * N + nc]) ++deg;
      }
      if (deg != 2) return false;
    }

    Vec<U8> vis(N * N, 0);
    Deque<I32> dq;
    dq.push_back(start);
    vis[start] = 1;
    I32 seen = 0;

    while (!dq.empty()) {
      const I32 v = dq.front();
      dq.pop_front();
      ++seen;
      const I32 r = v / N;
      const I32 c = v % N;
      FOR(k, 8) {
        const I32 nr = r + DR[k];
        const I32 nc = c + DC[k];
        if ((unsigned)nr >= (unsigned)N || (unsigned)nc >= (unsigned)N) continue;
        const I32 to = nr * N + nc;
        if (!used[to] || vis[to]) continue;
        vis[to] = 1;
        dq.push_back(to);
      }
    }

    return seen == total;
  };

  const Vec<I32> try_margin = {16, 18, 20};
  Vec<U8> final_used;

  for (const I32 M : try_margin) {
    const I32 L = M;
    const I32 R = N - M;
    const I32 core_side = R - L;
    if (core_side <= 0) continue;

    Vec<U8> sel(N * N, 0);
    FOR(r, L, R) {
      const I32 base = r * N;
      FOR(c, L, R) {
        const I32 v = (r + 2 * c) % 5;
        if (v == 0 || v == 1) sel[base + c] = 1;
      }
    }

    auto core_neighbors = [&](I32 r, I32 c, I32 out[2]) -> I32 {
      const I32 residue = (r + 2 * c) % 5;
      I32 cnt = 0;
      if (residue == 0) {
        I32 r1 = r + 2, c1 = c - 3;
        if (r1 >= L && r1 < R && c1 >= L && c1 < R) out[cnt++] = r1 * N + c1;
        I32 r2 = r - 3, c2 = c + 2;
        if (r2 >= L && r2 < R && c2 >= L && c2 < R) out[cnt++] = r2 * N + c2;
      } else {
        I32 r1 = r - 2, c1 = c + 3;
        if (r1 >= L && r1 < R && c1 >= L && c1 < R) out[cnt++] = r1 * N + c1;
        I32 r2 = r + 3, c2 = c - 2;
        if (r2 >= L && r2 < R && c2 >= L && c2 < R) out[cnt++] = r2 * N + c2;
      }
      return cnt;
    };

    Vec<U8> deg(N * N, 0);
    FOR(r, L, R) {
      FOR(c, L, R) {
        const I32 id = r * N + c;
        if (!sel[id]) continue;
        I32 nb[2];
        deg[id] = as<U8>(core_neighbors(r, c, nb));
      }
    }

    Vec<U8> vis(N * N, 0);
    Vec<Vec<I32>> paths;
    paths.reserve(2500);

    auto walk_path = [&](I32 start) -> Vec<I32> {
      Vec<I32> path;
      path.reserve(256);
      I32 cur = start, prev = -1;
      while (true) {
        vis[cur] = 1;
        path.push_back(cur);
        const I32 r = cur / N;
        const I32 c = cur % N;
        I32 nb[2];
        const I32 cnt = core_neighbors(r, c, nb);
        I32 next = -1;
        FOR(i, cnt) {
          if (nb[i] == prev || !sel[nb[i]] || vis[nb[i]]) continue;
          next = nb[i];
          break;
        }
        if (next == -1) break;
        prev = cur;
        cur = next;
      }
      return path;
    };

    FOR(r, L, R) {
      FOR(c, L, R) {
        const I32 id = r * N + c;
        if (!sel[id] || vis[id] || deg[id] > 1) continue;
        auto path = walk_path(id);
        if (as<I32>(path.size()) > 5) paths.push_back(std::move(path));
      }
    }

    FOR(r, L, R) {
      FOR(c, L, R) {
        const I32 id = r * N + c;
        if (!sel[id] || vis[id]) continue;
        auto cycle = walk_path(id);
        if (as<I32>(cycle.size()) <= 6) continue;
        cycle.pop_back();
        paths.push_back(std::move(cycle));
      }
    }

    for (auto& path : paths) {
      const I32 first_r = path.front() / N;
      const I32 last_r = path.back() / N;
      if (first_r > last_r) reverse(path.begin(), path.end());
    }

    sort(paths.begin(), paths.end(), [&](const auto& a, const auto& b) {
      const I32 ra = a.front() / N, ca = a.front() % N;
      const I32 rb = b.front() / N, cb = b.front() % N;
      return (ra + ca) < (rb + cb);
    });

    if (paths.size() % 2 == 1) paths.pop_back();
    const I32 K = as<I32>(paths.size());
    if (K < 2) continue;

    Vec<U8> used(N * N, 0);
    for (const auto& path : paths) {
      for (const I32 v : path) used[v] = 1;
    }

    auto connect_small = [&](I32 u, I32 v) -> bool {
      const I32 vr = v / N;
      const I32 vc = v % N;

      auto generate_candidates = [&](I32 cur) -> Vec<Candidate> {
        Vec<Candidate> candidates;
        candidates.reserve(8);
        const I32 cr = cur / N;
        const I32 cc = cur % N;

        FOR(k, 8) {
          const I32 nr = cr + DR[k];
          const I32 nc = cc + DC[k];
          if ((unsigned)nr >= (unsigned)N || (unsigned)nc >= (unsigned)N) continue;
          if (!in_ring(M, nr, nc)) continue;
          const I32 nid = nr * N + nc;
          if (used[nid]) continue;

          const auto nb = used_neighbors(used, nr, nc);
          const I32 cnt = nb[2];
          bool correct = false;
          bool fin = false;

          if (cnt == 1 && nb[0] == cur) {
            correct = true;
          } else if (cnt == 2) {
            const I32 a = nb[0], b = nb[1];
            if (((a == cur && b == v) || (a == v && b == cur)) && is_neighbor(nr, nc, vr, vc)) {
              correct = true;
              fin = true;
            }
          }
          if (!correct) continue;

          I32 onward = 0;
          FOR(t, 8) {
            const I32 rr = nr + DR[t];
            const I32 cc2 = nc + DC[t];
            if ((unsigned)rr >= (unsigned)N || (unsigned)cc2 >= (unsigned)N) continue;
            if (!in_ring(M, rr, cc2)) continue;
            const I32 id2 = rr * N + cc2;
            if (used[id2]) continue;
            const auto nb2 = used_neighbors(used, rr, cc2);
            if (nb2[2] == 1 && nb2[0] == nid) ++onward;
          }

          const I64 dist = as<I64>(nr - vr) * (nr - vr) + as<I64>(nc - vc) * (nc - vc);
          candidates.push_back({nid, fin, onward, dist});
        }

        sort(candidates.begin(), candidates.end(), [](const Candidate& a, const Candidate& b) {
          if (a.key1 != b.key1) return a.key1 < b.key1;
          if (a.key2 != b.key2) return a.key2 < b.key2;
          return a.v < b.v;
        });
        return candidates;
      };

      constexpr array<I32, 6> LIMITS = {24, 40, 64, 96, 140, 220};
      for (const I32 lim : LIMITS) {
        struct Frame {
          I32 cur;
          I32 prev;
          I32 it;
          Vec<Candidate> cand;
        };

        Vec<Frame> st;
        st.reserve(as<I64>(lim) + 2);
        st.push_back({u, -1, 0, {}});

        bool correct = false;
        while (!st.empty()) {
          auto& f = st.back();
          if (f.cand.empty()) {
            const I32 depth_used = as<I32>(st.size()) - 1;
            if (depth_used > lim) {
              if (f.cur != u) used[f.cur] = 0;
              st.pop_back();
              continue;
            }
            f.prev = (st.size() >= 2 ? st[st.size() - 2].cur : u);
            f.cand = generate_candidates(f.cur);
            f.it = 0;
          }

          if (f.it >= as<I32>(f.cand.size())) {
            if (f.cur != u) used[f.cur] = 0;
            st.pop_back();
            continue;
          }

          const Candidate cd = f.cand[f.it++];
          used[cd.v] = 1;
          if (cd.fin) {
            correct = true;
            break;
          }
          st.push_back({cd.v, f.cur, 0, {}});
        }

        if (correct) return true;
      }

      return false;
    };

    bool all_good = true;
    FOR(i, K) {
      I32 u, v;
      if (i % 2 == 0) {
        u = paths[i].back();
        v = paths[(i + 1) % K].back();
      } else {
        u = paths[i].front();
        v = paths[(i + 1) % K].front();
      }
      if (connect_small(u, v)) continue;
      all_good = false;
      break;
    }

    if (all_good && verify_cycle(used)) {
      final_used = std::move(used);
      break;
    }
  }

  if (final_used.empty()) {
    FOR(r, N) {
      string row(N, '0');
      OUT(row);
    }
    return;
  }

  FOR(r, N) {
    string row;
    row.reserve(N);
    const I32 base = r * N;
    FOR(c, N) {
      row.push_back(final_used[base + c] ? '1' : '0');
    }
    OUT(row);
  }
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
