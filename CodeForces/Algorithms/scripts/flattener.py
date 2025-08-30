#!/usr/bin/env python3
import sys
import re
from pathlib import Path
from collections import OrderedDict

NEED_MAPPING = OrderedDict([
    ("NEED_CORE", ["Types.hpp", "Constants.hpp", "Macros.hpp", "Math.hpp"]),
    ("NEED_IO", ["IO.hpp"]),
    ("NEED_BIT_OPS", ["Bit_Ops.hpp"]),
    ("NEED_MOD_INT", ["Mod_Int.hpp"]),
    ("NEED_CONTAINERS", ["Containers.hpp"]),
])

def main():
    if len(sys.argv) != 2:
        sys.stderr.write(f"Usage: {sys.argv[0]} <source_file.cpp>\n")
        sys.exit(1)

    source_file = Path(sys.argv[1])
    if not source_file.is_file():
        sys.stderr.write(f"Error: File not found: {source_file}\n")
        sys.exit(1)

    script_dir = Path(__file__).parent.resolve()
    project_root = script_dir.parent
    templates_dir = project_root / "templates"
    
    with open(source_file, 'r', encoding='utf-8') as f:
        source_content = f.read()
    
    preamble_path = templates_dir / "Preamble.hpp"
    with open(preamble_path, 'r', encoding='utf-8') as f:
        preamble_content = f.read()

    output = []

    # 1. Header comment from source file
    header_comment_match = re.search(r"^(//===.*?^//===.*?===//)", source_content, re.MULTILINE | re.DOTALL)
    if header_comment_match:
        output.append(header_comment_match.group(1).strip())
    output.append("\n\n")

    # 2. The entire content of Preamble.hpp, without its own #pragma once
    preamble_lines = preamble_content.splitlines(True)
    for line in preamble_lines:
        if "#pragma once" not in line:
            output.append(line)
    output.append("\n\n")

    # 3. Needed template files
    need_macros_found = { m.group(1) for m in re.finditer(r"#define\s+(NEED_\w+)", source_content) if m.group(1) in NEED_MAPPING }
    if need_macros_found and "NEED_CORE" not in need_macros_found:
        need_macros_found.add("NEED_CORE")

    files_to_include = []
    included_set = set()
    for macro, files in NEED_MAPPING.items():
        if macro in need_macros_found:
            for filename in files:
                if filename not in included_set:
                    files_to_include.append(templates_dir / filename)
                    included_set.add(filename)
    
    for filepath in files_to_include:
        if not filepath.is_file(): continue
        with open(filepath, 'r', encoding='utf-8') as f:
            for line in f:
                stripped = line.strip()
                if stripped == "#pragma once" or stripped.startswith('#include'):
                    continue
                output.append(line)
        output.append("\n")

    # 4. The actual code from the source file
    main_code_match = re.search(r'#include "templates/base.hpp"\s*\n(.*)', source_content, re.DOTALL)
    if main_code_match:
        main_code = main_code_match.group(1)
        main_code = re.sub(r"#define\s+NEED_\w+\s*\n?", "", main_code)
        output.append(main_code.strip())

    print("".join(output)) 

if __name__ == "__main__":
    main()