# Centralized CP round bootstrap entrypoint.
include_guard(GLOBAL)

set(CP_ROUND_CMAKE_ROOT "${CMAKE_CURRENT_LIST_DIR}")

include("${CP_ROUND_CMAKE_ROOT}/CPRoundConfig.cmake")
include("${CP_ROUND_CMAKE_ROOT}/CPRoundTargets.cmake")
include("${CP_ROUND_CMAKE_ROOT}/CPRoundSummary.cmake")
