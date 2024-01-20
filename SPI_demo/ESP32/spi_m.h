/*
 * spi_master.h
 *
 *  Created on: 24-Jul-2023
 *      Author: dyutimoy
 */

#ifndef MAIN_SPI_M_H_
#define MAIN_SPI_M_H_

#include "driver/spi_master.h"
#include "esp_log.h"
#include <stdint.h>

//IO14 - HSPICLK, IO12 - HSPIQ (MISO), IO13 - HSPID (MOSI), IO15 - HSPICS0. SPI2 used.
#define MISO 					12
#define MOSI 					13
#define CLK 					14
#define CS 						15

#define CLK_SPD 				1000000
#define TX_BUFF_SIZE 			8
#define RX_BUFF_SIZE 			8
#define MHz 					1000000

struct transactions
{
	spi_transaction_t transmit;
	spi_transaction_t receive;
};
const spi_bus_config_t bConfig =
{
		.mosi_io_num = MOSI,
		.miso_io_num = MISO,
		.sclk_io_num = CLK,
		.max_transfer_sz = 1
};
const spi_device_interface_config_t dConfig =
{
		.command_bits = 0,
		.address_bits = 0,
		.dummy_bits = 0,
		.mode = 3,
		.clock_speed_hz = CLK_SPD,
		.spics_io_num = CS,
		.queue_size = 1,
};
transactions t =
{
		.transmit =
		{
				.length 	= TX_BUFF_SIZE,
				.flags 		= SPI_TRANS_USE_TXDATA
		},
		.receive =
		{
				.length 	= RX_BUFF_SIZE,
				.flags 		= SPI_TRANS_USE_RXDATA
		}
};

spi_device_handle_t dHandle;

typedef struct transactions transactions;
static esp_err_t errCode 					= ESP_OK;
static const char null_char 				= '\0';

void configure(void);
void transmit(void);
void receive(void);

#endif /* MAIN_SPI_M_H_ */
