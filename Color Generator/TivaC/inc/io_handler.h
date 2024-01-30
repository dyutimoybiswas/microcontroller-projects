#ifndef IO_HANDLER_H
#define IO_HANDLER_H

#define RXD							(1U << 4)
#define TXD							(1U << 5)
#define TXE							(1U << 8)
#define RXE							(1U << 9)
#define UARTEN					    (1U << 0)
#define RXFE						(1U << 4)
#define TXFF						(1U << 5)
#define BUSY						(1U << 3)
#define NULL_CHAR				    '\0'

void uart_setup(void);
void write_to_uart(const char*);
void uart_write(char);
char uart_read(void);
void parse_input(int*, const char*);

#endif
