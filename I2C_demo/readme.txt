Project demonstrating I2C communication between Tiva C and ESP32 module.

ESP32 is configured as I2C slave and Tiva C as I2C master.
Tiva C transmits values - 1 & 0 at an interval of 1 second to ESP32, and ESP32 turns on and off an external LED accordingly.
Communication parameters: Clock speed- 100 kHz.

TODO: program ESP32 GPIO output pins to turn on/off external LED.