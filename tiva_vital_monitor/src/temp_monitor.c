#include "temp_monitor.h"
#include "task.h"
#include <stdio.h>

static bool xIsFirstReading = true;
static QueueHandle_t xTempQueue;
static TickType_t xLastWakeTime;

static void uart_print(const char *str)
{
    while (*str)
    {
        UARTCharPut(UART0_BASE, *str++);
    }
}

static void vTempReaderTask(void *pvParameters)
{
    uint32_t ulAdcValue;

    /* Initialize the last wake time */
    xLastWakeTime = xTaskGetTickCount();
    
    while (true)
    {
        /* Trigger ADC conversion */
        ADCProcessorTrigger(ADC0_BASE, ADC_SINGLE_SAMPLE);
        
        /* Wait for conversion to complete */
        while (!ADCIntStatus(ADC0_BASE, ADC_SINGLE_SAMPLE, false))
        {
            /* Wait for ADC conversion to complete */
        }
        
        /* Clear ADC interrupt */
        ADCIntClear(ADC0_BASE, ADC_SINGLE_SAMPLE);
        
        /* Read ADC value */
        ADCSequenceDataGet(ADC0_BASE, ADC_SINGLE_SAMPLE, &ulAdcValue);
        
        /* Process temperature value and send to queue */
        double xTemperatureC = (147.5 - ((247.5 * ulAdcValue) / 4096.0));
        xQueueSendToBack(xTempQueue, &xTemperatureC, pdMS_TO_TICKS(QUEUE_WAIT));
        xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TEMP_DELAY_MS));
    }
}

static void vTempProcessorTask(void *pvParameters)
{
    double xPrevTemperatureC;
    double xTemperatureC;
    char buffer[50];
    TempState_t ePrevState = TEMP_STATE_NORMAL;
    TempState_t eCurrentState;

    while (true)
    {
        if (xQueueReceive(xTempQueue, &xTemperatureC, portMAX_DELAY) == pdPASS)
        {
            /* Log base temperature */
            if (xIsFirstReading)
            {
                snprintf(buffer, sizeof(buffer), "Base Temperature: %.2f °C\r\n", xTemperatureC);
                uart_print(buffer);
                xIsFirstReading = false;
                eCurrentState = TEMP_STATE_NORMAL;
            }
            else
            {
                double tempChange = xTemperatureC - xPrevTemperatureC;
                tempChange = tempChange < 0 ? -tempChange : tempChange;  /* Absolute value */
                eCurrentState = tempChange >= TEMP_THRESHOLD_C ? TEMP_STATE_WARNING : TEMP_STATE_NORMAL;
                
                /* Log temperature change if state has changed */
                if (eCurrentState != ePrevState)
                {
                    snprintf(buffer, sizeof(buffer), "Temperature delta: %.2f °C → %s\r\n", tempChange,
                             eCurrentState == TEMP_STATE_WARNING ? "WARNING" : "NORMAL");
                    uart_print(buffer);

                    /* Toggle red LED */
                    GPIOPinWrite(GPIO_PORTF_AHB_BASE, RED_LED, eCurrentState == TEMP_STATE_WARNING ? RED_LED : 0);
                }
            }

            /* Update previous temperature and state */
            xPrevTemperatureC = xTemperatureC;
            ePrevState = eCurrentState;
        }
    }
}
    

int main(void)
{
    /* Set system clock to 16 MHz (internal oscillator) */
    SysCtlClockSet(SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    led_setup();
    adc_setup();
    uart_setup();

    /* Clear serial monitor */
    uart_print("\033[2J\033[H");

    /* Create queue for temperature data */
    xTempQueue = xQueueCreate(QUEUE_LENGTH, sizeof(double));

    /* Create tasks - need larger stack for snprintf */
    xTaskCreate(vTempReaderTask, "Temperature Reader", 256, NULL, 1, NULL);
    xTaskCreate(vTempProcessorTask, "Temperature Processor", 256, NULL, 1, NULL);

    /* Start the scheduler */
    vTaskStartScheduler();
    
    while (true)
    {
    }
}

