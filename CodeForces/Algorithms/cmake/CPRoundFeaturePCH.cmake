# ============================================================================ #
# -------------------------- CP ROUND FEATURE: PCH --------------------------- #
# ============================================================================ #
#
# Description:
#   Precompiled header management for faster compilation.
#   Uses PCH.h for Clang and bits/stdc++.h wrapper for GCC.
#
# Options:
#   CP_ENABLE_PCH          - Enable PCH (default: ON).
#   CP_FORCE_PCH_REBUILD   - Force PCH rebuild (default: OFF).
#   CP_PCH_TARGET_CONFIGS  - Build types using PCH (default: Debug;Sanitize).
#
# ============================================================================ #

include_guard(GLOBAL)

# PCH configuration options.
option(CP_ENABLE_PCH "Enable precompiled headers for faster compilation" ON)
option(CP_FORCE_PCH_REBUILD "Force PCH rebuild on next compilation" OFF)
set(CP_PCH_TARGET_CONFIGS "Debug;Sanitize" CACHE STRING "Build configurations that use PCH")

# PCH support detection and implementation.
if(CP_ENABLE_PCH AND CMAKE_VERSION VERSION_GREATER_EQUAL "3.16")
  # CMake 3.16+ has native PCH support.
  set(PCH_SUPPORTED TRUE)

  # Define the PCH header path based on compiler.
  #
  # Round directories historically expose the PCH header at
  #   algorithms/PCH.h  (symlink -> ../../../Algorithms/libs/PCH.h)
  # but if the round was created after the libs/ refactor the symlink may not
  # exist. We probe both locations so the round build degrades gracefully.
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang" OR USING_CLANG_FOR_SANITIZERS)
    set(_CP_PCH_HEADER_CANDIDATES
      "${CMAKE_CURRENT_SOURCE_DIR}/algorithms/PCH.h"
      "${CMAKE_CURRENT_SOURCE_DIR}/algorithms/libs/PCH.h"
    )
    set(PCH_TYPE "PCH.h")
  else()
    # For GCC, we can use bits/stdc++.h directly as PCH.
    set(_CP_PCH_HEADER_CANDIDATES
      "${CMAKE_CURRENT_SOURCE_DIR}/algorithms/PCH_Wrapper.h"
      "${CMAKE_CURRENT_SOURCE_DIR}/algorithms/libs/PCH_Wrapper.h"
    )
    set(PCH_TYPE "bits/stdc++.h wrapper")
  endif()

  set(PCH_HEADER_PATH "")
  foreach(_cp_pch_candidate IN LISTS _CP_PCH_HEADER_CANDIDATES)
    if(EXISTS "${_cp_pch_candidate}")
      set(PCH_HEADER_PATH "${_cp_pch_candidate}")
      break()
    endif()
  endforeach()
  unset(_CP_PCH_HEADER_CANDIDATES)

  # Verify PCH header was located.
  if(NOT PCH_HEADER_PATH)
    message(WARNING "${ANSI_COLOR_YELLOW}PCH: Header not found in algorithms/ or algorithms/libs/, disabling PCH${ANSI_COLOR_RESET}")
    set(PCH_SUPPORTED FALSE)
  else()
    message(STATUS "${ANSI_COLOR_GREEN}PCH: Enabled using ${PCH_TYPE}${ANSI_COLOR_RESET}")
    message(STATUS "${ANSI_COLOR_CYAN}PCH: Will be applied to configurations: ${CP_PCH_TARGET_CONFIGS}${ANSI_COLOR_RESET}")
  endif()
else()
  set(PCH_SUPPORTED FALSE)
  if(CP_ENABLE_PCH AND CMAKE_VERSION VERSION_LESS "3.16")
    message(WARNING "${ANSI_COLOR_YELLOW}PCH: CMake 3.16+ required for PCH support (current: ${CMAKE_VERSION})${ANSI_COLOR_RESET}")
  elseif(NOT CP_ENABLE_PCH)
    message(STATUS "${ANSI_COLOR_YELLOW}PCH: Disabled by user${ANSI_COLOR_RESET}")
  endif()
endif()

# PCH cache management.
if(CP_FORCE_PCH_REBUILD AND PCH_SUPPORTED)
  # Force rebuild of PCH by touching the header file.
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E touch "${PCH_HEADER_PATH}"
    RESULT_VARIABLE touch_result
  )
  if(touch_result EQUAL 0)
    message(STATUS "${ANSI_COLOR_CYAN}PCH: Forced rebuild by updating timestamp of ${PCH_HEADER_PATH}${ANSI_COLOR_RESET}")
  endif()
  # Reset the flag.
  set(CP_FORCE_PCH_REBUILD OFF CACHE BOOL "Force PCH rebuild on next compilation" FORCE)
endif()

# ============================================================================ #
# End of CP Round Feature: PCH.
