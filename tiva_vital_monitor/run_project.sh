#!/bin/bash
# Build and flash script for TM4C123 Tiva Vital Monitor

set -e

PROJECT_NAME="tiva_vital_monitor"
ELF="build/${PROJECT_NAME}.elf"
OPENOCD_CFG="board/ti_ek-tm4c123gxl.cfg"

usage() {
    echo "Usage: $0 [command]"
    echo ""
    echo "Commands:"
    echo "  build       Build the project (default)"
    echo "  clean       Clean build artifacts"
    echo "  rebuild     Clean and build"
    echo "  flash       Build and flash to board"
    echo "  flashonly   Flash without rebuilding"
    echo "  debug       Build and start GDB server"
    echo "  size        Show binary size"
    echo "  monitor     Open serial monitor (115200 baud)"
    echo "  all         Clean, build, flash, and monitor"
    echo ""
}

do_build() {
    echo "==> Building..."
    make
}

do_clean() {
    echo "==> Cleaning..."
    make clean
}

do_flash() {
    echo "==> Flashing..."
    openocd -f "$OPENOCD_CFG" -c "program $ELF verify reset exit"
}

do_debug() {
    echo "==> Starting GDB server on port 3333..."
    echo "    Connect with: arm-none-eabi-gdb $ELF -ex 'target remote :3333'"
    openocd -f "$OPENOCD_CFG"
}

do_size() {
    echo "==> Binary size:"
    arm-none-eabi-size "$ELF"
}

do_monitor() {
    # Find the USB serial device
    SERIAL_DEV=$(ls /dev/tty.usbmodem* 2>/dev/null | head -1)
    if [ -z "$SERIAL_DEV" ]; then
        SERIAL_DEV=$(ls /dev/cu.usbmodem* 2>/dev/null | head -1)
    fi
    
    if [ -z "$SERIAL_DEV" ]; then
        echo "Error: No USB serial device found"
        exit 1
    fi
    
    echo "==> Opening serial monitor on $SERIAL_DEV (115200 baud)"
    echo "    Press Ctrl+A then K to exit"
    screen "$SERIAL_DEV" 115200
}

# Main
case "${1:-build}" in
    build)
        do_build
        ;;
    clean)
        do_clean
        ;;
    rebuild)
        do_clean
        do_build
        ;;
    flash)
        do_build
        do_size
        do_flash
        ;;
    flashonly)
        do_flash
        ;;
    debug)
        do_build
        do_debug
        ;;
    size)
        do_size
        ;;
    monitor)
        do_monitor
        ;;
    all)
        do_clean
        do_build
        do_size
        do_flash
        echo ""
        echo "==> Flash complete. Starting monitor in 2 seconds..."
        sleep 2
        do_monitor
        ;;
    -h|--help|help)
        usage
        ;;
    *)
        echo "Unknown command: $1"
        usage
        exit 1
        ;;
esac
