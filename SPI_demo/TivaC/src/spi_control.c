#include "TM4C123GH6PM.h"
#include "spi_control.h"
#include "uart_logging.h"


void spi_receive(void)
{
	/* wait while receive FIFO is empty. */
	while(!(SSI3->SR & RNE));
	data = (unsigned char)SSI3->DR;
	uart_write(data);
}

void spi_setup(void)
{
	/* Enable SSI3. */
	SYSCTL->RCGCSSI |= R3;
	
	/* Enable Port D. */
	SYSCTL->RCGCGPIO |= R3;
	
	/* Configure GPIOD as SPI. */
	GPIOD->AFSEL	|= (PD0 | PD1 | PD2 | PD3);
	GPIOD->PCTL		|= (1U << 0) | (1U << 4) | (1U << 8) | (1U << 12);
	GPIOD->DEN		|= (PD0 | PD1 | PD2 | PD3);
	GPIOD->DIR 		&= ~PD1;
	
	/* Configure SPI. */
	SSI3->CR1 	&= ~SSE;
	SSI3->CR1 	= 0x04;
	SSI3->CC	= 0x05;
	SSI3->IM	|= RINT;
	
	/* bit rate = SSI_clk / (CPSDVSR * (1 + SCR)), where CPDVSR is configured in SSICPSR and SCR in SSICR0. */
	/* 1Mbps bit rate @ 1 MHz, 8 bits and SPI SPO = SPH = 1. CPSDVSR = 16 and SCR = 0. */
	SSI3->CPSR	= 0x10;
	SSI3->CR0	= 0x0C7;
	SSI3->CR1	|= SSE;
}

void spi_write(void)
{	
	/* wait till transmit FIFO is not empty. */
	while(!(SSI3->SR & TFN));
	
	/* write value. */
	SSI3->DR = msg_t;
	
	/* wait till write is in progress. */
	while(SSI3->SR & BUSY);
}
