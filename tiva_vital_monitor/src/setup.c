#include "setup.h"

void led_setup(void)
{
    /* Enable and configure GPIO Port F for RGB LED */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
        /* Wait for peripheral to be ready */
    }
    SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_AHB_BASE, RED_LED | GREEN_LED | BLUE_LED);
    
    /* Turn off all LEDs initially */
    GPIOPinWrite(GPIO_PORTF_AHB_BASE, RED_LED | GREEN_LED | BLUE_LED, 0);
}

void adc_setup(void)
{
    /* Enable ADC0 peripheral */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0))
    {
        /* Wait for peripheral to be ready */
    }

    /* Configure ADC sequence 3 for temperature sensor */
    ADCSequenceConfigure(ADC0_BASE, ADC_SINGLE_SAMPLE, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, ADC_SINGLE_SAMPLE, 0, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, ADC_SINGLE_SAMPLE);
}

void uart_setup(void)
{
    /* Enable GPIO Port A */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
        /* Wait for peripheral to be ready */
    }
    
    /* Enable UART0 peripheral */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0))
    {
        /* Wait for peripheral to be ready */
    }

    /* Configure RX and TX for UART */
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    /* Configure UART0 for console output */
    UARTConfigSetExpClk(UART0_BASE, SYSTEM_CLOCK_HZ, BAUD_RATE,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    
    /* Enable UART0 */
    UARTEnable(UART0_BASE);
}
