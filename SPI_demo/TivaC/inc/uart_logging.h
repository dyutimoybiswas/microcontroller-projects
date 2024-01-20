#ifndef UART_LOGGING_H
#define UART_LOGGING_H

#define BAUD_RATE 				9600
#define NULL_CHAR					'\0'

void uart_setup(void);
void uart_init(void);
void uart_write(unsigned char);
void write_to_uart(const char*);

static unsigned short size;
static const char* newline_carriage_return = "\n\r";

#endif
