#!/usr/bin/env bash

set -euo pipefail

if [[ $# -ne 1 ]]; then
  echo "Usage: $0 <problem_name>" >&2
  exit 1
fi

problem="$1"
script_dir="$(cd "$(dirname "$0")" && pwd)"
root_dir="$(cd "$script_dir/.." && pwd)"

src_file="$root_dir/src/${problem}.py"
template_file="$root_dir/templates/problem_template.py"
input_file="$root_dir/input_cases/${problem}_01.in"
expected_file="$root_dir/expected_output/${problem}_01.exp"

mkdir -p \
  "$root_dir/src" \
  "$root_dir/input_cases" \
  "$root_dir/expected_output" \
  "$root_dir/output_cases" \
  "$root_dir/submissions"

if [[ -e "$src_file" ]]; then
  echo "Source already exists: $src_file" >&2
  exit 1
fi

cp "$template_file" "$src_file"
touch "$input_file" "$expected_file"

echo "Created:"
echo "  $src_file"
echo "  $input_file"
echo "  $expected_file"
echo ""
echo "Next:"
echo "  1) Edit $src_file"
echo "  2) Fill $input_file and $expected_file"
echo "  3) Run: make test PROBLEM=$problem"

