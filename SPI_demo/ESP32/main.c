#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "esp_clk_tree.h"
#include "spi_m.h"

static const char TAG[] = "main";

void app_main(void)
{
	// logging
	ESP_LOGI(TAG, "Inside app_main...");

	// capture clock speed
	uint32_t clk_value;
	errCode = esp_clk_tree_src_get_freq_hz(SOC_MOD_CLK_APB,
			ESP_CLK_TREE_SRC_FREQ_PRECISION_APPROX, &clk_value);
	ESP_ERROR_CHECK(errCode);
	ESP_LOGI(TAG, "Clock frequency = %lu MHz", (clk_value / MHz));

	// configure SPI master
	configure();

    while (1) {
    	transmit();
    	receive();
    	sleep(2);
    }
}
