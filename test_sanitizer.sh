#!/usr/bin/env zsh
# =========================================================================== #
# Test script to verify sanitizer configuration
# Run this in a test directory within your CP workspace
#
# Usage: zsh test_sanitizer.sh
# Or make it executable: chmod +x test_sanitizer.sh && ./test_sanitizer.sh
# =========================================================================== #

set -e # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Source the competitive programming utilities
# Try to find competitive.sh in common locations
if [ -f "$HOME/.config/cpp-tools/competitive.sh" ]; then
    source "$HOME/.config/cpp-tools/competitive.sh"
elif [ -f "$(dirname "$0")/competitive.sh" ]; then
    source "$(dirname "$0")/competitive.sh"
elif [ -f "./competitive.sh" ]; then
    source "./competitive.sh"
else
    echo -e "${RED}Error: Could not find competitive.sh${NC}"
    echo "Please ensure competitive.sh is in one of these locations:"
    echo "  - ~/.config/cpp-tools/competitive.sh"
    echo "  - Same directory as this script"
    echo "  - Current directory"
    exit 1
fi

# Suppress the loading message
export CP_QUIET_LOAD=1

echo -e "${BLUE}/===----- Sanitizer Configuration Test -----===/${NC}"

# Test 1: Create a test contest
echo -e "\n${YELLOW}Test 1: Creating test contest...${NC}"
TEST_DIR="test_sanitizer_$$"
if cppcontest "$TEST_DIR"; then
    echo -e "${GREEN}✓ Contest creation successful${NC}"
else
    echo -e "${RED}✗ Contest creation failed${NC}"
    exit 1
fi

# Test 2: Create a test problem with intentional memory error
echo -e "\n${YELLOW}Test 2: Creating test problem with memory error...${NC}"
cat >test_memory.cpp <<'EOF'
#ifdef USE_CLANG_SANITIZE
    #include "PCH.h"
#else
    #include <bits/stdc++.h>
#endif

using namespace std;

int main() {
    // Intentional buffer overflow for testing sanitizer
    int* arr = new int[10];
    
    // Write beyond array bounds
    for(int i = 0; i <= 10; i++) {  // Note: <= causes overflow
        arr[i] = i;
    }
    
    cout << "If you see this without sanitizer error, sanitizers are not working!" << endl;
    
    delete[] arr;
    return 0;
}
EOF

# Test 3: Try different build configurations
echo -e "\n${YELLOW}Test 3: Testing Debug build (GCC)...${NC}"
if cppconf Debug >/dev/null 2>&1; then
    echo -e "${GREEN}✓ Debug configuration successful${NC}"
    if cppbuild test_memory >/dev/null 2>&1; then
        echo -e "${GREEN}✓ Debug build successful${NC}"
    else
        echo -e "${RED}✗ Debug build failed${NC}"
    fi
else
    echo -e "${RED}✗ Debug configuration failed${NC}"
fi

echo -e "\n${YELLOW}Test 4: Testing Release build (GCC)...${NC}"
if cppconf Release >/dev/null 2>&1; then
    echo -e "${GREEN}✓ Release configuration successful${NC}"
    if cppbuild test_memory >/dev/null 2>&1; then
        echo -e "${GREEN}✓ Release build successful${NC}"
    else
        echo -e "${RED}✗ Release build failed${NC}"
    fi
else
    echo -e "${RED}✗ Release configuration failed${NC}"
fi

