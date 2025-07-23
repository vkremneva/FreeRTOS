#include "dispatcher.h"

void vDispatcherTask(void *pvParameters) {
    department_t *pxDepartments = (department_t*)pvParameters;

    TickType_t xLastWakeTime = 0, xTimestamp = 0;
    BaseType_t xStatusReceive = 0, xStatusSend = 0;

    EventBits_t xEventGroupValue = 0;

    // TODO still not for n departments
    const EventBits_t xBitsToWaitFor = ( 
        deptAMBULANCE_AVAILABLE | \
        deptPOLICE_AVAILABLE | \
        deptFIRE_AVAILABLE | \
        deptCORONA_AVAILABLE 
    );

    event_t xEvent = {0, false};

    xLastWakeTime = xTaskGetTickCount();
    bool xFreeResourceFound = false;

    for ( ;; ) {
        xStatusReceive = xQueueReceive(xQueueEvents, &xEvent, portMAX_DELAY);

        if (xStatusReceive == pdPASS) {
            if (!xEvent.xRejected) {
                xStatusSend = xQueueSend(pxDepartments[xEvent.ucCode].xQueue, &xEvent, portMAX_DELAY);
                xTimestamp = xTaskGetTickCount();

                addToCSVLog(&log_to_csv, xTimestamp, xEvent.ucCode, "Dispatcher Task");
            } else {
                xEventGroupValue = xEventGroupWaitBits(
                    xDepartmentEventGroup,
                    xBitsToWaitFor,
                    pdFALSE,
                    pdFALSE,
                    deptMAX_GROUP_WAIT_TIME 
                );

                xFreeResourceFound = false;
                for (int i = 0; i < uxDepartmentsAmount; ++i) {
                    /* Checking the availability of free resources in each 
                     * department, starting from department with the lowest 
                     * priority up to the highest priority department. */
                    if( ( xEventGroupValue & pxDepartments[uxDeptPriorityOrder[i]].bits_available )) {
                        xFreeResourceFound = true;

                        xStatusSend = xQueueSend(pxDepartments[uxDeptPriorityOrder[i]].xQueue, &xEvent, portMAX_DELAY);
                        xTimestamp = xTaskGetTickCount();
                        
                        /* For debug puposes only. */
                        printf("*********EVENT %d SENT TO %d\n", xEvent.ucCode, i + 1);
                        addToCSVLog(&log_to_csv, xTimestamp, xEvent.ucCode, "Dispatcher Task");
                        break;
                    }
                }

                if (!xFreeResourceFound) {
                    vLogNoResourceAvailable("Dispatcher");

                    xStatusSend = xQueueSendToFront(xQueueEvents, &xEvent, portMAX_DELAY);
                    if (xStatusSend == errQUEUE_FULL) {
                        vLogQueueSendError("xQueueEvents");
                    }

                    /* For debug puposes only. */
                    printf("**********NO FREE RESOURCE FOUND\n");
                }

            }
        }

        vTaskDelayUntil( &xLastWakeTime, dispDELAY );
    }
}
