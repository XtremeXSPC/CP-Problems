#ifndef CP_MODULES_DATA_STRUCTURES_MO_HPP
#define CP_MODULES_DATA_STRUCTURES_MO_HPP

#include "_Common.hpp"

/**
 * @brief Mo's algorithm for offline range queries.
 *
 * Processes queries in an order that minimizes pointer movement.
 * Uses alternating direction on even/odd blocks for better cache behavior.
 * Queries use half-open intervals [l, r).
 */
struct Mo {
  I32 block_sz;

  Mo(I32 n) : block_sz(std::max(1, static_cast<I32>(std::sqrt(n)))) {}

  /**
   * @brief Processes range queries offline.
   * @param queries Vector of {l, r} pairs with half-open [l, r).
   * @param add Callable add(index) -- expand window to include index.
   * @param remove Callable remove(index) -- shrink window to exclude index.
   * @param answer Callable answer(query_index) -- record answer for query.
   */
  template <typename Add, typename Remove, typename Answer>
  void process(const Vec<PII>& queries, Add add, Remove remove, Answer answer) {
    I32 q = sz(queries);
    VI order(q);
    std::iota(all(order), 0);
    std::sort(all(order), [&](I32 a, I32 b) {
      I32 ba = queries[a].fi / block_sz, bb = queries[b].fi / block_sz;
      if (ba != bb) return ba < bb;
      return (ba & 1) ? queries[a].se > queries[b].se : queries[a].se < queries[b].se;
    });

    I32 cur_l = 0, cur_r = 0;
    for (I32 idx : order) {
      auto [ql, qr] = queries[idx];
      while (cur_r < qr) add(cur_r++);
      while (cur_l > ql) add(--cur_l);
      while (cur_r > qr) remove(--cur_r);
      while (cur_l < ql) remove(cur_l++);
      answer(idx);
    }
  }
};

#endif
