#!/bin/bash

# Usage: ./run_project.sh [clean]

set -e

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
ELF_FILE="$PROJECT_DIR/build/tiva_vital_monitor.elf"

cd "$PROJECT_DIR"

# Clean if requested
if [ "$1" == "clean" ]; then
    echo "==> Cleaning build..."
    make clean
fi

# Build
echo "==> Building project..."
make

# Show size
echo "==> Binary size:"
make size

# Flash
echo "==> Flashing to board..."
openocd -f board/ti_ek-tm4c123gxl.cfg -c "program $ELF_FILE verify reset exit"

echo "==> Done!"
