# ============================================================================ #
# -------------------- CP ALGORITHMS BUILD CONFIGURATION --------------------- #
# ============================================================================ #
#
# Description:
#   Workspace-level build configuration for the CP Algorithms library.
#   Generates compile_commands.json consumed by clangd and other language
#   servers, providing accurate IDE diagnostics across all public headers,
#   module verification sources, and template scaffolds.
#
#   This file is NOT used for contest round builds. Those are driven by
#   cmake/CPRoundBootstrap.cmake, included from each round's CMakeLists.txt.
#
# Probe Families (all EXCLUDE_FROM_ALL — nothing built by default):
#   Header Probes   - One translation unit per public header; ensures clangd
#                     can resolve every include path and symbol.
#   Verify Probes   - Builds verify/modules/*.cpp smoke tests that exercise
#                     module APIs without external test frameworks.
#   Scaffold Probes - Registers templates/cpp/*.cpp contest template sources
#                     so the language server indexes them.
#
# Aggregate Targets:
#   cp_algorithms_verify_probes      - All module verification probes.
#   cp_algorithms_header_probes      - All header probe targets.
#   cp_algorithms_template_scaffolds - All template scaffold targets.
#   cp_algorithms_all_probes         - All probe families at once.
#
# Options:
#   CP_ALGORITHMS_ENABLE_VERIFY_PROBES      (default: ON)
#   CP_ALGORITHMS_ENABLE_HEADER_PROBES      (default: ON)
#   CP_ALGORITHMS_ENABLE_TEMPLATE_SCAFFOLDS (default: ON)
#
# ============================================================================ #

