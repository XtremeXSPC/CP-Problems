#pragma once
#include "Preamble.hpp"

//===----------------------------------------------------------------------===//
/* Modular Inclusions Based on Requirements */

// Backward-compatible dependency behavior.
// Set to 1 to require fully explicit NEED_* dependencies in user sources.
#ifndef CP_STRICT_TEMPLATE_NEEDS
  #define CP_STRICT_TEMPLATE_NEEDS 0
#endif

// Core modules: fundamental types and utilities.
#ifdef NEED_CORE
  #include "Types.hpp"
  #include "Constants.hpp"
  #include "Macros.hpp"
  #include "Math.hpp"
#endif

// Policy-based data structures (optional).
#ifdef NEED_PBDS
  #if !defined(NEED_CORE) && !CP_STRICT_TEMPLATE_NEEDS
    #include "Types.hpp"
    #include "Macros.hpp"
  #endif
  #include "PBDS.hpp"
#endif

// High-performance buffered I/O (opt-in).
#ifdef NEED_FAST_IO
  #if !defined(NEED_CORE) && !CP_STRICT_TEMPLATE_NEEDS
    #include "Types.hpp"
    #include "Macros.hpp"
  #endif
  #include "Fast_IO.hpp"
#endif

// I/O system (lightweight by default).
#ifdef NEED_IO
  #if !defined(NEED_CORE) && !CP_STRICT_TEMPLATE_NEEDS
    #include "Types.hpp"
    #include "Macros.hpp"
  #endif
  #include "IO.hpp"
#endif

// Bitwise operations.
#ifdef NEED_BIT_OPS
  #if !defined(NEED_CORE) && !CP_STRICT_TEMPLATE_NEEDS
    #include "Types.hpp"
    #include "Macros.hpp"
  #endif
  #include "Bit_Ops.hpp"
#endif

// Modular arithmetic.
#ifdef NEED_MOD_INT
  #if !defined(NEED_CORE) && !CP_STRICT_TEMPLATE_NEEDS
    #include "Types.hpp"
    #include "Macros.hpp"
    #include "Constants.hpp"
  #endif
  #include "Mod_Int.hpp"
#endif

// Container utilities.
#ifdef NEED_CONTAINERS
  #if !defined(NEED_CORE) && !CP_STRICT_TEMPLATE_NEEDS
    #include "Types.hpp"
    #include "Macros.hpp"
  #endif
  #include "Containers.hpp"
#endif

// Namespace usage.
using namespace std;
