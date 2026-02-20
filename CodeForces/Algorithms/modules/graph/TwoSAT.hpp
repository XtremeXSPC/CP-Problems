#ifndef CP_MODULES_GRAPH_TWO_SAT_HPP
#define CP_MODULES_GRAPH_TWO_SAT_HPP

#include "_Common.hpp"

/**
 * @brief 2-SAT solver via implication graph + Kosaraju SCC.
 *
 * @details Variables are indexed in [0, n). Each clause is (x is x_val) OR
 * (y is y_val). Complexity: O(V + E).
 */
struct TwoSAT {
  I32 n = 0;
  Vec<VI> g, rg;
  VI comp;
  VI order;
  VI assignment;

  explicit TwoSAT(I32 variables = 0)
      : n(std::max<I32>(variables, 0)),
        g(static_cast<Size>(2 * n)),
        rg(static_cast<Size>(2 * n)),
        assignment(static_cast<Size>(n), 0) {}

  /// @brief Node id for literal (x == is_true).
  I32 node(I32 x, bool is_true) const { return 2 * x + (is_true ? 0 : 1); }

  /// @brief Negated literal node id.
  static I32 neg(I32 v) { return v ^ 1; }

  /// @brief Adds implication u => v.
  void add_implication(I32 u, I32 v) {
    g[static_cast<Size>(u)].push_back(v);
    rg[static_cast<Size>(v)].push_back(u);
  }

  /// @brief Adds clause: (x == x_val) OR (y == y_val).
  void add_or(I32 x, bool x_val, I32 y, bool y_val) {
    I32 a = node(x, x_val);
    I32 b = node(y, y_val);
    add_implication(neg(a), b);
    add_implication(neg(b), a);
  }

  /// @brief Adds unit clause: (x == value).
  void force_value(I32 x, bool value) { add_or(x, value, x, value); }

  /// @brief Adds clause: x XOR y.
  void add_xor(I32 x, bool x_val, I32 y, bool y_val) {
    add_or(x, x_val, y, y_val);
    add_or(x, !x_val, y, !y_val);
  }

  /// @brief Adds clause: x == y (with optional literal polarity).
  void add_equivalence(I32 x, bool x_val, I32 y, bool y_val) {
    add_or(x, !x_val, y, y_val);
    add_or(x, x_val, y, !y_val);
  }

  /// @brief Solves 2-SAT instance. Returns true iff satisfiable.
  bool solve() {
    I32 vertices = 2 * n;
    order.clear();
    order.reserve(static_cast<Size>(vertices));

    VB visited(static_cast<Size>(vertices), false);

    // Iterative DFS for topological order by exit time.
    FOR(start, vertices) {
      if (visited[static_cast<Size>(start)]) continue;
      Stack<Pair<I32, I32>> st;
      st.push({start, 0});
      visited[static_cast<Size>(start)] = true;

      while (!st.empty()) {
        auto [v, idx] = st.top();
        st.pop();

        if (idx < sz(g[static_cast<Size>(v)])) {
          st.push({v, idx + 1});
          I32 to = g[static_cast<Size>(v)][static_cast<Size>(idx)];
          if (!visited[static_cast<Size>(to)]) {
            visited[static_cast<Size>(to)] = true;
            st.push({to, 0});
          }
          continue;
        }

        order.push_back(v);
      }
    }

    comp.assign(static_cast<Size>(vertices), -1);
    I32 comp_id = 0;

    // Reverse graph DFS in decreasing finish order.
    FOR_R(i, static_cast<I32>(order.size())) {
      I32 v = order[static_cast<Size>(i)];
      if (comp[static_cast<Size>(v)] != -1) continue;

      Stack<I32> st;
      st.push(v);
      comp[static_cast<Size>(v)] = comp_id;

      while (!st.empty()) {
        I32 u = st.top();
        st.pop();
        for (I32 to : rg[static_cast<Size>(u)]) {
          if (comp[static_cast<Size>(to)] == -1) {
            comp[static_cast<Size>(to)] = comp_id;
            st.push(to);
          }
        }
      }
      ++comp_id;
    }

    FOR(x, n) {
      I32 t = node(x, true);
      I32 f = node(x, false);
      if (comp[static_cast<Size>(t)] == comp[static_cast<Size>(f)]) return false;
      assignment[static_cast<Size>(x)] =
          comp[static_cast<Size>(t)] > comp[static_cast<Size>(f)];
    }

    return true;
  }

  /// @brief Returns assignment of variable x after successful solve().
  bool value(I32 x) const { return assignment[static_cast<Size>(x)] != 0; }
};

#endif
