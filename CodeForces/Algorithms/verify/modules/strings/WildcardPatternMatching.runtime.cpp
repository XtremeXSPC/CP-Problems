#include "modules/strings/WildcardPatternMatching.hpp"

#include <cassert>

int main() {
  {
    const auto valid = WildcardPatternMatching::match_table("abacaba", "a?a");
    assert(sz(valid) == 5);
    assert(valid[0]);
    assert(!valid[1]);
    assert(valid[2]);
    assert(!valid[3]);
    assert(valid[4]);

    const VecI32 pos = {0, 2, 4};
    assert(WildcardPatternMatching::search("abacaba", "a?a") == pos);
  }

  {
    const VecI32 pos = {0, 1, 2};
    assert(WildcardPatternMatching::search("??ab", "??") == pos);
  }

  {
    const VecI32 pos = {0, 1};
    assert(WildcardPatternMatching::search("ab?cab", "?b?") == pos);
  }

  {
    const auto valid = WildcardPatternMatching::match_table("abc", "");
    assert(sz(valid) == 4);
    FOR(i, 4) assert(valid[i]);
  }

  {
    assert(WildcardPatternMatching::search("abc", "abcd").empty());
  }

  return 0;
}
