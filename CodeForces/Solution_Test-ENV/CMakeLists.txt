cmake_minimum_required(VERSION 3.20)

# ============================================================================ #
# ------------ Project Configuration for Competitive Programming ------------- #
# ============================================================================ #
project(competitive_programming LANGUAGES CXX)

# Set C++23 as standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================ #
# -------------- Competitive Programming Specific Configuration -------------- #
# ============================================================================ #

# Find GCC for bits/stdc++.h and pb_ds support
find_program(GCC_EXECUTABLE 
    NAMES g++-15 g++-14 g++-13 g++ 
    PATHS /opt/homebrew/bin /usr/local/bin
    NO_DEFAULT_PATH
)

# Check if source files need competitive programming headers
file(GLOB_RECURSE ALL_SOURCES "*.cpp" "*.cc" "*.cxx")
set(NEEDS_BITS_STDC FALSE)
set(NEEDS_PBDS FALSE)

foreach(source_file ${ALL_SOURCES})
    file(READ ${source_file} file_content)
    if(file_content MATCHES "bits/stdc\\+\\+\\.h")
        set(NEEDS_BITS_STDC TRUE)
    endif()
    if(file_content MATCHES "ext/pb_ds")
        set(NEEDS_PBDS TRUE)
    endif()
endforeach()

# ============================================================================ #
# ------------------- Compiler Selection and Configuration ------------------- #
# ============================================================================ #

if(NEEDS_BITS_STDC OR NEEDS_PBDS)
    if(NOT GCC_EXECUTABLE)
        message(FATAL_ERROR 
            "Competitive programming headers detected but GCC not found!\n"
            "Install with: brew install gcc")
    endif()
    
    # Force GCC for competitive programming
    set(CMAKE_CXX_COMPILER ${GCC_EXECUTABLE})
    message(STATUS "Using GCC for competitive programming: ${GCC_EXECUTABLE}")
    
    # Add GCC-specific include paths
    execute_process(
        COMMAND ${GCC_EXECUTABLE} -E -Wp,-v -x c++ /dev/null
        ERROR_VARIABLE gcc_include_paths
        ERROR_STRIP_TRAILING_WHITESPACE
    )
    
    # Parse include paths and add as system includes
    string(REGEX MATCHALL "/[^\\n]*c\\+\\+[^\\n]*" cpp_include_paths "${gcc_include_paths}")
    foreach(include_path ${cpp_include_paths})
        if(IS_DIRECTORY ${include_path})
            include_directories(SYSTEM ${include_path})
            message(STATUS "Added GCC include path: ${include_path}")
        endif()
    endforeach()
    
else()
    message(STATUS "Using default compiler - no competitive programming headers detected")
endif()

# ============================================================================ #
# ----------------- Build Types for Competitive Programming ------------------ #
# ============================================================================ #

# Debug build (default for development)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DLOCAL=1 -DDEBUG=1")

# Release build (for contest submissions)
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -march=native -mtune=native")

# Sanitize build (for debugging)
set(CMAKE_CXX_FLAGS_SANITIZE "-g -O1 -fsanitize=address,undefined -fno-omit-frame-pointer -DLOCAL=1")

# Add custom build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Set warning flags based on compiler
if(NEEDS_BITS_STDC OR NEEDS_PBDS)
    # More lenient warnings for competitive programming with GCC
    set(WARNING_FLAGS "-Wall -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare")
else()
    # Standard warnings for regular development
    set(WARNING_FLAGS "-Wall -Wextra -pedantic")
endif()

# Apply warning flags to all build types
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS}")

# ============================================================================ #
# ------------------------- Executable Configuration ------------------------- #
# ============================================================================ #

# Find all C++ source files
file(GLOB SOURCES "*.cpp" "*.cc" "*.cxx")

# Create executable for each source file (common in competitive programming)
foreach(source_file ${SOURCES})
    get_filename_component(exec_name ${source_file} NAME_WE)
    add_executable(${exec_name} ${source_file})
    
    # Set output directory to current directory
    set_target_properties(${exec_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    
    message(STATUS "Added executable: ${exec_name}")
endforeach()

# Default target (if main.cpp exists)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT main)
endif()

# ============================================================================ #
# -------------- Utility Functions for Competitive Programming --------------- #
# ============================================================================ #

# Custom target for contest builds (release mode)
add_custom_target(contest
    COMMAND ${CMAKE_COMMAND} --build . --config Release
    COMMENT "Building optimized version for contest submission"
)

# Custom target for running with input redirection
add_custom_target(run
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/main < input.txt 2>/dev/null || ${CMAKE_CURRENT_SOURCE_DIR}/main
    DEPENDS main
    COMMENT "Running main executable with input.txt if available"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# Custom target for testing with I/O redirection
add_custom_target(test
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/main < input.txt > output.txt 2>/dev/null || ${CMAKE_CURRENT_SOURCE_DIR}/main > output.txt
    DEPENDS main  
    COMMENT "Running main with I/O redirection"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# ============================================================================ #
# ---------------------------- IDE Configuration ----------------------------- #
# ============================================================================ #

# Generate compile_commands.json in source directory for clangd
if(CMAKE_EXPORT_COMPILE_COMMANDS)
    add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
        COMMENT "Copying compile_commands.json to source directory"
    )
endif()

# Print configuration summary
message(STATUS "=== Competitive Programming Configuration ===")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Needs bits/stdc++.h: ${NEEDS_BITS_STDC}")
message(STATUS "Needs pb_ds: ${NEEDS_PBDS}")
message(STATUS "Sources found: ${SOURCES}")
message(STATUS "==============================================")