#include "temp_monitor.h"
#include "task.h"

void led_setup()
{
    // Set system clock to 16 MHz (internal oscillator)
    SysCtlClockSet(SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // Enable and configure GPIO Port F for RGB LED
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
        // Wait for peripheral to be ready
    }
    SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_AHB_BASE, RED_LED | GREEN_LED | BLUE_LED);
    
    // Turn off all LEDs initially
    GPIOPinWrite(GPIO_PORTF_AHB_BASE, RED_LED | GREEN_LED | BLUE_LED, 0);
}

int main(void)
{
    led_setup();
    
    while (true)
    {
    }
}

