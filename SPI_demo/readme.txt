Project demonstrating SPI communication between Tiva C and ESP32 module.

ESP32 is configured as SPI master and Tiva C as SPI slave. 
ESP32 transmits a byte of data ('T') to Tiva C and receives a byte of data ('R') from Tiva C. 
Communication parameters: Clock - 1 MHz, rate - 1 Mbps, data width - 8 bits, SPI mode - 3 (PHA = POL = 1)

UART is used for both microcontrollers to monitor the exchange of data. UART is configured at 9600 bps for Tiva C and at 115200 bps for ESP32.