#include "dispatcher.h"

void vDispatcherTask(void *pvParameters) {
    department_t *pxDepartments = (department_t*)pvParameters;
    TickType_t xLastWakeTime;
    BaseType_t xStatusReceive = 0, xStatusSend = 0;
    int16_t sEventCode = 0;

    // get the current tick count
    xLastWakeTime = xTaskGetTickCount();

    for ( ;; ) {
        xStatusReceive = xQueueReceive(xQueueEvents, &sEventCode, portMAX_DELAY);
        if (xStatusReceive == pdPASS) {
            xStatusSend = xQueueSend(pxDepartments[sEventCode].queue, &sEventCode, portMAX_DELAY);
        }

        vTaskDelayUntil( &xLastWakeTime, dispatcherDELAY );
    }
}