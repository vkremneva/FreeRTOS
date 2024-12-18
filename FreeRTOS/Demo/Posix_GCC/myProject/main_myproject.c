#include <stdio.h>
#include <unistd.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
 
#include "events.h"
#include "dispatcher.h"
#include "department.h"

QueueHandle_t xQueueEvents;

// because I dont know how to send this to vDispatcherTask
const int DEPARTMENTS_AMOUNT = 4;

void main_myproject(void) {
    srand(time(NULL));

    xQueueEvents = xQueueCreate( eventsQUEUE_SIZE, eventsQUEUE_ITEM_SIZE );

    department_t xDepartments[] = {
        {"Police", deptPOLICE_ID, deptPOLICE_PRIORITY, deptPOLICE_CARS_TOTAL, deptPOLICE_CARS_TOTAL, 0, 0, NULL, NULL},
        {"Ambulance", deptAMBULANCE_ID, deptAMBULANCE_PRIORITY, deptAMBULANCE_CARS_TOTAL, deptAMBULANCE_CARS_TOTAL, 0, 0, NULL, NULL},
        {"Firefighters", deptFIREFIGHTERS_ID, deptFIREFIGHTERS_PRIORITY, deptFIREFIGHTERS_CARS_TOTAL, deptFIREFIGHTERS_CARS_TOTAL, 0, 0, NULL, NULL},
        {"Corona", deptCORONA_ID, deptCORONA_PRIORITY, deptCORONA_CARS_TOTAL, deptCORONA_CARS_TOTAL, 0, 0, NULL, NULL}
    };

    for (int i = 0; i < DEPARTMENTS_AMOUNT; ++i) {
        xDepartments[i].queue = xQueueCreate( deptQUEUE_SIZE, deptQUEUE_ITEM_SIZE );
        xDepartments[i].countingSemaphore = xSemaphoreCreateCounting( xDepartments[i].cars_total, xDepartments[i].cars_total );

        if (xDepartments[i].queue == NULL || xDepartments[i].countingSemaphore == NULL) {
            printf("Initializing resources for %s department FAILED.\n", xDepartments[i].name);
            return;
        }

        xTaskCreate( vDepartmentTask, xDepartments[i].name, configMINIMAL_STACK_SIZE, &xDepartments[i], xDepartments[i].priority, NULL );
    }

    xTaskCreate( vEventGeneratorTask, "Event Generator Task", configMINIMAL_STACK_SIZE, (void *)xQueueEvents, eventsPRIORITY, NULL );
    xTaskCreate( vDispatcherTask, "Dispatcher Task", configMINIMAL_STACK_SIZE, (void *)xDepartments, dispatcherPRIORITY, NULL );

    vTaskStartScheduler();

    for ( ;; )
        ;
}
