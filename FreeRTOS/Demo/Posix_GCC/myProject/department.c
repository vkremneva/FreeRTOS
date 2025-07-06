#include "department.h"

EventGroupHandle_t xDepartmentEventGroup = NULL;

void vDepartmentTask( void *pvParameters ) {
    department_t *xDepartment = (department_t *)pvParameters;
    BaseType_t xStatusReceive = 0, xStatusTake = 0, xStatusSend = 0;
    TickType_t xUsageStartTime = 0, xTimestamp = 0;
    int16_t sEventCode = 0;
    EventBits_t xEventGroupValue = 0;
    resource_request_t xRequest = { NULL, 0, 0, NULL };
    const EventBits_t xBitsToWaitFor = ( 
        deptAMBULANCE_AVAILABLE | \
        deptPOLICE_AVAILABLE | \
        deptFIRE_AVAILABLE | \
        deptCORONA_AVAILABLE 
    );

    for ( ;; ) {
        xStatusReceive = xQueueReceive( xDepartment->queue, &sEventCode, portMAX_DELAY ); 
        if (xStatusReceive == pdPASS) {
            xTimestamp = xTaskGetTickCount();
            addToCSVLog(&log, xTimestamp, sEventCode, xDepartment->name);

            xUsageStartTime = xTaskGetTickCount();

            xDepartment->cars_available = uxSemaphoreGetCount( xDepartment->countingSemaphore );

            if (xDepartment->cars_available == 0) {
                logNoResourceAvailable( xDepartment->name );

                // now this department can't give its resources to anybody
                xEventGroupClearBits(xDepartmentEventGroup, (1 << xDepartment->id));

                xEventGroupValue = xEventGroupWaitBits(
                    xDepartmentEventGroup,
                    xBitsToWaitFor,
                    pdFALSE,
                    pdFALSE,
                    deptMAX_GROUP_WAIT_TIME 
                );

                if( ( xEventGroupValue & deptAMBULANCE_AVAILABLE ) != 0 ) {
                    printf("Ambulance has available cars\n");
                }
                if( ( xEventGroupValue & deptPOLICE_AVAILABLE ) != 0 ) {
                    printf("Police has available cars\n");
                }
                if( ( xEventGroupValue & deptFIRE_AVAILABLE ) != 0 ) {
                    printf("Firefighters have available cars\n");
                }
                if( ( xEventGroupValue & deptCORONA_AVAILABLE ) != 0 ) {
                    printf("Corona has available cars\n");
                }

            } else {
                xEventGroupSetBits(xDepartmentEventGroup, (1 << xDepartment->id));
            }

            xStatusTake = xSemaphoreTake( xDepartment->countingSemaphore, portMAX_DELAY );
            if (xStatusTake == pdPASS) {
            
                xRequest.department_name = xDepartment->name;
                xRequest.event_code = sEventCode;
                xRequest.usage_start_time = xUsageStartTime;
                xRequest.xDepartmentSemaphore = xDepartment->countingSemaphore;

                xStatusSend = xQueueSend(xQueueResources, &xRequest, portMAX_DELAY);
                if (xStatusSend == errQUEUE_FULL) {
                    logQueueSendError("xQueueResources");
                }
            }
        }
    }
}