# ============================================================================ #
# ------------------------- CP ROUND FEATURE: Timing ------------------------- #
# ============================================================================ #
#
# Description:
#   Compilation timing reports for performance analysis.
#   Uses -ftime-report (GCC) or -ftime-trace (Clang) for detailed timing.
#
# Options:
#   CP_ENABLE_TIMING   - Enable timing reports (default: OFF).
#   CP_TIMING_CONFIGS  - Build types with timing (default: Debug;Sanitize).
#
# ============================================================================ #

include_guard(GLOBAL)

# Enable per-config compilation timing without affecting Release by default.
option(CP_ENABLE_TIMING "Enable detailed GCC/Clang compilation timing reports" OFF)
set(CP_TIMING_CONFIGS "Debug;Sanitize" CACHE STRING "Configs that receive timing flags")

if(CP_ENABLE_TIMING)
  message(STATUS "${ANSI_COLOR_CYAN}Compilation timing enabled.${ANSI_COLOR_RESET}")

  include(CheckCXXCompilerFlag)
  set(TIMING_FLAGS "")

  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    check_cxx_compiler_flag("-ftime-report" HAS_FTIME_REPORT)
    if(HAS_FTIME_REPORT)
      list(APPEND TIMING_FLAGS "-ftime-report")
    endif()
    string(REPLACE ";" "; " TIMING_FLAGS_DISPLAY "${TIMING_FLAGS}")
    message(STATUS "${ANSI_COLOR_CYAN}GCC timing flags detected: ${TIMING_FLAGS_DISPLAY}${ANSI_COLOR_RESET}")

  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
    check_cxx_compiler_flag("-ftime-trace" HAS_FTIME_TRACE)
    if(HAS_FTIME_TRACE)
      list(APPEND TIMING_FLAGS "-ftime-trace")
      check_cxx_compiler_flag("-ftime-trace-granularity=1" HAS_FTIME_TRACE_GRAN)
      if(HAS_FTIME_TRACE_GRAN)
        list(APPEND TIMING_FLAGS "-ftime-trace-granularity=1")
      endif()
    endif()
    check_cxx_compiler_flag("-ftime-report" HAS_CLANG_FTIME_REPORT)
    if(HAS_CLANG_FTIME_REPORT)
      list(APPEND TIMING_FLAGS "-ftime-report")
    endif()
    string(REPLACE ";" "; " TIMING_FLAGS_DISPLAY "${TIMING_FLAGS}")
    message(STATUS "${ANSI_COLOR_CYAN}Clang timing flags detected: ${TIMING_FLAGS_DISPLAY}${ANSI_COLOR_RESET}")
  endif()

  if(TIMING_FLAGS)
    foreach(cfg ${CP_TIMING_CONFIGS})
      foreach(f ${TIMING_FLAGS})
        add_compile_options("$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:${cfg}>>:${f}>")
      endforeach()
    endforeach()
    string(REPLACE ";" "; " TIMING_CONFIGS_DISPLAY "${CP_TIMING_CONFIGS}")
    message(STATUS "${ANSI_COLOR_CYAN}Timing applied to configs: ${TIMING_CONFIGS_DISPLAY}${ANSI_COLOR_RESET}")
  else()
    message(WARNING "${ANSI_COLOR_YELLOW}Timing requested, but no supported flags were found for this compiler.${ANSI_COLOR_RESET}")
  endif()
endif()

# ============================================================================ #
# End of CP Round Feature: Timing.
