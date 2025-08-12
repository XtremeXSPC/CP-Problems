#!/bin/bash
# =========================================================================== #
# Helper script for the Makefile to get GCC system include paths.
#
# It takes the path to a g++ compiler as an argument and prints the necessary
# -isystem flags for clangd and the build process.
# =========================================================================== #

set -euo pipefail

# Check if a compiler path was provided as an argument.
if [ -z "$1" ]; then
    echo "Usage: $0 /path/to/your/g++" >&2
    exit 1
fi

COMPILER_PATH="$1"

# ------------ Logic ported directly from the working CMakeLists ------------ #

# Get the Homebrew prefix for GCC.
BREW_GCC_PREFIX=$(brew --prefix gcc)

# Get the major version number (e.g., "15").
GCC_VERSION=$("$COMPILER_PATH" -dumpversion | awk -F. '{print $1}')

# Get the machine triple (e.g., aarch64-apple-darwin24).
GCC_MACHINE=$("$COMPILER_PATH" -dumpmachine)

# This array contains all the candidate paths from the working CMake configuration.
# These are the specific paths needed for GCC's libstdc++, without any
# conflicting system headers.
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

# =========================================================================== #
