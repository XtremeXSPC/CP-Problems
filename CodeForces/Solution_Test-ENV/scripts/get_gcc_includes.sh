#!/bin/bash
# =========================================================================== #
# Enhanced helper script for the Makefile to robustly get GCC system include
# paths and detect competitive programming headers.
#
# PLATFORM-AWARE with improved error handling but original detection logic:
# - On macOS: Uses the reliable "build-and-verify" logic based on Homebrew.
# - On Linux: Parses the compiler's own verbose output, which is the
#             standard way to find include paths on a native GCC system.
#
# FEATURES: Competitive programming header detection with original robustness.
# =========================================================================== #

set -euo pipefail

# Configuration
readonly DEBUG="${DEBUG:-true}"

# Logging functions
log_debug() {
  [[ "$DEBUG" == "true" ]] && echo "DEBUG: $*" >&2
}

log_error() {
  echo "ERROR: $*" >&2
}

# =========================================================================== #
# ========================= Function Definitions ============================ #

# Function to check if competitive programming headers are needed
# Usage: check_competitive_headers <source_directory>
check_competitive_headers() {
  local src_dir="$1"
  local found_competitive=false

  log_debug "Checking for competitive headers in: $src_dir"

  # Check if source directory exists
  if [ ! -d "$src_dir" ]; then
    log_debug "Source directory does not exist: $src_dir"
    echo "no"
    return 0
  fi

  # Find all C++ source files (restored original approach)
  local cpp_files
  cpp_files=$(find "$src_dir" -name "*.cpp" -o -name "*.cc" -o -name "*.cxx" 2>/dev/null || true)

  log_debug "Found files: $cpp_files"

  # Check each file for competitive programming headers
  for file in $cpp_files; do
    if [ ! -f "$file" ]; then
      log_debug "Skipping non-existent file: $file"
      continue
    fi

    log_debug "Checking file: $file"

    # Read first 50 lines (where includes are typically found)
    local file_content
    file_content=$(head -50 "$file" 2>/dev/null || true)

    log_debug "First few lines of $file:"
    echo "$file_content" | head -30 >&2

    # Check for bits/stdc++.h (multiple patterns to be safe)
    if echo "$file_content" | grep -q "#include.*bits/stdc"; then
      log_debug "Found bits/stdc++.h pattern in: $file"
      echo "$file_content" | grep "#include.*bits/stdc" >&2
      found_competitive=true
      break
    fi

    # Check for ext/pb_ds
    if echo "$file_content" | grep -q "#include.*ext/pb_ds"; then
      log_debug "Found ext/pb_ds pattern in: $file"
      echo "$file_content" | grep "#include.*ext/pb_ds" >&2
      found_competitive=true
      break
    fi

    # Additional check with line-by-line approach for stubborn cases
    while IFS= read -r line; do
      # Skip empty lines
      [ -z "$line" ] && continue

      # Remove leading/trailing whitespace manually
      line=$(echo "$line" | sed 's/^[[:space:]]*//' | sed 's/[[:space:]]*$//')

      # Skip comment lines
      case "$line" in
      "//"*) continue ;;
      "/*"*) continue ;;
      esac

      # Check for include patterns
      case "$line" in
      "#include"*"bits/stdc"*)
        log_debug "Line-by-line found bits/stdc in: $file"
        log_debug "Line: $line"
        found_competitive=true
        break 2
        ;;
      "#include"*"ext/pb_ds"*)
        log_debug "Line-by-line found ext/pb_ds in: $file"
        log_debug "Line: $line"
        found_competitive=true
        break 2
        ;;
      esac
    done <"$file"

    if [ "$found_competitive" = true ]; then
      break
    fi
  done

  log_debug "Final result: found_competitive=$found_competitive"

  if [ "$found_competitive" = true ]; then
    echo "yes"
  else
    echo "no"
  fi
}

