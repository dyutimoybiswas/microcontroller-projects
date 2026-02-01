#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

//-----------------------------------------------------------
// Application specific definitions for TM4C123GXL
// Running at 16 MHz (internal oscillator, no PLL)
//-----------------------------------------------------------

// Scheduler
#define configUSE_PREEMPTION            1
#define configCPU_CLOCK_HZ              ((unsigned long)16000000)
#define configTICK_RATE_HZ              ((TickType_t)1000)
#define configMAX_PRIORITIES            (5)

// Memory
#define configMINIMAL_STACK_SIZE        ((unsigned short)128)   // 512 / sizeof(uint32_t) in startup file
#define configTOTAL_HEAP_SIZE           ((size_t)(16 * 1024))   // 16 KB, half of 32 KB SRAM

// Features used
#define configUSE_COUNTING_SEMAPHORES   1
#define configUSE_16_BIT_TICKS          0   // 32-bit
#define configUSE_IDLE_HOOK             0
#define configUSE_TICK_HOOK             0

// APIs used
#define INCLUDE_vTaskDelay              1
#define INCLUDE_vTaskSuspend            1   // Required for portMAX_DELAY
#define INCLUDE_vTaskPrioritySet        0
#define INCLUDE_uxTaskPriorityGet       0
#define INCLUDE_vTaskDelete             0
#define INCLUDE_vTaskDelayUntil         0

// Cortex-M4F interrupt priorities (TM4C has 3 priority bits = 8 levels)
#define configKERNEL_INTERRUPT_PRIORITY         (7 << 5)    // Lowest priority, occupies bits [7:5] (upper bits)
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    (1 << 5)

#endif  // FREERTOS_CONFIG_H