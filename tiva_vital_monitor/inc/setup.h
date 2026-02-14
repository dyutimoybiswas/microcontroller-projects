#ifndef SETUP_H
#define SETUP_H

#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_memmap.h"

#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/watchdog.h"

/* GPIO Pin definitions */
#define RED_LED GPIO_PIN_1
#define BLUE_LED GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3

/* System configuration */
#define SYSTEM_CLOCK_HZ 16000000
#define BAUD_RATE 115200
#define ADC_SINGLE_SAMPLE 3

/* Setup function declarations */
void led_setup(void);
void adc_setup(void);
void uart_setup(void);
void uart_print(const char *str);
void watchdog_setup(void);
void watchdog_start(void);
void ADCIntHandler(void);

#endif /* SETUP_H */
