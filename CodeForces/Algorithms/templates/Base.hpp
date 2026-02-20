#pragma once
#include "Preamble.hpp"

//===----------------------------------------------------------------------===//
/* Modular Inclusions Based on Requirements */

// Core modules: fundamental types and utilities.
#ifdef NEED_CORE
  #include "Types.hpp"
  #include "Constants.hpp"
  #include "Macros.hpp"
  #include "Math.hpp"
#endif

// Policy-based data structures (optional).
#ifdef NEED_PBDS
  #ifndef NEED_CORE
    #include "Types.hpp"
  #endif
  #include "PBDS.hpp"
#endif

// High-performance buffered I/O (opt-in).
#ifdef NEED_FAST_IO
  #ifndef NEED_CORE
    #include "Types.hpp"
    #include "Macros.hpp"
  #endif
  #include "Fast_IO.hpp"
#endif

// I/O system (lightweight by default).
#ifdef NEED_IO
  #ifndef NEED_CORE
    #include "Types.hpp"
    #include "Macros.hpp"
  #endif
  #include "IO.hpp"
#endif

// Bitwise operations.
#ifdef NEED_BIT_OPS
  #ifndef NEED_CORE
    #include "Types.hpp"
  #endif
  #include "Bit_Ops.hpp"
#endif

// Modular arithmetic.
#ifdef NEED_MOD_INT
  #ifndef NEED_CORE
    #include "Types.hpp"
    #include "Constants.hpp"
  #endif
  #include "Mod_Int.hpp"
#endif

// Container utilities.
#ifdef NEED_CONTAINERS
  #ifndef NEED_CORE
    #include "Types.hpp"
    #include "Macros.hpp"
  #endif
  #include "Containers.hpp"
#endif

// Namespace usage.
using namespace std;
