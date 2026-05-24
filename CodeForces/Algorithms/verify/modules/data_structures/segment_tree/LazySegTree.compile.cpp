#include "modules/data_structures/segment_tree/LazySegTree.hpp"

struct Node {
  I64 sum = 0;
  I64 mx  = 0;
};

struct Tag {
  I64 add = 0;
};

struct ScaledTag {
  I64 add = 0;
  I32 len = 0;
};

auto operator+(const Node& a, const Node& b) -> Node {
  return {a.sum + b.sum, std::max(a.mx, b.mx)};
}

auto operator*(const Tag& tag, I32 len) -> ScaledTag {
  return {tag.add, len};
}

auto operator+=(Node& node, const ScaledTag& tag) -> Node& {
  node.sum += tag.add * tag.len;
  node.mx  += tag.add;
  return node;
}

auto operator+=(Tag& a, const Tag& b) -> Tag& {
  a.add += b.add;
  return a;
}

int main() {
  LazySegTree<Node, Tag> seg(4);
  seg.update(0, 4, Tag{2});
  return 0;
}
