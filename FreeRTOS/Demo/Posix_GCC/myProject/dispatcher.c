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

        if (xCheckPdPASS(xStatusReceive, "Dispatcher Task", logRECEIVE_CODE)) {
            for (int i = 0; i < DEPARTMENTS_AMOUNT; i++) { 
                if (sEventCode == pxDepartments[i].id) {
                    xStatusSend = xQueueSend(pxDepartments[i].queue, &sEventCode, portMAX_DELAY);
                    break;
                }
            }

            xCheckPdPASS(xStatusSend, "Dispatcher Task", logSEND_CODE);
        }

        vTaskDelayUntil( &xLastWakeTime, dispatcherDELAY );
    }
}