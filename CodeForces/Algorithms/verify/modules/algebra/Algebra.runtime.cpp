#include "modules/algebra/MonoidPower.hpp"
#include "modules/algebra/acted/RangeAddRangeCount.hpp"
#include "modules/algebra/acted/RangeAddRangeComposite.hpp"
#include "modules/algebra/acted/RangeAddRangeMinMax.hpp"
#include "modules/algebra/acted/RangeAffineRangeSum.hpp"
#include "modules/algebra/acted/RangeAssignRangeSum.hpp"
#include "modules/algebra/monoid/AddMonoid.hpp"
#include "modules/algebra/monoid/AffineMonoid.hpp"
#include "modules/algebra/monoid/IndexMonoids.hpp"
#include "modules/algebra/monoid/MinMaxMonoid.hpp"
#include "modules/algebra/monoid/MinMonoid.hpp"
#include "modules/algebra/monoid/MulMonoid.hpp"
#include "modules/algebra/monoid/SumMinMaxMonoids.hpp"
#include "modules/data_structures/segment_tree/ActedLazySegTree.hpp"
#include "modules/data_structures/segment_tree/MonoidSegmentTree.hpp"

#include <cassert>

int main() {
  static_assert(AddMonoid<I32>::combine(2, 3) == 5);
  static_assert(MinMonoid<I32>::identity() == INF32);
  static_assert(AffineMonoid<I64>::eval({2, 3}, 5) == 13);

  const auto f = AffineMonoid<I64>::combine({2, 1}, {3, 4});
  assert(f == PairI64(6, 7));
  assert(AffineMonoid<I64>::eval(f, 5) == 37);

  assert(monoid_power<AddMonoid<I64>>(7, 4) == 28);
  assert(monoid_power<MulMonoid<I64>>(3, 4) == 81);

  MonoidSegmentTree<MinIndexMonoid<I32>> idx_seg(Vec<PairI32>{{5, 0}, {1, 1}, {4, 2}, {1, 3}});
  assert(idx_seg.query(0, 4) == PairI32(1, 1));

  MonoidSegmentTree<MinMaxMonoid<I32>> minmax_seg(Vec<PairI32>{{3, 3}, {8, 8}, {-2, -2}, {5, 5}});
  assert(minmax_seg.query(0, 4) == PairI32(-2, 8));

  Vec<PairI32> min_count_values;
  for (const I32 x : VecI32{4, 2, 2, 7}) min_count_values.push_back(MinCountMonoid<I32>::from_element(x));
  ActedLazySegTree<RangeAddRangeMinCountActedMonoid<I32>> min_count_seg(min_count_values);
  assert(min_count_seg.query(0, 4) == PairI32(2, 2));
  min_count_seg.apply(1, 3, 5);
  assert(min_count_seg.query(0, 4) == PairI32(4, 1));

  ActedLazySegTree<RangeAffineRangeSumActedMonoid<I64>> affine_seg(VecI64{1, 2, 3, 4});
  affine_seg.apply(1, 4, {2, 1});
  assert(affine_seg.query(0, 4) == 22);
  affine_seg.apply(0, 2, {3, 0});
  assert((affine_seg.get_all() == VecI64{3, 15, 7, 9}));

  ActedLazySegTree<RangeAssignRangeSumActedMonoid<I64>> assign_sum_seg(VecI64{5, 1, 4, 2});
  assign_sum_seg.apply(1, 3, 10);
  assert(assign_sum_seg.query(0, 4) == 27);
  assign_sum_seg.apply(2, 4, std::nullopt);
  assert(assign_sum_seg.query(0, 4) == 27);

  ActedLazySegTree<RangeAddRangeMaxActedMonoid<I64>> max_seg(VecI64{3, 1, 4, 1});
  max_seg.apply(0, 2, 5);
  assert(max_seg.query(0, 4) == 8);

  Vec<PairI64> sum_min_values;
  for (const I64 x : VecI64{6, 1, 5}) sum_min_values.push_back(SumMinMonoid<I64>::from_element(x));
  ActedLazySegTree<RangeAddRangeSumMinActedMonoid<I64>> sum_min_seg(sum_min_values);
  sum_min_seg.apply(1, 3, 4);
  assert(sum_min_seg.query(0, 3) == PairI64(20, 5));

  ActedLazySegTree<RangeAddRangeMinMaxActedMonoid<I64>> minmax_add_seg(Vec<PairI64>{{3, 3}, {8, 8}, {-2, -2}});
  minmax_add_seg.apply(0, 2, -5);
  assert(minmax_add_seg.query(0, 3) == PairI64(-2, 3));

  return 0;
}
