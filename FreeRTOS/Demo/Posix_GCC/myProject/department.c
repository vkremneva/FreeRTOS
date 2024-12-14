#include "department.h"

department_t xPolice = { deptPOLICE_ID, deptPOLICE_CARS_TOTAL, deptPOLICE_CARS_TOTAL, 0, 0};
QueueHandle_t xPoliceQueue = NULL;
SemaphoreHandle_t xPoliceCountingSemaphore = NULL;

void vUseResource(void *param) {
    SemaphoreHandle_t xCountingSemaphore = (SemaphoreHandle_t)param;
    BaseType_t xStatusGive = 0;

    printf("Using Resource: STARTED\n");
    vTaskDelay(pdMS_TO_TICKS(500));

    xStatusGive = xSemaphoreGive(xCountingSemaphore);
    if (xStatusGive == pdPASS) {
        printf("Using Resource: FINISHED\n");
    } else {
        printf("Using Resource: semaphore ERROR\n");
    }
}

void vDeptPoliceTask(void *param) {
    BaseType_t xStatusReceive = 0, xStatusTake = 0;
    int16_t sEventCode = 0;

    for ( ;; ) {
        xStatusReceive = xQueueReceive(xPoliceQueue, &sEventCode, portMAX_DELAY);

        if (xStatusReceive == pdPASS) {
            printf("Police Task: Receiving the code SUCCESS: code is: %d\n", sEventCode);
            xStatusTake = xSemaphoreTake(xPoliceCountingSemaphore, portMAX_DELAY);
            if (xStatusTake == pdPASS) {
                printf("Police Task: semaphore taking SUCCESS\n");
                vUseResource((void *)xPoliceCountingSemaphore);
            }
        } else if (xStatusReceive == errQUEUE_EMPTY) {
            printf("Police Task: Receiving the code FAILURE: queue is empty\n");
        } else {
            // TODO to log
            printf("Police Task: Receiving the code FAILURE: queue error\n");
        }
    }
}