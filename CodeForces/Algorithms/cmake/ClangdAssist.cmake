# ------------------- Compiler Include Path Auto-Detection ------------------- #
# Shared logic for compiler system include path detection.

function(cp_detect_compiler_system_includes OUTPUT_VARIABLE)
  # Check cache first.
  if(DEFINED CACHE{COMPILER_SYSTEM_INCLUDES_CACHED})
    set(${OUTPUT_VARIABLE} "${COMPILER_SYSTEM_INCLUDES_CACHED}" PARENT_SCOPE)
    message(STATUS "Clangd Assist: Using cached compiler include paths")
    return()
  endif()

  set(DETECTED_PATHS "")

  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    if(APPLE)
      message(STATUS "Clangd Assist: Using selective path detection for GCC on macOS.")

      # Try to get exact brew prefix first, fallback to common locations.
      execute_process(
        COMMAND brew --prefix
        OUTPUT_VARIABLE BREW_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
        RESULT_VARIABLE BREW_RESULT
      )
      if(NOT BREW_RESULT EQUAL 0 OR NOT BREW_PREFIX)
        foreach(prefix "/opt/homebrew" "/usr/local")
          if(IS_DIRECTORY "${prefix}")
            set(BREW_PREFIX "${prefix}")
            break()
          endif()
        endforeach()
      endif()

      if(BREW_PREFIX)
        # Get GCC full version, fall back if needed.
        execute_process(
          COMMAND ${CMAKE_CXX_COMPILER} -dumpfullversion
          OUTPUT_VARIABLE GCC_FULL_VERSION
          OUTPUT_STRIP_TRAILING_WHITESPACE
          ERROR_QUIET
        )

        # Derive major version number (e.g. "15").
        string(REGEX MATCH "^[0-9]+" GCC_VERSION "${GCC_FULL_VERSION}")
        if(NOT GCC_VERSION)
          set(GCC_VERSION ${GCC_FULL_VERSION})
        endif()

        # Machine triple (e.g. aarch64-apple-darwin23).
        execute_process(
          COMMAND ${CMAKE_CXX_COMPILER} -dumpmachine
          OUTPUT_VARIABLE GCC_MACHINE
          OUTPUT_STRIP_TRAILING_WHITESPACE
          ERROR_QUIET
        )

        set(GCC_INCLUDE_PATHS
          "${BREW_PREFIX}/opt/gcc/include/c++/${GCC_VERSION}"
          "${BREW_PREFIX}/include/c++/${GCC_VERSION}"
          "${BREW_PREFIX}/opt/gcc/include/c++/${GCC_VERSION}/${GCC_MACHINE}"
          "${BREW_PREFIX}/include/c++/${GCC_VERSION}/${GCC_MACHINE}"
          "${BREW_PREFIX}/opt/gcc/include/c++/${GCC_VERSION}/backward"
          "${BREW_PREFIX}/include/c++/${GCC_VERSION}/backward"
          "${BREW_PREFIX}/opt/gcc/lib/gcc/${GCC_VERSION}/include"
          "${BREW_PREFIX}/lib/gcc/${GCC_VERSION}/include"
          "${BREW_PREFIX}/opt/gcc/lib/gcc/${GCC_VERSION}/include-fixed"
          "${BREW_PREFIX}/lib/gcc/${GCC_VERSION}/include-fixed"
        )

        foreach(p IN LISTS GCC_INCLUDE_PATHS)
          if(IS_DIRECTORY "${p}")
            list(APPEND DETECTED_PATHS "${p}")
          endif()
        endforeach()
      endif()
    else()
      # Generic GCC path detection for Linux.
      execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -E -x c++ -v /dev/null
        OUTPUT_VARIABLE GCC_VERBOSE_OUTPUT
        ERROR_VARIABLE GCC_VERBOSE_OUTPUT
        RESULT_VARIABLE EXIT_CODE
        TIMEOUT 5
      )

      if(EXIT_CODE EQUAL 0)
        string(REPLACE "\n" ";" OUTPUT_LINES "${GCC_VERBOSE_OUTPUT}")
        set(IS_PARSING_INCLUDES FALSE)

        foreach(line ${OUTPUT_LINES})
          if(line MATCHES "^#include <...> search starts here:")
            set(IS_PARSING_INCLUDES TRUE)
          elseif(line MATCHES "^End of search list.")
            break()
          elseif(IS_PARSING_INCLUDES)
            string(STRIP "${line}" path)
            if(IS_DIRECTORY "${path}")
              list(APPEND DETECTED_PATHS "${path}")
            endif()
          endif()
        endforeach()
      endif()
    endif()
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
    # Clang-specific path detection.
    message(STATUS "Clangd Assist: Detecting Clang system includes.")

    execute_process(
      COMMAND ${CMAKE_CXX_COMPILER} -E -x c++ -v /dev/null
      OUTPUT_VARIABLE CLANG_VERBOSE_OUTPUT
      ERROR_VARIABLE CLANG_VERBOSE_OUTPUT
      RESULT_VARIABLE EXIT_CODE
      TIMEOUT 5
    )

    if(EXIT_CODE EQUAL 0)
      string(REPLACE "\n" ";" OUTPUT_LINES "${CLANG_VERBOSE_OUTPUT}")
      set(IS_PARSING_INCLUDES FALSE)

      foreach(line ${OUTPUT_LINES})
        if(line MATCHES "^#include.*search starts here:")
          set(IS_PARSING_INCLUDES TRUE)
        elseif(line MATCHES "^End of search list.")
          break()
        elseif(IS_PARSING_INCLUDES)
          string(STRIP "${line}" path)
          # Exclude framework paths on macOS.
          if(IS_DIRECTORY "${path}" AND NOT path MATCHES "\\(framework directory\\)")
            list(APPEND DETECTED_PATHS "${path}")
          endif()
        endif()
      endforeach()
    endif()
  endif()

  # Keep only directories that actually exist.
  if(DETECTED_PATHS)
    list(REMOVE_DUPLICATES DETECTED_PATHS)
    set(COMPILER_SYSTEM_INCLUDES_CACHED "${DETECTED_PATHS}" CACHE INTERNAL "Cached compiler include paths")
    set(${OUTPUT_VARIABLE} "${DETECTED_PATHS}" PARENT_SCOPE)
    string(REPLACE ";" "\n   " PATHS_NL "${DETECTED_PATHS}")
    message(STATUS "Clangd Assist: Found compiler include paths:\n   ${PATHS_NL}")
  else()
    message(WARNING "Clangd Assist: Could not auto-detect compiler system include paths.")
    set(${OUTPUT_VARIABLE} "" PARENT_SCOPE)
  endif()
endfunction()

