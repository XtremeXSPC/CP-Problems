#include "modules/data_structures/utility/Intervals.hpp"

#include <cassert>

int main() {
  Intervals<I32> seg(10, -1);
  seg.set(2, 7, 3);
  assert(seg.active_count == 1);
  assert(seg.active_length == 5);
  assert((seg.get(4) == Tuple<I64, I64, I32>(2, 7, 3)));

  seg.set(4, 6, 5);
  assert(seg.active_count == 3);
  assert(seg.active_length == 5);
  assert((seg.get(5) == Tuple<I64, I64, I32>(4, 6, 5)));

  seg.set(3, 8, 3);
  assert(seg.active_count == 1);
  assert(seg.active_length == 6);
  assert((seg.get(6) == Tuple<I64, I64, I32>(2, 8, 3)));

  Vec<Tuple<I64, I64, I32>> parts;
  seg.enumerate(1, 9, [&](I64 l, I64 r, I32 x) { parts.push_back({l, r, x}); });
  assert((parts == Vec<Tuple<I64, I64, I32>>{{1, 2, -1}, {2, 8, 3}, {8, 9, -1}}));

  seg.set(2, 8, -1);
  assert(seg.active_count == 0);
  assert(seg.active_length == 0);
  return 0;
}
