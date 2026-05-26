#include "modules/data_structures/segment_tree/RollbackLazySegTree.hpp"
#include "modules/algebra/ActedMonoid.hpp"

struct LocalMinMonoid {
  using value_type = I32;

  static auto identity() -> value_type { return INF32; }
  static auto combine(const value_type& a, const value_type& b) -> value_type {
    return std::min(a, b);
  }
};

struct LocalAssignMonoid {
  struct Tag {
    I32 value = 0;
    bool active = false;
  };

  using value_type = Tag;

  static auto identity() -> value_type { return {}; }
  static auto combine(const value_type& a, const value_type& b) -> value_type {
    return (b.active ? b : a);
  }
};

struct AssignAction {
  static auto apply(const I32& value, const LocalAssignMonoid::Tag& tag, I32) -> I32 {
    return (tag.active ? tag.value : value);
  }
};

int main() {
  using Seg = RollbackLazySegTree<ActedMonoid<LocalMinMonoid, LocalAssignMonoid, AssignAction>>;
  Seg seg(VecI32{5, 7, 9});
  seg.apply(0, 2, {3, true});
  return seg.query(0, 3) == 3 ? 0 : 1;
}
