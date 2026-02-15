# ============================================================================ #
# -------------- CP ROUND BUILD CONFIGURATION (Phase 1: Config) -------------- #
# ============================================================================ #
#
# Description:
#   Core configuration for competitive programming builds.
#   Handles compiler setup, PCH management, TBB integration, RPATH fixing,
#   and build-type specific optimizations.
#
# Build Types:
#   Debug    - Debug symbols, no optimization, stack protection
#   Release  - Full optimization, vectorization, -march=native
#   Sanitize - ASan/UBSan/LSan (Clang preferred on macOS)
#
# Options:
#   CP_ENABLE_PCH        - Enable precompiled headers (default: ON).
#   CP_FORCE_PCH_REBUILD - Force PCH rebuild.
#   CP_ENABLE_TIMING     - Enable compilation timing reports.
#   CP_ENABLE_LTO        - Enable Link-Time Optimization.
#
# ============================================================================ #

include_guard(GLOBAL)

# ----------------------------- ANSI Color Codes ----------------------------- #
# Define variables for ANSI color codes to make message() output more readable.
if(UNIX OR APPLE OR CMAKE_HOST_WIN32)
  string(ASCII 27 Esc)
  set(ANSI_COLOR_RED     "${Esc}[31m")
  set(ANSI_COLOR_GREEN   "${Esc}[32m")
  set(ANSI_COLOR_YELLOW  "${Esc}[33m")
  set(ANSI_COLOR_BLUE    "${Esc}[34m")
  set(ANSI_COLOR_CYAN    "${Esc}[36m")
  set(ANSI_COLOR_BOLD    "${Esc}[1m")
  set(ANSI_COLOR_RESET   "${Esc}[0m")
endif()

# ----------------------- macOS RPATH Handling Cleanup ----------------------- #
if(APPLE)
  # Set a modern deployment target for macOS to ensure compatibility.
  set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "macOS deployment target")

  # Modern RPATH handling for macOS.
  if(NOT DEFINED CMAKE_SKIP_BUILD_RPATH)
    set(CMAKE_MACOSX_RPATH ON)
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

    # Disable automatic RPATH generation to avoid duplicates.
    set(CMAKE_SKIP_RPATH FALSE)
    set(CMAKE_SKIP_INSTALL_RPATH TRUE)
  endif()

  # Get the LLVM/Clang library path if using Homebrew LLVM.
  if(CMAKE_CXX_COMPILER MATCHES "llvm")
    get_filename_component(LLVM_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
    get_filename_component(LLVM_ROOT_DIR ${LLVM_BIN_DIR} DIRECTORY)
    set(LLVM_LIB_DIR "${LLVM_ROOT_DIR}/lib")

    if(EXISTS "${LLVM_LIB_DIR}")
      # Set a clean, single RPATH for LLVM.
      set(CMAKE_INSTALL_RPATH "${LLVM_LIB_DIR}")
      message(STATUS "RPATH Fix: Set single LLVM RPATH: ${LLVM_LIB_DIR}")
    endif()
  endif()
endif()

# ------------------------- Compilation Timing Setup ------------------------- #
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

# ------------------- TBB for Parallel Algorithms Support -------------------- #
# Find TBB to enable support for C++ parallel algorithms (std::execution::par).
find_package(TBB QUIET)
if(TBB_FOUND)
  message(STATUS "${ANSI_COLOR_GREEN}Found TBB, parallel algorithms support enabled.${ANSI_COLOR_RESET}")
else()
  message(WARNING "${ANSI_COLOR_YELLOW}TBB not found. Linking with parallel execution policies may fail.${ANSI_COLOR_RESET}")
endif()

# ---------------------------- LTO Configuration ----------------------------- #
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

# ------------------- Compiler Include Path Auto-Detection ------------------- #
# Shared Clangd assist module keeps CMakeLists compact and easier to maintain.
set(CP_CLANGD_ASSIST_MODULE "")
foreach(candidate
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ClangdAssist.cmake"
  "${CMAKE_CURRENT_SOURCE_DIR}/algorithms/cmake/ClangdAssist.cmake")
  if(EXISTS "${candidate}")
    set(CP_CLANGD_ASSIST_MODULE "${candidate}")
    break()
  endif()
endforeach()

if(CP_CLANGD_ASSIST_MODULE)
  include("${CP_CLANGD_ASSIST_MODULE}")
  cp_detect_compiler_system_includes(COMPILER_SYSTEM_INCLUDE_PATHS)
else()
  message(WARNING "Clangd Assist module not found. Include-path auto-detection disabled.")
  set(COMPILER_SYSTEM_INCLUDE_PATHS "")
endif()
# -------------------------- Compiler Verification --------------------------- #
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

# -------------------- Precompiled Header (PCH) Management ------------------- #
# PCH configuration options.
option(CP_ENABLE_PCH "Enable precompiled headers for faster compilation" ON)
option(CP_FORCE_PCH_REBUILD "Force PCH rebuild on next compilation" OFF)
set(CP_PCH_TARGET_CONFIGS "Debug;Sanitize" CACHE STRING "Build configurations that use PCH")

# PCH support detection and implementation.
if(CP_ENABLE_PCH AND CMAKE_VERSION VERSION_GREATER_EQUAL "3.16")
    # CMake 3.16+ has native PCH support.
    set(PCH_SUPPORTED TRUE)

    # Define the PCH header path based on compiler.
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang" OR USING_CLANG_FOR_SANITIZERS)
        set(PCH_HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/algorithms/PCH.h")
        set(PCH_TYPE "PCH.h")
    else()
        # For GCC, we can use bits/stdc++.h directly as PCH.
        set(PCH_HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/algorithms/PCH_Wrapper.h")
        set(PCH_TYPE "bits/stdc++.h wrapper")

        # Wrapper availability is checked in the generic PCH existence guard below.
    endif()

    # Verify PCH header exists.
    if(NOT EXISTS "${PCH_HEADER_PATH}")
        message(WARNING "${ANSI_COLOR_YELLOW}PCH: Header not found at ${PCH_HEADER_PATH}, disabling PCH${ANSI_COLOR_RESET}")
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

# ----------------------------- Project Settings ----------------------------- #
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(MAKE_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

# Enable ccache if available.
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    message(STATUS "Found ccache: ${CCACHE_PROGRAM} - builds will be faster!")
endif()

# ============================================================================ #
# End of CP Round Config Module.
