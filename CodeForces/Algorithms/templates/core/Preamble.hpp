#pragma once
#include "Config.hpp"
#include "Compiler.hpp"
#include "StdHeaders.hpp"
#include "FeatureDetect.hpp"
#include "Debug.hpp"

#ifdef __clang__
  #pragma clang diagnostic pop
#endif

#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC diagnostic pop
#endif
