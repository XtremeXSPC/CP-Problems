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
#   Debug     - Debug symbols, no optimization, stack protection
#   Release   - Full optimization, vectorization, -march=native
#   Sanitize  - ASan/UBSan/LSan (Clang preferred on macOS)
#
# Options:
#   CP_ENABLE_PCH         - Enable precompiled headers (default: ON).
#   CP_FORCE_PCH_REBUILD  - Force PCH rebuild.
#   CP_ENABLE_TIMING      - Enable compilation timing reports.
#   CP_ENABLE_LTO         - Enable Link-Time Optimization.
#
# ============================================================================ #

include_guard(GLOBAL)

if(NOT DEFINED CP_ROUND_CMAKE_ROOT)
  set(CP_ROUND_CMAKE_ROOT "${CMAKE_CURRENT_LIST_DIR}")
endif()

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
include("${CP_ROUND_CMAKE_ROOT}/CPRoundFeatureTiming.cmake")

# ------------------- TBB for Parallel Algorithms Support -------------------- #
# Find TBB to enable support for C++ parallel algorithms (std::execution::par).
find_package(TBB QUIET)
if(TBB_FOUND)
  message(STATUS "${ANSI_COLOR_GREEN}Found TBB, parallel algorithms support enabled.${ANSI_COLOR_RESET}")
else()
  message(WARNING "${ANSI_COLOR_YELLOW}TBB not found. Linking with parallel execution policies may fail.${ANSI_COLOR_RESET}")
endif()

# ---------------------------- LTO Configuration ----------------------------- #
include("${CP_ROUND_CMAKE_ROOT}/CPRoundFeatureLTO.cmake")

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
include("${CP_ROUND_CMAKE_ROOT}/CPRoundFeatureSanitizers.cmake")

# -------------------- Precompiled Header (PCH) Management ------------------- #
include("${CP_ROUND_CMAKE_ROOT}/CPRoundFeaturePCH.cmake")

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

# Enable ccache if available and usable.
find_program(CCACHE_PROGRAM ccache)
set(CP_CCACHE_ENABLED FALSE)
if(CCACHE_PROGRAM)
    set(CP_CCACHE_DISABLE_ENV "$ENV{CCACHE_DISABLE}")
    if(CP_CCACHE_DISABLE_ENV AND NOT CP_CCACHE_DISABLE_ENV STREQUAL "0")
        message(STATUS "ccache disabled via CCACHE_DISABLE=${CP_CCACHE_DISABLE_ENV}")
    else()
        set(CP_CCACHE_PROBE_OK TRUE)
        set(CP_CCACHE_DIR "$ENV{CCACHE_DIR}")
        if(NOT CP_CCACHE_DIR)
            if(DEFINED ENV{HOME} AND NOT "$ENV{HOME}" STREQUAL "")
                set(CP_CCACHE_DIR "$ENV{HOME}/.cache/ccache")
            endif()
        endif()

        if(CP_CCACHE_DIR)
            execute_process(
                COMMAND ${CMAKE_COMMAND} -E make_directory "${CP_CCACHE_DIR}/tmp"
                RESULT_VARIABLE CP_CCACHE_MKDIR_RESULT
                ERROR_VARIABLE CP_CCACHE_MKDIR_ERROR
            )
            if(NOT CP_CCACHE_MKDIR_RESULT EQUAL 0)
                set(CP_CCACHE_PROBE_OK FALSE)
            else()
                set(CP_CCACHE_PROBE_FILE "${CP_CCACHE_DIR}/tmp/.cp_ccache_probe")
                execute_process(
                    COMMAND ${CMAKE_COMMAND} -E touch "${CP_CCACHE_PROBE_FILE}"
                    RESULT_VARIABLE CP_CCACHE_TOUCH_RESULT
                    ERROR_VARIABLE CP_CCACHE_TOUCH_ERROR
                )
                if(NOT CP_CCACHE_TOUCH_RESULT EQUAL 0)
                    set(CP_CCACHE_PROBE_OK FALSE)
                else()
                    execute_process(
                        COMMAND ${CMAKE_COMMAND} -E rm -f "${CP_CCACHE_PROBE_FILE}"
                    )
                endif()
            endif()
        endif()

        if(CP_CCACHE_PROBE_OK)
            set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
            set(CP_CCACHE_ENABLED TRUE)
            message(STATUS "Found ccache: ${CCACHE_PROGRAM} - builds will be faster!")
        else()
            message(WARNING
                "ccache found but cache directory is not writable. "
                "Build will continue without ccache."
            )
        endif()
    endif()
endif()

# ============================================================================ #
# End of CP Round Config Module.
