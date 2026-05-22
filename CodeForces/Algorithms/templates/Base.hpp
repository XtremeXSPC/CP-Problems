#pragma once
#include "Preamble.hpp"

//===----------------------------------------------------------------------===//
/* Modular Inclusions Based on Requirements */

#include "Base_profiles.hpp"

// Core modules: fundamental types and utilities.
#ifdef NEED_CORE
#include "Constants.hpp"
#include "Macros.hpp"
#include "Types.hpp"
#if CP_CORE_ENABLE_MATH
#include "IntegerMath.hpp"
#include "MinMax.hpp"
#include "Random.hpp"
#endif
#endif

// Granular core building blocks for compact submissions.
#ifdef NEED_SCALAR_TYPES
#include "ScalarTypes.hpp"
#endif

#ifdef NEED_TYPES
#include "Types.hpp"
#endif

#ifdef NEED_CONSTANTS
#include "Constants.hpp"
#endif

#ifdef NEED_MACROS
#include "Macros.hpp"
#endif

// Math: now expanded directly to its three constituents (Math.hpp removed).
#ifdef NEED_MATH
#include "IntegerMath.hpp"
#include "MinMax.hpp"
#include "Random.hpp"
#endif

#ifdef NEED_INTEGER_MATH
#include "IntegerMath.hpp"
#endif

#ifdef NEED_MINMAX
#include "MinMax.hpp"
#endif

#ifdef NEED_RANDOM
#include "Random.hpp"
#endif

// Timer utilities (Stopwatch).
#ifdef NEED_TIMER
#include "Timer.hpp"
#endif

// Type-safety utilities are optional to avoid increasing default flatten size.
#ifdef NEED_TYPE_SAFETY
#include "Cast.hpp"
#include "Concepts.hpp"
#include "Strong_Type.hpp"
#endif

// Randomized hash utilities for unordered containers (anti-collision).
#ifdef NEED_HASHING
#include "Hashing.hpp"
#endif

// Policy-based data structures (optional).
#ifdef NEED_PBDS
#include "PBDS.hpp"
#endif

// High-performance buffered I/O (opt-in).
// Optional Fast_IO extensions:
// - CP_FAST_IO_ENABLE_MODINT defaults to 1 when NEED_MOD_INT is defined.
// - CP_FAST_IO_ENABLE_STRONG_TYPE defaults to 0 (enable explicitly when needed).
#ifdef NEED_FAST_IO
#include "Fast_IO.hpp"
#endif

// I/O system (lightweight by default).
#ifdef NEED_IO
#include "IO.hpp"
#endif

// Bitwise operations.
#ifdef NEED_BIT_OPS
#include "Bit_Ops.hpp"
#endif

// Modular arithmetic.
#ifdef NEED_MOD_INT
#include "Mod_Int.hpp"
#endif

// Container utilities.
#ifdef NEED_CONTAINERS
#include "Containers.hpp"
#endif

// Namespace usage.
#if CP_USE_GLOBAL_STD_NAMESPACE
using namespace std;
#endif
