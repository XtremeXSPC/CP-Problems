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
  # On macOS, we prefer Clang for sanitizers due to better library support.
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
    set(USING_CLANG_FOR_SANITIZERS TRUE)
    message(STATUS "${ANSI_COLOR_CYAN}Using Clang for Sanitize build on macOS (better sanitizer support)${ANSI_COLOR_RESET}")
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # Check if GCC has sanitizer support on this system.
    include(CheckCXXCompilerFlag)
    set(CMAKE_REQUIRED_FLAGS "-fsanitize=address")
    check_cxx_compiler_flag("-fsanitize=address" HAS_ASAN)
    unset(CMAKE_REQUIRED_FLAGS)

    if(NOT HAS_ASAN)
      message(WARNING
        "${ANSI_COLOR_YELLOW}GCC on this macOS system lacks sanitizer support.\n"
        "Consider using 'cppconf Sanitize clang' to use Clang for sanitizers.${ANSI_COLOR_RESET}")
    endif()
  endif()
elseif(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
  message(FATAL_ERROR "This project requires GCC or Clang. Found: ${CMAKE_CXX_COMPILER_ID}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Sanitize" AND CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
  set(USING_CLANG_FOR_SANITIZERS TRUE)
  message(STATUS "${ANSI_COLOR_CYAN}Using Clang for Sanitize build${ANSI_COLOR_RESET}")
elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Sanitize" AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT APPLE)
  # Only show warning for non-Apple systems when using non-GCC for non-Sanitize builds.
  message(WARNING
    "${ANSI_COLOR_YELLOW}Non-GCC compiler detected for non-Sanitize build.\n"
    "Detected compiler: ${CMAKE_CXX_COMPILER_ID}\n"
    "Some features like <bits/stdc++.h> may not be available.${ANSI_COLOR_RESET}")
endif()

# ============================================================================ #
# End of CP Round Feature: Sanitizers.
