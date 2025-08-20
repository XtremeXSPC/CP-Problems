# =========================================================================== #
# --------------------- CMake Toolchain File for Clang ---------------------- #
# =========================================================================== #
#
# Description:
#   This toolchain file intelligently selects the appropriate compiler based
#   on the build type (CMAKE_BUILD_TYPE), preserving the detailed search and
#   verification logic from the original separate files.
#
#   - For "Sanitize" builds:
#     It enforces Clang using its specific detailed search logic, which is
#     necessary for proper sanitizer support, especially on macOS.
#
#   - For all other builds (e.g., "Debug", "Release"):
#     It enforces GCC using its specific detailed search logic to ensure
#     compatibility with non-standard headers like <bits/stdc++.h>.
#
# Usage:
#   cmake -DCMAKE_TOOLCHAIN_FILE=path/to/this/file.cmake -DCMAKE_BUILD_TYPE=<Type>
#
# ============================================================================ #

# Platform detection for appropriate compiler search strategy
if(APPLE)
    set(PLATFORM_NAME "macOS")
elseif(UNIX AND NOT APPLE)
    set(PLATFORM_NAME "Linux")
else()
    set(PLATFORM_NAME "Other Unix")
endif()

message(STATUS "Detected platform: ${PLATFORM_NAME}")

# Define ANSI color codes for pretty printing in supported terminals
if(CMAKE_COLOR_MAKEFILE)
    string(ASCII 27 ANSI_ESCAPE_SEQUENCE)
    set(ANSI_COLOR_RED    "${ANSI_ESCAPE_SEQUENCE}[31m")
    set(ANSI_COLOR_GREEN  "${ANSI_ESCAPE_SEQUENCE}[32m")
    set(ANSI_COLOR_RESET  "${ANSI_ESCAPE_SEQUENCE}[0m")
endif()

# ------------------------- Compiler Selection Logic ------------------------- #

