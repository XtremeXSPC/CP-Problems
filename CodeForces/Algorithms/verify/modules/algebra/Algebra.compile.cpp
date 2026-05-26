#include "modules/algebra/acted/RangeAssignRangeMinMax.hpp"
#include "modules/algebra/monoid/AddGroup.hpp"
#include "modules/algebra/monoid/BitwiseMonoids.hpp"
#include "modules/algebra/monoid/GcdLcmMonoids.hpp"
#include "modules/data_structures/fenwick/GroupFenwickTree.hpp"
#include "modules/data_structures/range_query/MonoidSparseTable.hpp"
#include "modules/data_structures/segment_tree/ActedLazySegTree.hpp"
#include "modules/data_structures/segment_tree/MonoidSegmentTree.hpp"

int main() {
  GroupFenwickTree<AddGroup<I64>> bit(VecI64{1, 2, 3});
  MonoidSparseTable<GcdMonoid<I32>> gcd_st(VecI32{12, 18, 30});
  MonoidSegmentTree<XorMonoid<I32>> xor_seg(VecI32{1, 3, 7});
  ActedLazySegTree<RangeAssignRangeMinActedMonoid<I32>> min_seg(VecI32{5, 2, 8});
  ActedLazySegTree<RangeAssignRangeMaxActedMonoid<I32>> max_seg(VecI32{5, 2, 8});

  bit.apply(0, 4);
  min_seg.apply(0, 2, 1);
  max_seg.apply(1, 3, 9);

  return bit.query(0, 3) == 10    &&
         gcd_st.query(0, 3)  == 6 &&
         xor_seg.query(0, 3) == 5 &&
         min_seg.query(0, 3) == 1 &&
         max_seg.query(0, 3) == 9 ? 0 : 1;
}
