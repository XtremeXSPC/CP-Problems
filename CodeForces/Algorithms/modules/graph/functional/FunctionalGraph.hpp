#ifndef CP_MODULES_GRAPH_FUNCTIONAL_FUNCTIONAL_GRAPH_HPP
#define CP_MODULES_GRAPH_FUNCTIONAL_FUNCTIONAL_GRAPH_HPP

#include "../_Common.hpp"

/// @brief Decomposition of a functional graph into cycles and in-trees.
struct FunctionalGraph {
  I32 n = 0;
  VecI32 nxt;
  VecI32 comp;
  VecI32 cycle_id;
  VecI32 cycle_pos;
  VecI32 depth;
  Vec2D<I32> cycles;
  VecI32 root;

  FunctionalGraph() = default;

  explicit FunctionalGraph(const VecI32& next_) { build(next_); }

  /// @brief Builds from next pointers; each value must be in [0, n).
  void build(const VecI32& next_) {
    n = isz(next_);
    nxt = next_;
    for (const I32 to : nxt) my_assert(0 <= to && to < n);

    comp.assign(n, -1);
    cycle_id.assign(n, -1);
    cycle_pos.assign(n, -1);
    depth.assign(n, -1);
    root.assign(n, -1);
    cycles.clear();

    VecI32 state(n, 0);
    FOR(start, n) {
      if (state[start] != 0) continue;
      VecI32 path;
      I32 v = start;
      while (state[v] == 0) {
        state[v] = 1;
        path.push_back(v);
        v = nxt[v];
      }
      VecI32 new_cycle;
      if (state[v] == 1) {
        VecI32 cyc;
        while (true) {
          const I32 x = path.back();
          path.pop_back();
          cyc.push_back(x);
          if (x == v) break;
        }
        std::ranges::reverse(cyc);
        const I32 id = isz(cycles);
        FOR(i, isz(cyc)) {
          const I32 x = cyc[i];
          comp[x] = id;
          cycle_id[x] = id;
          cycle_pos[x] = i;
          depth[x] = 0;
          root[x] = x;
        }
        new_cycle = cyc;
        cycles.push_back(cyc);
      }
      FOR_R(i, isz(path)) {
        const I32 x = path[i];
        const I32 to = nxt[x];
        comp[x] = comp[to];
        depth[x] = depth[to] + 1;
        root[x] = root[to];
      }
      for (const I32 x : path) state[x] = 2;
      for (const I32 x : new_cycle) state[x] = 2;
    }
  }

  /// @brief Returns whether vertex v is on a directed cycle.
  [[nodiscard]] auto in_cycle(I32 v) const -> bool { return depth[v] == 0; }

  /// @brief Advances every vertex by one step.
  [[nodiscard]] auto next(I32 v) const -> I32 { return nxt[v]; }

  /// @brief Returns cycle vertices for a component id.
  [[nodiscard]] auto cycle(I32 id) const -> const VecI32& { return cycles[id]; }
};

#endif
