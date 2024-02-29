/*
 * i2c_s.c
 *
 *  Created on: 18-Feb-2024
 *      Author: dyutimoy
 */

#include "driver/i2c.h"
//#include "driver/i2c_slave.h"
//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
#include "esp_log.h"
#include "i2c_s.h"

static const char TAG[] = "I2C_S";

/* Reference - https://docs.espressif.com/projects/esp-idf/en/v4.3/esp32/api-reference/peripherals/i2c.html#api-reference. */

#if 0
i2c_slave_config_t i2c_slv_config = {
	.clk_source 				  = I2C_CLK_SRC_DEFAULT,
	.i2c_port 				      = I2C_PORT,
	.scl_io_num 				  = I2C_SLAVE_SCL_IO,
	.sda_io_num 				  = I2C_SLAVE_SDA_IO,
	.slave_addr 				  = SLAVE_ADDR,
};

i2c_slave_dev_handle_t slave_handle;

i2c_slave_rx_done_event_data_t rx_data;

uint8_t data_rd;

static IRAM_ATTR bool i2c_slave_rx_done_callback(i2c_slave_dev_handle_t channel, const i2c_slave_rx_done_event_data_t *edata, void *user_data)
{
    BaseType_t high_task_wakeup = pdFALSE;
    QueueHandle_t receive_queue = (QueueHandle_t)user_data;
    xQueueSendFromISR(receive_queue, edata, &high_task_wakeup);
    return high_task_wakeup == pdTRUE;
}

i2c_slave_event_callbacks_t cbs = {
    .on_recv_done = i2c_slave_rx_done_callback,
};

void i2c_setup(void)
{
	ESP_LOGI(TAG, "Setting up I2C...");
	ESP_ERROR_CHECK(i2c_new_slave_device(&i2c_slv_config, &slave_handle));
}

uint8_t i2c_receive(void)
{
	QueueHandle_t s_receive_queue = xQueueCreate(1, sizeof(i2c_slave_rx_done_event_data_t));
	ESP_ERROR_CHECK(i2c_slave_register_event_callbacks(slave_handle, &cbs, s_receive_queue));

	ESP_ERROR_CHECK(i2c_slave_receive(slave_handle, &data_rd, DATA_LENGTH));
	xQueueReceive(s_receive_queue, &rx_data, pdMS_TO_TICKS(1000));

	return data_rd;
}
#endif

void i2c_setup(void)
{
	i2c_config_t conf_slave = {
		.sda_io_num = I2C_SLAVE_SDA_IO,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_io_num = I2C_SLAVE_SCL_IO,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.mode = I2C_MODE_SLAVE,
		.slave.addr_10bit_en = 0,
		.slave.slave_addr = SLAVE_ADDR,
	};

	ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &conf_slave));
	ESP_ERROR_CHECK(i2c_driver_install(I2C_PORT, conf_slave.mode, 2 * DATA_LENGTH, 2 * DATA_LENGTH, 0));

	ESP_LOGI(TAG, "I2C setup done.");
}
