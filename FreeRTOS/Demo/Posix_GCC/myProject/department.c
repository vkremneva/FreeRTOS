#include "department.h"

void vDepartmentTask( void *pvParameters ) {
    department_t *xDepartment = (department_t *)pvParameters;
    BaseType_t xStatusReceive = 0, xStatusTake = 0, xStatusSend = 0;
    TickType_t xUsageStartTime = 0;
    int16_t sEventCode = 0;
    resource_request_t xRequest = { 0, 0 };

    for ( ;; ) {
        xStatusReceive = xQueueReceive( xDepartment->queue, &sEventCode, portMAX_DELAY ); 
        if (xStatusReceive == pdPASS) {

            xUsageStartTime = xTaskGetTickCount();
            
            xStatusTake = xSemaphoreTake( xDepartment->countingSemaphore, portMAX_DELAY );
            if (xStatusTake == pdPASS) {
                
                xRequest.department_name = xDepartment->name;
                xRequest.event_code = sEventCode;
                xRequest.usage_start_time = xUsageStartTime;
                xRequest.xDepartmentSemaphore = xDepartment->countingSemaphore;

                xStatusSend = xQueueSend(xQueueResources, &xRequest, portMAX_DELAY);
            } else {
                logNoResourceAvailable( xDepartment->name );
            }
        }
    }
}