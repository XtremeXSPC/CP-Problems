#ifndef CP_MODULES_GRAPH_TREE_VIRTUAL_TREE_HPP
#define CP_MODULES_GRAPH_TREE_VIRTUAL_TREE_HPP

#include "../HLD.hpp"

/// @brief Builds the virtual tree induced by marked vertices on a rooted tree.
template <typename Weight = I64>
struct VirtualTree {
  struct Edge {
    I32 to   = 0;
    I32 dist = 0;
  };

  const HLD<Weight>& hld;
  VecI32 nodes;
  Vec2D<Edge> tree;

  explicit VirtualTree(const HLD<Weight>& hld_) : hld(hld_) {}

  /// @brief Rebuilds from marked vertices. Returned ids index into `nodes`.
  void build(VecI32 marked) {
    nodes.clear();
    tree.clear();
    if (marked.empty()) return;

    std::ranges::sort(marked, [&](I32 a, I32 b) { return hld.pos[a] < hld.pos[b]; });
    marked.erase(std::ranges::unique(marked).begin(), marked.end());

    const I32 base = isz(marked);
    FOR(i, base) {
      const I32 j = (i + 1 == base ? 0 : i + 1);
      marked.push_back(hld.lca(marked[i], marked[j]));
    }
    std::ranges::sort(marked, [&](I32 a, I32 b) { return hld.pos[a] < hld.pos[b]; });
    marked.erase(std::ranges::unique(marked).begin(), marked.end());
    nodes = marked;
    tree.assign(isz(nodes), {});

    VecI32 st{0};
    FOR(i, 1, isz(nodes)) {
      while (!is_ancestor(nodes[st.back()], nodes[i])) st.pop_back();
      const I32 parent_id = st.back();
      const I32 dist = hld.depth[nodes[i]] - hld.depth[nodes[parent_id]];
      tree[parent_id].push_back({i, dist});
      st.push_back(i);
    }
  }

  /// @brief Original vertex represented by virtual-tree node id.
  [[nodiscard]] auto vertex(I32 id) const -> I32 {
    my_assert(0 <= id && id < isz(nodes));
    return nodes[id];
  }

private:
  [[nodiscard]] auto is_ancestor(I32 p, I32 v) const -> bool {
    return hld.pos[p] <= hld.pos[v] && hld.pos[v] < hld.pos[p] + hld.subtree_size[p];
  }
};

#endif
