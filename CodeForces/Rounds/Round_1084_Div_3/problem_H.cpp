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

struct Node {
  I128 k;
  I128 step;
  I128 mod7;
  I128 mod6;
  VecI64 active;
};

struct QueueItem {
  I128 k;
  I32 idx;
};

struct QueueCmp {
  auto operator()(const QueueItem& lhs, const QueueItem& rhs) const -> bool {
    return lhs.k > rhs.k;
  }
};

auto to_string_i128(I128 x) -> String {
  if (x == 0) return "0";
  bool neg = false;
  if (x < 0) {
    neg = true;
    x = -x;
  }

  String s;
  while (x > 0) {
    const I32 digit = as<I32>(x % 10);
    s.push_back(as<char>('0' + digit));
    x /= 10;
  }
  if (neg) s.push_back('-');
  std::reverse(s.begin(), s.end());
  return s;
}

auto solve() -> void {
  INT(n);
  VecI64 a(n);
  IN(a);

  const I64 rem0 = a[0] % 6;
  FOR(i, 1, n) {
    if (a[i] % 6 != rem0) {
      OUT(-1);
      return;
    }
  }

  const I64 k0 = (6 - rem0) % 6;

  Vec<Node> nodes;
  nodes.reserve(512);
  nodes.push_back(Node{as<I128>(k0), as<I128>(6), as<I128>(7), as<I128>(36), a});

  PriorityQueueBy<QueueItem, Vec<QueueItem>, QueueCmp> pq;
  pq.push(QueueItem{as<I128>(k0), 0});

  while (!pq.empty()) {
    const QueueItem top = pq.top();
    pq.pop();

    Node cur = std::move(nodes[top.idx]);
    nodes[top.idx].active.clear();

    if (cur.active.empty()) {
      OUT(to_string_i128(cur.k));
      return;
    }

    const I128 next_step = cur.step * 42;
    const I128 next_mod7 = cur.mod7 * 7;
    const I128 next_mod6 = cur.mod6 * 6;

    FOR(j, 0, 42) {
      const I128 candidate_k = cur.k + as<I128>(j) * cur.step;

      bool good = true;
      VecI64 next_active;
      next_active.reserve(cur.active.size() / 7 + 1);

      for (const I64 v : cur.active) {
        const I128 x = as<I128>(v) + candidate_k;
        if (x % cur.mod7 != 0) continue;

        if (x % cur.mod6 != 0) {
          good = false;
          break;
        }
        next_active.push_back(v);
      }

      if (!good) continue;

      const I32 idx = as<I32>(nodes.size());
      nodes.push_back(Node{candidate_k, next_step, next_mod7, next_mod6, std::move(next_active)});
      pq.push(QueueItem{candidate_k, idx});
    }
  }

  OUT(-1);
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
