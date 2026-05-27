#include "modules/data_structures/utility/IndexCompression.hpp"

#include <cassert>

int main() {
  VecI32 a{10, -3, 10, 7, -3};
  IndexCompression<I32> comp(a);

  assert(comp.size() == 3);
  assert(comp.value(0) == -3);
  assert(comp[1] == 7);
  assert(comp[2] == 10);

  assert(comp.rank(-10) == 0);
  assert(comp.rank(-3) == 0);
  assert(comp.rank(0) == 1);
  assert(comp.rank(8) == 2);
  assert(comp.rank(20) == 3);

  assert(comp.contains(7));
  assert(!comp.contains(8));
  assert(comp.id(10) == 2);

  const VecI32 compressed = comp.compress(a);
  assert((compressed == VecI32{2, 0, 2, 1, 0}));

  IndexCompression<String> words(VecStr{"pear", "apple", "pear", "banana"});
  assert(words.size() == 3);
  assert(words.id("apple") == 0);
  assert(words.id("banana") == 1);
  assert(words.id("pear") == 2);

  IndexCompression<I64> rebuilt;
  const VecI32 ids = rebuilt.build_and_compress(VecI64{5, 4, 5, 3});
  assert((ids == VecI32{2, 1, 2, 0}));
  assert(rebuilt.rank(4) == 1);

  return 0;
}
