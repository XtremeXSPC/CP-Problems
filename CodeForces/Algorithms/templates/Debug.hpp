#pragma once
#include "FeatureDetect.hpp"

//===----------------------------------------------------------------------===//
/* Debug and Assertion Policy */

#ifdef LOCAL
  #include "debug.h"
#else
  #define debug(...)
  #define debug_if(...)
  #define debug_tree(...)
  #define debug_tree_verbose(...)
  #define debug_line()
  #define my_assert(cond) do { \
    if (!(cond)) [[unlikely]] { \
      std::cerr << "Assertion failed: " #cond " at " __FILE__ ":" << __LINE__ << '\n'; \
      std::abort(); \
    } \
  } while (0)
  #define COUNT_CALLS(...)
#endif
