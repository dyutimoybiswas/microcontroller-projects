Project to control on-board RGB LEDs on Tiva C from an android application.

Values are entered by the user corresponding to red, green or blue in the application.
The values are communicated to Tiva C from ESP32 via UART.
Based on the values, Port F PWM is configured and the desired color is generated.
