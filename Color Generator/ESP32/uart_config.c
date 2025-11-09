/*
 * uart_config.c
 *
 *  Created on: 26-Jan-2024
 *      Author: dyutimoy
 */
#include <stdlib.h>
#include <stdint.h>
#include "driver/uart.h"
#include "uart_config.h"
#include "esp_check.h"
#include "esp_log.h"

static const char TAG[] = "uart_config";

static char* msg_r;

static esp_err_t errCode = ESP_OK;

static int bytes = 0;

uart_config_t uCfg = {
    .baud_rate 	= BAUD_RATE,
    .data_bits 	= UART_DATA_8_BITS,
    .parity 	= UART_PARITY_DISABLE,
    .stop_bits 	= UART_STOP_BITS_1,
	.flow_ctrl 	= UART_HW_FLOWCTRL_DISABLE,
	.source_clk = UART_SCLK_DEFAULT
};

void uart_setup(void)
{
	ESP_LOGI(TAG, "UART baud rate = %u bps", BAUD_RATE);

	errCode = uart_param_config(UART_PORT, &uCfg);
	ESP_ERROR_CHECK(errCode);

	errCode = uart_set_pin(UART_PORT, TXD, RXD, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	ESP_ERROR_CHECK(errCode);

	errCode = uart_driver_install(UART_PORT, BUFF_SIZE, BUFF_SIZE, 0, NULL, 0);
	ESP_ERROR_CHECK(errCode);
}

void write_to_uart(const char* str)
{
	for(uint32_t i = 0; str[i] != NULL_CHAR; i++)
	{
		uart_write(str[i]);
	}
	uart_write(NULL_CHAR);
	ESP_LOGI(TAG, "UART wrote data: %s", str);
}

void uart_write(char data)
{
	bytes = uart_write_bytes(UART_PORT, &data, 1);
	if (bytes == -1) {
		ESP_LOGI(TAG, "Parameter error occurred while writing.");
	}
}

void uart_read(void)
{
	uint32_t dataLength = 0;
	errCode = uart_get_buffered_data_len(UART_PORT, (size_t*)&dataLength);
	ESP_ERROR_CHECK(errCode);

	msg_r = (char*)malloc(bytes * sizeof(char));
	bytes = uart_read_bytes(UART_PORT, msg_r, dataLength, READ_TICKS);
	if (bytes == -1) {
		ESP_LOGI(TAG, "Parameter error occurred while reading.");
	}
	else {
		ESP_LOGI(TAG, "UART read data: %s", msg_r);
	}
}
