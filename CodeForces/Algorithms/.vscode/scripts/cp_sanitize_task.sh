#!/usr/bin/env sh
# ============================================================================ #
# VS Code helper for sanitizer-driven cpptools tasks.
#
# Enforces platform/toolchain compatibility:
#   - macOS: sanitizer tasks only with Clang
#   - Linux: sanitizer tasks with Clang or GCC
# ============================================================================ #

set -eu

usage() {
  cat <<'USAGE'
Usage:
  cp_sanitize_task.sh conf [compiler]
  cp_sanitize_task.sh build <target> [compiler]
  cp_sanitize_task.sh go <target> [compiler] [input_file]
  cp_sanitize_task.sh judge <target> [compiler]
  cp_sanitize_task.sh stress <target> [compiler] [iterations]

compiler:
  auto (default), clang, gcc
USAGE
}

resolve_compiler() {
  requested="$1"
  os_name="$2"

  case "$requested" in
    ""|auto)
      # Prefer Clang for sanitizer workflows by default.
      echo "clang"
      ;;
    clang|gcc)
      echo "$requested"
      ;;
    *)
      echo "Error: Unsupported sanitizer compiler '$requested'. Use auto|clang|gcc." >&2
      exit 2
      ;;
  esac

  # Keep shellcheck quiet for functions in POSIX sh.
  return 0
}

require_target() {
  action_name="$1"
  target_name="$2"
  if [ -z "$target_name" ]; then
    echo "Error: Missing target for action '$action_name'." >&2
    usage
    exit 2
  fi
}

if [ "$#" -lt 1 ]; then
  usage
  exit 2
fi

action="$1"
shift

os_name="$(uname -s 2>/dev/null || echo Unknown)"

case "$action" in
  conf)
    requested_compiler="${1:-auto}"
    resolved_compiler="$(resolve_compiler "$requested_compiler" "$os_name")"
    ;;
  build|go|judge|stress)
    target="${1:-}"
    requested_compiler="${2:-auto}"
    resolved_compiler="$(resolve_compiler "$requested_compiler" "$os_name")"
    ;;
  *)
    echo "Error: Unsupported action '$action'." >&2
    usage
    exit 2
    ;;
esac

if [ "$os_name" = "Darwin" ] && [ "$resolved_compiler" = "gcc" ]; then
  echo "Error: GCC sanitizer tasks are not supported on macOS. Use Clang instead." >&2
  exit 2
fi

# Runtime sanitizer defaults; callers can still override from environment.
export ASAN_OPTIONS="${ASAN_OPTIONS:-detect_leaks=1:check_initialization_order=1:strict_init_order=1:detect_stack_use_after_return=1:halt_on_error=1}"
export UBSAN_OPTIONS="${UBSAN_OPTIONS:-print_stacktrace=1:halt_on_error=1}"
export LSAN_OPTIONS="${LSAN_OPTIONS:-verbosity=1:log_threads=1}"

cpptools conf Sanitize "$resolved_compiler"

case "$action" in
  conf)
    ;;
  build)
    require_target "$action" "$target"
    cpptools build "$target"
    ;;
  go)
    require_target "$action" "$target"
    input_file="${3:-}"
    if [ -n "$input_file" ]; then
      cpptools go "$target" "$input_file"
    else
      cpptools go "$target"
    fi
    ;;
  judge)
    require_target "$action" "$target"
    cpptools judge "$target"
    ;;
  stress)
    require_target "$action" "$target"
    iterations="${3:-200}"
    cpptools stress "$target" "$iterations"
    ;;
esac

# ============================================================================ #
# End of cp_sanitize_task.sh
