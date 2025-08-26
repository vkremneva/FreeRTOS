#include "include/resource.h"

QueueHandle_t xQueueResources;

void vResourceTask( void * pvParameters )
{
    TickType_t xTicksToWait = 0;
    BaseType_t xStatusReceive = 0, xStatusSend = 0;

    resource_request_t xRequest = { NULL, 0, 0, 0, NULL };
    event_t xFreeResource = { 0, false, eventsFREE_RESOURCE, 0 };

    for( ;; )
    {
        xStatusReceive = xQueueReceive( xQueueResources, &xRequest, portMAX_DELAY );

        if( xStatusReceive == pdPASS )
        {
            /* For debug puposes only. */
            addToCSVLog( &log_to_csv, xTaskGetTickCount(), xRequest.ucEventCode, pcTaskGetTaskName( NULL ));

            xTicksToWait = pdMS_TO_TICKS( resourceUSE_MIN_TIME + rand() % ( resourceUSE_MAX_TIME - resourceUSE_MIN_TIME + 1 ));
            vTaskDelay( xTicksToWait );

            xFreeResource.ucCode = xRequest.ucEventCode;
            xFreeResource.xUsageStartTime = xRequest.xUsageStartTime;

            xStatusSend = xQueueSendToFront( xRequest.xDepartmentQueue, &xFreeResource, portMAX_DELAY );

            if( xStatusSend == errQUEUE_FULL )
            {
                vLogQueueSendError( xRequest.psDepartmentName );
            }

            /* For debug puposes only. */
            addToCSVLog( &log_to_csv, xTaskGetTickCount(), xRequest.ucEventCode, pcTaskGetTaskName( NULL ));
        }
    }
}
