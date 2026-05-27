#include "modules/data_structures/utility/StaticRangeFrequency.hpp"

int main() {
  StaticRangeFrequency<String> freq(VecStr{"a", "b", "a"});
  return freq.query(0, 3, "a") == 2 ? 0 : 1;
}
