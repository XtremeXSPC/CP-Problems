#!/bin/bash
# =========================================================================== #
# Helper script for the Makefile to robustly get GCC system include paths.
#
# It is now PLATFORM-AWARE:
# - On macOS: It uses the reliable "build-and-verify" logic based on Homebrew.
# - On Linux: It parses the compiler's own verbose output, which is the
#             standard way to find include paths on a native GCC system.
# =========================================================================== #

set -euo pipefail

# Check if a compiler path was provided as an argument.
if [ -z "$1" ]; then
    echo "Usage: $0 /path/to/your/g++" >&2
    exit 1
fi

COMPILER_PATH="$1"
PLATFORM=$(uname)

# Use platform-specific logic to find the include paths

if [[ "$PLATFORM" == "Darwin" ]]; then
    # -------------------- macOS Logic (Homebrew-based) --------------------- #
    
    # Get the Homebrew prefix for GCC.
    BREW_GCC_PREFIX=$(brew --prefix gcc)

    # Get the major version number (e.g., "15").
    GCC_VERSION=$("$COMPILER_PATH" -dumpversion | awk -F. '{print $1}')

    # Get the machine triple (e.g., aarch64-apple-darwin24).
    GCC_MACHINE=$("$COMPILER_PATH" -dumpmachine)

    # This array contains all the candidate paths from the working CMake configuration.
    CANDIDATE_PATHS=(
        "${BREW_GCC_PREFIX}/include/c++/${GCC_VERSION}"
        "${BREW_GCC_PREFIX}/include/c++/${GCC_VERSION}/${GCC_MACHINE}"
        "${BREW_GCC_PREFIX}/include/c++/${GCC_VERSION}/backward"
        "${BREW_GCC_PREFIX}/lib/gcc/current/gcc/${GCC_MACHINE}/${GCC_VERSION}/include"
        "${BREW_GCC_PREFIX}/lib/gcc/current/gcc/${GCC_MACHINE}/${GCC_VERSION}/include-fixed"
        # Older Homebrew layouts might use these, included for compatibility
        "${BREW_GCC_PREFIX}/lib/gcc/${GCC_VERSION}/include"
        "${BREW_GCC_PREFIX}/lib/gcc/${GCC_VERSION}/include-fixed"
    )

    # Loop through the candidate paths, and if a directory exists, print the
    # corresponding -isystem flag.
    for path in "${CANDIDATE_PATHS[@]}"; do
        if [ -d "$path" ]; then
            echo "-isystem $path"
        fi
    done

elif [[ "$PLATFORM" == "Linux" ]]; then
    # -------------------- Linux Logic (Compiler-based) --------------------- #
    # On Linux, parsing the compiler's own output is the most reliable way.
    # We do not need to filter out conflicting SDKs as on macOS.
    echo "" | "$COMPILER_PATH" -E -x c++ -v - 2>&1 | \
    awk '
        BEGIN { in_section = 0; }
        /#include <...> search starts here/ { in_section = 1; next; }
        /End of search list/ { in_section = 0; }
        {
            if (in_section) {
                # Trim leading/trailing whitespace and print the -isystem flag.
                sub(/^[[:space:]]+/, "");
                sub(/[[:space:]]+$/, "");
                if (length($0) > 0) {
                    print "-isystem " $0;
                }
            }
        }
    '
fi

# =========================================================================== #
