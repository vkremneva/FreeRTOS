#include "department.h"

EventGroupHandle_t xDepartmentEventGroup = NULL;
UBaseType_t xDepartmentsAmount = 0;
UBaseType_t xResourcesAmount = 0;
UBaseType_t xIndByPriority[deptMAX_DEPARTMENTS_AMOUNT];

department_t* xInitDepartments() {
    // static is possible here because each department task will have access
    //   only to its own element in xDepartments[]
    // adding "Null" department to maintain deptartment.id = generated event code
    static department_t xDepartments[] = {
        {"Null", 0, 1, 0, 0, 0, 0, 0, NULL, NULL},
        {"Police", deptPOLICE_ID, deptPOLICE_PRIORITY, deptPOLICE_CARS_TOTAL, deptPOLICE_CARS_TOTAL, 0, 0, deptPOLICE_AVAILABLE, NULL, NULL},
        {"Ambulance", deptAMBULANCE_ID, deptAMBULANCE_PRIORITY, deptAMBULANCE_CARS_TOTAL, deptAMBULANCE_CARS_TOTAL, 0, 0, deptAMBULANCE_AVAILABLE, NULL, NULL},
        {"Firefighters", deptFIREFIGHTERS_ID, deptFIREFIGHTERS_PRIORITY, deptFIREFIGHTERS_CARS_TOTAL, deptFIREFIGHTERS_CARS_TOTAL, 0, 0, deptFIRE_AVAILABLE, NULL, NULL},
        {"Corona", deptCORONA_ID, deptCORONA_PRIORITY, deptCORONA_CARS_TOTAL, deptCORONA_CARS_TOTAL, 0, 0, deptCORONA_AVAILABLE, NULL, NULL}
    };

    xDepartmentsAmount = (sizeof(xDepartments) / sizeof(xDepartments[0])) - 1; // to exclude "Null" department from total

    xResourcesAmount = 0;
    for (int i = 1; i <= xDepartmentsAmount; ++i) {
        xResourcesAmount += xDepartments[i].cars_total;
    }

    // getting initial values of indexes
    for (int i = 0; i < xDepartmentsAmount; ++i) {
        xIndByPriority[i] = xDepartments[i + 1].id;
    }
    // sorting indexes of departments by priority of those departments
    int j = 0, tmp = 0;
    for (int i = 1; i < xDepartmentsAmount; ++i) {
        j = i - 1;
        while ((j >= 0) && (xDepartments[xIndByPriority[j]].priority > xDepartments[xIndByPriority[j + 1]].priority)) {
            tmp = xIndByPriority[j];
            xIndByPriority[j] = xIndByPriority[j + 1];
            xIndByPriority[j + 1] = tmp;
            j--;
        }
    }

    return xDepartments;
}

void vDepartmentTask( void *pvParameters ) {
    department_t *xDepartment = (department_t *)pvParameters;

    BaseType_t xStatusReceive = 0, xStatusTake = 0, xStatusSend = 0;
    TickType_t xUsageStartTime = 0, xTimestamp = 0;

    resource_request_t xRequest = { NULL, 0, 0, NULL };
    event_t xEvent = { 0, false };

    for ( ;; ) {
        xStatusReceive = xQueueReceive( xDepartment->queue, &xEvent, portMAX_DELAY ); 
        if (xStatusReceive == pdPASS) {
            xTimestamp = xTaskGetTickCount();
            addToCSVLog(&log_to_csv, xTimestamp, xEvent.code, xDepartment->name);

            xUsageStartTime = xTaskGetTickCount();

            xDepartment->cars_available = uxSemaphoreGetCount( xDepartment->countingSemaphore );
            if (xDepartment->cars_available == 0) {
                xEvent.rejected = true;
                logNoResourceAvailable( xDepartment->name );

                xEventGroupClearBits(xDepartmentEventGroup, (1 << xDepartment->id));                
                xStatusSend = xQueueSendToFront(xQueueEvents, &xEvent, portMAX_DELAY);
                if (xStatusSend == errQUEUE_FULL) {
                    logQueueSendError("xQueueEvents");
                }
            } else {
                xEventGroupSetBits(xDepartmentEventGroup, (1 << xDepartment->id));
            }

            xStatusTake = xSemaphoreTake( xDepartment->countingSemaphore, portMAX_DELAY );
            if (xStatusTake == pdPASS) {
            
                xRequest.department_name = xDepartment->name;
                xRequest.event_code = xEvent.code;
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