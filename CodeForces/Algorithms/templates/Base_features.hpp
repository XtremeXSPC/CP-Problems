#pragma once

// Generated from profiles.toml. Do not edit by hand.

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

#ifdef NEED_TIMER
  #include "core/Timer.hpp"
#endif

#ifdef NEED_HASHING
  #include "modules/Hashing.hpp"
#endif

#ifdef NEED_PBDS
  #include "modules/PBDS.hpp"
#endif

#ifdef NEED_FAST_IO
  #include "modules/Fast_IO.hpp"
#endif

#ifdef NEED_FAST_IO_MINIMAL
  #include "modules/Fast_IO_Minimal.hpp"
#endif

#ifdef NEED_IO
  #include "modules/IO.hpp"
#endif

#ifdef NEED_BIT_OPS
  #include "modules/Bit_Ops.hpp"
#endif

#ifdef NEED_MOD_INT
  #include "modules/Mod_Int.hpp"
#endif

#ifdef NEED_CONTAINERS
  #include "modules/Containers.hpp"
#endif
