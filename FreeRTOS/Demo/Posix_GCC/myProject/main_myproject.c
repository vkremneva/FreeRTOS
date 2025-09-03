#include <stdio.h>
#include <unistd.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "include/events.h"
#include "include/dispatcher.h"
#include "include/department.h"
#include "include/resource.h"

void main_myproject( void )
{
    srand( time( NULL ));

    BaseType_t xStatus = 0;

    department_t * xDepartments = pxInitDepartments();

    for(int i = 1; i <= uxDepartmentsAmount; ++i)
    {
        xDepartments[ i ].xQueue = xQueueCreate( deptQUEUE_SIZE, deptQUEUE_ITEM_SIZE );
        configASSERT( xDepartments[ i ].xQueue != NULL );

        xStatus = xTaskCreate( vDepartmentTask, xDepartments[ i ].psName, configMINIMAL_STACK_SIZE, &xDepartments[ i ], xDepartments[ i ].ucPriority, NULL );
        configASSERT( xStatus == pdPASS );
    }

    xQueueEvents = xQueueCreate( eventsQUEUE_SIZE, eventsQUEUE_ITEM_SIZE );
    configASSERT( xQueueEvents != NULL );

    xQueueResources = xQueueCreate( resourceQUEUE_SIZE, resourceQUEUE_ITEM_SIZE );
    configASSERT( xQueueResources != NULL );

    xQueueLogging = xQueueCreate( logQUEUE_SIZE, logQUEUE_ITEM_SIZE );
    configASSERT( xQueueLogging != NULL );

    char sResourceTaskName[ 30 ];

    for(int i = 0; i < uxResourcesAmount; ++i)
    {
        snprintf( sResourceTaskName, sizeof( sResourceTaskName ), "ResTask %02d", i + 1 );
        xStatus = xTaskCreate( vResourceTask, sResourceTaskName, configMINIMAL_STACK_SIZE, NULL, resourceTASK_PRIORITY, NULL );
        configASSERT( xStatus == pdPASS );
    }

    xStatus = xTaskCreate( vEventGeneratorTask, "EvGenTask", configMINIMAL_STACK_SIZE, NULL, eventsPRIORITY, NULL );
    configASSERT( xStatus == pdPASS );

    xStatus = xTaskCreate( vDispatcherTask, "DisptTask", configMINIMAL_STACK_SIZE, ( void * ) xDepartments, dispPRIORITY, NULL );
    configASSERT( xStatus == pdPASS );

    xStatus = xTaskCreate( vLoggingTask, "LogTask", configMINIMAL_STACK_SIZE, NULL, logPRIORITY, NULL );
    configASSERT( xStatus == pdPASS );

    xDepartmentEventGroup = xEventGroupCreate();
    configASSERT( xDepartmentEventGroup != NULL );

    xEventGroupSetBits( xDepartmentEventGroup, uxBitsAvailableAll );

    vTaskStartScheduler();

    for( ;; )
    {
    }
}
