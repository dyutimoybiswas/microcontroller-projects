#include <stdint.h>
#include "TM4C123GH6PM.h"
#include "io_handler.h"
#include "utils.h"


void uart_setup(void)
{
	/* UART4, Port C at 115200 bps. */
	SYSCTL->RCGCUART |= (1U << 4);
	SYSCTL->RCGCGPIO |= (1U << 2);
	
	/* configure GPIO. */
	GPIOC->DEN 		|= TXD | RXD;
	GPIOC->AFSEL 	|= TXD | RXD;
	GPIOC->PCTL		|= (0x1 << 16) | (0x1 << 20);
	
	/* configure UART. */
	/* BRD = sys_clk / (clk_div * baud_rate) where clk_div = 16 as high speed mode is not used.*/
	/* IBRD = int(BRD). */
	/* FBRD = int(frac(BRD) * 64 + 0.5). */
	UART4->CTL 	&= ~(TXE | RXE | UARTEN);
	UART4->IBRD = 8U;
	UART4->FBRD = 44U;
	UART4->LCRH = 0x60;
	UART4->CC	= 0x0;
	UART4->CTL 	|= TXE | RXE | UARTEN;
}

void write_to_uart(const char* str)
{
	uint32_t i = 0, length = str_length(str);
	while(i < length)
	{
		uart_write(str[i++]);
	}
}

void uart_write(char ch)
{
	/* wait while transmit buffer is full. */
	while(UART4->FR & TXFF);
	
	/* write to buffer. */
	UART4->DR = ch;
	
	/* wait till transmit in progress. */
	while(UART4->FR & BUSY);
}

char uart_read(void)
{
	/* wait while receive buffer is empty. */
	while(UART4->FR & RXFE);
	
	/* return value read from buffer. */
	return (char)UART4->DR;
}

void parse_input(int* rgb, const char* input)
{
	uint8_t i = 0;
	char color = 'R';
	
	while(input[i] != NULL_CHAR)
	{
		if(input[i] == ',')
		{
			color = color == 'R' ? 'G' : 'B';
			i++;
			continue;
		}
		
		/* extract red value. */
		if(color == 'R')
		{
			rgb[0] = rgb[0] * 10 + char_to_int(input[i]);
		}
		
		/* extract green value. */
		else if(color == 'G')
		{
			rgb[1] = rgb[1] * 10 + char_to_int(input[i]);
		}
		
		/* extract blue value. */
		else if(color == 'B')
		{
			rgb[2] = rgb[2] * 10 + char_to_int(input[i]);
		}
		
		i++;
	}
}
