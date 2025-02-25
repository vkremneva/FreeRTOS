#include "resource.h"

QueueHandle_t xQueueResources;
SemaphoreHandle_t xResourcesSemaphore;

void vResourceTask( void *pvParameters ) {
    resource_request_t xRequest;
    TickType_t xUsageEndTime = 0;

    BaseType_t xStatusReceive = 0, xStatusGive = 0;

    for ( ;; ) {
        xStatusReceive = xQueueReceive( xQueueResources, &xRequest, portMAX_DELAY );
        if (xStatusReceive == pdPASS) {

            TickType_t xTicksToWait = pdMS_TO_TICKS( rand() % resourceUSE_MAX_TIME + resourceUSE_MIN_TIME );
            printf("Resource Task: Received request from the %s, processing in %d ticks\n", xRequest.department_name, xTicksToWait);
            vTaskDelay( xTicksToWait );

            xStatusGive = xSemaphoreGive( xRequest.xDepartmentSemaphore );
            if (xStatusGive == pdPASS) {
                xUsageEndTime = xTaskGetTickCount(); 
                logDepartmentUsage( xRequest.department_name, 
                                    xRequest.event_code, 
                                    ( xUsageEndTime - xRequest.usage_start_time ) 
                );
            }
        }
    }
}