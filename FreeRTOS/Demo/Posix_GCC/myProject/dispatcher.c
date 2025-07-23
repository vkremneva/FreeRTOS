#include "dispatcher.h"

void vDispatcherTask(void *pvParameters) {
    department_t *pxDepartments = (department_t*)pvParameters;
    TickType_t xLastWakeTime = 0, xTimestamp = 0;
    BaseType_t xStatusReceive = 0, xStatusSend = 0;

    EventBits_t xEventGroupValue = 0;
    const EventBits_t xBitsToWaitFor = ( 
        deptAMBULANCE_AVAILABLE | \
        deptPOLICE_AVAILABLE | \
        deptFIRE_AVAILABLE | \
        deptCORONA_AVAILABLE 
    );

    event_t xEvent = {0, false};

    // get the current tick count
    xLastWakeTime = xTaskGetTickCount();
    bool free_resource_found = false;

    for ( ;; ) {
        xStatusReceive = xQueueReceive(xQueueEvents, &xEvent, portMAX_DELAY);

        if (xStatusReceive == pdPASS) {
            if (!xEvent.rejected) {
                xStatusSend = xQueueSend(pxDepartments[xEvent.code].queue, &xEvent, portMAX_DELAY);
                xTimestamp = xTaskGetTickCount();

                addToCSVLog(&log_to_csv, xTimestamp, xEvent.code, "Dispatcher Task");
            } else {
                xEventGroupValue = xEventGroupWaitBits(
                    xDepartmentEventGroup,
                    xBitsToWaitFor,
                    pdFALSE,
                    pdFALSE,
                    deptMAX_GROUP_WAIT_TIME 
                );

                free_resource_found = false;
                for (int i = 0; i < xDepartmentsAmount; ++i) {
                    if( ( xEventGroupValue & pxDepartments[xIndByPriority[i]].bits_available )) {
                        free_resource_found = true;

                        xStatusSend = xQueueSend(pxDepartments[xIndByPriority[i]].queue, &xEvent, portMAX_DELAY);
                        xTimestamp = xTaskGetTickCount();
                        
                        // debug output
                        printf("*********EVENT %d SENT TO %d\n", xEvent.code, i + 1);
                        addToCSVLog(&log_to_csv, xTimestamp, xEvent.code, "Dispatcher Task");
                        break;
                    }
                }

                if (!free_resource_found) {
                    logNoResourceAvailable("Dispatcher");

                    xStatusSend = xQueueSendToFront(xQueueEvents, &xEvent, portMAX_DELAY);
                    if (xStatusSend == errQUEUE_FULL) {
                        logQueueSendError("xQueueEvents");
                    }

                    // debug output
                    printf("**********NO FREE RESOURCE FOUND\n");
                }

            }
        }

        vTaskDelayUntil( &xLastWakeTime, dispatcherDELAY );
    }
}