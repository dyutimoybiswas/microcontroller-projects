# Tiva Vital Monitor

## Description
Simulate real time monitor of patient vital - temperature using on-board temperature sensor present in [EK-TM4C123GXL](https://www.ti.com/tool/EK-TM4C123GXL) microcontroller. Sample temperature readings every 500 ms and compute a moving average (up to 5 samples). On temperature change:
- Log **normal**, or **warning** using UART by comparing moving average delta against a preconfigured threshold value
- Red LED blinks continuously while in **warning** state, stays off for **normal** range
- UART logging happens only on state change from **warning** to **normal** or vice-versa
- Green LED displays heartbeat pattern (double-blink) to indicate system health
- Watchdog timer (~1 second) resets the system if temperature processor task stops responding
- Reset cause is logged on startup (POWER-ON, WATCHDOG, EXTERNAL, SOFTWARE, etc.)

## Prerequisites

### macOS
```bash
# ARM GCC Toolchain
brew install osx-cross/arm/arm-gcc-bin

# OpenOCD for flashing
brew install openocd
```

### Linux (Ubuntu/Debian)
```bash
sudo apt install gcc-arm-none-eabi openocd
```

## Build & Flash

Build and flash to board:
```bash
./run_project.sh flash
```

Other commands:
```bash
./run_project.sh build      # Build only
./run_project.sh rebuild    # Clean + build
./run_project.sh flashonly  # Flash without rebuilding
./run_project.sh size       # Show binary size
./run_project.sh monitor    # Open serial monitor
./run_project.sh all        # Clean, build, flash, and monitor
./run_project.sh --help     # Show all options
```

## Serial Monitor

Connect to the LaunchPad's virtual COM port at **115200 baud**:

```bash
# macOS
screen /dev/tty.usbmodem* 115200

# Linux
screen /dev/ttyACM0 115200
```

Press `Ctrl+A` then `K` to exit screen.

Expected output:
```
Reset cause: SOFTWARE
Temperature delta: 0.42 °C → WARNING
Temperature delta: 0.12 °C → NORMAL
...
```

The red LED blinks continuously when delta exceeds threshold (WARNING), and stops when returning to NORMAL. The green LED displays a heartbeat pattern (double-blink followed by pause) to indicate the system is running normally.

## Project Structure
```
tiva_vital_monitor/
├── inc/                    # Header files
│   ├── FreeRTOSConfig.h    # FreeRTOS configuration
│   ├── setup.h             # Hardware configuration and setup declarations
│   └── temp_monitor.h      # Application configuration
├── src/                    # Source files
│   ├── setup.c             # Hardware initialization (LED, ADC, UART)
│   ├── temp_monitor.c      # Main application with FreeRTOS tasks
│   └── startup_gcc.c       # Startup code
├── tivaware/               # TivaWare SDK (headers + prebuilt lib)
├── Makefile                # Build system
├── tm4c.ld                 # Linker script
└── run_project.sh          # Build/flash helper script
```

## Configuration

Edit `inc/temp_monitor.h` for application settings:
- `TEMP_DELAY_MS` — Sampling interval (default: 500ms)
- `TEMP_THRESHOLD_C` — Warning threshold for temperature change (default: 0.3°C)
- `AVG_WINDOW_SIZE` — Moving average window size (default: 5)
- `LED_BLINK_DURATION_MS` — Red LED on/off duration during warning (default: 400ms)
- `HEARTBEAT_INTERVAL_MS` — Green LED heartbeat pulse duration (default: 160ms)

Edit `inc/setup.h` for hardware settings:
- `BAUD_RATE` — UART baud rate (default: 115200)
- `SYSTEM_CLOCK_HZ` — System clock frequency (default: 16MHz)

## Testing Watchdog

To test the watchdog reset functionality, comment out the health flag in `src/temp_monitor.c`:
```c
/* Update processor health status */
// xIsProcessorHealthy = true;
```

This prevents the watchdog from being kicked, causing a reset after ~1 second. The serial monitor will display `Reset cause: WATCHDOG` on restart.