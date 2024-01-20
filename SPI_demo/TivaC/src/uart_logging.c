#include "uart_logging.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"


void uart_setup(void)
{
    // enable gpio port A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
        // wait till peripheral is ready
    }
    GPIOPinConfigure(GPIO_PA0_U0RX | GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // enable UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0))
    {
        // wait till peripheral is ready
    }
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), BAUD_RATE, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    UARTEnable(UART0_BASE);
		
		uart_init();
}

void uart_init(void)
{
    // Clear screen.
    write_to_uart("\x1b[2J");

    // Set cursor to home position.
    write_to_uart("\x1b[H");
}

void uart_write(unsigned char ch)
{
    UARTCharPut(UART0_BASE, ch);
    while (UARTBusy(UART0_BASE))
    {
        // wait till transmit FIFO is empty
    }
}

void write_to_uart(const char* str)
{
	for(size = 0; str[size] != NULL_CHAR; size++) 
	{
		uart_write(str[size]);
	}
}
