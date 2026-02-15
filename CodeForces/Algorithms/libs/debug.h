//===----------------------------------------------------------------------===//
//============================= DEBUG SHIM HEADER ============================//

#ifndef MODERN_DEBUG_H
#define MODERN_DEBUG_H

#if __has_include("debug_core.h")
  #include "debug_core.h"
#elif __has_include("../../Algorithms/libs/debug_core.h")
  #include "../../Algorithms/libs/debug_core.h"
#elif __has_include("../../../Algorithms/libs/debug_core.h")
  #include "../../../Algorithms/libs/debug_core.h"
#elif __has_include("../../../../Algorithms/libs/debug_core.h")
  #include "../../../../Algorithms/libs/debug_core.h"
#elif __has_include("../../../../../Algorithms/libs/debug_core.h")
  #include "../../../../../Algorithms/libs/debug_core.h"
#else
  #error "debug_core.h not found. Ensure Algorithms/libs headers are available."
#endif

#if CP_DEBUG_ENABLE_PERF || CP_DEBUG_ENABLE_MEMORY
  #if __has_include("debug_perf.h")
    #include "debug_perf.h"
  #elif __has_include("../../Algorithms/libs/debug_perf.h")
    #include "../../Algorithms/libs/debug_perf.h"
  #elif __has_include("../../../Algorithms/libs/debug_perf.h")
    #include "../../../Algorithms/libs/debug_perf.h"
  #elif __has_include("../../../../Algorithms/libs/debug_perf.h")
    #include "../../../../Algorithms/libs/debug_perf.h"
  #elif __has_include("../../../../../Algorithms/libs/debug_perf.h")
    #include "../../../../../Algorithms/libs/debug_perf.h"
  #else
    #error "debug_perf.h not found with CP_DEBUG_ENABLE_PERF/CP_DEBUG_ENABLE_MEMORY."
  #endif
#endif

#if CP_DEBUG_ENABLE_TREE
  #if __has_include("debug_tree.h")
    #include "debug_tree.h"
  #elif __has_include("../../Algorithms/libs/debug_tree.h")
    #include "../../Algorithms/libs/debug_tree.h"
  #elif __has_include("../../../Algorithms/libs/debug_tree.h")
    #include "../../../Algorithms/libs/debug_tree.h"
  #elif __has_include("../../../../Algorithms/libs/debug_tree.h")
    #include "../../../../Algorithms/libs/debug_tree.h"
  #elif __has_include("../../../../../Algorithms/libs/debug_tree.h")
    #include "../../../../../Algorithms/libs/debug_tree.h"
  #else
    #error "debug_tree.h not found with CP_DEBUG_ENABLE_TREE."
  #endif
#endif

#if CP_DEBUG_ENABLE_WATCH
  #if __has_include("debug_watch.h")
    #include "debug_watch.h"
  #elif __has_include("../../Algorithms/libs/debug_watch.h")
    #include "../../Algorithms/libs/debug_watch.h"
  #elif __has_include("../../../Algorithms/libs/debug_watch.h")
    #include "../../../Algorithms/libs/debug_watch.h"
  #elif __has_include("../../../../Algorithms/libs/debug_watch.h")
    #include "../../../../Algorithms/libs/debug_watch.h"
  #elif __has_include("../../../../../Algorithms/libs/debug_watch.h")
    #include "../../../../../Algorithms/libs/debug_watch.h"
  #else
    #error "debug_watch.h not found with CP_DEBUG_ENABLE_WATCH."
  #endif
#endif

#endif // MODERN_DEBUG_H
