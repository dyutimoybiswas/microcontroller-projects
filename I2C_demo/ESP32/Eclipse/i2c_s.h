/*
 * i2c_s.h
 *
 *  Created on: 18-Feb-2024
 *      Author: dyutimoy
 */

#ifndef MAIN_I2C_S_H_
#define MAIN_I2C_S_H_

#include <stdint.h>

#define I2C_PORT			1
#define I2C_SLAVE_SCL_IO	25
#define I2C_SLAVE_SDA_IO	26
#define SLAVE_ADDR			0x58
#define DATA_LENGTH			128
#define RW_TEST_LENGTH		128

void i2c_setup(void);
uint8_t i2c_receive(void);

#endif /* MAIN_I2C_S_H_ */
