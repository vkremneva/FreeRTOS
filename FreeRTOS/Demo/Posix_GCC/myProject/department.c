#include "department.h"

/*-----------------------------------------------------------*/
EventBits_t uxBitsAvailableAll = 0;
EventGroupHandle_t xDepartmentEventGroup = NULL;

UBaseType_t uxDepartmentsAmount = 0;
UBaseType_t uxResourcesAmount = 0;

UBaseType_t uxDeptPriorityOrder[deptMAX_DEPARTMENTS_AMOUNT];

department_t* pxInitDepartments() {
    /* static is possible here because each department task will have access
     * only to its own element in xDepartments[]. Adding "Null" department 
     * to maintain deptartment.id = generated event code. */
    static department_t xDepartments[] = {
        {"Null", 0, 1, 0, 0, 0, 0, NULL},
        {"Police", deptPOLICE_ID, deptPOLICE_PRIORITY, deptPOLICE_CARS_TOTAL, deptPOLICE_CARS_TOTAL, 0, 0, NULL},
        {"Ambulance", deptAMBULANCE_ID, deptAMBULANCE_PRIORITY, deptAMBULANCE_CARS_TOTAL, deptAMBULANCE_CARS_TOTAL, 0, 0, NULL},
        {"Firefighters", deptFIREFIGHTERS_ID, deptFIREFIGHTERS_PRIORITY, deptFIREFIGHTERS_CARS_TOTAL, deptFIREFIGHTERS_CARS_TOTAL, 0, 0, NULL},
        {"Corona", deptCORONA_ID, deptCORONA_PRIORITY, deptCORONA_CARS_TOTAL, deptCORONA_CARS_TOTAL, 0, 0, NULL}
    };

    /* Minus 1 here to exclude "Null" department from total. */
    uxDepartmentsAmount = (sizeof(xDepartments) / sizeof(xDepartments[0])) - 1; 

    uxResourcesAmount = 0;
    uxBitsAvailableAll = 0;
    for (int i = 1; i <= uxDepartmentsAmount; ++i) {
        xDepartments[i].uxBitsAvailable = ( 1 << xDepartments[i].ucID );

        uxResourcesAmount += xDepartments[i].uxCarsTotal;
        uxBitsAvailableAll |= xDepartments[i].uxBitsAvailable;

        /* Getting initial values of indexes of each department. */
        uxDeptPriorityOrder[i - 1] = xDepartments[i].ucID;
    }

    /* Sorting indexes of departments by priority of those departments. This 
     * will be used by dispatcher when he will have to search for available 
     * resources so he could start from department with the lowest priority. */
    int j = 0, tmp = 0;
    for (int i = 1; i < uxDepartmentsAmount; ++i) {
        j = i - 1;
        while ((j >= 0) && (xDepartments[uxDeptPriorityOrder[j]].ucPriority > xDepartments[uxDeptPriorityOrder[j + 1]].ucPriority)) {
            tmp = uxDeptPriorityOrder[j];
            uxDeptPriorityOrder[j] = uxDeptPriorityOrder[j + 1];
            uxDeptPriorityOrder[j + 1] = tmp;
            j--;
        }
    }

    return xDepartments;
}
/*-----------------------------------------------------------*/

void vDepartmentTask( void *pvParameters ) {
    department_t *xDepartment = (department_t *)pvParameters;

    BaseType_t xStatusReceive = 0, xStatusTake = 0, xStatusSend = 0;
    TickType_t xUsageStartTime = 0;

    resource_request_t xRequest = { xDepartment->psName, xDepartment->ucID, 0, 0, xDepartment->xQueue };
    event_t xEvent = { 0, false };

    for ( ;; ) {
        xStatusReceive = xQueueReceive( xDepartment->xQueue, &xEvent, portMAX_DELAY ); 
        if (xStatusReceive == pdPASS) {

            if (xEvent.uxType == eventCODE) {
                /* For debug puposes only. */
                addToCSVLog(&log_to_csv, xTaskGetTickCount(), xEvent.ucCode, xDepartment->psName);

                xUsageStartTime = xTaskGetTickCount();
        
                if (xDepartment->uxCarsAvailable == 0) {
                    xEvent.xRejected = true;
                    vLogNoResourceAvailable( xDepartment->psName );

                    xEventGroupClearBits(xDepartmentEventGroup, xDepartment->uxBitsAvailable);   

                    xStatusSend = xQueueSendToFront(xQueueEvents, &xEvent, portMAX_DELAY);
                    if (xStatusSend == errQUEUE_FULL) {
                        vLogQueueSendError("xQueueEvents");
                    }
                } else {
                    xRequest.ucEventCode = xEvent.ucCode;
                    xRequest.xUsageStartTime = xUsageStartTime;

                    xStatusSend = xQueueSend(xQueueResources, &xRequest, portMAX_DELAY);
                    if (xStatusSend != errQUEUE_FULL) {
                        xDepartment->uxCarsAvailable -= 1;
                    } else {
                        vLogQueueSendError("xQueueResources");
                    }
                }
            }
            if (xEvent.uxType == eventFREE_RESOURCE) {
                xDepartment->uxCarsAvailable += 1;

                if (xDepartment->uxCarsAvailable == 1) {
                    xEventGroupSetBits(xDepartmentEventGroup, xDepartment->uxBitsAvailable);
                }
            }
        }
    }
}
