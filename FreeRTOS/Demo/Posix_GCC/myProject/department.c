#include "department.h"

/*-----------------------------------------------------------*/
EventGroupHandle_t xDepartmentEventGroup = NULL;

UBaseType_t uxDepartmentsAmount = 0;
UBaseType_t uxResourcesAmount = 0;

UBaseType_t uxDeptPriorityOrder[deptMAX_DEPARTMENTS_AMOUNT];

department_t* pxInitDepartments() {
    /* static is possible here because each department task will have access
     * only to its own element in xDepartments[]. Adding "Null" department 
     * to maintain deptartment.id = generated event code. */
    static department_t xDepartments[] = {
        {"Null", 0, 1, 0, 0, 0, 0, 0, NULL, NULL},
        {"Police", deptPOLICE_ID, deptPOLICE_PRIORITY, deptPOLICE_CARS_TOTAL, deptPOLICE_CARS_TOTAL, 0, deptPOLICE_AVAILABLE, NULL, NULL},
        {"Ambulance", deptAMBULANCE_ID, deptAMBULANCE_PRIORITY, deptAMBULANCE_CARS_TOTAL, deptAMBULANCE_CARS_TOTAL, 0, deptAMBULANCE_AVAILABLE, NULL, NULL},
        {"Firefighters", deptFIREFIGHTERS_ID, deptFIREFIGHTERS_PRIORITY, deptFIREFIGHTERS_CARS_TOTAL, deptFIREFIGHTERS_CARS_TOTAL, 0, deptFIRE_AVAILABLE, NULL, NULL},
        {"Corona", deptCORONA_ID, deptCORONA_PRIORITY, deptCORONA_CARS_TOTAL, deptCORONA_CARS_TOTAL, 0, deptCORONA_AVAILABLE, NULL, NULL}
    };

    /* Minus 1 here to exclude "Null" department from total. */
    uxDepartmentsAmount = (sizeof(xDepartments) / sizeof(xDepartments[0])) - 1; 

    uxResourcesAmount = 0;
    for (int i = 1; i <= uxDepartmentsAmount; ++i) {
        uxResourcesAmount += xDepartments[i].uxCarsTotal;
    }

    /* Getting initial values of indexes of each department. */
    for (int i = 0; i < uxDepartmentsAmount; ++i) {
        uxDeptPriorityOrder[i] = xDepartments[i + 1].ucID;
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
    TickType_t xUsageStartTime = 0, xTimestamp = 0;

    resource_request_t xRequest = { NULL, 0, 0, NULL };
    event_t xEvent = { 0, false };

    for ( ;; ) {
        xStatusReceive = xQueueReceive( xDepartment->xQueue, &xEvent, portMAX_DELAY ); 
        if (xStatusReceive == pdPASS) {
            xTimestamp = xTaskGetTickCount();
            addToCSVLog(&log_to_csv, xTimestamp, xEvent.ucCode, xDepartment->psName);

            xUsageStartTime = xTaskGetTickCount();

            xDepartment->uxCarsAvailable = uxSemaphoreGetCount( xDepartment->xCountSemaphore );
            if (xDepartment->uxCarsAvailable == 0) {
                xEvent.xRejected = true;
                vLogNoResourceAvailable( xDepartment->psName );

                xEventGroupClearBits(xDepartmentEventGroup, (1 << xDepartment->ucID));                
                xStatusSend = xQueueSendToFront(xQueueEvents, &xEvent, portMAX_DELAY);
                if (xStatusSend == errQUEUE_FULL) {
                    vLogQueueSendError("xQueueEvents");
                }
            } else {
                //xEventGroupSetBits(xDepartmentEventGroup, (1 << xDepartment->id)); TODO check that I don't need this
                xEventGroupSetBits(xDepartmentEventGroup, xDepartment->bits_available);
            }

            xStatusTake = xSemaphoreTake( xDepartment->xCountSemaphore, portMAX_DELAY );
            if (xStatusTake == pdPASS) {
            
                xRequest.psDepartmentName = xDepartment->psName;
                xRequest.ucEventCode = xEvent.ucCode;
                xRequest.xUsageStartTime = xUsageStartTime;
                xRequest.pxDepartmentSemaphore = xDepartment->xCountSemaphore; // TODO не должен ли тут быть адрес

                xStatusSend = xQueueSend(xQueueResources, &xRequest, portMAX_DELAY);
                if (xStatusSend == errQUEUE_FULL) {
                    vLogQueueSendError("xQueueResources");
                }
            }
        }
    }
}
