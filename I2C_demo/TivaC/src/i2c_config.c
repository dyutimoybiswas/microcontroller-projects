#include <stdbool.h>
#include "TM4C123GH6PM.h"
#include "delay.h"
#include "i2c_config.h"


void i2c_setup(void)
{	
	/* Use I2C-2 module (PE4 - SCL & PE5 - SDA) */
	SYSCTL->RCGCI2C 	|= R2;
	SYSCTL->RCGCGPIO 	|= R4;
	
	/* Configure GPIO. */
	GPIOE->DEN		|= SCL | SDA;
	GPIOE->AFSEL	|= SCL | SDA;
	GPIOE->ODR		|= SDA;
	GPIOE->PCTL		|= (0x03 << 16) | (0x03 << 20);
	
	/* Configure I2C. clock = 100 kHz, system clock = 16 MHz.*/
	I2C2->MCR			= 0x10;
	I2C2->MTPR		= 0x07;					// MTPR = (sys_clk_freq/(2*10*i2c_freq)) - 1
	
	/* Set slave address (GPIOA, BANK = 1) and specify transmission. */
	I2C2->MSA = (SLAVE_ADDR << 1);
}	


void i2c_write(uint8_t data, uint8_t conditions)
{	
	/* Write data. */
	I2C2->MDR = data;
	
	/* write control bits. */
	I2C2->MCS = conditions;
	
	/* Wait till transmission is in progress. */
	while(I2C2->MCS & BUSY);
	
	if(I2C2->MCS & ERROR)
	{
		/* Write stop bit. */
		I2C2->MCS = STOP;
	}
}

int main(void)
{
	systick_setup();
	i2c_setup();
	
	while(true)
	{
		// TODO
	}
}
