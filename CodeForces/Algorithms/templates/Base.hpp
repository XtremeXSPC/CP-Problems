#pragma once
#include "core/Preamble.hpp"

//===----------------------------------------------------------------------===//
/* Modular Inclusions Based on Requirements */

#include "Base_profiles.hpp"

// Type-safety utilities (concepts, strong types, cast wrappers) are an opt-in layer.
// Define CP_USE_ADVANCED to 1 before including this header to enable them (it is a
// 0/1 switch: leaving it undefined or setting it to 0 keeps the layer off). The
// legacy NEED_TYPE_SAFETY macro is treated as a synonym.
#if defined(NEED_TYPE_SAFETY) && !defined(CP_USE_ADVANCED)
  #define CP_USE_ADVANCED 1
#endif

#if CP_USE_ADVANCED
  #include "advanced/Cast.hpp"
  #include "advanced/Concepts.hpp"
  #include "advanced/Strong_Type.hpp"
#endif

#include "Base_features.hpp"

// Namespace usage.
#if CP_USE_GLOBAL_STD_NAMESPACE
using namespace std;
#endif
