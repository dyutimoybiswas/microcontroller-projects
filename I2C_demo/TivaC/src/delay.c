#include "delay.h"
#include "i2c_config.h"

void systick_handler(void)
{
    /* Slave address is automatically sent before each transmission. */
		/* Turn on external LED, turn off after some delay. */
		i2c_write(on_off_flag, RUN | START | STOP);
		on_off_flag = !on_off_flag;
}

void systick_setup(void)
{
    SysTickPeriodSet(LOAD_VALUE);
    SysTickIntRegister(systick_handler);
    SysTickIntEnable();
    SysTickEnable();
}
