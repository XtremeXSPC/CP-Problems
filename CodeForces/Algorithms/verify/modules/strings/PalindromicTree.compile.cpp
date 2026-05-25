#include "modules/strings/PalindromicTree.hpp"

int main() {
  PalindromicTree tree;
  tree.extend('a');
  return tree.longest_suffix_length() == 1 ? 0 : 1;
}
