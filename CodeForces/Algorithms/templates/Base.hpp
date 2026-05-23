#pragma once
#include "core/Preamble.hpp"

//===----------------------------------------------------------------------===//
/* Modular Inclusions Based on Requirements */

#include "Base_profiles.hpp"

// Type-safety utilities (concepts, strong types, cast wrappers) are an opt-in layer.
// Define CP_USE_ADVANCED before including this header to enable them; the legacy
// NEED_TYPE_SAFETY macro is treated as a synonym.
#if defined(NEED_TYPE_SAFETY) && !defined(CP_USE_ADVANCED)
  #define CP_USE_ADVANCED
#endif

#ifdef CP_USE_ADVANCED
  #include "advanced/Cast.hpp"
  #include "advanced/Concepts.hpp"
  #include "advanced/Strong_Type.hpp"
#endif

// Core modules: fundamental types and utilities.
#ifdef NEED_CORE
  #include "core/Constants.hpp"
  #include "core/Macros.hpp"
  #include "core/Types.hpp"
  #if CP_CORE_ENABLE_MATH
    #include "core/MinMax.hpp"
    #include "core/Random.hpp"
    #include "modules/IntegerMath.hpp"
  #endif
#endif

// Granular core building blocks for compact submissions.
#ifdef NEED_SCALAR_TYPES
  #include "core/ScalarTypes.hpp"
#endif

#ifdef NEED_TYPES
  #include "core/Types.hpp"
#endif

#ifdef NEED_CONSTANTS
  #include "core/Constants.hpp"
#endif

#ifdef NEED_MACROS
  #include "core/Macros.hpp"
#endif

// Math: expanded directly to its three constituents.
#ifdef NEED_MATH
  #include "core/MinMax.hpp"
  #include "core/Random.hpp"
  #include "modules/IntegerMath.hpp"
#endif

#ifdef NEED_INTEGER_MATH
  #include "modules/IntegerMath.hpp"
#endif

#ifdef NEED_MINMAX
  #include "core/MinMax.hpp"
#endif

#ifdef NEED_RANDOM
  #include "core/Random.hpp"
#endif

// Timer utilities (Stopwatch).
#ifdef NEED_TIMER
  #include "core/Timer.hpp"
#endif

// Randomized hash utilities for unordered containers (anti-collision).
#ifdef NEED_HASHING
  #include "modules/Hashing.hpp"
#endif

// Policy-based data structures (optional).
#ifdef NEED_PBDS
  #include "modules/PBDS.hpp"
#endif

// High-performance buffered I/O (opt-in).
// Optional Fast_IO extensions:
// - CP_FAST_IO_ENABLE_MODINT defaults to 1 when NEED_MOD_INT is defined.
// - CP_FAST_IO_ENABLE_STRONG_TYPE defaults to 0; requires CP_USE_ADVANCED.
#ifdef NEED_FAST_IO
  #include "modules/Fast_IO.hpp"
#endif

// I/O system (lightweight by default).
#ifdef NEED_IO
  #include "modules/IO.hpp"
#endif

// Bitwise operations.
#ifdef NEED_BIT_OPS
  #include "modules/Bit_Ops.hpp"
#endif

// Modular arithmetic.
#ifdef NEED_MOD_INT
  #include "modules/Mod_Int.hpp"
#endif

// Container utilities.
#ifdef NEED_CONTAINERS
  #include "modules/Containers.hpp"
#endif

// Namespace usage.
#if CP_USE_GLOBAL_STD_NAMESPACE
using namespace std;
#endif
