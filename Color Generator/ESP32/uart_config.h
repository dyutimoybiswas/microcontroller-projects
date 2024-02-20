/*
 * uart_config.h
 *
 *  Created on: 26-Jan-2024
 *      Author: dyutimoy
 */

#ifndef MAIN_UART_CONFIG_H_
#define MAIN_UART_CONFIG_H_

#define BAUD_RATE		115200
#define UART_PORT		UART_NUM_1
#define TXD				21
#define RXD				22
#define BUFF_SIZE		2048
#define READ_TICKS		0
#define NULL_CHAR		'\0'

void uart_setup(void);
void uart_write(char);
void write_to_uart(const char*);
void uart_read(void);

#endif /* MAIN_UART_CONFIG_H_ */
