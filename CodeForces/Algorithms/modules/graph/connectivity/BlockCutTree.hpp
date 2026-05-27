#ifndef CP_MODULES_GRAPH_BLOCK_CUT_TREE_HPP
#define CP_MODULES_GRAPH_BLOCK_CUT_TREE_HPP

#include "LowLink.hpp"

/// @brief Block-cut tree for an undirected graph.
///
/// Original vertices are nodes `[0, n)`. Biconnected component nodes are
/// `[n, n + block_count)`.
struct BlockCutTree {
  LowLink graph;
  I32 block_count = 0;
  Vec2D<I32> blocks;
  Vec2D<I32> vertex_blocks;
  Vec2D<I32> tree;
  Vec<char> is_articulation;

  BlockCutTree() = default;

  explicit BlockCutTree(I32 n) : graph(n) {}

  /// @brief Adds one undirected edge before build().
  auto add_edge(I32 u, I32 v) -> I32 { return graph.add_edge(u, v); }

  /// @brief Builds biconnected components and the expanded block-cut tree.
  void build() {
    graph.build();
    blocks.clear();
    vertex_blocks.assign(graph.n, {});
    is_articulation = graph.is_articulation;
    VecI32 edge_stack;
    VecI32 seen(graph.n, -1);
    I32 token = 0;

    VecI32 ord_local(graph.n, -1);
    VecI32 low_local(graph.n, -1);
    I32 timer = 0;
    FOR(v, graph.n) {
      if (ord_local[v] == -1) {
        if (graph.adj[v].empty()) {
          add_block(VecI32{I32(v)});
        } else {
          dfs(v, -1, timer, edge_stack, ord_local, low_local, seen, token);
          if (!edge_stack.empty()) flush_until(edge_stack.front(), edge_stack, seen, token);
        }
      }
    }

    block_count = isz(blocks);
    tree.assign(graph.n + block_count, {});
    FOR(id, block_count) {
      const I32 node = graph.n + id;
      for (const I32 v : blocks[id]) {
        tree[node].push_back(v);
        tree[v].push_back(node);
        vertex_blocks[v].push_back(id);
      }
    }
  }

private:
  void dfs(I32 v, I32 parent_edge, I32& timer, VecI32& edge_stack, VecI32& ord_local,
           VecI32& low_local, VecI32& seen, I32& token) {
    ord_local[v] = low_local[v] = timer++;
    for (const auto& e : graph.adj[v]) {
      if (e.id == parent_edge) continue;
      const I32 to = e.to;
      if (ord_local[to] == -1) {
        edge_stack.push_back(e.id);
        dfs(to, e.id, timer, edge_stack, ord_local, low_local, seen, token);
        low_local[v] = std::min(low_local[v], low_local[to]);
        if (low_local[to] >= ord_local[v]) flush_until(e.id, edge_stack, seen, token);
      } else if (ord_local[to] < ord_local[v]) {
        edge_stack.push_back(e.id);
        low_local[v] = std::min(low_local[v], ord_local[to]);
      }
    }
  }

  void flush_until(I32 last_edge, VecI32& edge_stack, VecI32& seen, I32& token) {
    VecI32 vs;
    ++token;
    while (!edge_stack.empty()) {
      const I32 id = edge_stack.back();
      edge_stack.pop_back();
      auto [u, v] = graph.edges[id];
      if (seen[u] != token) {
        seen[u] = token;
        vs.push_back(u);
      }
      if (seen[v] != token) {
        seen[v] = token;
        vs.push_back(v);
      }
      if (id == last_edge) break;
    }
    add_block(vs);
  }

  void add_block(VecI32 vs) {
    std::ranges::sort(vs);
    vs.erase(std::ranges::unique(vs).begin(), vs.end());
    blocks.push_back(std::move(vs));
  }
};

#endif
