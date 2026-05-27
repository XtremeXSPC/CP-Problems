#include "modules/data_structures/utility/IndexCompression.hpp"

int main() {
  const Array<I32, 4> raw{4, 2, 4, 9};
  IndexCompression<I32> comp(raw);
  return comp.size() == 3 && comp.id(9) == 2 && comp.rank(3) == 1 ? 0 : 1;
}
