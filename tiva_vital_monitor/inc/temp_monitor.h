#ifndef TEMP_MONITOR_H
#define TEMP_MONITOR_H

#include "setup.h"
#include "FreeRTOS.h"
#include "queue.h"

/* Application configuration */
#define TEMP_DELAY_MS           500
#define TEMP_THRESHOLD_C        0.3
#define QUEUE_LENGTH            5
#define QUEUE_WAIT              100
#define AVG_WINDOW_SIZE         5
#define LED_BLINK_DURATION_MS   400
#define HEARTBEAT_INTERVAL_MS   160

typedef enum TempState_t
{
    TEMP_STATE_NORMAL,
    TEMP_STATE_WARNING
} TempState_t;

#endif  /* TEMP_MONITOR_H */
