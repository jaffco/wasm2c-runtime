#!/bin/bash
# run.sh - Build and execute the WASM2C example

set -e  # Exit on error

echo "╔════════════════════════════════════════════════════╗"
echo "║         WASM2C Runtime Example Builder           ║"
echo "╚════════════════════════════════════════════════════╝"
echo ""

# Check for required tools
echo "Checking for required tools..."
MISSING_TOOLS=()

if ! command -v emcc &> /dev/null; then
    MISSING_TOOLS+=("emcc (Emscripten)")
fi

if ! command -v wasm2c &> /dev/null; then
    MISSING_TOOLS+=("wasm2c (WABT)")
fi

if ! command -v g++ &> /dev/null; then
    MISSING_TOOLS+=("g++")
fi

if [ ${#MISSING_TOOLS[@]} -ne 0 ]; then
    echo "✗ Missing required tools:"
    for tool in "${MISSING_TOOLS[@]}"; do
        echo "  - $tool"
    done
    echo ""
    echo "Installation hints:"
    echo "  - Emscripten: https://emscripten.org/docs/getting_started/downloads.html"
    echo "  - WABT (wasm2c): https://github.com/WebAssembly/wabt"
    exit 1
fi

echo "✓ All required tools found"
echo ""

# Parse command line arguments
CLEAN=false
BUILD=true
RUN=true

while [[ $# -gt 0 ]]; do
    case $1 in
        --clean|-c)
            CLEAN=true
            shift
            ;;
        --build-only|-b)
            RUN=false
            shift
            ;;
        --help|-h)
            echo "Usage: $0 [OPTIONS]"
            echo ""
            echo "Options:"
            echo "  -c, --clean        Clean build directory before building"
            echo "  -b, --build-only   Build but don't run the example"
            echo "  -h, --help         Show this help message"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Clean if requested
if [ "$CLEAN" = true ]; then
    echo "Cleaning previous build..."
    make clean
    echo ""
fi

# Build
echo "Building example..."
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
make all
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""

# Run if requested
if [ "$RUN" = true ]; then
    echo "Executing example..."
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    ./build/example
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
else
    echo "Build complete. Run './build/example' to execute."
fi
