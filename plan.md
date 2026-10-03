# Plan

Initial projects (Tiva C = EK-TM4C123GXL, BBB = custom Yocto image):

1. Tiva C as USB device, discovered by BBB
2. UART between Tiva C and BBB
3. SPI between Tiva C and BBB (one run: Tiva master/BBB slave, another run: Tiva slave/BBB master)
4. I2C between Tiva C and BBB (Tiva master and BBB master, separate runs)

## Tiva C — open questions

- Which macOS toolchain path to finalize: arm-none-eabi-gcc + Make + OpenOCD + VS Code (Cortex-Debug) — chosen direction, but needs an end-to-end verify against the EK-TM4C123GXL's onboard ICDI (does OpenOCD's `ti-icdi` interface config actually detect/flash it, or is `lm4flash` needed as a fallback flasher?).
- What is the OTG connector's power-source jumper currently set to (self-powered vs bus-powered), and does it need to change before connecting to a BBB host port?
- Does TivaWare's `usblib` CDC-ACM device example enumerate cleanly against Linux's `cdc_acm` driver on the BBB, or does it need customization?
- Does the SSI peripheral's slave-mode behavior (already used in `SPI_demo` against the ESP32 master) hold up against BBB's McSPI master timing/chip-select behavior, or are there differences to account for?

## BBB (Yocto) — open questions

- Is `omap2_mcspi` (or whatever SPI controller driver is in use) built with `SPI_CONTROLLER_SLAVE` support? This determines whether the "BBB as SPI slave" run is even feasible, or needs a kernel patch / different approach (e.g. GPIO bit-banging).
- Are `CONFIG_SPI_SPIDEV`, `CONFIG_I2C_CHARDEV`, and `CONFIG_USB_ACM` enabled in the current kernel config?
- Does the current u-boot build have device-tree overlay support (`CONFIG_OF_OVERLAY`) enabled for `uEnv.txt`-based overlay loading?
- Which SPI bus/pins and I2C bus/pins will be exposed, and do the corresponding device tree overlays exist yet (or need to be written, using `bb.org-overlays` as a reference)?
- Does the chosen I2C bus already have onboard pull-up resistors (like I2C2 does for the cape EEPROM), or will external pull-ups need to be added?
- Which packages still need to be added to the distro beyond `systemd`/`openssh`: `i2c-tools`, `usbutils`, an SPI test utility (`spidev-test`/spi-tools), `picocom`/`minicom`?
