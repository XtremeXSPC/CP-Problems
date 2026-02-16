# ============================================================================ #
# ----------------------- CP ROUND FEATURE: Sanitizers ----------------------- #
# ============================================================================ #
#
# Description:
#   Compiler verification and sanitizer support detection.
#   Automatically selects Clang for Sanitize builds on macOS for better
#   sanitizer runtime support.
#
# ============================================================================ #

include_guard(GLOBAL)

# Special case: Allow Clang for Sanitize builds on macOS.
set(USING_CLANG_FOR_SANITIZERS FALSE)

if(CMAKE_BUILD_TYPE STREQUAL "Sanitize" AND APPLE)
  # On macOS, sanitizer workflows are supported only with Clang.
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
    set(USING_CLANG_FOR_SANITIZERS TRUE)
    message(STATUS "${ANSI_COLOR_CYAN}Using Clang for Sanitize build on macOS (required for sanitizer support)${ANSI_COLOR_RESET}")
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    message(FATAL_ERROR
      "${ANSI_COLOR_RED}Sanitize + GCC is not supported on macOS in this toolchain setup.\n"
      "Use Clang instead (for example: 'cppconf Sanitize clang' or preset 'cp-sanitize-clang').${ANSI_COLOR_RESET}")
  endif()
elseif(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
  message(FATAL_ERROR "This project requires GCC or Clang. Found: ${CMAKE_CXX_COMPILER_ID}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Sanitize" AND CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
  set(USING_CLANG_FOR_SANITIZERS TRUE)
  message(STATUS "${ANSI_COLOR_CYAN}Using Clang for Sanitize build${ANSI_COLOR_RESET}")
elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Sanitize" AND CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
  # Clang debug/release builds are valid; avoid noisy warnings on Linux.
  message(STATUS
    "${ANSI_COLOR_CYAN}Using Clang for non-Sanitize build (${CMAKE_CXX_COMPILER_ID}).${ANSI_COLOR_RESET}")
endif()

# ============================================================================ #
# End of CP Round Feature: Sanitizers.
