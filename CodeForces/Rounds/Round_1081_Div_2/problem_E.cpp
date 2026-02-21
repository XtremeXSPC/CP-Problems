#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

using namespace std;

void solve() {
  INT(n);

  Vec<I32> a(n), b(n);
  Vec<I32> total(n + 1, 0);

  FOR(i, n) {
    IN(a[i]);
    ++total[a[i]];
  }
  FOR(i, n) {
    IN(b[i]);
    ++total[b[i]];
  }

  FOR(v, 1, n + 1) {
    if (total[v] & 1) {
      OUT(-1);
      return;
    }
  }

  Vec<I32> head(n + 1, -1);
  Vec<I32> to(2 * n), nxt(2 * n);

  FOR(edge_id, n) {
    const I32 u = a[edge_id];
    const I32 v = b[edge_id];
    const I32 h0 = 2 * edge_id;
    const I32 h1 = h0 ^ 1;

    to[h0] = v;
    nxt[h0] = head[u];
    head[u] = h0;

    to[h1] = u;
    nxt[h1] = head[v];
    head[v] = h1;
  }

  Vec<I32> cur = head;
  Vec<char> used_edge(n, 0);
  Vec<I32> tail(n, 0);

  Vec<I32> vertex_stack;
  vertex_stack.reserve(n);

  FOR(start, 1, n + 1) {
    I32 h = cur[start];
    while (h != -1 && used_edge[h >> 1]) h = nxt[h];
    cur[start] = h;
    if (h == -1) continue;

    vertex_stack.clear();
    vertex_stack.eb(start);

    while (!vertex_stack.empty()) {
      const I32 v = vertex_stack.back();

      I32 hh = cur[v];
      while (hh != -1 && used_edge[hh >> 1]) hh = nxt[hh];
      cur[v] = hh;

      if (hh == -1) {
        vertex_stack.pop_back();
        continue;
      }

      const I32 edge_id = hh >> 1;
      used_edge[edge_id] = 1;
      tail[edge_id] = v;

      vertex_stack.eb(to[hh]);
      cur[v] = nxt[hh];
    }
  }

  Vec<I32> swaps;
  swaps.reserve(n);
  FOR(i, n) {
    if (tail[i] != a[i]) swaps.eb(i + 1);
  }

  OUT(sz(swaps));
  if (swaps.empty()) {
    cout << '\n';
    return;
  }

  FOR(i, sz(swaps)) {
    if (i) cout << ' ';
    cout << swaps[i];
  }
  cout << '\n';
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
