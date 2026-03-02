#pragma once
#include "Preamble.hpp"

//===----------------------------------------------------------------------===//
/* Modular Inclusions Based on Requirements */

// Strict profile: safer defaults and reduced template surface.
#if defined(CP_TEMPLATE_PROFILE_STRICT)
  #ifndef CP_STRICT_TEMPLATE_NEEDS
    #define CP_STRICT_TEMPLATE_NEEDS 1
  #endif
  #ifndef CP_CORE_ENABLE_MATH
    #define CP_CORE_ENABLE_MATH 0
  #endif
  #ifndef CP_USE_GLOBAL_STD_NAMESPACE
    #define CP_USE_GLOBAL_STD_NAMESPACE 0
  #endif
  #ifndef CP_ENABLE_LEGACY_IO_VEC_MACROS
    #define CP_ENABLE_LEGACY_IO_VEC_MACROS 0
  #endif
  #ifndef CP_ENABLE_DEPRECATED_COMPAT
    #define CP_ENABLE_DEPRECATED_COMPAT 0
  #endif
#endif

// Backward-compatible dependency behavior.
// Set to 1 to require fully explicit NEED_* dependencies in user sources.
#ifndef CP_STRICT_TEMPLATE_NEEDS
  #define CP_STRICT_TEMPLATE_NEEDS 0
#endif

// Keep Math in NEED_CORE by default for backward compatibility.
// Set to 0 to reduce flatten size and include NEED_MATH explicitly where needed.
#ifndef CP_CORE_ENABLE_MATH
  #define CP_CORE_ENABLE_MATH 1
#endif

// Toggle "using namespace std;" emission from templates.
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

// Optional I/O profile shortcuts.
// - CP_IO_PROFILE_SIMPLE:        enables NEED_IO
// - CP_IO_PROFILE_FAST_MINIMAL:  enables NEED_FAST_IO
// - CP_IO_PROFILE_FAST_EXTENDED: enables NEED_FAST_IO + NEED_MOD_INT + NEED_TYPE_SAFETY
//                                and turns on Fast_IO ModInt/StrongType extensions.
#if (defined(CP_IO_PROFILE_SIMPLE) + defined(CP_IO_PROFILE_FAST_MINIMAL) + defined(CP_IO_PROFILE_FAST_EXTENDED)) > 1
  #error "Choose only one I/O profile macro: CP_IO_PROFILE_SIMPLE, CP_IO_PROFILE_FAST_MINIMAL, or CP_IO_PROFILE_FAST_EXTENDED."
#endif

#ifdef CP_IO_PROFILE_SIMPLE
  #ifndef NEED_IO
    #define NEED_IO
  #endif
#endif

#ifdef CP_IO_PROFILE_FAST_MINIMAL
  #ifndef NEED_FAST_IO
    #define NEED_FAST_IO
  #endif
#endif

#ifdef CP_IO_PROFILE_FAST_EXTENDED
  #ifndef NEED_FAST_IO
    #define NEED_FAST_IO
  #endif
  #ifndef NEED_MOD_INT
    #define NEED_MOD_INT
  #endif
  #ifndef NEED_TYPE_SAFETY
    #define NEED_TYPE_SAFETY
  #endif
  #ifndef CP_FAST_IO_ENABLE_MODINT
    #define CP_FAST_IO_ENABLE_MODINT 1
  #endif
  #ifndef CP_FAST_IO_ENABLE_STRONG_TYPE
    #define CP_FAST_IO_ENABLE_STRONG_TYPE 1
  #endif
#endif

// Core modules: fundamental types and utilities.
#ifdef NEED_CORE
  #include "Types.hpp"
  #include "Constants.hpp"
  #include "Macros.hpp"
  #if CP_CORE_ENABLE_MATH
    #include "Math.hpp"
  #endif
#endif

// Math utilities as a standalone module (opt-in granularity).
#ifdef NEED_MATH
  #if !defined(NEED_CORE) && !CP_STRICT_TEMPLATE_NEEDS
    #include "Types.hpp"
  #endif
  #include "Math.hpp"
#endif

// Timer utilities (Stopwatch).
#ifdef NEED_TIMER
  #if !defined(NEED_CORE) && !defined(NEED_MATH) && !CP_STRICT_TEMPLATE_NEEDS
    #include "Types.hpp"
  #endif
  #include "Timer.hpp"
#endif

// Type-safety utilities are optional to avoid increasing default flatten size.
#ifdef NEED_TYPE_SAFETY
  #if !defined(NEED_CORE) && !CP_STRICT_TEMPLATE_NEEDS
    #include "Types.hpp"
  #endif
  #include "Concepts.hpp"
  #include "Cast.hpp"
  #include "Strong_Type.hpp"
#endif

// Randomized hash utilities for unordered containers (anti-collision).
#ifdef NEED_HASHING
  #if !defined(NEED_CORE) && !CP_STRICT_TEMPLATE_NEEDS
    #include "Types.hpp"
  #endif
  #include "Hashing.hpp"
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
// Optional Fast_IO extensions:
// - CP_FAST_IO_ENABLE_MODINT defaults to 1 when NEED_MOD_INT is defined.
// - CP_FAST_IO_ENABLE_STRONG_TYPE defaults to 0 (enable explicitly when needed).
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
#if CP_USE_GLOBAL_STD_NAMESPACE
  using namespace std;
#endif
