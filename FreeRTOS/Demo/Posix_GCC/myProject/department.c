#include "department.h"

void vUseResource(void *pvParameters) {
    // TODO log time and department name in the same log message
    SemaphoreHandle_t xCountingSemaphore = (SemaphoreHandle_t)pvParameters;
    BaseType_t xStatusGive = 0;
    TickType_t xTicksToWait = pdMS_TO_TICKS( rand() % 3000 + 500 );

    vTaskDelay( xTicksToWait );

    xStatusGive = xSemaphoreGive(xCountingSemaphore);
    xCheckPdPASS(xStatusGive, "Using the resource", logGIVE_SEMAPHORE);
    // printf("Using Resource: FINISHED in %d ticks\n", xTicksToWait);
}

void vDepartmentTask(void *pvParameters) {
    department_t *xDepartment = (department_t *)pvParameters;

    BaseType_t xStatusReceive = 0, xStatusTake = 0;
    int16_t sEventCode = 0;

    for ( ;; ) {
        xStatusReceive = xQueueReceive(xDepartment->queue, &sEventCode, portMAX_DELAY);
        if (xCheckPdPASS(xStatusReceive, xDepartment->name, logRECEIVE_CODE)) {

            xStatusTake = xSemaphoreTake(xDepartment->countingSemaphore, portMAX_DELAY);
            if (xCheckPdPASS(xStatusTake, xDepartment->name, logTAKE_SEMAPHORE)) {

                vUseResource((void *)xDepartment->countingSemaphore);
            }
        }
    }
}