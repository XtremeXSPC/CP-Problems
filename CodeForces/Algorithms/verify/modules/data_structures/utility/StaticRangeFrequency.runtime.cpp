#include "modules/data_structures/utility/StaticRangeFrequency.hpp"

#include <cassert>

int main() {
  StaticRangeFrequency<I32> freq(VecI32{5, 1, 5, 2, 5, 1});
  assert(freq.query(0, 6, 5) == 3);
  assert(freq.query(1, 5, 5) == 2);
  assert(freq.query(0, 3, 1) == 1);
  assert(freq.query(0, 6, 7) == 0);

  assert(freq.range_query(0, 6, 1, 3) == 3);
  assert(freq.range_query(2, 5, 2, 6) == 3);

  StaticRangeFrequency<I64> generated(5, [](I32 i) { return I64(i % 2); });
  assert(generated.query(0, 5, 1) == 2);
  return 0;
}
