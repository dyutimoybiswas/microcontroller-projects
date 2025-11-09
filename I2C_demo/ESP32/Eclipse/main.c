#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "esp_log.h"
#include "i2c_s.h"
#include "driver/i2c.h"
//#include "gpio_confg.h"

static const char TAG[] = "main";

uint8_t data_rd[RW_TEST_LENGTH];

void app_main(void)
{
	i2c_setup();

	ESP_LOGI(TAG, "Configuring GPIO...");
//	gpio_setup();

    while (true) {
    	ESP_ERROR_CHECK(i2c_slave_read_buffer(I2C_PORT, data_rd, RW_TEST_LENGTH, 500 / portTICK_PERIOD_MS));	// pdMS_TO_TICKS(100)
        printf("Received data = %u\n", data_rd[0]);
    }
}