# Function to get GCC include paths (RESTORED original logic)
get_gcc_includes() {
  local compiler_path="$1"
  local platform
  platform=$(uname)

  if [[ "$platform" == "Darwin" ]]; then
    # -------------------- macOS Logic (Homebrew-based) --------------------- #

    # Get the Homebrew prefix for GCC.
    local brew_gcc_prefix
    brew_gcc_prefix=$(brew --prefix gcc 2>/dev/null || echo "")

    if [ -z "$brew_gcc_prefix" ]; then
      echo "ERROR: Homebrew GCC not found" >&2
      return 1
    fi

    # Get the major version number (e.g., "15").
    local gcc_version
    gcc_version=$("$compiler_path" -dumpversion | awk -F. '{print $1}')

    # Get the machine triple (e.g., aarch64-apple-darwin24).
    local gcc_machine
    gcc_machine=$("$compiler_path" -dumpmachine)

    # RESTORED: Original candidate paths from working configuration
    local candidate_paths=(
      "${brew_gcc_prefix}/include/c++/${gcc_version}"
      "${brew_gcc_prefix}/include/c++/${gcc_version}/${gcc_machine}"
      "${brew_gcc_prefix}/include/c++/${gcc_version}/backward"
      "${brew_gcc_prefix}/lib/gcc/current/gcc/${gcc_machine}/${gcc_version}/include"
      "${brew_gcc_prefix}/lib/gcc/current/gcc/${gcc_machine}/${gcc_version}/include-fixed"
      # Older Homebrew layouts might use these, included for compatibility
      "${brew_gcc_prefix}/lib/gcc/${gcc_version}/include"
      "${brew_gcc_prefix}/lib/gcc/${gcc_version}/include-fixed"
    )

    # Loop through the candidate paths, and if a directory exists, print the
    # corresponding -isystem flag.
    for path in "${candidate_paths[@]}"; do
      if [ -d "$path" ]; then
        echo "-isystem $path"
      fi
    done

  elif [[ "$platform" == "Linux" ]]; then
    # -------------------- Linux Logic (Compiler-based) --------------------- #
    # RESTORED: Original Linux logic that was working
    echo "" | "$compiler_path" -E -x c++ -v - 2>&1 |
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
  else
    echo "ERROR: Unsupported platform: $platform" >&2
    return 1
  fi
}

# =========================================================================== #
# ----------------------------- Main Logic ---------------------------------- #
# =========================================================================== #

# RESTORED: Original argument parsing
case "${1:-}" in
--check-competitive)
  # Check for competitive programming headers
  if [ -z "${2:-}" ]; then
    echo "Usage: $0 --check-competitive <source_directory>" >&2
    exit 1
  fi
  check_competitive_headers "$2"
  ;;
--get-includes)
  # Get GCC include paths
  if [ -z "${2:-}" ]; then
    echo "Usage: $0 --get-includes <compiler_path>" >&2
    exit 1
  fi
  get_gcc_includes "$2"
  ;;
--help | -h)
  # Show help
  cat <<EOF
Usage: $0 [OPTION] [ARGUMENTS]

Options:
    --check-competitive <src_dir>    Check if competitive programming headers
                                     are used in the source directory
                                     Returns: "yes" or "no"
    
    --get-includes <compiler_path>   Get GCC system include paths for the
                                     specified compiler
                                     Returns: -isystem flags
    
    --help, -h                       Show this help message

Legacy usage (for backward compatibility):
    $0 <compiler_path>        Same as --get-includes <compiler_path>

Examples:
    $0 --check-competitive ./src
    $0 --get-includes /usr/local/bin/g++-14
    $0 /usr/local/bin/g++-14         # Legacy format
EOF
  ;;
"")
  echo "Usage: $0 [--check-competitive <src_dir> | --get-includes <compiler_path> | --help]" >&2
  echo "Run '$0 --help' for more information." >&2
  exit 1
  ;;
*)
  # Legacy support: if first argument doesn't start with --, treat as compiler path
  if [[ "$1" != --* ]]; then
    get_gcc_includes "$1"
  else
    echo "Unknown option: $1" >&2
    echo "Run '$0 --help' for usage information." >&2
    exit 1
  fi
  ;;
esac

# =========================================================================== #
# End of script