cmake_minimum_required(VERSION 3.20)
project(CPAlgorithmsWorkspace LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(CP_ALGORITHMS_ENABLE_VERIFY_PROBES "Register module verification probes as build targets." ON)
option(CP_ALGORITHMS_ENABLE_HEADER_PROBES "Generate one probe translation unit per public header for clangd coverage." ON)
option(CP_ALGORITHMS_ENABLE_TEMPLATE_SCAFFOLDS "Register template scaffold sources as build targets." ON)

set(CP_ALGORITHMS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(CP_ALGORITHMS_PROBE_DIR "${CMAKE_CURRENT_BINARY_DIR}/clangd_probes")
set(CP_ALGORITHMS_RUNTIME_DIR "${CMAKE_CURRENT_BINARY_DIR}/bin/probes")

add_library(cp_algorithms_workspace INTERFACE)
target_compile_features(cp_algorithms_workspace INTERFACE cxx_std_23)
target_include_directories(
  cp_algorithms_workspace
  INTERFACE
    "${CP_ALGORITHMS_ROOT}"
    "${CP_ALGORITHMS_ROOT}/templates"
    "${CP_ALGORITHMS_ROOT}/modules"
    "${CP_ALGORITHMS_ROOT}/libs"
    "${CP_ALGORITHMS_ROOT}/verify"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(cp_algorithms_workspace INTERFACE -Wall -Wextra -Wpedantic)
endif()

function(cp_algorithms_sanitize_identifier out_var raw_value)
  string(MAKE_C_IDENTIFIER "${raw_value}" sanitized_value)
  set(${out_var} "${sanitized_value}" PARENT_SCOPE)
endfunction()

function(cp_algorithms_register_probe property_name target_stem source_path)
  cp_algorithms_sanitize_identifier(target_name "${target_stem}")
  add_executable("${target_name}" EXCLUDE_FROM_ALL "${source_path}")
  target_link_libraries("${target_name}" PRIVATE cp_algorithms_workspace)
  set_target_properties(
    "${target_name}"
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CP_ALGORITHMS_RUNTIME_DIR}"
      FOLDER "probes"
  )
  set_property(GLOBAL APPEND PROPERTY "${property_name}" "${target_name}")
endfunction()

function(cp_algorithms_generate_header_probe header_path)
  file(RELATIVE_PATH relative_header "${CP_ALGORITHMS_ROOT}" "${header_path}")
  set(probe_source "${CP_ALGORITHMS_PROBE_DIR}/${relative_header}.cpp")
  get_filename_component(probe_directory "${probe_source}" DIRECTORY)
  file(MAKE_DIRECTORY "${probe_directory}")
  file(
    WRITE "${probe_source}"
    "#include \"${relative_header}\"\n"
    "int main() { return 0; }\n"
  )
  cp_algorithms_register_probe(
    "CP_ALGORITHMS_HEADER_TARGETS"
    "cp_algorithms_header_${relative_header}"
    "${probe_source}"
  )
endfunction()

if(CP_ALGORITHMS_ENABLE_TEMPLATE_SCAFFOLDS)
  file(GLOB cp_algorithms_template_scaffolds CONFIGURE_DEPENDS "${CP_ALGORITHMS_ROOT}/templates/cpp/*.cpp")
  foreach(scaffold_source IN LISTS cp_algorithms_template_scaffolds)
    file(RELATIVE_PATH relative_scaffold "${CP_ALGORITHMS_ROOT}" "${scaffold_source}")
    cp_algorithms_register_probe(
      "CP_ALGORITHMS_SCAFFOLD_TARGETS"
      "cp_algorithms_scaffold_${relative_scaffold}"
      "${scaffold_source}"
    )
  endforeach()
endif()

if(CP_ALGORITHMS_ENABLE_VERIFY_PROBES)
  file(GLOB_RECURSE cp_algorithms_verify_sources CONFIGURE_DEPENDS "${CP_ALGORITHMS_ROOT}/verify/modules/*.cpp")
  foreach(verify_source IN LISTS cp_algorithms_verify_sources)
    file(RELATIVE_PATH relative_verify "${CP_ALGORITHMS_ROOT}" "${verify_source}")
    cp_algorithms_register_probe(
      "CP_ALGORITHMS_VERIFY_TARGETS"
      "cp_algorithms_verify_${relative_verify}"
      "${verify_source}"
    )
  endforeach()
endif()

if(CP_ALGORITHMS_ENABLE_HEADER_PROBES)
  file(GLOB_RECURSE cp_algorithms_public_headers CONFIGURE_DEPENDS
    "${CP_ALGORITHMS_ROOT}/templates/*.hpp"
    "${CP_ALGORITHMS_ROOT}/modules/*.hpp"
    "${CP_ALGORITHMS_ROOT}/libs/*.h"
    "${CP_ALGORITHMS_ROOT}/libs/*.hpp"
  )
  foreach(header_path IN LISTS cp_algorithms_public_headers)
    cp_algorithms_generate_header_probe("${header_path}")
  endforeach()
endif()

get_property(cp_algorithms_verify_targets GLOBAL PROPERTY CP_ALGORITHMS_VERIFY_TARGETS)
if(cp_algorithms_verify_targets)
  add_custom_target(cp_algorithms_verify_probes DEPENDS ${cp_algorithms_verify_targets})
  set_target_properties(cp_algorithms_verify_probes PROPERTIES FOLDER "probes")
endif()

get_property(cp_algorithms_header_targets GLOBAL PROPERTY CP_ALGORITHMS_HEADER_TARGETS)
if(cp_algorithms_header_targets)
  add_custom_target(cp_algorithms_header_probes DEPENDS ${cp_algorithms_header_targets})
  set_target_properties(cp_algorithms_header_probes PROPERTIES FOLDER "probes")
endif()

get_property(cp_algorithms_scaffold_targets GLOBAL PROPERTY CP_ALGORITHMS_SCAFFOLD_TARGETS)
if(cp_algorithms_scaffold_targets)
  add_custom_target(cp_algorithms_template_scaffolds DEPENDS ${cp_algorithms_scaffold_targets})
  set_target_properties(cp_algorithms_template_scaffolds PROPERTIES FOLDER "probes")
endif()

# Umbrella target: build every enabled probe family with a single invocation.
set(_cp_all_probe_deps "")
if(cp_algorithms_verify_targets)
  list(APPEND _cp_all_probe_deps cp_algorithms_verify_probes)
endif()
if(cp_algorithms_header_targets)
  list(APPEND _cp_all_probe_deps cp_algorithms_header_probes)
endif()
if(cp_algorithms_scaffold_targets)
  list(APPEND _cp_all_probe_deps cp_algorithms_template_scaffolds)
endif()
if(_cp_all_probe_deps)
  add_custom_target(cp_algorithms_all_probes DEPENDS ${_cp_all_probe_deps})
  set_target_properties(cp_algorithms_all_probes PROPERTIES FOLDER "probes")
endif()

# Configure-time summary.
list(LENGTH cp_algorithms_verify_targets  _cp_n_verify)
list(LENGTH cp_algorithms_header_targets  _cp_n_headers)
list(LENGTH cp_algorithms_scaffold_targets _cp_n_scaffolds)
message(STATUS "CP Algorithms Workspace configured:")
message(STATUS "  Verify probes   : ${_cp_n_verify}")
message(STATUS "  Header probes   : ${_cp_n_headers}")
message(STATUS "  Scaffold probes : ${_cp_n_scaffolds}")

# ============================================================================ #
