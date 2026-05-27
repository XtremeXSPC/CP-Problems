#include "modules/data_structures/utility/Intervals.hpp"

int main() {
  Intervals<char, I32> seg(0, 5, '.');
  seg.set(1, 4, '#');
  return std::get<2>(seg.get(2)) == '#' ? 0 : 1;
}
