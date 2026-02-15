# ============================================================================ #
# ---------------- CP ROUND CONFIGURATION (Phase 2: Targets) ----------------- #
# ============================================================================ #
#
# Description:
#   Target creation for competitive programming problems with automatic
#   detection, PCH management, TBB integration, and sanitizer configuration.
#
# Compiler Flags:
#   Debug:    -g2 -O0 -fstack-protector-strong
#   Release:  -O2 -funroll-loops -ftree-vectorize -march=native
#   Sanitize: -g -O1 -fsanitize=address,undefined,leak
#
# Functions:
#   cp_add_problem(TARGET_NAME SOURCE_FILE) - Creates configured target
#
# Utility Targets:
#   symlink_clangd - Creates compile_commands.json symlinks.
#   all_problems   - Builds all detected problems.
#   rebuild        - Clean rebuild.
#
# ============================================================================ #

include_guard(GLOBAL)

# ============================================================================ #
# --------------------- Helper Function to Add a Problem --------------------- #
# ============================================================================ #

function(cp_add_problem TARGET_NAME SOURCE_FILE)
  add_executable(${TARGET_NAME} ${SOURCE_FILE})

  # Set C++23 standard for the target.
  set_target_properties(${TARGET_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
  )

  # ----- Precompiled Header Integration ----- #
set(USE_PCH_FOR_TARGET FALSE)
  set(PCH_REASON "")

  # Determine if this target should use PCH.
  if(PCH_SUPPORTED)
    # Check if current build type supports PCH.
    if(CMAKE_BUILD_TYPE IN_LIST CP_PCH_TARGET_CONFIGS)
      set(USE_PCH_FOR_TARGET TRUE)
      set(PCH_REASON "PCH enabled for ${CMAKE_BUILD_TYPE}")

      # IMPORTANT: Apply PCH directly to the target using CMake's native support.
      target_precompile_headers(${TARGET_NAME}
        PRIVATE
          "${PCH_HEADER_PATH}"
      )

      # Add PCH-specific definitions.
      target_compile_definitions(${TARGET_NAME} PRIVATE
        PCH_ENABLED
        USE_PCH
      )

      # PCH optimization flags (optional, CMake handles most of this automatically).
      if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        # GCC: Ensure PCH is used correctly.
        # Note: -fpch-preprocess is automatically added by CMake when needed.
        target_compile_options(${TARGET_NAME} PRIVATE
          -Winvalid-pch  # Warn if PCH is invalid/outdated.
        )
      elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
        # Clang: Enable better PCH diagnostics.
        target_compile_options(${TARGET_NAME} PRIVATE
          -Wno-pch-date-time  # Don't warn about PCH timestamp differences.
        )
      endif()

    else()
      set(PCH_REASON "PCH not configured for ${CMAKE_BUILD_TYPE}")
    endif()
  else()
    if(USING_CLANG_FOR_SANITIZERS OR CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
      # Fallback: use PCH.h as regular header for Clang.
      target_compile_definitions(${TARGET_NAME} PRIVATE USE_CLANG_SANITIZE)
      set(PCH_REASON "Using PCH.h as regular header (Clang fallback)")
    else()
      set(PCH_REASON "Using bits/stdc++.h or manual includes")
    endif()
  endif()

  # ----- TBB Integration ----- #
  # Link with TBB for parallel algorithm support, if found.
  if(TBB_FOUND)
    target_link_libraries(${TARGET_NAME} PRIVATE TBB::tbb)
  endif()

  # ----- Target-specific compiler definitions ----- #
  target_compile_definitions(${TARGET_NAME} PRIVATE
    # Define LOCAL for debug builds.
    $<$<CONFIG:Debug,Sanitize>:LOCAL>
    # Define NDEBUG for release builds.
    $<$<CONFIG:Release>:NDEBUG>
    # Add _GLIBCXX_DEBUG for better STL debugging in debug mode (GCC only).
    $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:_GLIBCXX_DEBUG>
    # Enable C++23 specific features.
    $<$<CXX_COMPILER_ID:GNU>:_GLIBCXX_ASSERTIONS>
    $<$<AND:$<CXX_COMPILER_ID:GNU>,$<VERSION_GREATER_EQUAL:${CMAKE_CXX_COMPILER_VERSION},13>>:_GLIBCXX_USE_CXX23_ABI>
    # Use modern libc++ hardening mode for Clang/AppleClang.
    $<$<CXX_COMPILER_ID:Clang,AppleClang>:_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE>
  )

  # ----- Compiler-specific flags ----- #
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # GCC flags.
    set(COMMON_FLAGS -Wall -Wextra -Wpedantic -Wshadow)
    set(DEBUG_FLAGS -g2 -O0 -fstack-protector-strong)
    set(RELEASE_FLAGS -O2 -funroll-loops -ftree-vectorize -ffast-math)

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64|AMD64)")
      list(APPEND RELEASE_FLAGS -march=native)
    endif()

    # GCC sanitizer flags (might not work on macOS).
    set(SANITIZE_FLAGS -g -O1 -fsanitize=address,undefined,leak
      -fsanitize-address-use-after-scope
      -fno-omit-frame-pointer
      -fno-sanitize-recover=all)

  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
    # Clang flags.
    set(COMMON_FLAGS -Wall -Wextra -Wpedantic -Wshadow)
    set(DEBUG_FLAGS -g2 -O0 -fstack-protector-strong)
    set(RELEASE_FLAGS -O2 -funroll-loops -fvectorize)

    # Clang sanitizer flags (work well on all platforms).
    set(SANITIZE_FLAGS -g -O1
      -fsanitize=address,undefined
      -fsanitize-address-use-after-scope
      -fno-omit-frame-pointer
      -fno-sanitize-recover=all)

    # Add integer and nullability sanitizers on newer Clang.
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "10.0")
      list(APPEND SANITIZE_FLAGS -fsanitize=integer,nullability)
    endif()
  endif()

  # Apply compiler flags.
  target_compile_options(${TARGET_NAME} PRIVATE
    # Use C++23 standard.
    -std=c++23
    ${COMMON_FLAGS}
    # Suppress common warnings.
    -Wno-unused-const-variable
    -Wno-sign-conversion
    -Wno-unused-parameter
    -Wno-unused-variable

    $<$<CONFIG:Debug>:${DEBUG_FLAGS}>
    $<$<CONFIG:Release>:${RELEASE_FLAGS}>
    $<$<CONFIG:Sanitize>:${SANITIZE_FLAGS}>
  )

  # ----- Include directories ----- #
  if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/algorithms)
    target_include_directories(${TARGET_NAME} PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/algorithms
    )
  endif()

  # ----- Platform and compiler specific adjustments ----- #
  if(APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # Use libstdc++ for GCC on macOS.
    target_compile_options(${TARGET_NAME} PRIVATE -stdlib=libstdc++)
    target_link_options(${TARGET_NAME} PRIVATE -stdlib=libstdc++)
  elseif(APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
    # Use libc++ for Clang on macOS, being explicit to avoid linking issues.
    target_compile_options(${TARGET_NAME} PRIVATE -stdlib=libc++)
    target_link_options(${TARGET_NAME} PRIVATE -stdlib=libc++)
  endif()

  # Add system include paths for better clangd support.
  if(COMPILER_SYSTEM_INCLUDE_PATHS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
      if(APPLE)
        # For GCC on macOS, use nostdinc++ and add paths manually.
        # This is reported to be necessary for clangd with Homebrew GCC.
        target_compile_options(${TARGET_NAME} PRIVATE -nostdinc++)
        foreach(dir IN LISTS COMPILER_SYSTEM_INCLUDE_PATHS)
          target_compile_options(${TARGET_NAME} PRIVATE "-isystem${dir}")
        endforeach()
      endif()
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
      # For Clang, just add as system includes without nostdinc++.
      foreach(dir IN LISTS COMPILER_SYSTEM_INCLUDE_PATHS)
        target_include_directories(${TARGET_NAME} SYSTEM PRIVATE ${dir})
      endforeach()
      # RPATH fix for this specific target.
      if(LLVM_LIB_DIR AND EXISTS "${LLVM_LIB_DIR}")
        set_target_properties(${TARGET_NAME} PROPERTIES
          INSTALL_RPATH "${LLVM_LIB_DIR}"
          BUILD_WITH_INSTALL_RPATH TRUE
          SKIP_BUILD_RPATH FALSE
        )
      endif()
    endif()
  endif()

  # ----- Linker options ----- #
  target_link_options(${TARGET_NAME} PRIVATE
    # Sanitizer linking.
    $<$<CONFIG:Sanitize>:${SANITIZE_FLAGS}>
    # Strip symbols in release.
    $<$<CONFIG:Release>:-s>
  )

  # ----- Platform-specific linking for std::stacktrace ----- #
  # Check for stacktrace support and required libraries.
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND NOT APPLE)
    # Test if stacktrace libraries are available.
    include(CheckCXXSourceCompiles)
    set(CMAKE_REQUIRED_FLAGS "-std=c++22")
    set(CMAKE_REQUIRED_LIBRARIES "stdc++_libbacktrace")

    check_cxx_source_compiles(
      "#include <stacktrace>
       int main() {
         auto trace = std::stacktrace::current();
         return trace.size();
       }"
      STACKTRACE_WORKS
    )

    if(STACKTRACE_WORKS)
      target_link_libraries(${TARGET_NAME} PRIVATE stdc++_libbacktrace)
      message(STATUS "Stacktrace support enabled for ${TARGET_NAME}")
    else()
      target_compile_definitions(${TARGET_NAME} PRIVATE _GLIBCXX_STACKTRACE_DISABLED)
      message(STATUS "Stacktrace support disabled for ${TARGET_NAME} (libraries not found)")
    endif()

    # Reset CMAKE_REQUIRED_* variables.
    unset(CMAKE_REQUIRED_FLAGS)
    unset(CMAKE_REQUIRED_LIBRARIES)
  endif()

  # Additional linking for debug builds.
  if(CMAKE_BUILD_TYPE MATCHES "Debug|Sanitize")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND NOT APPLE)
      # Try to link additional debug libraries if available.
      find_library(BACKTRACE_LIB backtrace)
      if(BACKTRACE_LIB)
        target_link_libraries(${TARGET_NAME} PRIVATE ${BACKTRACE_LIB})
      endif()
    endif()
  endif()

  if(USE_PCH_FOR_TARGET)
    set(PCH_STATUS "Yes (Active)")

    # Optional: Print PCH file location for debugging.
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_VERBOSE_MAKEFILE)
      get_target_property(pch_header ${TARGET_NAME} PRECOMPILE_HEADERS)
      message(STATUS "  PCH Header: ${pch_header}")
    endif()
  else()
    set(PCH_STATUS "No (${PCH_REASON})")
  endif()

  message(STATUS "Added problem: ${TARGET_NAME} "
                 "[Compiler: ${CMAKE_CXX_COMPILER_ID}, "
                 "PCH: ${PCH_STATUS}]")
