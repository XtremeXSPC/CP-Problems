# ============================================================================ #
# --------------------- CP ROUND BUILD SYSTEM BOOTSTRAP ---------------------- #
# ============================================================================ #
#
# Description:
#   Main entry point orchestrating build system phases:
#   1) CPRoundConfig   - Core configuration + feature modules.
#   2) CPRoundTargets  - Target definitions and problem detection.
#   3) CPRoundSummary  - Configuration summary and diagnostics.
#
# Usage:
#   include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPRoundBootstrap.cmake")
#
# ============================================================================ #

include_guard(GLOBAL)

set(CP_ROUND_CMAKE_ROOT "${CMAKE_CURRENT_LIST_DIR}")

include("${CP_ROUND_CMAKE_ROOT}/CPRoundConfig.cmake")
include("${CP_ROUND_CMAKE_ROOT}/CPRoundTargets.cmake")
include("${CP_ROUND_CMAKE_ROOT}/CPRoundSummary.cmake")

# ============================================================================ #
# End of CP Round Bootstrap Module.
