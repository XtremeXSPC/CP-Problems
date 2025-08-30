#pragma once
#include "Preamble.hpp"

//===----------------------------------------------------------------------===//
/* Modular Inclusions Based on Requirements */

// Core headers:
#ifdef NEED_CORE
  #include "Types.hpp"
  #include "Constants.hpp"
  #include "Macros.hpp"
  #include "Math.hpp"
#endif

// Optional Modules:
#ifdef NEED_BIT_OPS
  #include "Bit_Ops.hpp"
#endif

#ifdef NEED_MOD_INT
  #include "Mod_Int.hpp"
#endif

#ifdef NEED_CONTAINERS
  #include "Containers.hpp"
#endif

#ifdef NEED_IO
  #include "IO.hpp"
#endif