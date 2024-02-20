#include <stdbool.h>
#include "color_generator.h"


void sysclk_setup(void)
{
		// set system clock frequency
    SysCtlClockSet(SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
}

// Note: D = 0 and D = 1 cannot be generated with PWM
void pwm_setup(void)
{
		// set PWM clock frequency
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    // Port F GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
        // wait for peripheral to be enabled
    }

    // Port F PWM
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1))
    {
        // wait for peripheral to be enabled
    }
}

void generate_color(int red_value, int green_value, int blue_value)
{	
    /* configure red. */
		GPIOPinConfigure(GPIO_PF1_M1PWM5);
    GPIOPinTypePWM(GPIO_PORTF_BASE, RED_LED);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, SysCtlClockGet() / FREQ);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, (uint32_t)red_value);
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
	
		/* configure green. */
    GPIOPinConfigure(GPIO_PF3_M1PWM7);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GREEN_LED);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, SysCtlClockGet() / FREQ);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, (uint32_t)green_value);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);
    PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
    
		/* configure blue. */
    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinTypePWM(GPIO_PORTF_BASE, BLUE_LED);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, SysCtlClockGet() / FREQ);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, (uint32_t)blue_value);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);
    PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
}
