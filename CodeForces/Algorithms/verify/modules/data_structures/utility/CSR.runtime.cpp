#include "modules/data_structures/utility/CSR.hpp"

#include <cassert>

int main() {
  CSR<PairI32> csr(4);
  csr.add(2, {20, 1});
  csr.add(0, {0, 0});
  csr.add(2, {21, 2});
  csr.add(3, {30, 3});
  csr.build();

  assert(csr.size() == 4);
  assert(csr.row_size(1) == 0);
  assert(csr[0].size() == 1);
  assert(csr[2].size() == 2);
  assert(csr[2][0] == PairI32(20, 1));
  assert(csr[2][1] == PairI32(21, 2));
  assert(csr[3][0] == PairI32(30, 3));
  return 0;
}
