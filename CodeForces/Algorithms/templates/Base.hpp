#pragma once
#include "Preamble.hpp"

//===----------------------------------------------------------------------===//
/* Modular Inclusions Based on Requirements */

// Core Modules - Fundamental types and utilities:
#ifdef NEED_CORE
  #include "Types.hpp"
  #include "Constants.hpp"
  #include "Macros.hpp"
  #include "Math.hpp"
#endif

// I/O System:
#ifdef NEED_IO
  #ifndef NEED_CORE
    #include "Types.hpp"
    #include "Macros.hpp"
  #endif
  #include "IO.hpp"
#endif

// Bitwise Operations:
#ifdef NEED_BIT_OPS
  #ifndef NEED_CORE
    #include "Types.hpp"
  #endif
  #include "Bit_Ops.hpp"
#endif

// Modular arithmetic
#ifdef NEED_MOD_INT
  #ifndef NEED_CORE
    #include "Types.hpp"
    #include "Constants.hpp"
  #endif
  #include "Mod_Int.hpp"
#endif

// Container Utilities:
#ifdef NEED_CONTAINERS
  #ifndef NEED_CORE
    #include "Types.hpp"
    #include "Macros.hpp"
  #endif
  #include "Containers.hpp"
#endif

// Namespace usage and global setup:
using namespace std;

// Fast I/O setup (only if IO is included):
#ifdef NEED_IO
struct FastIOSetup {
  FastIOSetup() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::cout << std::fixed << std::setprecision(10);
  }
} fast_io_setup;
#endif