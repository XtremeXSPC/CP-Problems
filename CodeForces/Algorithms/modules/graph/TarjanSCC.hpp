#ifndef CP_MODULES_GRAPH_TARJAN_SCC_HPP
#define CP_MODULES_GRAPH_TARJAN_SCC_HPP

#include "_Common.hpp"
#include "Graph.hpp"

/**
 * @brief Tarjan strongly connected components (iterative DFS).
 *
 * @details Computes SCC ids in O(V + E), avoiding recursion depth issues.
 * Component ids are assigned in reverse topological order of condensation DAG.
 */
struct TarjanSCC {
  I32 n = 0;
  Vec<VI> adj;
  VI index;
  VI low;
  VI comp;
  VB on_stack;
  Stack<I32> st;
  I32 timer = 0;
  I32 comp_cnt = 0;
  Vec<VI> components;

  explicit TarjanSCC(I32 n = 0) { init(n); }

  explicit TarjanSCC(const Vec<VI>& g) {
    init(as<I32>(g.size()));
    adj = g;
  }

  template <typename Weight>
  explicit TarjanSCC(const Graph<Weight>& g) {
    init(g.n);
    FOR(v, g.n) {
      for (const auto& e : g.adj[as<Size>(v)]) {
        adj[as<Size>(v)].push_back(e.to);
      }
    }
  }

  void init(I32 vertices) {
    n = std::max<I32>(vertices, 0);
    adj.assign(as<Size>(n), VI{});
    index.assign(as<Size>(n), -1);
    low.assign(as<Size>(n), 0);
    comp.assign(as<Size>(n), -1);
    on_stack.assign(as<Size>(n), false);
    while (!st.empty()) st.pop();
    timer = 0;
    comp_cnt = 0;
    components.clear();
  }

  void add_edge(I32 u, I32 v) {
    if (u < 0 || u >= n || v < 0 || v >= n) return;
    adj[as<Size>(u)].push_back(v);
  }

  /// @brief Runs SCC decomposition and returns comp[v] for each vertex.
  const VI& build() {
    index.assign(as<Size>(n), -1);
    low.assign(as<Size>(n), 0);
    comp.assign(as<Size>(n), -1);
    on_stack.assign(as<Size>(n), false);
    while (!st.empty()) st.pop();
    timer = 0;
    comp_cnt = 0;
    components.clear();

    struct Frame {
      I32 v;
      I32 it;
      I32 parent;
    };

    for (I32 start = 0; start < n; ++start) {
      if (index[as<Size>(start)] != -1) continue;
      Stack<Frame> dfs;
      dfs.push({start, 0, -1});

      while (!dfs.empty()) {
        Frame cur = dfs.top();
        dfs.pop();
        I32 v = cur.v;

        if (cur.it == 0 && index[as<Size>(v)] == -1) {
          index[as<Size>(v)] = low[as<Size>(v)] = timer++;
          st.push(v);
          on_stack[as<Size>(v)] = true;
        }

        if (cur.it < as<I32>(adj[as<Size>(v)].size())) {
          I32 to = adj[as<Size>(v)][as<Size>(cur.it)];
          dfs.push({v, cur.it + 1, cur.parent});
          if (index[as<Size>(to)] == -1) {
            dfs.push({to, 0, v});
          } else if (on_stack[as<Size>(to)]) {
            low[as<Size>(v)] = std::min(low[as<Size>(v)], index[as<Size>(to)]);
          }
          continue;
        }

        if (cur.parent != -1) {
          low[as<Size>(cur.parent)] = std::min(
              low[as<Size>(cur.parent)], low[as<Size>(v)]);
        }

        if (low[as<Size>(v)] == index[as<Size>(v)]) {
          components.push_back({});
          while (true) {
            I32 u = st.top();
            st.pop();
            on_stack[as<Size>(u)] = false;
            comp[as<Size>(u)] = comp_cnt;
            components.back().push_back(u);
            if (u == v) break;
          }
          ++comp_cnt;
        }
      }
    }

    return comp;
  }
};

#endif
