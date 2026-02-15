# ============================================================================ #
# ---------------------- CP Round Feature: LTO ------------------------------- #
# ============================================================================ #
#
# Description:
#   Link-Time Optimization configuration for Release builds.
#   Enables whole-program optimization when compiler support is available.
#
# Options:
#   CP_ENABLE_LTO  - Enable LTO for Release builds (default: OFF).
#
# ============================================================================ #

include_guard(GLOBAL)

option(CP_ENABLE_LTO "Enable Link Time Optimization (if supported)" OFF)

if(CP_ENABLE_LTO AND CMAKE_BUILD_TYPE STREQUAL "Release")
  include(CheckIPOSupported)
  check_ipo_supported(RESULT ipo_supported OUTPUT ipo_output)

  if(ipo_supported)
    message(STATUS "${ANSI_COLOR_GREEN}Link Time Optimization (LTO) enabled${ANSI_COLOR_RESET}")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  else()
    message(WARNING "${ANSI_COLOR_YELLOW}LTO not supported: ${ipo_output}${ANSI_COLOR_RESET}")
  endif()
endif()

# ============================================================================ #
# End of CP Round Feature: LTO.
