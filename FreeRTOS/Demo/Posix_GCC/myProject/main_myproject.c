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

    department_t * xDepartments = pxInitDepartments();

    for(int i = 1; i <= uxDepartmentsAmount; ++i)
    {
        xDepartments[ i ].xQueue = xQueueCreate( deptQUEUE_SIZE, deptQUEUE_ITEM_SIZE );

        if( xDepartments[ i ].xQueue == NULL )
        {
            printf( "Initializing resources for %s department FAILED.\n", xDepartments[ i ].psName );
            return;
        }

        xTaskCreate( vDepartmentTask, xDepartments[ i ].psName, configMINIMAL_STACK_SIZE, &xDepartments[ i ], xDepartments[ i ].ucPriority, NULL );
    }

    xQueueEvents = xQueueCreate( eventsQUEUE_SIZE, eventsQUEUE_ITEM_SIZE );
    xQueueResources = xQueueCreate( resourceQUEUE_SIZE, resourceQUEUE_ITEM_SIZE );
    xQueueLogging = xQueueCreate( logQUEUE_SIZE, logQUEUE_ITEM_SIZE );

    char sResourceTaskName[ 30 ];

    for(int i = 0; i < uxResourcesAmount; ++i)
    {
        snprintf( sResourceTaskName, sizeof( sResourceTaskName ), "ResTask %02d", i + 1 );
        xTaskCreate( vResourceTask, sResourceTaskName, configMINIMAL_STACK_SIZE, NULL, resourceTASK_PRIORITY, NULL );
    }

    xTaskCreate( vEventGeneratorTask, "EvGenTask", configMINIMAL_STACK_SIZE, ( void * ) xQueueEvents, eventsPRIORITY, NULL );
    xTaskCreate( vDispatcherTask, "DisptTask", configMINIMAL_STACK_SIZE, ( void * ) xDepartments, dispPRIORITY, NULL );
    xTaskCreate( vLoggingTask, "LogTask", configMINIMAL_STACK_SIZE, ( void * ) xQueueLogging, logPRIORITY, NULL );

    xDepartmentEventGroup = xEventGroupCreate();

    if( xDepartmentEventGroup == NULL )
    {
        printf( "Creating event group FAILED.\n" );
    }

    xEventGroupSetBits( xDepartmentEventGroup, uxBitsAvailableAll );

    vTaskStartScheduler();

    for( ;; )
    {
    }
}
