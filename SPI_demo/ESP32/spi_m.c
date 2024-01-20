/*
 * spi_master.c
 *
 *  Created on: 24-Jul-2023
 *      Author: dyutimoy
 */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "esp_check.h"
#include "driver/spi_common.h"
#include "hal/spi_types.h"
#include "spi_m.h"

static const char TAG[] = "spi_m";

static const char msg_t = 'T';

void configure(void)
{
	errCode = spi_bus_initialize(SPI2_HOST, &bConfig, SPI_DMA_CH_AUTO);
	ESP_ERROR_CHECK(errCode);
	ESP_LOGI(TAG, "SPI bus initialized.");

	errCode = spi_bus_add_device(SPI2_HOST, &dConfig, &dHandle);
	ESP_ERROR_CHECK(errCode);
	ESP_LOGI(TAG, "SPI bus device added.");

	int spi_freq;
	errCode = spi_device_get_actual_freq(dHandle, &spi_freq);
	ESP_ERROR_CHECK(errCode);
	ESP_LOGI(TAG, "SPI frequency is %d kHz.", spi_freq);
}

void transmit(void)
{
	// transmit messages
	errCode = spi_device_acquire_bus(dHandle, portMAX_DELAY);
	ESP_ERROR_CHECK(errCode);
	ESP_LOGI(TAG, "SPI device acquired bus.");

	t.transmit.tx_data[0] = msg_t;
	errCode = spi_device_transmit(dHandle, &t.transmit);
	ESP_ERROR_CHECK(errCode);
	printf("SPI transmitted data: %c\n",msg_t);

	spi_device_release_bus(dHandle);
	ESP_LOGI(TAG, "SPI device released bus.");
}

void receive(void)
{
	errCode = spi_device_transmit(dHandle, &t.receive);
	if (errCode != ESP_OK)
	{
		printf("Some error occurred while receiving.\n");
	}
	else {
		char ch = t.receive.rx_data[0];
		printf("Received message: %c\n", ch);
	}
}
