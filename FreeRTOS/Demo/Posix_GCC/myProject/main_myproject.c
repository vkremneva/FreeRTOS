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

struct logCSV log_to_csv;

void main_myproject(void) {
    srand(time(NULL));

    log_to_csv.ind = 0;

    department_t *xDepartments = pxInitDepartments();

    for (int i = 1; i <= uxDepartmentsAmount; ++i) {
        xDepartments[i].xQueue = xQueueCreate( deptQUEUE_SIZE, deptQUEUE_ITEM_SIZE );
        xDepartments[i].xCountSemaphore = xSemaphoreCreateCounting( xDepartments[i].uxCarsTotal, xDepartments[i].uxCarsTotal );

        if (xDepartments[i].xQueue == NULL || xDepartments[i].xCountSemaphore == NULL) {
            printf("Initializing resources for %s department FAILED.\n", xDepartments[i].psName);
            return;
        }

        xTaskCreate( vDepartmentTask, xDepartments[i].psName, configMINIMAL_STACK_SIZE, &xDepartments[i], xDepartments[i].ucPriority, NULL );
    }

    xQueueEvents = xQueueCreate( eventsQUEUE_SIZE, eventsQUEUE_ITEM_SIZE);
    xQueueResources = xQueueCreate( resourceQUEUE_SIZE, resourceQUEUE_ITEM_SIZE );

    char sResourceTaskName[30];
    for (int i = 0; i < uxResourcesAmount; ++i) {
        snprintf(sResourceTaskName, sizeof(sResourceTaskName), "ResTask %02d", i + 1);
        xTaskCreate( vResourceTask, sResourceTaskName, configMINIMAL_STACK_SIZE, NULL, resourceTASK_PRIORITY, NULL );
    }

    xTaskCreate( vEventGeneratorTask, "EvGenTask", configMINIMAL_STACK_SIZE, (void *)xQueueEvents, eventsPRIORITY, NULL );
    xTaskCreate( vDispatcherTask, "DisptTask", configMINIMAL_STACK_SIZE, (void *)xDepartments, dispPRIORITY, NULL );

    xDepartmentEventGroup = xEventGroupCreate();
    if (xDepartmentEventGroup == NULL) {
        printf("Creating event group FAILED.\n");
    }

   xEventGroupSetBits( xDepartmentEventGroup, uxBitsAvailableAll ); 

    vTaskStartScheduler();

    for ( ;; )
        ;
}
