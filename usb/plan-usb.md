# Plan — USB (Tiva C as USB device on BBB)

## Objectives

- Enumerate the Tiva C (EK-TM4C123GXL) as a USB device when connected to a BBB USB host port.
- Write a BBB-side client program that sends the string `"hello"` to the Tiva C USB device.
- Tiva C firmware echoes the received data back unchanged to the BBB.
- Tiva C-side USB handling must be interrupt-driven (not polled) — USB interrupt triggers reception/transmission handling.
- End state: a round-trip `"hello"` -> `"hello"` exchange verified over USB between BBB and Tiva C.

## Pre-requisites

- Confirm Tiva C OTG connector power-source jumper setting (self-powered vs bus-powered) — see open question in [plan.md](plan.md). Must be set appropriately before plugging into a BBB host port (BBB host port supplies VBUS; Tiva C should not also try to source VBUS).
- Confirm arm-none-eabi-gcc + Make + OpenOCD + VS Code (Cortex-Debug) toolchain is working end-to-end for flashing the Tiva C (OpenOCD `ti-icdi` config detects/flashes the EK-TM4C123GXL's onboard ICDI, or `lm4flash` fallback is available).
- Confirm TivaWare's `usblib` is present/available (used previously in other board bring-up) and identify the relevant USB device class example to base the firmware on (e.g. bulk/CDC device class).
- Confirm BBB kernel has USB host support enabled and a way to observe enumeration (`dmesg`, `lsusb`).
- Confirm BBB has required USB host packages available (`usbutils` for `lsusb`, plus whatever is needed to write a raw USB client — e.g. `libusb` dev headers, or `usbfs` access permissions).
- Decide the USB device class to implement first (raw/vendor-specific bulk device via `libusb` vs CDC-ACM exposing a `/dev/ttyACM*`-like interface) — this determines both the Tiva C descriptor setup and the BBB client approach.

## Steps

### Step 1 — Tiva C enumerates as a bare USB device (no data transfer yet)

#### TODO
- Tiva C: Set up USB clock, pins, and minimal device descriptors (vendor/product ID, device class) using TivaWare `usblib`. No endpoints handling real data yet — just enumerate.
- Tiva C: Configure USB interrupt vector/handler (even if handler body is a stub) so the pipeline is interrupt-based from the start.
- BBB: Connect Tiva C to a BBB USB host port.

#### Verification
- BBB: `dmesg` shows a new USB device attach event, no enumeration errors.
- BBB: `lsusb` lists the Tiva C with the expected vendor/product ID.
- Tiva C: No USB fault/error interrupts firing continuously (check via debugger or UART log if instrumented).

### Step 2 — Tiva C exposes a data endpoint; BBB can see the interface

#### TODO
- Tiva C: Add the chosen device class's data endpoint(s) (bulk IN/OUT, or CDC-ACM) to the descriptors.
- Tiva C: Implement the USB interrupt handler to recognize endpoint events (e.g. data received on OUT endpoint) without yet echoing anything back — just log over UART that a transfer was received.
- BBB: Write minimal client code that opens the device (via `libusb` or the `/dev/ttyACM*`-equivalent node) and performs a single write of `"hello"`.

#### Verification
- BBB: Client program successfully opens/claims the USB interface without errors.
- Tiva C: UART log confirms the OUT endpoint interrupt fired and the correct byte count/content (`"hello"`) was received.
- BBB: `lsusb -v` shows the expected endpoint configuration (direction, type, max packet size).

### Step 3 — Full echo round-trip

#### TODO
- Tiva C: In the USB interrupt handler, after receiving data on the OUT endpoint, queue the same data back on the IN endpoint (echo), still fully interrupt-driven.
- BBB: Extend the client to, after writing `"hello"`, issue a read on the IN endpoint and capture the response.

#### Verification
- BBB: Client program prints the echoed string and confirms it matches the sent string (`"hello" == "hello"`).
- Tiva C: UART log shows both the receive and the echo-transmit interrupt events per exchange, with no dropped/duplicate interrupts.
- Repeat the exchange multiple times in a row (e.g. a loop of 10 sends) to confirm no missed interrupts, stalled endpoints, or BBB-side timeouts.

### Step 4 — Robustness pass

#### TODO
- Tiva C: Handle edge cases in the interrupt handler — partial packets, zero-length packets, USB reset/suspend events — without falling back to polling.
- BBB: Add basic error handling in the client (timeouts, device disconnect/reconnect, retry logic).

#### Verification
- BBB: Unplug/replug the Tiva C mid-test and confirm the client detects the disconnect and can re-establish communication after replug without a BBB reboot.
- Tiva C: Confirm firmware recovers cleanly from a USB bus reset (re-enumerates, interrupt handling resumes) without requiring a power cycle.

## Notes

_(empty — fill in as development progresses: shortcomings, challenges, surprises, things learned)_