if(CMAKE_BUILD_TYPE STREQUAL "Sanitize")
    # --------------- CLANG FOR SANITIZER BUILDS (Full Logic) ---------------- #

    message(STATUS "Build type is 'Sanitize'. Using Clang Toolchain Logic.")

    # Search for Clang compiler
    if(APPLE)
        # On macOS, prefer LLVM clang (has best sanitizer support)
        set(COMPILER_SEARCH_NAMES
            clang++ clang++-20 clang++-19 clang++-18 clang++-17 clang++-16
        )
        set(COMPILER_SEARCH_PATHS
            /opt/homebrew/opt/llvm/bin  # Homebrew LLVM
            /usr/local/opt/llvm/bin     # Homebrew LLVM (Intel)
            /opt/local/bin              # MacPorts
            /usr/bin                    # System (AppleClang)
        )
    else()
        # Linux
        set(COMPILER_SEARCH_NAMES
            clang++-20 clang++-19 clang++-18 clang++-17 clang++-16 clang++-15 clang++
        )
        set(COMPILER_SEARCH_PATHS
            /usr/bin /usr/local/bin /opt/llvm/bin $ENV{HOME}/.local/bin
        )
    endif()

    # Find Clang executable
    find_program(CLANG_EXECUTABLE
        NAMES ${COMPILER_SEARCH_NAMES} PATHS ${COMPILER_SEARCH_PATHS}
        DOC "Path to the clang++ executable" NO_DEFAULT_PATH
    )
    if(NOT CLANG_EXECUTABLE)
        find_program(CLANG_EXECUTABLE NAMES ${COMPILER_SEARCH_NAMES} DOC "Path to the clang++ executable")
    endif()

    # Error if Clang not found
    if(NOT CLANG_EXECUTABLE)
        set(ERROR_MESSAGE
            "\n"
            "//===----------------------------------------------------------------------===//\n"
            "                          CLANG COMPILER NOT FOUND!                             \n"
            "//===----------------------------------------------------------------------===//\n"
            "\nClang is required for sanitizer builds on this platform.\n"
            "\nInstallation instructions:\n"
        )
        if(APPLE)
            string(APPEND ERROR_MESSAGE
                "  macOS:\n"
                "    Xcode Command Line Tools: xcode-select --install\n"
                "    Homebrew LLVM (recommended): brew install llvm\n"
            )
        else()
            string(APPEND ERROR_MESSAGE
                "  Linux:\n"
                "    Debian/Ubuntu: sudo apt install clang\n"
                "    Fedora/RHEL:   sudo dnf install clang\n"
                "    Arch:          sudo pacman -S clang\n"
            )
        endif()
        string(APPEND ERROR_MESSAGE
            "\nAfter installation, please try running the command again.\n"
            "//===----------------------------------------------------------------------===//\n"
        )
        message(FATAL_ERROR "${ERROR_MESSAGE}")
    endif()

    # Verify it's actually Clang and get version
    execute_process(
        COMMAND ${CLANG_EXECUTABLE} --version
        OUTPUT_VARIABLE CLANG_VERSION_OUTPUT RESULT_VARIABLE CLANG_VERSION_RESULT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(NOT CLANG_VERSION_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to execute ${CLANG_EXECUTABLE} --version.")
    endif()

    if(CLANG_VERSION_OUTPUT MATCHES "Apple clang version ([0-9]+\\.[0-9]+)")
        set(CLANG_VERSION "${CMAKE_MATCH_1}")
        set(IS_APPLE_CLANG TRUE)
    elseif(CLANG_VERSION_OUTPUT MATCHES "clang version ([0-9]+\\.[0-9]+)")
        set(CLANG_VERSION "${CMAKE_MATCH_1}")
        set(IS_APPLE_CLANG FALSE)
    else()
        set(CLANG_VERSION "unknown")
    endif()

    # Find corresponding C compiler
    get_filename_component(CLANG_DIR ${CLANG_EXECUTABLE} DIRECTORY)
    string(REPLACE "clang++" "clang" C_COMPILER_NAME ${CLANG_EXECUTABLE})
    find_program(C_COMPILER_PATH NAMES ${C_COMPILER_NAME} HINTS ${CLANG_DIR} NO_DEFAULT_PATH)
    if(NOT C_COMPILER_PATH)
        message(WARNING "Could not find matching Clang C compiler. Using clang++ for both.")
        set(C_COMPILER_PATH ${CLANG_EXECUTABLE})
    endif()

    # Set the compilers
    set(CMAKE_C_COMPILER   ${C_COMPILER_PATH} CACHE PATH "C compiler"   FORCE)
    set(CMAKE_CXX_COMPILER ${CLANG_EXECUTABLE} CACHE PATH "C++ compiler" FORCE)
    set(CMAKE_C_COMPILER_ID "Clang")
    set(CMAKE_CXX_COMPILER_ID "Clang")

    # Check for sanitizer support
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-fsanitize=address" HAS_ASAN)
    check_cxx_compiler_flag("-fsanitize=undefined" HAS_UBSAN)

    # Final success message
    message(STATUS "")
    message(STATUS "//===----------------------------------------------------------------------===//")
    message(STATUS "                   Clang Toolchain Successfully Configured                     ")
    message(STATUS "//===----------------------------------------------------------------------===//")
    message(STATUS "  C++ compiler : ${CMAKE_CXX_COMPILER}")
    message(STATUS "  Clang version: ${CLANG_VERSION}")
    if(IS_APPLE_CLANG)
        message(STATUS "  Type         : Apple Clang (Xcode)")
    else()
        message(STATUS "  Type         : LLVM Clang")
    endif()
    message(STATUS "  C compiler   : ${CMAKE_C_COMPILER}")
    message(STATUS "  Build type   : ${CMAKE_BUILD_TYPE}")
    message(STATUS "")
    message(STATUS "  Sanitizers:")
    if(HAS_ASAN)
        message(STATUS "    AddressSanitizer  : ${ANSI_COLOR_GREEN}Available${ANSI_COLOR_RESET}")
    else()
        message(STATUS "    AddressSanitizer  : ${ANSI_COLOR_RED}Not Available${ANSI_COLOR_RESET}")
    endif()
    if(HAS_UBSAN)
        message(STATUS "    UBSanitizer       : ${ANSI_COLOR_GREEN}Available${ANSI_COLOR_RESET}")
    else()
        message(STATUS "    UBSanitizer       : ${ANSI_COLOR_RED}Not Available${ANSI_COLOR_RESET}")
    endif()
    message(STATUS "//===----------------------------------------------------------------------===//")

else()
    # ---------------- GCC FOR STANDARD BUILDS (Full Logic) ----------------- #

    message(STATUS "Build type is '${CMAKE_BUILD_TYPE}'. Using GCC Toolchain Logic.")

    # Platform-specific compiler search paths and version preferences
    if(APPLE)
        set(COMPILER_SEARCH_NAMES g++-15 g++-14 g++-13 g++-12 g++-11 g++)
        set(COMPILER_SEARCH_PATHS /opt/homebrew/bin /usr/local/bin /opt/local/bin)
    else()
        set(COMPILER_SEARCH_NAMES g++-15 g++-14 g++-13 g++-12 g++-11 g++-10 g++-9 g++)
        set(COMPILER_SEARCH_PATHS /usr/bin /usr/local/bin /opt/gcc/bin /snap/bin)
    endif()

    # Find the latest available GCC/G++ executable
    find_program(GCC_EXECUTABLE
        NAMES ${COMPILER_SEARCH_NAMES} PATHS ${COMPILER_SEARCH_PATHS}
        DOC "Path to the g++ executable" NO_DEFAULT_PATH
    )
    if(NOT GCC_EXECUTABLE)
        find_program(GCC_EXECUTABLE NAMES ${COMPILER_SEARCH_NAMES} DOC "Path to the g++ executable")
    endif()

    if(NOT GCC_EXECUTABLE)
        set(ERROR_MESSAGE
            "\n"
            "//===----------------------------------------------------------------------===//\n"
            "                           GCC COMPILER NOT FOUND!                              \n"
            "//===----------------------------------------------------------------------===//\n"
            "\nGCC is required for standard builds (e.g., for <bits/stdc++.h>).\n"
            "\nInstallation instructions:\n"
        )
        if(APPLE)
            string(APPEND ERROR_MESSAGE "  macOS: brew install gcc\n")
        else()
            string(APPEND ERROR_MESSAGE
                "  Linux:\n"
                "    Debian/Ubuntu: sudo apt install g++\n"
                "    Fedora/RHEL:   sudo dnf install gcc-c++\n"
                "    Arch:          sudo pacman -S gcc\n"
            )
        endif()
        string(APPEND ERROR_MESSAGE
            "\nAfter installation, please try running the command again.\n"
            "//===----------------------------------------------------------------------===//\n"
        )
        message(FATAL_ERROR "${ERROR_MESSAGE}")
    endif()

    # Verify the found compiler is actually GCC and get version info
    execute_process(
        COMMAND ${GCC_EXECUTABLE} -dumpversion
        OUTPUT_VARIABLE GCC_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    string(REGEX MATCH "^([0-9]+)" GCC_MAJOR_VERSION "${GCC_VERSION}")
    if(GCC_MAJOR_VERSION AND GCC_MAJOR_VERSION LESS 9)
        message(WARNING "Found GCC version ${GCC_VERSION} is quite old (< 9.0).")
    endif()

    # Find the corresponding C compiler (gcc)
    get_filename_component(GCC_DIR ${GCC_EXECUTABLE} DIRECTORY)
    string(REPLACE "g++" "gcc" C_COMPILER_NAME_BASE ${GCC_EXECUTABLE})
    find_program(C_COMPILER_PATH NAMES ${C_COMPILER_NAME_BASE} HINTS ${GCC_DIR} NO_DEFAULT_PATH)
    if(NOT C_COMPILER_PATH)
        find_program(C_COMPILER_PATH NAMES gcc HINTS ${GCC_DIR})
    endif()
    if(NOT C_COMPILER_PATH)
        message(WARNING "Matching C compiler 'gcc' not found. Using g++ for C files as well.")
        set(C_COMPILER_PATH ${GCC_EXECUTABLE})
    endif()

    # Set GCC as the compiler
    set(CMAKE_C_COMPILER   ${C_COMPILER_PATH}   CACHE PATH "C compiler"   FORCE)
    set(CMAKE_CXX_COMPILER ${GCC_EXECUTABLE}  CACHE PATH "C++ compiler" FORCE)
    set(CMAKE_C_COMPILER_ID "GNU")
    set(CMAKE_CXX_COMPILER_ID "GNU")

    # Final success message
    message(STATUS "")
    message(STATUS "//===----------------------------------------------------------------------===//")
    message(STATUS "                    GCC Toolchain Successfully Configured                      ")
    message(STATUS "//===----------------------------------------------------------------------===//")
    message(STATUS "  C++ compiler: ${CMAKE_CXX_COMPILER}")
    message(STATUS "  GCC version:  ${GCC_VERSION}")
    message(STATUS "  C compiler:   ${CMAKE_C_COMPILER}")
    message(STATUS "  Build type:   ${CMAKE_BUILD_TYPE}")
    message(STATUS "//===----------------------------------------------------------------------===//")
    message(STATUS "")

endif()

# -------------------- Common Settings for Both Compilers -------------------- #

# Ensure the compilers support the required standards
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the build type to Debug by default if not specified by the user.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Default build type" FORCE)
    message(STATUS "Setting build type to 'Debug' (default).")
endif()

message(STATUS "Final build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Toolchain configuration complete.")

# ============================================================================ #
