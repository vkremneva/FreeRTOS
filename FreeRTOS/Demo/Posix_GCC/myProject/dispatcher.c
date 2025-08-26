#include "include/dispatcher.h"

void vDispatcherTask( void * pvParameters )
{
    department_t * pxDepartments = ( department_t * ) pvParameters;

    TickType_t xLastWakeTime = 0;
    BaseType_t xStatusReceive = 0, xStatusSend = 0;

    EventBits_t xEventGroupValue = 0;

    event_t xEvent = { 0, false, eventsCODE, 0, 0 };

    xLastWakeTime = xTaskGetTickCount();
    bool xFreeResourceFound = false;

    for( ;; )
    {
        xStatusReceive = xQueueReceive( xQueueEvents, &xEvent, portMAX_DELAY );

        if( xStatusReceive == pdPASS )
        {
            /* For debug puposes only. */
            addToCSVLog( &log_to_csv, xTaskGetTickCount(), xEvent.ucCode, "Dispatcher Task" );

            if( !xEvent.xRejected )
            {
                xEvent.xUsageStartTime = xTaskGetTickCount();

                xStatusSend = xQueueSend( pxDepartments[ xEvent.ucCode ].xQueue, &xEvent, portMAX_DELAY );

                if( xStatusSend == errQUEUE_FULL )
                {
                    vLogQueueSendError( pxDepartments[ xEvent.ucCode ].psName );
                }
            }
            else
            {
                if( xEvent.uxCounterRejected != eventsCOUNTER_REJECTED_MAX )
                {
                    xEventGroupValue = xEventGroupWaitBits(
                        xDepartmentEventGroup,
                        uxBitsAvailableAll,
                        pdFALSE,
                        pdFALSE,
                        deptMAX_GROUP_WAIT_TIME
                        );

                    xFreeResourceFound = false;

                    for(int i = 0; i < uxDepartmentsAmount; ++i)
                    {
                        /* Checking the availability of free resources in each
                         * department, starting from department with the lowest
                         * priority up to the highest priority department. */
                        if(( xEventGroupValue & pxDepartments[ uxDeptPriorityOrder[ i ] ].uxBitsAvailable ))
                        {
                            xFreeResourceFound = true;

                            xStatusSend = xQueueSend( pxDepartments[ uxDeptPriorityOrder[ i ] ].xQueue, &xEvent, portMAX_DELAY );

                            if( xStatusSend == errQUEUE_FULL )
                            {
                                vLogQueueSendError( pxDepartments[ xEvent.ucCode ].psName );
                            }

                            /* For debug puposes only. */
                            printf( "*********EVENT %d SENT TO %d\n", xEvent.ucCode, i + 1 );
                            addToCSVLog( &log_to_csv, xTaskGetTickCount(), xEvent.ucCode, "Dispatcher Task" );
                            /* End of debug purposes. */
                            break;
                        }
                    }

                    if( !xFreeResourceFound )
                    {
                        vLogNoResourceAvailable( "All departments" );
                        xEvent.uxCounterRejected += 1;

                        xStatusSend = xQueueSendToFront( xQueueEvents, &xEvent, portMAX_DELAY );

                        if( xStatusSend == errQUEUE_FULL )
                        {
                            vLogQueueSendError( "Events" );
                        }

                        /* For debug puposes only. */
                        printf( "**********NO FREE RESOURCE FOUND\n" );
                    }
                }
                else
                {
                    vLogEventRejectedMax( xEvent, pxDepartments[ xEvent.ucCode ].psName );
                }
            }
        }

        vTaskDelayUntil( &xLastWakeTime, dispDELAY );
    }
}
