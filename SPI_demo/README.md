## About
Project demonstrating SPI communication between Tiva C and ESP32 module.

## Description
- ESP32 is configured as SPI master and Tiva C as SPI slave. 
- ESP32 transmits a byte of data (`T`) to Tiva C and receives a byte of data (`R`) from Tiva C. 
- UART is used for both microcontrollers to monitor the exchange of data.

### Communication parameters

| Parameter        | Value                |
|------------------|---------------------|
| Clock            | 1 MHz               |
| Data Rate        | 1 Mbps              |
| Data Width       | 8 bits              |
| SPI Mode         | 3 (PHA = 1, POL = 1)|
| Baud rate (UART) | 9600 bps (Tiva C), 115200 bps (ESP32)|

### Hardware connections

| ESP32 | Tiva C |
|-------|--------|
| GND   | GND    |
| IO14  | PD0    |
| IO15  | PD1    |
| IO13  | PD2    |
| IO12  | PD3    |

Check [Keil_config](TivaC/Keil_config.md) for instructions related to Keil configuration