#include "temp_monitor.h"
#include "setup.h"
#include "task.h"

#include <stdio.h>

static double temperatureReadings[AVG_WINDOW_SIZE];
static uint8_t readingIndex = 0;
static uint8_t readingCount = 0;
static QueueHandle_t xTempQueue;
static TaskHandle_t xLedTaskHandle = NULL;
static TaskHandle_t xAdcTaskHandle = NULL;
static bool xIsProcessorHealthy = false;
static volatile uint32_t ulAdcValue = 0;

/* Calculate the moving average temperature */
static double calculateAverageTemperature(void) {
  double sum = 0.0;
  for (uint8_t i = 0; i < readingCount; i++) {
    sum += temperatureReadings[i];
  }
  return readingCount > 0 ? sum / readingCount : 0.0;
}

void ADCIntHandler(void) {
  /* Initialize to 0 */
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  /* Read ADC value */
  ADCSequenceDataGet(ADC0_BASE, ADC_SINGLE_SAMPLE, (uint32_t *)&ulAdcValue);

  /* Clear the ADC interrupt */
  ADCIntClear(ADC0_BASE, ADC_SINGLE_SAMPLE);

  /* Notify the ADC task that conversion is complete */
  vTaskNotifyGiveFromISR(xAdcTaskHandle, &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void vTempReaderTask(void *pvParameters) {
  /* Initialize the last wake time */
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while (true) {
    /* Trigger ADC conversion */
    ADCProcessorTrigger(ADC0_BASE, ADC_SINGLE_SAMPLE);

    /* Wait for ISR to signal conversion complete */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    /* Process temperature value and send to queue */
    double xTemperatureC = (147.5 - ((247.5 * ulAdcValue) / 4096.0));
    xQueueSendToBack(xTempQueue, &xTemperatureC, pdMS_TO_TICKS(QUEUE_WAIT));
    xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TEMP_DELAY_MS));
  }
}

static void vTempProcessorTask(void *pvParameters) {
  bool xIsFirstComparison = true;
  double xPrevAvgTemperatureC = 0.0;
  double xAvgTemperatureC = 0.0;
  double xRawTemperatureC;
  char buffer[50];
  TempState_t ePrevState = TEMP_STATE_NORMAL;
  TempState_t eCurrentState;

  while (true) {
    if (xQueueReceive(xTempQueue, &xRawTemperatureC, portMAX_DELAY) == pdPASS) {
      /* Store the latest reading in the circular buffer */
      temperatureReadings[readingIndex] = xRawTemperatureC;
      readingIndex = (readingIndex + 1) % AVG_WINDOW_SIZE;
      if (readingCount < AVG_WINDOW_SIZE) {
        readingCount++;
      }

      /* Calculate average temperature */
      xAvgTemperatureC = calculateAverageTemperature();

      /* Skip first comparison to establish baseline */
      if (xIsFirstComparison) {
        xPrevAvgTemperatureC = xAvgTemperatureC;
        xIsFirstComparison = false;
        continue;
      }

      double tempChange = xAvgTemperatureC - xPrevAvgTemperatureC;
      tempChange =
          tempChange < 0 ? -tempChange : tempChange; /* Absolute value */
      eCurrentState = tempChange >= TEMP_THRESHOLD_C ? TEMP_STATE_WARNING
                                                     : TEMP_STATE_NORMAL;

      /* Log temperature change if state has changed */
      if (eCurrentState != ePrevState) {
        snprintf(buffer, sizeof(buffer), "Temperature delta: %.2f °C → %s\r\n",
                 tempChange,
                 eCurrentState == TEMP_STATE_WARNING ? "WARNING" : "NORMAL");
        uart_print(buffer);
      }

      /* Update previous temperature and state */
      xPrevAvgTemperatureC = xAvgTemperatureC;
      ePrevState = eCurrentState;

      /* Update processor health status */
      xIsProcessorHealthy = true;

      /* Notify LED task */
      if (eCurrentState == TEMP_STATE_WARNING) {
        xTaskNotify(xLedTaskHandle, 1, eSetValueWithOverwrite);
      } else {
        xTaskNotify(xLedTaskHandle, 0, eSetValueWithOverwrite);
      }
    }
  }
}

