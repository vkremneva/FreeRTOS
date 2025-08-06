#include "resource.h"

QueueHandle_t xQueueResources;
SemaphoreHandle_t xResourcesSemaphore;

void vResourceTask( void *pvParameters ) {
    resource_request_t xRequest;

    TickType_t xUsageEndTime = 0, xTicksToWait = 0;
    BaseType_t xStatusReceive = 0, xStatusSend = 0;

    event_t xFreeResource = { 0, false, eventFREE_RESOURCE };

    for ( ;; ) {
        xStatusReceive = xQueueReceive( xQueueResources, &xRequest, portMAX_DELAY );
        if (xStatusReceive == pdPASS) {
            /* For debug puposes only. */
            addToCSVLog(&log_to_csv, xTaskGetTickCount(), xRequest.ucEventCode, pcTaskGetTaskName(NULL));
            
            //xTicksToWait = pdMS_TO_TICKS( (rand() % (resourceUSE_MAX_TIME - resourceUSE_MIN_TIME + 1)) + resourceUSE_MIN_TIME);
            xTicksToWait = pdMS_TO_TICKS( rand() % resourceUSE_MAX_TIME + resourceUSE_MIN_TIME);
            vTaskDelay( xTicksToWait );

            xFreeResource.ucCode = xRequest.ucDepartmentID;
            
            xStatusSend = xQueueSendToFront(xRequest.xDepartmentQueue, &xFreeResource, portMAX_DELAY);
            if (xStatusSend == errQUEUE_FULL) {
                vLogQueueSendError(xRequest.psDepartmentName);
            } 

            /* For debug puposes only. */
            addToCSVLog(&log_to_csv, xTaskGetTickCount(), xRequest.ucEventCode, pcTaskGetTaskName(NULL));
        }
    }
}
