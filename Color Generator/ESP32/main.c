#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "esp_log.h"
#include "uart_config.h"

static const char TAG[] = "main";

//TODO:
//1. generate_color inside loop to respond to real time changes in UI.
//3. Disable UART when UI not detecting a change in values for 3 seconds. (Watchdog?)

void app_main(void)
{
	ESP_LOGI(TAG, "Configuring UART...");
	uart_setup();

	const char* rgb = "204,253,51";

    while (true) {
    	write_to_uart(rgb);
    	uart_read();
    	sleep(4);
    	break;
    }
}
