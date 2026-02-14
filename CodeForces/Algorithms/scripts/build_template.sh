#!/bin/bash
# Build system for modular template.
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEMPLATE_DIR="$SCRIPT_DIR/../templates"

# Function to generate the complete template.
generate_template() {
    local output_file="${1:-template.hpp}"
    [[ -d "$TEMPLATE_DIR" ]] || { echo "Template directory not found: $TEMPLATE_DIR" >&2; return 1; }

    cat >"$output_file" <<'EOF'
//===----------------------------------------------------------------------===//
//===---------- Auto-generated Competitive Programming Template -----------===//
//===----------------------------------------------------------------------===//

#ifndef COMPETITIVE_TEMPLATE_HPP
#define COMPETITIVE_TEMPLATE_HPP

EOF

    # Combine modules in order.
    echo '#line 1 "Base.hpp"' >>"$output_file"
    cat "$TEMPLATE_DIR/Base.hpp" >>"$output_file"

    echo '#line 1 "Types.hpp"' >>"$output_file"
    cat "$TEMPLATE_DIR/Types.hpp" >>"$output_file"

    echo '#line 1 "Constants.hpp"' >>"$output_file"
    cat "$TEMPLATE_DIR/Constants.hpp" >>"$output_file"

    echo '#line 1 "Macros.hpp"' >>"$output_file"
    cat "$TEMPLATE_DIR/Macros.hpp" >>"$output_file"

    echo '#line 1 "IO.hpp"' >>"$output_file"
    cat "$TEMPLATE_DIR/IO.hpp" >>"$output_file"

    echo '#line 1 "Bit_Ops.hpp"' >>"$output_file"
    cat "$TEMPLATE_DIR/Bit_Ops.hpp" >>"$output_file"

    echo '#line 1 "Math.hpp"' >>"$output_file"
    cat "$TEMPLATE_DIR/Math.hpp" >>"$output_file"

    echo '#line 1 "Containers.hpp"' >>"$output_file"
    cat "$TEMPLATE_DIR/Containers.hpp" >>"$output_file"

    echo '#line 1 "Mod_Int.hpp"' >>"$output_file"
    cat "$TEMPLATE_DIR/Mod_Int.hpp" >>"$output_file"

    echo '' >>"$output_file"
    echo '#endif // COMPETITIVE_TEMPLATE_HPP' >>"$output_file"

    echo "Template generated: $output_file"
}

# Execute if called directly.
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    generate_template "$1"
fi
