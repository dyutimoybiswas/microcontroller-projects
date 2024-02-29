#ifndef I2C_CONFIG_H
#define I2C_CONFIG_H

#include <stdint.h>

#define R2								(1U << 2)
#define R4								(1U << 4)
#define SCL								(1U << 4)
#define SDA								(1U << 5)

#define BUSY							(1U << 0)
#define ERROR							(1U << 1)

#define ADRACK						(1U << 2)
#define DATACK						(1U << 3)

#define RUN								(1U << 0)
#define START							(1U << 1)
#define STOP							(1U << 2)

#define SLAVE_ADDR				0x58

void i2c_setup(void);
void i2c_write(uint8_t, uint8_t);

static uint8_t on_off_flag = 1;

#endif
