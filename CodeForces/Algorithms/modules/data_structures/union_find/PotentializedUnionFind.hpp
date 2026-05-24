#ifndef CP_MODULES_DATA_STRUCTURES_POTENTIALIZED_UNION_FIND_HPP
#define CP_MODULES_DATA_STRUCTURES_POTENTIALIZED_UNION_FIND_HPP

#include "../_Common.hpp"

template <typename Group>
struct PotentializedUnionFind {
  using Value = typename Group::value_type;

  I32 components;
  VecI32 parent;
  VecI32 comp_size;
  Vec<Value> diff_to_parent;

  explicit PotentializedUnionFind(I32 n)
      : components(n), parent(n), comp_size(n, 1), diff_to_parent(n, Group::unit()) {
    std::iota(all(parent), 0);
  }

  Pair<I32, Value> find(I32 v) {
    if (parent[v] == v) return {v, Group::unit()};
    auto [root, up] = find(parent[v]);
    diff_to_parent[v] = Group::op(diff_to_parent[v], up);
    parent[v] = root;
    return {root, diff_to_parent[v]};
  }

  Pair<I32, Value> operator[](I32 v) { return find(v); }

  [[nodiscard]] bool same(I32 u, I32 v) { return find(u).first == find(v).first; }

  Optional<Value> diff(I32 from, I32 to) {
    auto [rf, xf] = find(from);
    auto [rt, xt] = find(to);
    if (rf != rt) return std::nullopt;
    return Group::op(Group::inverse(xf), xt);
  }

  bool unite(I32 from, I32 to, Value delta) {
    auto [rf, xf] = find(from);
    auto [rt, xt] = find(to);
    if (rf == rt) return false;
    if (comp_size[rf] < comp_size[rt]) {
      std::swap(rf, rt);
      std::swap(xf, xt);
      delta = Group::inverse(delta);
    }
    delta = Group::op(xf, delta);
    delta = Group::op(delta, Group::inverse(xt));
    parent[rt] = rf;
    diff_to_parent[rt] = delta;
    comp_size[rf] += comp_size[rt];
    --components;
    return true;
  }

  [[nodiscard]] I32 component_size(I32 v) { return comp_size[find(v).first]; }

  [[nodiscard]] I32 num_components() const { return components; }
};

#endif
