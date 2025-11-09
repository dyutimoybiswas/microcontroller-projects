#ifndef DELAY_H
#define DELAY_H

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"

#define LOAD_VALUE          15999999U

void systick_setup(void);
void systick_handler(void);

#endif