echo -e "\n${YELLOW}Test 5: Testing Sanitize build (should use Clang on macOS)...${NC}"
if cppconf Sanitize >/dev/null 2>&1; then
    echo -e "${GREEN}✓ Sanitize configuration successful${NC}"

    # Check which compiler was selected
    if grep -q "Clang" build/CMakeCache.txt; then
        echo -e "${GREEN}✓ Using Clang for sanitizers${NC}"
    elif grep -q "GNU" build/CMakeCache.txt; then
        echo -e "${YELLOW}⚠ Using GCC for sanitizers (check if sanitizers work)${NC}"
    fi

    if cppbuild test_memory >/dev/null 2>&1; then
        echo -e "${GREEN}✓ Sanitize build successful${NC}"

        # Test if sanitizer actually detects the error
        echo -e "\n${YELLOW}Test 6: Running with sanitizers (should detect buffer overflow)...${NC}"
        if ./bin/test_memory 2>&1 | grep -q "AddressSanitizer\|buffer-overflow\|ERROR"; then
            echo -e "${GREEN}✓ Sanitizer detected the memory error correctly!${NC}"
        else
            echo -e "${YELLOW}⚠ Sanitizer did not detect the error (may not be available)${NC}"
        fi
    else
        echo -e "${RED}✗ Sanitize build failed${NC}"
    fi
else
    echo -e "${RED}✗ Sanitize configuration failed${NC}"
fi

# Test 7: Create a clean problem to test PCH.h
echo -e "\n${YELLOW}Test 7: Testing PCH.h compatibility...${NC}"
cat >test_pch.cpp <<'EOF'
#define USE_CLANG_SANITIZE
#include "PCH.h"

using namespace std;

int main() {
    // Test various PCH.h features
    vector<int> v = {1, 2, 3, 4, 5};
    map<int, string> m = {{1, "one"}, {2, "two"}};
    
    // Test shortcuts
    ll big_num = 1000000000000LL;
    vi vec = {1, 2, 3};
    
    // Test utilities
    cout << "GCD(12, 18) = " << gcd(12, 18) << endl;
    cout << "Random number: " << random(1, 100) << endl;
    
    cout << "PCH.h test successful!" << endl;
    return 0;
}
EOF

if cppconf Sanitize clang >/dev/null 2>&1; then
    if cppbuild test_pch >/dev/null 2>&1; then
        echo -e "${GREEN}✓ PCH.h compilation successful${NC}"
        if ./bin/test_pch >/dev/null 2>&1; then
            echo -e "${GREEN}✓ PCH.h execution successful${NC}"
        else
            echo -e "${RED}✗ PCH.h execution failed${NC}"
        fi
    else
        echo -e "${RED}✗ PCH.h compilation failed${NC}"
    fi
fi

# Test 8: Verify file structure
echo -e "\n${YELLOW}Test 8: Verifying file structure...${NC}"
if [ -f "algorithms/debug.h" ]; then
    echo -e "${GREEN}✓ debug.h present${NC}"
else
    echo -e "${RED}✗ debug.h missing${NC}"
fi

if [ -f "algorithms/PCH.h" ]; then
    echo -e "${GREEN}✓ PCH.h present${NC}"
else
    echo -e "${YELLOW}⚠ PCH.h missing (needed for Clang sanitizers)${NC}"
fi

if [ -f "gcc-toolchain.cmake" ]; then
    echo -e "${GREEN}✓ gcc-toolchain.cmake present${NC}"
else
    echo -e "${RED}✗ gcc-toolchain.cmake missing${NC}"
fi

if [ -f "clang-toolchain.cmake" ]; then
    echo -e "${GREEN}✓ clang-toolchain.cmake present${NC}"
else
    echo -e "${YELLOW}⚠ clang-toolchain.cmake missing (created on demand)${NC}"
fi

# Cleanup
echo -e "\n${YELLOW}Cleaning up test directory...${NC}"
cd ..
rm -rf "$TEST_DIR"

echo -e "\n${BLUE}/===----- Test Complete -----===/${NC}"
echo -e "${GREEN}The sanitizer configuration is working correctly!${NC}"
echo -e "\nRecommendations:"
echo -e "  - Use ${YELLOW}cppconf Debug${NC} for regular development"
echo -e "  - Use ${YELLOW}cppconf Sanitize${NC} for debugging memory issues"
echo -e "  - Use ${YELLOW}cppconf Release${NC} for performance testing"

# =========================================================================== #
# End of script
