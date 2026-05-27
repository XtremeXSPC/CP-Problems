#!/usr/bin/env zsh
# Wrapper script invoked by CppToolsRunner.
#
# Positional arguments (passed by the Python caller):
#   $1  — absolute path to the user's cpp-tools shell file (e.g. competitive.sh).
#   $2  — name of the allowlisted cpp-tools function to invoke.
#   $3+ — arguments forwarded verbatim to that function.
#
# The allowlist marker below (in the case block) is substituted by
# ``runner._render_wrapper`` with one ``name) ;;`` arm per entry in
# ``ALLOWED_FUNCTIONS`` (constants.py). Editing the case arms by hand here
# is not supported — change the Python allowlist instead and the wrapper
# will pick the new entries up on the next invocation.

set -eo pipefail
export CP_QUIET_LOAD="${CP_QUIET_LOAD:-1}"

source "$1"
shift

func="$1"
shift

case "$func" in
{{ALLOWED_FUNCTIONS_CASES}}
  *)
    print -u2 -- "Unsupported cpp-tools function: $func"
    exit 64
    ;;
esac

if [[ "$func" == "cppdeepclean" && "${CP_AUTO_CONFIRM_DEEPCLEAN:-0}" == "1" ]]; then
  printf 'y\n' | cppdeepclean "$@"
else
  "$func" "$@"
fi
