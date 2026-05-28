#pragma once

// Minimal Fast_IO shim: selects the no-extension variant of Fast_IO.hpp.
// Use through NEED_FAST_IO_MINIMAL / CP_IO_PROFILE_FAST_MINIMAL when buffered
// I/O is needed without ModInt/StrongType compile-time checking baggage.
#ifndef CP_FAST_IO_VARIANT
  #define CP_FAST_IO_VARIANT 0
#endif

#include "Fast_IO.hpp"
