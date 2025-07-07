#include <stdio.h>
#include <unistd.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
 
#include "events.h"
#include "dispatcher.h"
#include "department.h"
#include "resource.h"

// because I dont know how to send this to vDispatcherTask
const int DEPARTMENTS_AMOUNT = 4;

struct logCSV log;

BaseType_t xGetResourcesAmount( department_t *xDepartments);

void main_myproject(void) {
    srand(time(NULL));

    log.ind = 0;

    department_t xDepartments[] = {
        {"Null", 0, 1, 0, 0, 0, 0, NULL, NULL},
        {"Police", deptPOLICE_ID, deptPOLICE_PRIORITY, deptPOLICE_CARS_TOTAL, deptPOLICE_CARS_TOTAL, 0, 0, NULL, NULL},
        {"Ambulance", deptAMBULANCE_ID, deptAMBULANCE_PRIORITY, deptAMBULANCE_CARS_TOTAL, deptAMBULANCE_CARS_TOTAL, 0, 0, NULL, NULL},
        {"Firefighters", deptFIREFIGHTERS_ID, deptFIREFIGHTERS_PRIORITY, deptFIREFIGHTERS_CARS_TOTAL, deptFIREFIGHTERS_CARS_TOTAL, 0, 0, NULL, NULL},
        {"Corona", deptCORONA_ID, deptCORONA_PRIORITY, deptCORONA_CARS_TOTAL, deptCORONA_CARS_TOTAL, 0, 0, NULL, NULL}
    };

    for (int i = 1; i <= DEPARTMENTS_AMOUNT; ++i) {
        xDepartments[i].queue = xQueueCreate( deptQUEUE_SIZE, deptQUEUE_ITEM_SIZE );
        xDepartments[i].countingSemaphore = xSemaphoreCreateCounting( xDepartments[i].cars_total, xDepartments[i].cars_total );

        if (xDepartments[i].queue == NULL || xDepartments[i].countingSemaphore == NULL) {
            printf("Initializing resources for %s department FAILED.\n", xDepartments[i].name);
            return;
        }

        xTaskCreate( vDepartmentTask, xDepartments[i].name, configMINIMAL_STACK_SIZE, &xDepartments[i], xDepartments[i].priority, NULL );
    }

    xQueueEvents = xQueueCreate( eventsQUEUE_SIZE, eventsQUEUE_ITEM_SIZE);
    xQueueResources = xQueueCreate( resourceQUEUE_SIZE, resourceQUEUE_ITEM_SIZE );

    BaseType_t RESOURCES_AMOUNT = xGetResourcesAmount(xDepartments);
    char sResourceTaskName[30];
    for (int i = 0; i < RESOURCES_AMOUNT; ++i) {
        snprintf(sResourceTaskName, sizeof(sResourceTaskName), "ResTask %02d", i + 1);
        xTaskCreate( vResourceTask, sResourceTaskName, configMINIMAL_STACK_SIZE, NULL, resourceTASK_PRIORITY, NULL );
    }

    xTaskCreate( vEventGeneratorTask, "EvGenTask", configMINIMAL_STACK_SIZE, (void *)xQueueEvents, eventsPRIORITY, NULL );
    xTaskCreate( vDispatcherTask, "DisptTask", configMINIMAL_STACK_SIZE, (void *)xDepartments, dispatcherPRIORITY, NULL );

    xDepartmentEventGroup = xEventGroupCreate();
    if (xDepartmentEventGroup == NULL) {
        printf("Creating event group FAILED.\n");
    }

    xEventGroupSetBits( xDepartmentEventGroup, 
                       deptPOLICE_AVAILABLE | 
                       deptAMBULANCE_AVAILABLE | 
                       deptFIRE_AVAILABLE | 
                       deptCORONA_AVAILABLE
    );

    vTaskStartScheduler();

    for ( ;; )
        ;
}

BaseType_t xGetResourcesAmount( department_t *xDepartments) {
    BaseType_t xTotalSum = 0;

    for (int i = 1; i <= DEPARTMENTS_AMOUNT; ++i) {
        xTotalSum += xDepartments[i].cars_total;
    }

    return xTotalSum;
}
