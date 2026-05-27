#include "modules/data_structures/utility/Cumsum2D.hpp"
#include "modules/algebra/monoid/AddGroup.hpp"

int main() {
  Cumsum2D<AddGroup<I32>> pref(1, 1, [](I32, I32) { return 7; });
  return pref.query(0, 0, 1, 1) == 7 ? 0 : 1;
}
