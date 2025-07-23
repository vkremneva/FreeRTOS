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

    department_t *xDepartments = xInitDepartments();

    for (int i = 1; i <= xDepartmentsAmount; ++i) {
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

    char sResourceTaskName[30];
    for (int i = 0; i < xResourcesAmount; ++i) {
        snprintf(sResourceTaskName, sizeof(sResourceTaskName), "ResTask %02d", i + 1);
        xTaskCreate( vResourceTask, sResourceTaskName, configMINIMAL_STACK_SIZE, NULL, resourceTASK_PRIORITY, NULL );
    }

    xTaskCreate( vEventGeneratorTask, "EvGenTask", configMINIMAL_STACK_SIZE, (void *)xQueueEvents, eventsPRIORITY, NULL );
    xTaskCreate( vDispatcherTask, "DisptTask", configMINIMAL_STACK_SIZE, (void *)xDepartments, dispatcherPRIORITY, NULL );

    xDepartmentEventGroup = xEventGroupCreate();
    if (xDepartmentEventGroup == NULL) {
        printf("Creating event group FAILED.\n");
    }

    // still not for n departments
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