endfunction()

# ============================================================================ #
# ----------------------- Automatic Problem Detection ------------------------ #
# ============================================================================ #

file(GLOB PROBLEM_SOURCES LIST_DIRECTORIES false "*.cpp" "*.cc" "*.cxx")
list(SORT PROBLEM_SOURCES)
list(FILTER PROBLEM_SOURCES EXCLUDE REGEX ".*template.*")

foreach(source_file ${PROBLEM_SOURCES})
    get_filename_component(exec_name ${source_file} NAME_WE)
    cp_add_problem(${exec_name} ${source_file})
endforeach()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT main)
endif()

# ============================================================================ #
# ----------------------------- Utility Targets ------------------------------ #
# ============================================================================ #

add_custom_target(symlink_clangd
    COMMAND ${CMAKE_COMMAND} -E create_symlink
            "${CMAKE_BINARY_DIR}/compile_commands.json"
            "${CMAKE_SOURCE_DIR}/.ide-configs/compile_commands.json"
    COMMAND ${CMAKE_COMMAND} -E create_symlink
            "${CMAKE_BINARY_DIR}/compile_commands.json"
            "${CMAKE_SOURCE_DIR}/compile_commands.json"
    COMMENT "Creating symlinks for compile_commands.json"
    VERBATIM
)

add_custom_target(all_problems)
foreach(source_file ${PROBLEM_SOURCES})
    get_filename_component(exec_name ${source_file} NAME_WE)
    add_dependencies(all_problems ${exec_name})
endforeach()

add_custom_target(rebuild
    COMMAND ${CMAKE_COMMAND} --build . --target clean
    COMMAND ${CMAKE_COMMAND} --build . -j
    COMMENT "Clean rebuild of all targets"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# ============================================================================ #
# End of CP Round Targets Module.
