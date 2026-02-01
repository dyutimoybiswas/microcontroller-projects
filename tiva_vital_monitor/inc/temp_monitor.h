#ifndef TEMP_MONITOR_H
#define TEMP_MONITOR_H

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "FreeRTOS.h"
#include "semphr.h"

#define RED_LED            GPIO_PIN_1
#define BLUE_LED           GPIO_PIN_2
#define GREEN_LED          GPIO_PIN_3

void led_setup();

#endif  // TEMP_MONITOR_H