static void vLedActionTask(void *pvParameters) {
  uint32_t ulNotificationValue = 0;

  while (true) {
    /* Block until notification arrives */
    xTaskNotifyWait(0, 0, &ulNotificationValue, portMAX_DELAY);

    while (ulNotificationValue) {
      GPIOPinWrite(GPIO_PORTF_AHB_BASE, RED_LED, RED_LED);
      vTaskDelay(pdMS_TO_TICKS(LED_BLINK_DURATION_MS));
      GPIOPinWrite(GPIO_PORTF_AHB_BASE, RED_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(LED_BLINK_DURATION_MS));

      /* Non-blocking check for state change */
      xTaskNotifyWait(0, 0, &ulNotificationValue, 0);
    }

    /* Turn off LED for normal state */
    GPIOPinWrite(GPIO_PORTF_AHB_BASE, RED_LED, 0);
  }
}

static void vHeartbeatTask(void *pvParameters) {
  while (true) {
    /* Kick watchdog if processor is healthy (working) */
    if (xIsProcessorHealthy) {
      xIsProcessorHealthy = false; /* Reset health status for next cycle */
      WatchdogReloadSet(WATCHDOG0_BASE, SYSTEM_CLOCK_HZ);
    }

    /* Heartbeat pattern */
    GPIOPinWrite(GPIO_PORTF_AHB_BASE, GREEN_LED, GREEN_LED);
    vTaskDelay(pdMS_TO_TICKS(HEARTBEAT_INTERVAL_MS));
    GPIOPinWrite(GPIO_PORTF_AHB_BASE, GREEN_LED, 0);
    vTaskDelay(pdMS_TO_TICKS(HEARTBEAT_INTERVAL_MS));
    GPIOPinWrite(GPIO_PORTF_AHB_BASE, GREEN_LED, GREEN_LED);
    vTaskDelay(pdMS_TO_TICKS(HEARTBEAT_INTERVAL_MS));
    GPIOPinWrite(GPIO_PORTF_AHB_BASE, GREEN_LED, 0);
    vTaskDelay(pdMS_TO_TICKS(HEARTBEAT_INTERVAL_MS));

    /* Pause before next heartbeat */
    vTaskDelay(pdMS_TO_TICKS(HEARTBEAT_INTERVAL_MS * 3 / 2));
  }
}

static void logResetCause(void) {
  uint32_t cause = SysCtlResetCauseGet();
  SysCtlResetCauseClear(cause);

  uart_print("Reset cause: ");

  /* Printed on power cycle */
  if (cause & SYSCTL_CAUSE_POR) {
    uart_print("POWER-ON");
  }

  /* Printed on watchdog reset (e.g. updating HEARTBEAT_INTERVAL_MS) */
  if (cause & SYSCTL_CAUSE_WDOG0) {
    uart_print("WATCHDOG");
  }

  if (cause & SYSCTL_CAUSE_BOR) {
    uart_print("BROWN-OUT");
  }

  /* Printed on external reset (e.g. pressing reset button) */
  if (cause & SYSCTL_CAUSE_EXT) {
    uart_print("EXTERNAL");
  }

  /* Printed on software reset (e.g. flashing binary) */
  if (cause & SYSCTL_CAUSE_SW) {
    uart_print("SOFTWARE");
  }

  if (cause & SYSCTL_CAUSE_HIB) {
    uart_print("HIBERNATE");
  }

  uart_print("\r\n");
}

int main(void) {
  /* Set system clock to 16 MHz (internal oscillator) */
  SysCtlClockSet(SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

  /* Initialize peripherals */
  watchdog_setup();
  led_setup();
  adc_setup();
  uart_setup();

  /* Clear serial monitor */
  uart_print("\033[2J\033[H");

  /* ~3s delay for serial monitor connection and ADC stabilization */
  SysCtlDelay(SYSTEM_CLOCK_HZ);

  /* Log the cause of last reset */
  logResetCause();

  /* Arm watchdog after startup delay */
  watchdog_start();

  /* Create queue for temperature data */
  xTempQueue = xQueueCreate(QUEUE_LENGTH, sizeof(double));

  /* Create tasks - need larger stack for snprintf */
  xTaskCreate(vHeartbeatTask, "Heartbeat", configMINIMAL_STACK_SIZE, NULL, 1,
              NULL);
  xTaskCreate(vTempReaderTask, "Temperature Reader", 256, NULL, 2,
              &xAdcTaskHandle);
  xTaskCreate(vTempProcessorTask, "Temperature Processor", 256, NULL, 2, NULL);
  xTaskCreate(vLedActionTask, "LED Action", configMINIMAL_STACK_SIZE, NULL, 2,
              &xLedTaskHandle);

  /* Start the scheduler */
  vTaskStartScheduler();

  while (true) {
    /* Should never reach here */
  }
}
