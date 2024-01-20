#include <stdbool.h>
#include "uart_logging.h"
#include "spi_control.h"
#include "TM4C123GH6PM.h"


int main(void)
{
	uart_setup();
	spi_setup();
	
	while(true)
	{
		/* receive data. */
		write_to_uart("Received message: ");
		spi_receive();
		write_to_uart(newline_carriage_return);
		
		/* write data to master. */
		write_to_uart("Transmitted message: ");
		uart_write(msg_t);
		write_to_uart(newline_carriage_return);
		spi_write();
	}
}
