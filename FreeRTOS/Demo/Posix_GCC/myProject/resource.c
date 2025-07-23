#include "resource.h"

QueueHandle_t xQueueResources;
SemaphoreHandle_t xResourcesSemaphore;

void vResourceTask( void *pvParameters ) {
    resource_request_t xRequest;
    TickType_t xUsageEndTime = 0, xTimestamp = 0;

    BaseType_t xStatusReceive = 0, xStatusGive = 0;

    for ( ;; ) {
        xStatusReceive = xQueueReceive( xQueueResources, &xRequest, portMAX_DELAY );
        if (xStatusReceive == pdPASS) {
            /* For debug puposes only. */
            xTimestamp = xTaskGetTickCount();
            addToCSVLog(&log_to_csv, xTimestamp, xRequest.ucEventCode, pcTaskGetTaskName(NULL));
            /* End of debug purposes. */

            TickType_t xTicksToWait = pdMS_TO_TICKS( rand() % resourceUSE_MAX_TIME + resourceUSE_MIN_TIME );
            printf("%s", pcTaskGetTaskName(NULL));
            printf(": Received request from the %s, processing in %d ticks\n", xRequest.psDepartmentName, xTicksToWait);
            vTaskDelay( xTicksToWait );

            xStatusGive = xSemaphoreGive( xRequest.pxDepartmentSemaphore );
            if (xStatusGive == pdPASS) {
                xUsageEndTime = xTaskGetTickCount(); 
                vLogDepartmentUsage( xRequest.psDepartmentName, 
                                     xRequest.ucEventCode, 
                                    ( xUsageEndTime - xRequest.xUsageStartTime ) 
                );
            }
            xTimestamp = xTaskGetTickCount();

            /* For debug puposes only. */
            addToCSVLog(&log_to_csv, xTimestamp, xRequest.ucEventCode, pcTaskGetTaskName(NULL));
        }
    }
}
