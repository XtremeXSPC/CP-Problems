#!/usr/bin/env python3
import sys
import re
from pathlib import Path
from collections import OrderedDict

# ------------------------------ CONFIGURATION ------------------------------- #

# Mapping from NEED_* macros to the list of template files to include.
NEED_MAPPING = OrderedDict(
    [
        ("NEED_CORE", ["Types.hpp", "Constants.hpp", "Macros.hpp", "Math.hpp"]),
        ("NEED_IO", ["IO.hpp"]),
        ("NEED_BIT_OPS", ["Bit_Ops.hpp"]),
        ("NEED_MOD_INT", ["Mod_Int.hpp"]),
        ("NEED_CONTAINERS", ["Containers.hpp"]),
    ]
)


def process_file_content(filepath, *, preserve_includes=False):
    """Process a single file and return its content."""
    if not filepath.is_file():
        return ""

    content_lines = []
    with open(filepath, "r", encoding="utf-8") as f:
        for line in f:
            stripped = line.strip()
            # Always skip pragma once.
            if stripped == "#pragma once":
                continue
            # Skip includes only if not preserving them.
            if not preserve_includes and stripped.startswith("#include"):
                continue
            content_lines.append(line)

    # Remove leading and trailing empty lines.
    while content_lines and not content_lines[0].strip():
        content_lines.pop(0)
    while content_lines and not content_lines[-1].strip():
        content_lines.pop()

    return "".join(content_lines)


# -------------------------------- MAIN LOGIC -------------------------------- #


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

    with open(source_file, "r", encoding="utf-8") as f:
        source_content = f.read()

    # 1. Find which templates are needed.
    need_macros_found = {
        m.group(1)
        for m in re.finditer(r"#define\s+(NEED_\w+)", source_content)
        if m.group(1) in NEED_MAPPING
    }

    # Always include NEED_CORE if any macro is found.
    if need_macros_found and "NEED_CORE" not in need_macros_found:
        need_macros_found.add("NEED_CORE")

    # 2. Build list of files to include (preserving order).
    files_to_include = []
    included_set = set()

    for macro, files in NEED_MAPPING.items():
        if macro in need_macros_found:
            for filename in files:
                if filename not in included_set:
                    files_to_include.append(templates_dir / filename)
                    included_set.add(filename)

    # 3. Process each template file.
    template_sections = []
    for filepath in files_to_include:
        content = process_file_content(filepath)
        if content:
            template_sections.append(content)

    # 4. Get preamble content, preserving its includes.
    preamble_path = templates_dir / "Preamble.hpp"
    preamble_content = process_file_content(preamble_path, preserve_includes=True)

    # 5. Process the source file and replace the base.hpp include.
    output_lines = []
    skip_need_defines = False

    with open(source_file, "r", encoding="utf-8") as f:
        for line in f:
            stripped = line.strip()

            # Skip NEED_* defines.
            if stripped.startswith("#define NEED_"):
                skip_need_defines = True
                continue

            # Replace the base.hpp include with our flattened content.
            if stripped == '#include "templates/base.hpp"':
                # Remove trailing blank lines from the source file's header section.
                while output_lines and not output_lines[-1].strip():
                    output_lines.pop()

                # Add a blank line for separation before the preamble.
                output_lines.append("\n")

                # Add preamble.
                output_lines.append(preamble_content)
                if preamble_content and not preamble_content.endswith("\n\n"):
                    output_lines.append("\n")

                # Add template sections with proper spacing.
                for i, section in enumerate(template_sections):
                    if i > 0:  # Add spacing between sections.
                        output_lines.append("\n")
                    output_lines.append(section)
                    if section and not section.endswith("\n"):
                        output_lines.append("\n")
            else:
                # Add back the line if it's not a NEED_* define.
                if (
                    skip_need_defines
                    and stripped
                    and not stripped.startswith("#define NEED_")
                ):
                    skip_need_defines = False
                    # Add a blank line before the next section if appropriate.
                    if output_lines and output_lines[-1].strip():
                        output_lines.append("\n")

                output_lines.append(line)

    # Print the final output.
    print("".join(output_lines), end="")


if __name__ == "__main__":
    main()
