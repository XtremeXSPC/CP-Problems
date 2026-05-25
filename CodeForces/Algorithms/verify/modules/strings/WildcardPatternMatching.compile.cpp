#include "modules/strings/WildcardPatternMatching.hpp"

int main() {
  const auto pos = WildcardPatternMatching::search("abc", "a?");
  return (pos.size() == 1 && pos[0] == 0) ? 0 : 1;
}
