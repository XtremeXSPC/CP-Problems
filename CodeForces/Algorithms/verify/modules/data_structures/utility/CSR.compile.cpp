#include "modules/data_structures/utility/CSR.hpp"

int main() {
  CSR<I32> csr(2);
  csr.add(1, 5);
  csr.build();
  return csr[1][0] == 5 ? 0 : 1;
}
