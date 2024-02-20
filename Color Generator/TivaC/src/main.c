#include <stdbool.h>
#include <stdint.h>
#include "color_generator.h"
#include "io_handler.h"
#include "utils.h"

static uint32_t counter = 0;
static char msg[13];
static int rgb_values[] = {0, 0, 0};
static char data;

int main(void)
{
	sysclk_setup();
	uart_setup();
	pwm_setup();
	
	while(true)
	{
		data = uart_read();
		msg[counter++] = data;
		
		if(data == NULL_CHAR)
		{
			/* echo received info to ESP32 and parse received data. */
			write_to_uart(msg);
			counter = 0;
			parse_input(rgb_values, msg);
			generate_color(rgb_values[0], rgb_values[1], rgb_values[2]);
		}
	}
}
