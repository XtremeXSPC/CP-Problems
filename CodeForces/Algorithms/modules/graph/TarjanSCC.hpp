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
  Vec2D<I32> adj;
  VecI32 index;
  VecI32 low;
  VecI32 comp;
  VecBool on_stack;
  Stack<I32> st;
  I32 timer = 0;
  I32 comp_cnt = 0;
  Vec2D<I32> components;

  explicit TarjanSCC(I32 n = 0) { init(n); }

  explicit TarjanSCC(const Vec2D<I32>& g) {
    init(isz(g));
    adj = g;
  }

  template <typename Weight>
  explicit TarjanSCC(const Graph<Weight>& g) {
    init(g.n);
    FOR(v, g.n) {
      for (const auto& e : g.adj[v]) {
        adj[v].push_back(e.to);
      }
    }
  }

  void init(I32 vertices) {
    n = std::max<I32>(vertices, 0);
    adj.assign(n, VecI32{});
    index.assign(n, -1);
    low.assign(n, 0);
    comp.assign(n, -1);
    on_stack.assign(n, false);
    while (!st.empty()) st.pop();
    timer = 0;
    comp_cnt = 0;
    components.clear();
  }

  void add_edge(I32 u, I32 v) {
    if (u < 0 || u >= n || v < 0 || v >= n) return;
    adj[u].push_back(v);
  }

  /// @brief Runs SCC decomposition and returns comp[v] for each vertex.
  const VecI32& build() {
    index.assign(n, -1);
    low.assign(n, 0);
    comp.assign(n, -1);
    on_stack.assign(n, false);
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
      if (index[start] != -1) continue;
      Stack<Frame> dfs;
      dfs.push({start, 0, -1});

      while (!dfs.empty()) {
        Frame cur = dfs.top();
        dfs.pop();
        I32 v = cur.v;

        if (cur.it == 0 && index[v] == -1) {
          index[v] = low[v] = timer++;
          st.push(v);
          on_stack[v] = true;
        }

        if (cur.it < isz(adj[v])) {
          I32 to = adj[v][cur.it];
          dfs.push({v, cur.it + 1, cur.parent});
          if (index[to] == -1) {
            dfs.push({to, 0, v});
          } else if (on_stack[to]) {
            low[v] = std::min(low[v], index[to]);
          }
          continue;
        }

        if (cur.parent != -1) {
          low[cur.parent] = std::min(
              low[cur.parent], low[v]);
        }

        if (low[v] == index[v]) {
          components.push_back({});
          while (true) {
            I32 u = st.top();
            st.pop();
            on_stack[u] = false;
            comp[u] = comp_cnt;
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
