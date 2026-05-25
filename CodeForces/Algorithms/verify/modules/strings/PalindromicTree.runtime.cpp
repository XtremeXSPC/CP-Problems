#include "modules/strings/PalindromicTree.hpp"

#include <cassert>

int main() {
  PalindromicTree tree("abacaba");

  assert(tree.distinct_palindromes() == 7);
  assert(tree.longest_suffix_length() == 7);
  assert(tree.longest_suffix_length(3) == 3);
  assert(tree.longest_suffix_length(5) == 3);
  assert(tree.max_suffix_length(0, 7) == 7);
  assert(tree.max_suffix_length(1, 7) == 3);
  assert(tree.max_suffix_length(2, 7) == 3);
  assert(tree.suffix_count(7) == 3);

  const I32 whole = tree.suffix_node(7);
  const I32 aba = tree.suffix_node(3);
  const I32 c = tree.suffix_node(4);

  assert(tree.nodes[whole].len == 7);
  assert(tree.nodes[aba].len == 3);
  assert(tree.nodes[c].len == 1);
  const Pair<I32, I32> whole_interval = {0, 7};
  const Pair<I32, I32> aba_interval = {0, 3};
  assert(tree.interval(whole) == whole_interval);
  assert(tree.interval(aba) == aba_interval);

  assert(tree.occurrences(whole) == 1);
  assert(tree.occurrences(aba) == 2);
  assert(tree.occurrences(c) == 1);

  tree.build("abba");
  assert(tree.distinct_palindromes() == 4);
  assert(tree.longest_suffix_length() == 4);
  assert(tree.suffix_count(4) == 2);

  PalindromicTree online;
  online.extend('a');
  online.extend('b');
  online.extend('b');
  online.extend('a');
  assert(online.longest_suffix_length() == 4);
  assert(online.max_suffix_length(1, 4) == 1);
  assert(online.suffix_count(4) == 2);
  return 0;
}
